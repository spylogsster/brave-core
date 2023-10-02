/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.util;

import android.text.TextUtils;

import androidx.annotation.NonNull;
import org.chromium.base.Callback;
import org.chromium.brave_wallet.mojom.BlockchainRegistry;
import org.chromium.brave_wallet.mojom.BlockchainToken;
import org.chromium.brave_wallet.mojom.BraveWalletService;
import org.chromium.brave_wallet.mojom.CoinType;
import org.chromium.brave_wallet.mojom.NetworkInfo;
import org.chromium.brave_wallet.mojom.OnRampProvider;
import org.chromium.mojo.bindings.Callbacks;
import org.chromium.mojo.bindings.Callbacks.Callback1;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Stream;

public class TokenUtils {
    /**
     * Type of token used for filtering an array of {@code BlockchainToken}.
     * See {@link #filterTokens(NetworkInfo, BlockchainToken[], TokenUtils.TokenType, boolean)}.
     */
    public enum TokenType {
        // Filter out all tokens that are not NFTs.
        // Note: native assets won't be included when using this token type.
        NFTS,

        // Filter out all tokens that don't belong to ERC20 standard.
        ERC20,

        // Filter out all tokens that don't belong to ERC721 standard.
        ERC721,

        // Filter out all tokens that are not solana coins.
        SOL,

        // Filter out all tokens that are NFTs.
        // Note: a custom asset may or may not be an NFT.
        NON_NFTS,

        // Don't apply any filter, the list will include all tokens.
        ALL
    }

    /**
     * Filter tokens by type and add native token (when needed).
     *
     * BlockchainRegistry.getAllTokens does not return the chain's native token;
     * BraveWalletService.getUserAssets contains the native asset on init, but:
     *   - the returned BlockchainToken object has no logo
     *   - the asset can be removed at some point by removeUserAsset
     * To make things consistent, here we do the following:
     *   - Decide whether a native asset exists in the input token array with isSameToken
     *   - If exists, replace the BlockchainToken object with the one generated by
     * Utils.makeNetworkAsset
     *   - If not exist, insert the BlockchainToken object generated by Utils.makeNetworkAsset to
     * front
     *
     * See `refreshVisibleTokenInfo` in components/brave_wallet_ui/common/async/lib.ts.
     */
    private static BlockchainToken[] filterTokens(NetworkInfo selectedNetwork,
            BlockchainToken[] tokens, TokenType tokenType, boolean keepVisibleOnly) {
        BlockchainToken nativeAsset = Utils.makeNetworkAsset(selectedNetwork);
        Stream<BlockchainToken> tokenStream = Arrays.stream(tokens).filter(token -> {
            final boolean typeFilter;
            switch (tokenType) {
                case NFTS:
                    typeFilter = token.isNft;
                    break;
                case ERC20:
                    typeFilter = token.isErc20;
                    break;
                case ERC721:
                    typeFilter = token.isErc721;
                    break;
                case SOL:
                    typeFilter = token.coin == CoinType.SOL;
                    break;
                case NON_NFTS:
                    typeFilter = !token.isNft;
                    break;
                case ALL:
                    typeFilter = true;
                    break;
                default:
                    throw new UnsupportedOperationException("Token type not supported.");
            }
            return typeFilter && !isSameToken(token, nativeAsset)
                    && (!keepVisibleOnly || token.visible);
        });

        // When token type is NFTS the native assets should not be added.
        // For all the other cases we add them.
        if (tokenType != TokenType.NFTS) {
            tokenStream = Stream.concat(Stream.of(nativeAsset), tokenStream);
        }
        return tokenStream.toArray(BlockchainToken[] ::new);
    }

    public static void getUserAssetsFiltered(BraveWalletService braveWalletService,
            NetworkInfo selectedNetwork, int coinType, TokenType tokenType,
            Callbacks.Callback1<BlockchainToken[]> callback) {
        braveWalletService.getUserAssets(
                selectedNetwork.chainId, coinType, (BlockchainToken[] tokens) -> {
                    BlockchainToken[] filteredTokens =
                            filterTokens(selectedNetwork, tokens, tokenType, true);
                    callback.call(filteredTokens);
                });
    }

    /**
     * Get tokens of all networks from <code>networkInfos<code/> list.
     * @param blockchainRegistry to get tokens from core
     * @param callback to get array of tokens
     */
    public static void getAllTokensFiltered(BlockchainRegistry blockchainRegistry,
            List<NetworkInfo> networkInfos, TokenType tokenType,
            Callbacks.Callback1<BlockchainToken[]> callback) {
        AsyncUtils.MultiResponseHandler allNetworkTokenCollector =
                new AsyncUtils.MultiResponseHandler(networkInfos.size());
        ArrayList<AsyncUtils.GetNetworkAllTokensContext> allTokenContexts = new ArrayList<>();

        for (NetworkInfo networkInfo : networkInfos) {
            AsyncUtils.GetNetworkAllTokensContext context =
                    new AsyncUtils.GetNetworkAllTokensContext(
                            allNetworkTokenCollector.singleResponseComplete, networkInfo);
            blockchainRegistry.getAllTokens(networkInfo.chainId, networkInfo.coin, context);
            allTokenContexts.add(context);
        }
        allNetworkTokenCollector.setWhenAllCompletedAction(() -> {
            callback.call(allTokenContexts.stream()
                                  .map(context
                                          -> filterTokens(context.networkInfo, context.tokens,
                                                  tokenType, false))
                                  .flatMap(tokens -> Arrays.stream(tokens))
                                  .toArray(BlockchainToken[] ::new));
        });
    }

    /*
     * Wrapper for BlockchainRegistry.getAllTokens with Goerli contract address modifications.
     */
    public static void getAllTokens(BlockchainRegistry blockchainRegistry, String chainId,
            int coinType, Callbacks.Callback1<BlockchainToken[]> callback) {
        blockchainRegistry.getAllTokens(chainId, coinType, tokens -> {
            tokens = Utils.fixupTokensRegistry(tokens, chainId);
            callback.call(tokens);
        });
    }

    public static void getAllTokensFiltered(BraveWalletService braveWalletService,
            BlockchainRegistry blockchainRegistry, NetworkInfo selectedNetwork, int coinType,
            TokenType tokenType, Callbacks.Callback1<BlockchainToken[]> callback) {
        getAllTokens(blockchainRegistry, selectedNetwork.chainId, coinType, tokens -> {
            braveWalletService.getUserAssets(selectedNetwork.chainId, coinType, userTokens -> {
                BlockchainToken[] filteredTokens = filterTokens(selectedNetwork,
                        distinctiveConcatenatedArrays(tokens, userTokens), tokenType, false);
                callback.call(filteredTokens);
            });
        });
    }

    /**
     * Retrieves user tokens and all tokens of a given type for a selected network.
     *
     * @param braveWalletService BraveWalletService instance used to get the user tokens.
     * @param blockchainRegistry BlockchainRegistry instance used to get all the tokens.
     * @param selectedNetwork Selected network used to retrieve the tokens.
     * @param coinType Coin type of the selected network.
     * @param tokenType Token type used for filtering.
     * @param callback Callback returning two arrays: all tokens, and user tokens.
     * <b>Note:</b>: the first array containing all the tokens will include the user tokens.
     */
    public static void getUserAndAllTokensFiltered(BraveWalletService braveWalletService,
            BlockchainRegistry blockchainRegistry, NetworkInfo selectedNetwork, int coinType,
            TokenType tokenType,
            Callbacks.Callback2<BlockchainToken[], BlockchainToken[]> callback) {
        getAllTokens(blockchainRegistry, selectedNetwork.chainId, coinType,
                tokens
                -> braveWalletService.getUserAssets(
                        selectedNetwork.chainId, coinType, userTokens -> {
                            BlockchainToken[] filteredTokens = filterTokens(selectedNetwork,
                                    distinctiveConcatenatedArrays(tokens, userTokens), tokenType,
                                    false);
                            BlockchainToken[] filteredUserTokens =
                                    filterTokens(selectedNetwork, userTokens, tokenType, true);
                            callback.call(filteredTokens, filteredUserTokens);
                        }));
    }

    public static void getUserOrAllTokensFiltered(BraveWalletService braveWalletService,
            BlockchainRegistry blockchainRegistry, NetworkInfo selectedNetwork, int coinType,
            TokenType tokenType, boolean userAssetsOnly,
            Callbacks.Callback1<BlockchainToken[]> callback) {
        if (JavaUtils.anyNull(braveWalletService, blockchainRegistry)) return;
        if (userAssetsOnly)
            getUserAssetsFiltered(
                    braveWalletService, selectedNetwork, coinType, tokenType, callback);
        else
            getAllTokensFiltered(braveWalletService, blockchainRegistry, selectedNetwork, coinType,
                    tokenType, callback);
    }

    private static final int[] SUPPORTED_RAMP_PROVIDERS = {
            OnRampProvider.RAMP, OnRampProvider.SARDINE, OnRampProvider.TRANSAK};

    public static void getBuyTokensFiltered(BlockchainRegistry blockchainRegistry,
            NetworkInfo selectedNetwork, TokenType tokenType,
            Callbacks.Callback1<BlockchainToken[]> callback) {
        int[] rampProviders = SUPPORTED_RAMP_PROVIDERS;
        blockchainRegistry.getProvidersBuyTokens(rampProviders, selectedNetwork.chainId, tokens -> {
            // blockchainRegistry.getProvidersBuyTokens returns a full list of tokens that are
            // allowed to buy by given rampProviders. We just need to check on native assets logo
            // and fill them if they are empty.
            for (BlockchainToken tokenFromAll : tokens) {
                if (tokenFromAll.logo.isEmpty()) {
                    tokenFromAll.logo =
                            Utils.getNetworkIconName(tokenFromAll.chainId, tokenFromAll.coin);
                }
            }
            Arrays.sort(tokens, blockchainTokenComparatorPerGasOrBatType);
            callback.call(removeDuplicates(tokens));
        });
    }

    public static void isBuySupported(BlockchainRegistry blockchainRegistry,
            NetworkInfo selectedNetwork, String assetSymbol, String contractAddress, String chainId,
            Callback1<Boolean> callback) {
        getBuyTokensFiltered(
                blockchainRegistry, selectedNetwork, TokenUtils.TokenType.ALL, tokens -> {
                    callback.call(JavaUtils.includes(tokens,
                            iToken
                            -> AssetUtils.Filters.isSameToken(
                                    iToken, assetSymbol, contractAddress, chainId)));
                });
    }

    public static void isCustomToken(@NonNull BlockchainRegistry blockchainRegistry,
            @NonNull NetworkInfo selectedNetwork, int coinType, @NonNull BlockchainToken token,
            Callbacks.Callback1<Boolean> callback) {
        assert !JavaUtils.anyNull(token, selectedNetwork) : "Token or network should not be null";
        if (JavaUtils.anyNull(token, selectedNetwork)) return;

        if (TextUtils.isEmpty(token.contractAddress)) {
            callback.call(false);
            return;
        }
        // Any token with a token ID should be considered a custom token
        if (!TextUtils.isEmpty(token.tokenId)) {
            callback.call(true);
            return;
        }
        getAllTokens(blockchainRegistry, selectedNetwork.chainId, coinType, tokens -> {
            boolean isCustom = true;
            tokens = filterTokens(selectedNetwork, tokens, TokenType.ALL, false);
            for (BlockchainToken tokenFromAll : tokens) {
                if (token.contractAddress.equals(tokenFromAll.contractAddress)) {
                    isCustom = false;
                    break;
                }
            }
            callback.call(isCustom);
        });
    }

    /**
     * Concatenates arrays, add only elements of arraySecond that are not present in the arrayFirst
     * @param arrayFirst first array to be added in the result
     * @param arraySecond second array, only distinctive elements are added in result by comparing
     *         with the items of arrayFirst
     * @return concatenated array
     */
    public static BlockchainToken[] distinctiveConcatenatedArrays(
            BlockchainToken[] arrayFirst, BlockchainToken[] arraySecond) {
        List<BlockchainToken> both = new ArrayList<>();

        Collections.addAll(both, arrayFirst);
        for (BlockchainToken tokenSecond : arraySecond) {
            boolean add = true;
            for (BlockchainToken tokenFirst : arrayFirst) {
                if (isSameToken(tokenFirst, tokenSecond)) {
                    add = false;
                    break;
                }
            }
            if (add) {
                both.add(tokenSecond);
            }
        }

        return both.toArray(new BlockchainToken[0]);
    }

    /**
     * Checks if two tokens are equal. Two tokens are equal if:
     * - Chain ID matches.
     * - Symbol matches.
     * - Token ID matches.
     * - Contract address (ignore case) matches.
     *
     * @param token1 First token to compare.
     * @param token2 Second token to compare.
     * @return {@code} true if two tokens are equal, {@code false} otherwise.
     */
    public static boolean isSameToken(
            @NonNull BlockchainToken token1, @NonNull BlockchainToken token2) {
        return token1.chainId.equals(token2.chainId) && token1.symbol.equals(token2.symbol)
                && token1.tokenId.equals(token2.tokenId)
                && token1.contractAddress.equalsIgnoreCase(token2.contractAddress);
    }

    public static void getExactUserAsset(BraveWalletService braveWalletService,
            NetworkInfo selectedNetwork, int coinType, String assetSymbol, String assetName,
            String assetId, String contractAddress, int assetDecimals,
            Callback<BlockchainToken> callback) {
        getUserAssetsFiltered(
                braveWalletService, selectedNetwork, coinType, TokenType.ALL, userAssets -> {
                    BlockchainToken resultToken = null;
                    for (BlockchainToken userAsset : userAssets) {
                        if (selectedNetwork.chainId.equals(userAsset.chainId)
                                && assetSymbol.equals(userAsset.symbol)
                                && assetName.equals(userAsset.name)
                                && (assetId.isEmpty() || assetId.equals(userAsset.tokenId))
                                && contractAddress.equals(userAsset.contractAddress)
                                && assetDecimals == userAsset.decimals) {
                            resultToken = userAsset;
                        }
                    }

                    callback.onResult(resultToken);
                });
    }

    private static final Comparator<BlockchainToken> blockchainTokenComparatorPerGasOrBatType =
            (token1, token2) -> {
        boolean isNativeToken1 = AssetUtils.isNativeToken(token1);
        boolean isNativeToken2 = AssetUtils.isNativeToken(token2);
        boolean isBatToken1 = AssetUtils.isBatToken(token1);
        boolean isBatToken2 = AssetUtils.isBatToken(token2);
        if (isNativeToken1 && !isNativeToken2)
            return -1;
        else if (!isNativeToken1 && isNativeToken2)
            return 1;
        else if (isBatToken1 && !isBatToken2)
            return -1;
        else if (!isBatToken1 && isBatToken2)
            return 1;
        else
            return token1.symbol.compareTo(token2.symbol);
    };

    // Returns an array of available assets <b>per ramp provider</b> and removes duplicates with the
    // same contract address using case insensitive contractAddress comparison.
    // The `getProvidersBuyTokens` API can return a merged list containing the available assets
    // <b>per ramp provider</b>. It's not unusual to have the same asset multiple times with the
    // same contract address all upper case from a ramp provider and all lower case from another
    // one. Thus it's important to compare the contract addresses ignoring case.
    private static BlockchainToken[] removeDuplicates(BlockchainToken[] tokens) {
        List<BlockchainToken> result = new ArrayList<>();
        for (BlockchainToken item : tokens) {
            String contractAddress = item.contractAddress;
            boolean duplicate = false;
            for (BlockchainToken itemResult : result) {
                // IMPORTANT: use `equalsIgnoreCase` to detect two contract addresses with different
                // capitalization.
                if (contractAddress.equalsIgnoreCase(itemResult.contractAddress)) {
                    duplicate = true;
                    break;
                }
            }
            // Do not add duplicated item.
            if (!duplicate) {
                result.add(item);
            }
        }
        return result.toArray(new BlockchainToken[0]);
    }
}

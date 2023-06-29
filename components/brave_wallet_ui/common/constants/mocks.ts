// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// types
import {
  AppsListType,
  BraveWallet,
  SerializableTransactionInfo,
  WalletAccountType,
  SpotPriceRegistry
} from '../../constants/types'
import { NftsPinningStatusType } from '../../page/constants/action_types'

// utils
import { getAssetIdKey } from '../../utils/asset-utils'
import { getPriceIdForToken } from '../../utils/api-utils'

// mocks
import {
  mockBasicAttentionToken,
  mockEthToken,
  mockMoonCatNFT
} from '../../stories/mock-data/mock-asset-options'
import { mockNFTMetadata } from '../../stories/mock-data/mock-nft-metadata'
import { mockEthMainnet } from '../../stories/mock-data/mock-networks'


type EIP1559SerializableTransactionInfo = SerializableTransactionInfo & {
  txDataUnion: { ethTxData1559: BraveWallet.TxData1559 }
}

export const getMockedTransactionInfo =
  (): EIP1559SerializableTransactionInfo => {
    return {
      chainId: BraveWallet.LOCALHOST_CHAIN_ID,
      id: '1',
      fromAddress: '0x8b52c24d6e2600bdb8dbb6e8da849ed38ab7e81f',
      txHash: '',
      txDataUnion: {
        ethTxData1559: {
          baseData: {
            to: '0x8b52c24d6e2600bdb8dbb6e8da849ed38ab7e81f',
            value: '0x01706a99bf354000', // 103700000000000000 wei (0.1037 ETH)
            // data: new Uint8Array(0),
            data: [] as number[],
            nonce: '0x03',
            gasLimit: '0x5208', // 2100
            gasPrice: '0x22ecb25c00', // 150 Gwei
            signOnly: false,
            signedTransaction: undefined
          },
          chainId: BraveWallet.LOCALHOST_CHAIN_ID,
          maxPriorityFeePerGas: '',
          maxFeePerGas: '',
          gasEstimation: undefined
        },
        ethTxData: {} as any,
        filTxData: undefined,
        solanaTxData: undefined
      },
      txStatus: BraveWallet.TransactionStatus.Approved,
      txType: BraveWallet.TransactionType.Other,
      txParams: [],
      txArgs: [],
      createdTime: { microseconds: 0 },
      submittedTime: { microseconds: 0 },
      confirmedTime: { microseconds: 0 },
      originInfo: {
        origin: {
          scheme: 'https',
          host: 'brave.com',
          port: 443,
          nonceIfOpaque: undefined
        },
        originSpec: 'https://brave.com',
        eTldPlusOne: 'brave.com'
      },
      groupId: undefined,
      effectiveRecipient: '0x8b52c24d6e2600bdb8dbb6e8da849ed38ab7e81f'
    }
  }

export const mockNetwork: BraveWallet.NetworkInfo = {
  chainId: '0x1',
  chainName: 'Ethereum Main Net',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://api.node.glif.io/rpc/v0' }],
  blockExplorerUrls: ['https://etherscan.io'],
  symbol: 'ETH',
  symbolName: 'Ethereum',
  decimals: 18,
  iconUrls: [],
  coin: BraveWallet.CoinType.ETH,
  isEip1559: false
}

export const mockFilecoinMainnetNetwork: BraveWallet.NetworkInfo = {
  chainId: 'f',
  chainName: 'Filecoin Mainnet',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://calibration.node.glif.io/rpc/v0' }],
  blockExplorerUrls: ['https://filscan.io/tipset/message-detail'],
  symbol: 'FIL',
  symbolName: 'Filecoin',
  decimals: 18,
  iconUrls: [],
  coin: BraveWallet.CoinType.FIL,
  isEip1559: false
}

export const mockFilecoinTestnetNetwork: BraveWallet.NetworkInfo = {
  chainId: 't',
  chainName: 'Filecoin Testnet',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://mainnet-beta-solana.brave.com/rpc' }],
  blockExplorerUrls: ['https://calibration.filscan.io/tipset/message-detail'],
  symbol: 'FIL',
  symbolName: 'Filecoin',
  decimals: 18,
  iconUrls: [],
  coin: BraveWallet.CoinType.FIL,
  isEip1559: false
}

export const mockSolanaMainnetNetwork: BraveWallet.NetworkInfo = {
  chainId: '0x65',
  chainName: 'Solana Mainnet Beta',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://api.testnet.solana.com' }],
  blockExplorerUrls: ['https://explorer.solana.com'],
  symbol: 'SOL',
  symbolName: 'Solana',
  decimals: 9,
  iconUrls: [],
  coin: BraveWallet.CoinType.SOL,
  isEip1559: false
}

export const mockSolanaTestnetNetwork: BraveWallet.NetworkInfo = {
  chainId: '0x66',
  chainName: 'Solana Testnet',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://api.testnet.solana.com' }],
  blockExplorerUrls: ['https://explorer.solana.com?cluster=testnet'],
  symbol: 'SOL',
  symbolName: 'Solana',
  decimals: 9,
  iconUrls: [],
  coin: BraveWallet.CoinType.SOL,
  isEip1559: false
}

export const mockERC20Token: BraveWallet.BlockchainToken = {
  contractAddress: 'mockContractAddress',
  name: 'Dog Coin',
  symbol: 'DOG',
  logo: '',
  isErc20: true,
  isErc721: false,
  isErc1155: false,
  isNft: false,
  isSpam: false,
  decimals: 18,
  visible: true,
  tokenId: '',
  coingeckoId: '',
  coin: BraveWallet.CoinType.ETH,
  chainId: BraveWallet.MAINNET_CHAIN_ID
}

export const mockErc721Token: BraveWallet.BlockchainToken = {
  contractAddress: '0x59468516a8259058bad1ca5f8f4bff190d30e066',
  name: 'Invisible Friends',
  symbol: 'INVSBLE',
  logo: 'https://ipfs.io/ipfs/QmX4nfgA35MiW5APoc4P815hMcH8hAt7edi5H3wXkFm485/2D/2585.gif',
  isErc20: false,
  isErc721: true,
  isErc1155: false,
  isNft: true,
  isSpam: false,
  decimals: 18,
  visible: true,
  tokenId: '0x0a19',
  coingeckoId: '',
  coin: BraveWallet.CoinType.ETH,
  chainId: BraveWallet.MAINNET_CHAIN_ID
}

export const mockNftPinningStatus: NftsPinningStatusType = {
  [getAssetIdKey(mockErc721Token)]: {
    code: BraveWallet.TokenPinStatusCode.STATUS_PINNED,
    error: undefined
  }
}

export const mockAccount: WalletAccountType = {
  name: 'mockAccountName',
  address: 'mockAddress',
  nativeBalanceRegistry: {
    0x1: '123456'
  },
  accountId: {
    coin: BraveWallet.CoinType.ETH,
    keyringId: BraveWallet.KeyringId.kDefault,
    kind: BraveWallet.AccountKind.kDerived,
    address: 'mockAddress',
    uniqueKey: 'mockId',
  },
  tokenBalanceRegistry: {},
  hardware: undefined,
}

export const mockEthAccountInfo: BraveWallet.AccountInfo = {
  hardware: undefined,
  name: 'mockEthAccountName',
  address: 'mockEthAddress',
  accountId: {
    coin: BraveWallet.CoinType.ETH,
    keyringId: BraveWallet.KeyringId.kDefault,
    kind: BraveWallet.AccountKind.kDerived,
    address: 'mockEthAddress',
    uniqueKey: 'mockEthAddress',
  },
}

export const mockSolanaAccount: WalletAccountType = {
  name: 'MockSolanaAccount',
  address: '5sDWP4vCRgDrGsmS1RRuWGRWKo5mhP5wKw8RNqK6zRer',
  nativeBalanceRegistry: {
    [BraveWallet.SOLANA_MAINNET]: '1000000000',
    [BraveWallet.SOLANA_DEVNET]: '1000000000',
    [BraveWallet.SOLANA_TESTNET]: '1000000000'
  },
  accountId: {
    coin: BraveWallet.CoinType.SOL,
    keyringId: BraveWallet.KeyringId.kSolana,
    kind: BraveWallet.AccountKind.kDerived,
    address: '5sDWP4vCRgDrGsmS1RRuWGRWKo5mhP5wKw8RNqK6zRer',
    uniqueKey: 'mockId-2',
  },
  tokenBalanceRegistry: {},
  hardware: undefined,
}

export const mockSolanaAccountInfo: BraveWallet.AccountInfo = {
  name: 'MockSolanaAccount',
  address: '5sDWP4vCRgDrGsmS1RRuWGRWKo5mhP5wKw8RNqK6zRer',
  accountId: {
    coin: BraveWallet.CoinType.SOL,
    keyringId: BraveWallet.KeyringId.kSolana,
    kind: BraveWallet.AccountKind.kDerived,
    address: '5sDWP4vCRgDrGsmS1RRuWGRWKo5mhP5wKw8RNqK6zRer',
    uniqueKey: '5sDWP4vCRgDrGsmS1RRuWGRWKo5mhP5wKw8RNqK6zRer',
  },
  hardware: undefined
}

export const mockFilecoinAccount: WalletAccountType = {
  name: 'MockFilecoinAccount',
  address: 't1alebc2ujfh4kuxs5bvzmx5b2w5ixrqrl3ni5rti',
  nativeBalanceRegistry: {
    [BraveWallet.FILECOIN_MAINNET]: '1000000000'
  },
  accountId: {
    coin: BraveWallet.CoinType.FIL,
    keyringId: BraveWallet.KeyringId.kFilecoinTestnet,
    kind: BraveWallet.AccountKind.kDerived,
    address: 't1alebc2ujfh4kuxs5bvzmx5b2w5ixrqrl3ni5rti',
    uniqueKey: 'mockId-3',
  },
  tokenBalanceRegistry: {},
  hardware: undefined,
}

export const mockFilecoinAccountInfo: BraveWallet.AccountInfo = {
  name: 'MockFilecoinAccount',
  address: 't1alebc2ujfh4kuxs5bvzmx5b2w5ixrqrl3ni5rti',
  accountId: {
    coin: BraveWallet.CoinType.FIL,
    keyringId: BraveWallet.KeyringId.kFilecoinTestnet,
    kind: BraveWallet.AccountKind.kDerived,
    address: 't1alebc2ujfh4kuxs5bvzmx5b2w5ixrqrl3ni5rti',
    uniqueKey: 't1alebc2ujfh4kuxs5bvzmx5b2w5ixrqrl3ni5rti',
  },
  hardware: undefined
}

export const mockSpotPriceRegistry: SpotPriceRegistry = {
  eth: {
    fromAsset: 'ETH',
    price: '4000',
    toAsset: 'mockValue',
    assetTimeframeChange: 'mockValue'
  },
  dog: {
    fromAsset: 'DOG',
    price: '100',
    toAsset: 'mockValue',
    assetTimeframeChange: 'mockValue'
  },
  [getPriceIdForToken(mockBasicAttentionToken)]: {
    fromAsset: mockBasicAttentionToken.symbol,
    price: '0.88',
    toAsset: 'mockValue',
    assetTimeframeChange: 'mockValue'
  }
}

export const mockAddresses: string[] = [
  '0xea674fdde714fd979de3edf0f56aa9716b898ec8',
  '0xdbf41e98f541f19bb044e604d2520f3893eefc79',
  '0xcee177039c99d03a6f74e95bbba2923ceea43ea2'
]

export const mockFilAddresses: string[] = [
  't1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  'f1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  't3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a'
]

export const mockFilInvalilAddresses: string[] = [
  '',
  't1lqarsh4nkg545ilaoqdsbtj4uofplt6sto2ziy',
  'f1lqarsh4nkg545ilaoqdsbtj4uofplt6sto2f6ziy',
  't3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3ju3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'f3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jfpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'a1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  'b1lqarsh4nkg545ilaoqdsbtj4uofplt6sto26ziy',
  'c3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a',
  'd3wv3u6pmfi3j6pf3fhjkch372pkyg2tgtlb3jpu3eo6mnt7ttsft6x2xr54ct7fl2oz4o4tpa4mvigcrayh4a'
]

export const mockAppsList: AppsListType[] = [
  {
    category: 'category1',
    categoryButtonText: 'categoryButtonText1',
    appList: [
      {
        name: 'foo',
        description: 'description1'
      }
    ] as BraveWallet.AppItem[]
  },
  {
    category: 'category2',
    categoryButtonText: 'categoryButtonText2',
    appList: [
      {
        name: 'bar',
        description: 'description2'
      }
    ] as BraveWallet.AppItem[]
  }
]

export const mockSolDappSignTransactionRequest: BraveWallet.SignTransactionRequest = {
  originInfo: {
    origin: {
      scheme: 'https',
      host: 'f40y4d.csb.app',
      port: 443,
      nonceIfOpaque: undefined
    },
    originSpec: 'https://f40y4d.csb.app',
    eTldPlusOne: 'csb.app'
  },
  id: 0,
  fromAddress: mockSolanaAccount.address,
  txData: {
    ethTxData: undefined,
    ethTxData1559: undefined,
    filTxData: undefined,
    solanaTxData: {
      recentBlockhash: 'B7Kg79jDm48LMdB4JB2hu82Yfsuz5xYm2cQDBYmKdDSn',
      lastValidBlockHeight: 0 as unknown as bigint,
      feePayer: mockSolanaAccount.address,
      toWalletAddress: '',
      splTokenMintAddress: '',
      lamports: 0 as unknown as bigint,
      amount: 0 as unknown as bigint,
      txType: 12,
      instructions: [
        {
          programId: '11111111111111111111111111111111',
          accountMetas: [
            {
              pubkey: mockSolanaAccount.address,
              addrTableLookupIndex: undefined,
              isSigner: true,
              isWritable: true
            },
            {
              pubkey: mockSolanaAccount.address,
              addrTableLookupIndex: undefined,
              isSigner: true,
              isWritable: true
            }
          ],
          data: [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0],
          decodedData: undefined
        }
      ],
      version: BraveWallet.SolanaMessageVersion.kLegacy,
      messageHeader: {
        numRequiredSignatures: 1,
        numReadonlySignedAccounts: 0,
        numReadonlyUnsignedAccounts: 1
      },
      staticAccountKeys: [
        mockSolanaAccount.address,
        '11111111111111111111111111111111'
      ],
      addressTableLookups: [],
      sendOptions: undefined,
      signTransactionParam: undefined
    }
  },
  rawMessage: { bytes: [1], str: undefined },
  coin: BraveWallet.CoinType.SOL,
  chainId: BraveWallet.SOLANA_MAINNET
}

// BraveWallet.TransactionInfo (selectedPendingTransaction)
export const mockSolDappSignAndSendTransactionRequest: SerializableTransactionInfo = {
  chainId: '0x67',
  id: 'e1eae32d-5bc2-40ac-85e5-2a4a5fbe8a5f',
  fromAddress: mockSolanaAccount.address,
  txHash: '',
  txDataUnion: {
    ethTxData: undefined,
    ethTxData1559: undefined,
    filTxData: undefined,
    solanaTxData: {
      recentBlockhash: 'C115cyMDVoGGYNd4r8vFy5qPJEUdoJQQCXMYYKQTQimn',
      lastValidBlockHeight: '0',
      feePayer: mockSolanaAccount.address,
      toWalletAddress: '',
      splTokenMintAddress: '',
      lamports: '0',
      amount: '0',
      txType: 11,
      instructions: [
        {
          programId: '11111111111111111111111111111111',
          accountMetas: [
            {
              pubkey: mockSolanaAccount.address,
              addrTableLookupIndex: undefined,
              isSigner: true,
              isWritable: true
            },
            {
              pubkey: mockSolanaAccount.address,
              addrTableLookupIndex: { val: 1 },
              isSigner: true,
              isWritable: true
            }
          ],
          data: [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0],
          decodedData: {
            accountParams: [
              {
                localizedName: 'From',
                name: BraveWallet.FROM_ACCOUNT
              },
              {
                localizedName: 'To',
                name: BraveWallet.TO_ACCOUNT
              }
            ],
            instructionType: BraveWallet.SolanaSystemInstruction.kTransfer,
            params: [
              {
                localizedName: 'lamports',
                name: BraveWallet.LAMPORTS,
                value: '1',
                type: BraveWallet.SolanaInstructionParamType.kUint64
              }
            ]
          }
        }
      ],
      version: BraveWallet.SolanaMessageVersion.kLegacy,
      messageHeader: {
        numRequiredSignatures: 1,
        numReadonlySignedAccounts: 0,
        numReadonlyUnsignedAccounts: 1
      },
      staticAccountKeys: [
        mockSolanaAccount.address,
        '11111111111111111111111111111111'
      ],
      addressTableLookups: [],
      sendOptions: undefined,
      signTransactionParam: undefined
    }
  },
  txStatus: 0,
  txType: 11,
  txParams: [],
  txArgs: [],
  createdTime: { microseconds: 1654540245386000 },
  submittedTime: { microseconds: 0 },
  confirmedTime: { microseconds: 0 },
  originInfo: {
    origin: {
      scheme: 'https',
      host: 'f40y4d.csb.app',
      port: 443,
      nonceIfOpaque: undefined
    },
    originSpec: 'https://f40y4d.csb.app',
    eTldPlusOne: 'csb.app'
  },
  groupId: undefined,
  effectiveRecipient: undefined
}

export const mockSolDappSignAllTransactionsRequest: BraveWallet.SignAllTransactionsRequest = {
  originInfo: {
    origin: {
      scheme: 'https',
      host: 'f40y4d.csb.app',
      port: 443,
      nonceIfOpaque: undefined
    },
    originSpec: 'https://f40y4d.csb.app',
    eTldPlusOne: 'csb.app'
  },
  id: 3,
  fromAddress: mockSolanaAccount.address,
  txDatas: [
    {
      ethTxData: undefined,
      ethTxData1559: undefined,
      filTxData: undefined,
      solanaTxData: {
        recentBlockhash: '8Yq6DGZBh9oEJsCVhUjTqN9kPiLoeYJ7J4n9TnpPYjqW',
        lastValidBlockHeight: 0 as unknown as bigint,
        feePayer: mockSolanaAccount.address,
        toWalletAddress: '',
        splTokenMintAddress: '',
        lamports: 0 as unknown as bigint,
        amount: 0 as unknown as bigint,
        txType: 12,
        instructions: [{
          programId: '11111111111111111111111111111111',
          accountMetas: [
            {
              pubkey: mockSolanaAccount.address,
              addrTableLookupIndex: undefined,
              isSigner: true,
              isWritable: true
            },
            {
              pubkey: mockSolanaAccount.address,
              addrTableLookupIndex: undefined,
              isSigner: true,
              isWritable: true
            }
          ],
          data: [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0],
          decodedData: undefined
        }],
        version: BraveWallet.SolanaMessageVersion.kLegacy,
        messageHeader: {
          numRequiredSignatures: 1,
          numReadonlySignedAccounts: 0,
          numReadonlyUnsignedAccounts: 1
        },
        staticAccountKeys: [
          mockSolanaAccount.address,
          '11111111111111111111111111111111'
        ],
        addressTableLookups: [],
        sendOptions: undefined,
        signTransactionParam: undefined
      }
    },
    {
      ethTxData: undefined,
      ethTxData1559: undefined,
      filTxData: undefined,
      solanaTxData: {
        recentBlockhash: '8Yq6DGZBh9oEJsCVhUjTqN9kPiLoeYJ7J4n9TnpPYjqW',
        lastValidBlockHeight: 0 as unknown as bigint,
        feePayer: mockSolanaAccount.address,
        toWalletAddress: '',
        splTokenMintAddress: '',
        lamports: 0 as unknown as bigint,
        amount: 0 as unknown as bigint,
        txType: 12,
        instructions: [
          {
            programId: '11111111111111111111111111111111',
            accountMetas: [
              {
                pubkey: mockSolanaAccount.address,
                addrTableLookupIndex: undefined,
                isSigner: true,
                isWritable: true
              },
              {
                pubkey: mockSolanaAccount.address,
                addrTableLookupIndex: undefined,
                isSigner: true,
                isWritable: true
              }
            ],
            data: [2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0],
            decodedData: undefined
          }
        ],
        version: BraveWallet.SolanaMessageVersion.kLegacy,
        messageHeader: {
          numRequiredSignatures: 1,
          numReadonlySignedAccounts: 0,
          numReadonlyUnsignedAccounts: 1
        },
        staticAccountKeys: [
          mockSolanaAccount.address,
          '11111111111111111111111111111111'
        ],
        addressTableLookups: [],
        sendOptions: undefined,
        signTransactionParam: undefined
      }
    }
  ],
  rawMessages: [{ bytes: [1], str: undefined }],
  coin: BraveWallet.CoinType.SOL,
  chainId: BraveWallet.SOLANA_MAINNET
}

const emptyEvmStateChangeRawInfoData = {
  erc20TransferData: undefined,
  erc1155ApprovalForAllData: undefined,
  erc1155TransferData: undefined,
  erc20ApprovalData: undefined,
  erc721ApprovalData: undefined,
  erc721ApprovalForAllData: undefined,
  erc721TransferData: undefined,
  nativeAssetTransferData: undefined
}

const mockSimulatedGoerliLink = {
  address: '0x326c977e6efc84e512bb9c30f76e30c160ed06fb',
  symbol: 'LINK',
  name: 'ChainLink Token',
  decimals: 18,
  verified: false,
  lists: [],
  imageUrl: undefined,
  price: undefined
}
const goerliLinkTransferData = {
  contract: {
    // Goerli LINK
    address: '0x326c977e6efc84e512bb9c30f76e30c160ed06fb',
    kind: 'ACCOUNT'
  },
  amount: {
    before: '28907865866843658798',
    after: '14453965866843658798'
  },
  asset: mockSimulatedGoerliLink
}

/**
 * - Send 14.4539 LINK
 * - Receive 14.4539 LINK
 * - Approve 10 LINK
 * - Approve 1 NFT
 * - Send 1 NFT
 * - Send 1 ETH
 */
export const mockEvmSimulatedResponse: BraveWallet.EVMSimulationResponse = {
  action: 'NONE',
  simulationResults: {
    error: undefined,
    expectedStateChanges: [
      {
        humanReadableDiff: 'Send 14.4539 LINK',
        rawInfo: {
          kind: 'ERC20_TRANSFER',
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc20TransferData: goerliLinkTransferData
          }
        }
      },
      {
        humanReadableDiff: 'Receive 14.4539 LINK',
        rawInfo: {
          kind: 'ERC20_TRANSFER',
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc20TransferData: {
              ...goerliLinkTransferData,
              amount: {
                after: goerliLinkTransferData.amount.before,
                before: goerliLinkTransferData.amount.after
              }
            }
          }
        }
      },
      {
        humanReadableDiff: 'Approve 10 LINK',
        rawInfo: {
          kind: '', // TODO
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc20ApprovalData: {
              amount: {
                after: '10000000000000000000',
                before: '1'
              },
              asset: mockSimulatedGoerliLink,
              contract: {
                address: mockSimulatedGoerliLink.address,
                kind: '' // todo
              },
              owner: {
                address: mockAccount.address,
                kind: '' // TODO
              },
              spender: {
                address: mockAccount.address,
                kind: '' // TODO
              }
            }
          }
        }
      },
      {
        humanReadableDiff: `Send ${
          mockErc721Token.name //
        } #${mockErc721Token.tokenId}`,
        rawInfo: {
          kind: 'ERC721_TRANSFER',
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc721TransferData: {
              amount: {
                after: '0',
                before: '1'
              },
              assetPrice: {
                dollarValuePerToken: '100', // $100
                lastUpdatedAt: new Date().toUTCString(),
                source: 'mocked'
              },
              contract: {
                address: mockErc721Token.contractAddress,
                kind: '' // TODO
              },
              metadata: {
                rawImageUrl: mockNFTMetadata[0].imageURL || ''
              },
              name: mockErc721Token.name,
              symbol: mockErc721Token.symbol,
              tokenId: mockErc721Token.tokenId
            }
          }
        }
      },
      {
        humanReadableDiff: `Send ${
          mockErc721Token.symbol //
        } #${mockErc721Token.tokenId}`,
        rawInfo: {
          kind: '', // TODO
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc1155TransferData: {
              amount: {
                after: '0',
                before: '1'
              },
              assetPrice: {
                dollarValuePerToken: '100', // $100
                lastUpdatedAt: new Date().toUTCString(),
                source: 'mocked'
              },
              contract: {
                address: mockErc721Token.contractAddress,
                kind: '' // TODO
              },
              metadata: {
                rawImageUrl: mockNFTMetadata[0].imageURL || ''
              },
              tokenId: mockErc721Token.tokenId
            }
          }
        }
      },
      {
        humanReadableDiff: `Send 1 ETH`,
        rawInfo: {
          kind: '', // TODO
          data: {
            ...emptyEvmStateChangeRawInfoData,
            nativeAssetTransferData: {
              amount: {
                after: '0',
                before: '1000000000000000000'
              },
              asset: {
                address: '',
                decimals: mockEthMainnet.decimals,
                imageUrl: mockEthToken.logo,
                lists: [],
                name: mockEthToken.name,
                price: {
                  dollarValuePerToken: '1600',
                  lastUpdatedAt: new Date().toISOString(),
                  source: 'mocked'
                },
                symbol: mockEthToken.symbol,
                verified: true
              },
              contract: {
                address: '',
                kind: '' // TODO
              }
            }
          }
        }
      },
      {
        humanReadableDiff: `Approve ${
          mockMoonCatNFT.name //
        } #${mockMoonCatNFT.tokenId}`,
        rawInfo: {
          kind: '', // TODO
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc721ApprovalData: {
              amount: {
                after: '0',
                before: '1'
              },
              assetPrice: {
                dollarValuePerToken: '100',
                lastUpdatedAt: new Date().toISOString(),
                source: 'mocked'
              },
              metadata: {
                rawImageUrl: mockMoonCatNFT.logo
              },
              contract: {
                address: '',
                kind: '' // TODO
              },
              name: mockMoonCatNFT.name,
              owner: { kind: 'ACCOUNT', address: mockAccount.address },
              spender: {
                address: mockAccount.address,
                kind: 'ACCOUNT'
              },
              symbol: mockMoonCatNFT.symbol,
              tokenId: mockMoonCatNFT.tokenId
            }
          }
        }
      }
    ]
  },
  warnings: []
}

/**
 * ERC20 Swap ETH For DAI
 * - Send 1 ETH
 * - Receive 1530.81307 DAI
 */
export const mockSimulatedSwapETHForDAI: BraveWallet.EVMSimulationResponse = {
  action: 'NONE',
  warnings: [],
  simulationResults: {
    error: undefined,
    expectedStateChanges: [
      {
        humanReadableDiff: 'Receive 1530.81307 DAI',
        rawInfo: {
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc20TransferData: {
              amount: {
                after: '557039306766411381864245',
                before: '555508493698012633714742'
              },
              contract: {
                address: '0x6b175474e89094c44da98b954eedeac495271d0f',
                kind: 'ACCOUNT'
              },
              asset: {
                address: '0x6b175474e89094c44da98b954eedeac495271d0f',
                symbol: 'DAI',
                name: 'Dai Stablecoin',
                decimals: 18,
                verified: true,
                lists: [
                  'COINGECKO',
                  'ZERION',
                  'ONE_INCH',
                  'UNISWAP',
                  'MY_CRYPTO_API',
                  'KLEROS_TOKENS'
                ],
                imageUrl:
                  'https://d1ts37qlq4uz4s.cloudfront.net/evm__evm%3A%3Aethereum__evm%3A%3Aethereum%3A%3Amainnet__0x6b175474e89094c44da98b954eedeac495271d0f.png',
                price: {
                  source: 'Coingecko',
                  lastUpdatedAt: '1679331222',
                  dollarValuePerToken: '0.99'
                }
              }
            }
          },
          kind: 'ERC20_TRANSFER'
        }
      },
      {
        humanReadableDiff: 'Send 1 ETH',
        rawInfo: {
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc20TransferData: {
              amount: {
                after: '1182957389356504134754',
                before: '1183957389356504134754'
              },
              contract: {
                address: '0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee',
                kind: 'ACCOUNT'
              },
              asset: {
                address: '0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee',
                symbol: 'ETH',
                name: 'Ether',
                decimals: 18,
                verified: true,
                imageUrl:
                  'https://d1ts37qlq4uz4s.cloudfront.net/evm__evm%3A%3Aethereum__evm%3A%3Aethereum%3A%3Amainnet__0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2.png',
                price: {
                  source: 'Coingecko',
                  lastUpdatedAt: '1681958792',
                  dollarValuePerToken: '1945.92'
                },
                lists: []
              }
            }
          },
          kind: 'NATIVE_ASSET_TRANSFER'
        }
      }
    ]
  }
}

/**
 * ERC20 Approval
 * - Approve to transfer up to 1000 USDT
 */
export const mockEvmSimulatedERC20Approval: BraveWallet.EVMSimulationResponse =
  {
    action: 'NONE',
    warnings: [],
    simulationResults: {
      error: undefined,
      expectedStateChanges: [
        {
          humanReadableDiff: 'Approve to transfer up to 1000 USDT',
          rawInfo: {
            data: {
              ...emptyEvmStateChangeRawInfoData,
              erc20ApprovalData: {
                amount: {
                  after: '1000000000',
                  before: '0'
                },
                asset: {
                  address: '0xdac17f958d2ee523a2206206994597c13d831ec7',
                  name: 'Tether USD',
                  decimals: 6,
                  lists: [
                    'COINGECKO',
                    'ZERION',
                    'ONE_INCH',
                    'UNISWAP',
                    'MY_CRYPTO_API',
                    'KLEROS_TOKENS'
                  ],
                  symbol: 'USDT',
                  verified: true,
                  imageUrl:
                    'https://d1ts37qlq4uz4s.cloudfront.net/evm__evm%3A%3Aethereum__evm%3A%3Aethereum%3A%3Amainnet__0xdac17f958d2ee523a2206206994597c13d831ec7.png',
                  price: {
                    source: 'Coingecko',
                    lastUpdatedAt: '1679331222',
                    dollarValuePerToken: '0.99'
                  }
                },
                contract: {
                  address: '0xdac17f958d2ee523a2206206994597c13d831ec7',
                  kind: 'ACCOUNT'
                },
                owner: {
                  address: '0xd8da6bf26964af9d7eed9e03e53415d37aa96045',
                  kind: 'ACCOUNT'
                },
                spender: {
                  address: '0x68b3465833fb72a70ecdf485e0e4c7bd8665fc45',
                  kind: 'ACCOUNT'
                }
              }
            },
            kind: 'ERC20_APPROVAL'
          }
        }
      ]
    }
  }

/**
 * Buy An ERC721 NFT With ETH (Simulated)
 * - Receive PudgyPenguins #7238
 * - Send 3.181 ETH
 */
export const mockSimulatedBuyNFTWithETH: BraveWallet.EVMSimulationResponse = {
  action: 'NONE',
  warnings: [],
  simulationResults: {
    error: undefined,
    expectedStateChanges: [
      {
        humanReadableDiff: 'Receive PudgyPenguins #7238',
        rawInfo: {
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc721TransferData: {
              amount: {
                after: '1',
                before: '0'
              },
              contract: {
                address: '0xbd3531da5cf5857e7cfaa92426877b022e612cf8',
                kind: 'ACCOUNT'
              },
              metadata: {
                rawImageUrl: 'https://cdn.simplehash.com/assets/97e1c9e3e9eb21a1114351f9c5c14fe611c94916f360c4eb3aa9263afd8b837b.png'
              },
              name: 'PudgyPenguins',
              symbol: 'PPG',
              tokenId: '7238',
              assetPrice: {
                source: 'Simplehash',
                lastUpdatedAt: '1679331222',
                dollarValuePerToken: '594.99'
              }
            }
          },
          kind: 'ERC721_TRANSFER'
        }
      },
      {
        humanReadableDiff: 'Send 3.181 ETH',
        rawInfo: {
          data: {
            ...emptyEvmStateChangeRawInfoData,
            nativeAssetTransferData: {
              amount: {
                after: '998426264937289938488',
                before: '1001607264937289938488'
              },
              contract: {
                address: "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
                kind: "ACCOUNT"
              },
              asset: {
                lists: [],
                address: "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
                symbol: "ETH",
                name: "Ether",
                decimals: 18,
                verified: true,
                imageUrl: "https://d1ts37qlq4uz4s.cloudfront.net/evm__evm%3A%3Aethereum__evm%3A%3Aethereum%3A%3Amainnet__0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2.png",
                price: {
                  source: "Coingecko",
                  lastUpdatedAt: '1681958792',
                  dollarValuePerToken: '1945.92'
                }
              }
            }
          },
          kind: 'NATIVE_ASSET_TRANSFER'
        }
      }
    ]
  }
}

/**
 * Simulated ERC721 Approve For All
 * - Approve to transfer all your BoredApeYachtClub
 */
export const mockERC721ApproveForAllSim: BraveWallet.EVMSimulationResponse = {
  action: 'WARN',
  warnings: [
    {
      kind: 'UNLIMITED_ALLOWANCE_TO_NFTS',
      message:
        'You are allowing this website ' +
        'to withdraw funds from your account in the future',
      severity: 'WARNING'
    }
  ],
  simulationResults: {
    error: undefined,
    expectedStateChanges: [
      {
        humanReadableDiff: 'Approve to transfer all your BoredApeYachtClub',
        rawInfo: {
          kind: 'ERC721_APPROVAL_FOR_ALL',
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc721ApprovalForAllData: {
              amount: {
                after:
                  '1157920892373161954235709850086879078532699846656405640394',
                before: '0'
              },
              contract: {
                address: '0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d',
                kind: 'ACCOUNT'
              },
              name: 'BoredApeYachtClub',
              owner: {
                address: '0x38191ca1307ebf67ca1a7caf5346dbd91d882ca6',
                kind: 'ACCOUNT'
              },
              spender: {
                address: '0x1e0049783f008a0085193e00003d00cd54003c71',
                kind: 'ACCOUNT'
              },
              symbol: 'BAYC',
              assetPrice: {
                source: 'Simplehash',
                lastUpdatedAt: '1679331222',
                dollarValuePerToken: '7865.43'
              }
            }
          }
        }
      }
    ]
  }
}

/**
 * ERC721 Approve (Simulated)
 * Approve to transfer BoredApeYachtClub
 */
export const mockSimulatedERC721Approve: BraveWallet.EVMSimulationResponse = {
  action: 'NONE',
  warnings: [],
  simulationResults: {
    error: undefined,
    expectedStateChanges: [
      {
        humanReadableDiff: 'Approve to transfer BoredApeYachtClub',
        rawInfo: {
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc721ApprovalData: {
              amount: {
                after: '1',
                before: '0'
              },
              contract: {
                address: '0xbc4ca0eda7647a8ab7c2061c2e118a18a936f13d',
                kind: 'ACCOUNT'
              },
              metadata: {
                rawImageUrl:
                  'https://cdn.simplehash.com/assets/beca5f0f88c267276318edd8a6019b6b47327f42efd0ba22a3835e77f27732e5.png'
              },
              name: 'BoredApeYachtClub',
              owner: {
                address: '0xed2ab4948ba6a909a7751dec4f34f303eb8c7236',
                kind: 'ACCOUNT'
              },
              spender: {
                address: '0x1e0049783f008a0085193e00003d00cd54003c71',
                kind: 'ACCOUNT'
              },
              symbol: 'BAYC',
              tokenId: '6603',
              assetPrice: {
                source: 'Simplehash',
                lastUpdatedAt: '1679331222',
                dollarValuePerToken: '7865.43'
              }
            }
          },
          kind: 'ERC721_APPROVAL'
        }
      }
    ]
  }
}

/**
 * Buy An ERC1155 Token With ETH
 * - Send 0.033 ETH
 * - Receive Corgi
 */
export const mockSimulatedBuyERC1155Token: BraveWallet.EVMSimulationResponse =
  {
    action: 'NONE',
    warnings: [],
    simulationResults: {
      error: undefined,
      expectedStateChanges: [
        {
          humanReadableDiff: 'Send 0.033 ETH',
          rawInfo: {
            data: {
              ...emptyEvmStateChangeRawInfoData,
              nativeAssetTransferData: {
                amount: {
                  after: "71057321770366572",
                  before: "104057321770366572"
                },
                contract: {
                  address: "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
                  kind: "ACCOUNT"
                },
                asset: {
                  address: "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
                  symbol: "ETH",
                  "name": "Ether",
                  "decimals": 18,
                  "verified": true,
                  "imageUrl": "https://d1ts37qlq4uz4s.cloudfront.net/evm__evm%3A%3Aethereum__evm%3A%3Aethereum%3A%3Amainnet__0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2.png",
                  "price": {
                    "source": "Coingecko",
                    "lastUpdatedAt": '1681958792',
                    "dollarValuePerToken": '1945.92'
                  },
                  lists: []
                }
              }
            },
            kind: 'NATIVE_ASSET_TRANSFER'
          }
        },
        {
          humanReadableDiff: 'Receive Corgi',
          rawInfo: {
            data: {
              ...emptyEvmStateChangeRawInfoData,
              erc1155TransferData: {
                "amount": {
                  "after": "1",
                  "before": "0"
                },
                "contract": {
                  "address": "0x51e613727fdd2e0b91b51c3e5427e9440a7957e4",
                  "kind": "ACCOUNT"
                },
                "metadata": {
                  "rawImageUrl": "https://cdn.simplehash.com/assets/4bedd702e7ea8c4a9d04d83302138fa5b63d0cca0f06df9b87bdb09cff253b88.png"
                },
                "tokenId": "13014975",
                "assetPrice": {
                  "source": "Simplehash",
                  "lastUpdatedAt": '1679331222',
                  "dollarValuePerToken": '232.43'
                }
              }
            },
            kind: 'ERC1155_TRANSFER'
          }
        }
      ]
    }
  }

/**
 * ERC1155 Approve For All (Simulated)
 * - Approve to transfer all your Sandbox's ASSETs
 */
export const mockEvmERC1155ApproveForAll: BraveWallet.EVMSimulationResponse = {
  action: 'NONE',
  warnings: [
    {
      kind: 'UNLIMITED_ALLOWANCE_TO_NFTS',
      message:
        'You are allowing this website to withdraw funds ' +
        'from your account in the future',
      severity: 'WARNING'
    }
  ],
  simulationResults: {
    error: undefined,
    expectedStateChanges: [
      {
        humanReadableDiff: `Approve to transfer all your Sandbox's ASSETs`,
        rawInfo: {
          kind: 'ERC1155_APPROVAL_FOR_ALL',
          data: {
            ...emptyEvmStateChangeRawInfoData,
            erc1155ApprovalForAllData: {
              amount: {
                after:
                  '1157920892373161954235709850086879078532699846656405640394',
                before: '0'
              },
              contract: {
                address: '0xa342f5d851e866e18ff98f351f2c6637f4478db5',
                kind: 'ACCOUNT'
              },
              owner: {
                address: '0xed2ab4948ba6a909a7751dec4f34f303eb8c7236',
                kind: 'ACCOUNT'
              },
              spender: {
                address: '0x00000000006c3852cbef3e08e8df289169ede581',
                kind: 'ACCOUNT'
              },
              assetPrice: {
                source: 'Simplehash',
                lastUpdatedAt: '1679331222',
                dollarValuePerToken: '232.43'
              }
            }
          }
        }
      }
    ]
  }
}

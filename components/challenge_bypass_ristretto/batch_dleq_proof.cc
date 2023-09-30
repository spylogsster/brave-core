/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/batch_dleq_proof.h"

#include <utility>

#include "brave/components/challenge_bypass_ristretto/blinded_token.h"
#include "brave/components/challenge_bypass_ristretto/public_key.h"
#include "brave/components/challenge_bypass_ristretto/signed_token.h"
#include "brave/components/challenge_bypass_ristretto/signing_key.h"
#include "brave/components/challenge_bypass_ristretto/token.h"
#include "brave/components/challenge_bypass_ristretto/unblinded_token.h"

namespace challenge_bypass_ristretto {

BatchDLEQProof::BatchDLEQProof(CxxBatchDLEQProofBox raw)
    : raw_(base::MakeRefCounted<CxxBatchDLEQProofRefData>(std::move(raw))) {}

BatchDLEQProof::BatchDLEQProof(const BatchDLEQProof& other) = default;

BatchDLEQProof& BatchDLEQProof::operator=(const BatchDLEQProof& other) =
    default;

BatchDLEQProof::BatchDLEQProof(BatchDLEQProof&& other) noexcept = default;

BatchDLEQProof& BatchDLEQProof::operator=(BatchDLEQProof&& other) noexcept =
    default;

BatchDLEQProof::~BatchDLEQProof() = default;

// static
base::expected<BatchDLEQProof, std::string> BatchDLEQProof::Create(
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const SigningKey& signing_key) {
  if (blinded_tokens.size() != signed_tokens.size()) {
    return base::unexpected(
        "Blinded tokens and signed tokens must have the same length");
  }

  rust::Vec<cbr_ffi::CxxBlindedTokenRef> raw_blinded_tokens;
  rust::Vec<cbr_ffi::CxxSignedTokenRef> raw_signed_tokens;

  for (size_t i = 0; i < blinded_tokens.size(); i++) {
    raw_blinded_tokens.push_back(
        cbr_ffi::CxxBlindedTokenRef{*blinded_tokens[i].raw()});
    raw_signed_tokens.push_back(
        cbr_ffi::CxxSignedTokenRef{*signed_tokens[i].raw()});
  }

  cbr_ffi::CxxBatchDLEQProofResult raw_proof_result(
      cbr_ffi::batch_dleq_proof_new(raw_blinded_tokens, raw_signed_tokens,
                                    *signing_key.raw()));

  if (!raw_proof_result.value) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return BatchDLEQProof(CxxBatchDLEQProofBox::from_raw(raw_proof_result.value));
}

base::expected<bool, std::string> BatchDLEQProof::Verify(
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const PublicKey& public_key) {
  if (blinded_tokens.size() != signed_tokens.size()) {
    return base::unexpected(
        "Blinded tokens and signed tokens must have the same length");
  }

  rust::Vec<cbr_ffi::CxxBlindedTokenRef> raw_blinded_tokens;
  rust::Vec<cbr_ffi::CxxSignedTokenRef> raw_signed_tokens;

  for (size_t i = 0; i < blinded_tokens.size(); i++) {
    raw_blinded_tokens.push_back(
        cbr_ffi::CxxBlindedTokenRef{*blinded_tokens[i].raw()});
    raw_signed_tokens.push_back(
        cbr_ffi::CxxSignedTokenRef{*signed_tokens[i].raw()});
  }

  int result = raw()->batch_dleq_proof_invalid(
      raw_blinded_tokens, raw_signed_tokens, *public_key.raw());
  if (result < 0) {
    return base::unexpected("Could not verify DLEQ proof");
  }
  return result == 0;
}

base::expected<std::vector<UnblindedToken>, std::string>
BatchDLEQProof::VerifyAndUnblind(
    const std::vector<Token>& tokens,
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const PublicKey& public_key) {
  if (tokens.size() != blinded_tokens.size() ||
      tokens.size() != signed_tokens.size()) {
    return base::unexpected(
        "Tokens, blinded tokens and signed tokens must have the same length");
  }

  rust::Vec<cbr_ffi::CxxTokenRef> raw_tokens;
  rust::Vec<cbr_ffi::CxxBlindedTokenRef> raw_blinded_tokens;
  rust::Vec<cbr_ffi::CxxSignedTokenRef> raw_signed_tokens;

  for (size_t i = 0; i < blinded_tokens.size(); i++) {
    raw_tokens.push_back(cbr_ffi::CxxTokenRef{*tokens[i].raw()});
    raw_blinded_tokens.push_back(
        cbr_ffi::CxxBlindedTokenRef{*blinded_tokens[i].raw()});
    raw_signed_tokens.push_back(
        cbr_ffi::CxxSignedTokenRef{*signed_tokens[i].raw()});
  }

  cbr_ffi::CxxUnblindedTokensResult raw_unblinded_tokens_result =
      raw()->batch_dleq_proof_verify_and_unblind(
          raw_tokens, raw_blinded_tokens, raw_signed_tokens, *public_key.raw());

  if (!raw_unblinded_tokens_result.success) {
    return base::unexpected(
        static_cast<std::string>(raw_unblinded_tokens_result.error_message));
  }

  std::vector<UnblindedToken> unblinded_tokens;
  for (auto& raw_unblinded_token : raw_unblinded_tokens_result.value) {
    CHECK(raw_unblinded_token.value);
    unblinded_tokens.emplace_back(
        rust::Box<cbr_ffi::CxxUnblindedToken>::from_raw(
            raw_unblinded_token.value));
  }

  return unblinded_tokens;
}

base::expected<BatchDLEQProof, std::string> BatchDLEQProof::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxBatchDLEQProofResult raw_proof_result(
      cbr_ffi::batch_dleq_proof_decode_base64(encoded));

  if (!raw_proof_result.value) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return BatchDLEQProof(
      rust::Box<cbr_ffi::CxxBatchDLEQProof>::from_raw(raw_proof_result.value));
}

base::expected<std::string, std::string> BatchDLEQProof::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool BatchDLEQProof::operator==(const BatchDLEQProof& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool BatchDLEQProof::operator!=(const BatchDLEQProof& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

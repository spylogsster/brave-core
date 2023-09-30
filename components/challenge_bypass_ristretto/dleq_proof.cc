/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/dleq_proof.h"

#include <utility>

#include "brave/components/challenge_bypass_ristretto/blinded_token.h"
#include "brave/components/challenge_bypass_ristretto/public_key.h"
#include "brave/components/challenge_bypass_ristretto/signed_token.h"
#include "brave/components/challenge_bypass_ristretto/signing_key.h"

namespace challenge_bypass_ristretto {

DLEQProof::DLEQProof(CxxDLEQProofBox raw)
    : raw_(base::MakeRefCounted<CxxDLEQProofRefData>(std::move(raw))) {}

DLEQProof::DLEQProof(const DLEQProof& other) = default;

DLEQProof& DLEQProof::operator=(const DLEQProof& other) = default;

DLEQProof::DLEQProof(DLEQProof&& other) noexcept = default;

DLEQProof& DLEQProof::operator=(DLEQProof&& other) noexcept = default;

DLEQProof::~DLEQProof() = default;

// static
base::expected<DLEQProof, std::string> DLEQProof::Create(
    const BlindedToken& blinded_token,
    const SignedToken& signed_token,
    const SigningKey& signing_key) {
  cbr_ffi::CxxDLEQProofResult raw_proof_result(cbr_ffi::dleq_proof_new(
      *blinded_token.raw(), *signed_token.raw(), *signing_key.raw()));

  if (!raw_proof_result.value) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return DLEQProof(CxxDLEQProofBox::from_raw(raw_proof_result.value));
}

base::expected<bool, std::string> DLEQProof::Verify(
    const BlindedToken& blinded_token,
    const SignedToken& signed_token,
    const PublicKey& public_key) {
  const int8_t result = raw()->dleq_proof_invalid(
      *blinded_token.raw(), *signed_token.raw(), *public_key.raw());
  if (result < 0) {
    return base::unexpected("Failed to verify DLEQ proof");
  }
  return result == 0;
}

base::expected<DLEQProof, std::string> DLEQProof::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxDLEQProofResult raw_proof_result(
      cbr_ffi::dleq_proof_decode_base64(encoded));

  if (!raw_proof_result.value) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return DLEQProof(CxxDLEQProofBox::from_raw(raw_proof_result.value));
}

base::expected<std::string, std::string> DLEQProof::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool DLEQProof::operator==(const DLEQProof& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool DLEQProof::operator!=(const DLEQProof& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

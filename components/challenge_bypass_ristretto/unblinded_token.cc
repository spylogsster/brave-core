/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/unblinded_token.h"

#include <utility>

#include "brave/components/challenge_bypass_ristretto/token_preimage.h"
#include "brave/components/challenge_bypass_ristretto/verification_key.h"

namespace challenge_bypass_ristretto {

UnblindedToken::UnblindedToken(CxxUnblindedTokenBox raw)
    : raw_(base::MakeRefCounted<CxxUnblindedTokenRefData>(std::move(raw))) {}

UnblindedToken::UnblindedToken(const UnblindedToken& other) = default;

UnblindedToken& UnblindedToken::operator=(const UnblindedToken& other) =
    default;

UnblindedToken::UnblindedToken(UnblindedToken&& other) noexcept = default;

UnblindedToken& UnblindedToken::operator=(UnblindedToken&& other) noexcept =
    default;

UnblindedToken::~UnblindedToken() = default;

VerificationKey UnblindedToken::DeriveVerificationKey() const {
  return VerificationKey(rust::Box<cbr_ffi::CxxVerificationKey>(
      raw()->unblinded_token_derive_verification_key_sha512()));
}

TokenPreimage UnblindedToken::Preimage() const {
  return TokenPreimage(raw()->unblinded_token_preimage());
}

// static
base::expected<UnblindedToken, std::string> UnblindedToken::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxUnblindedTokenResult raw_unblinded_token_result(
      cbr_ffi::unblinded_token_decode_base64(encoded));

  if (!raw_unblinded_token_result.value) {
    return base::unexpected(raw_unblinded_token_result.error_message.c_str());
  }

  return UnblindedToken(
      CxxUnblindedTokenBox::from_raw(raw_unblinded_token_result.value));
}

base::expected<std::string, std::string> UnblindedToken::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool UnblindedToken::operator==(const UnblindedToken& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool UnblindedToken::operator!=(const UnblindedToken& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

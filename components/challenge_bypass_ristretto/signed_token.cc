/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/signed_token.h"

#include <utility>

namespace challenge_bypass_ristretto {

SignedToken::SignedToken(CxxSignedTokenBox raw)
    : raw_(base::MakeRefCounted<CxxSignedTokenRefData>(std::move(raw))) {}

SignedToken::SignedToken(const SignedToken&) = default;

SignedToken& SignedToken::operator=(const SignedToken&) = default;

SignedToken::SignedToken(SignedToken&&) noexcept = default;

SignedToken& SignedToken::operator=(SignedToken&&) noexcept = default;

SignedToken::~SignedToken() = default;

base::expected<SignedToken, std::string> SignedToken::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxSignedTokenResult raw_signed_token_result(
      cbr_ffi::signed_token_decode_base64(encoded));

  if (!raw_signed_token_result.value) {
    return base::unexpected(raw_signed_token_result.error_message.c_str());
  }

  return SignedToken(
      CxxSignedTokenBox::from_raw(raw_signed_token_result.value));
}

base::expected<std::string, std::string> SignedToken::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool SignedToken::operator==(const SignedToken& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool SignedToken::operator!=(const SignedToken& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

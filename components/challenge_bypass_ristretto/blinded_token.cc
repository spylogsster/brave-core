/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/blinded_token.h"

#include <utility>

namespace challenge_bypass_ristretto {

BlindedToken::BlindedToken(CxxBlindedTokenBox raw)
    : raw_(base::MakeRefCounted<CxxBlindedTokenRefData>(std::move(raw))) {}

BlindedToken::BlindedToken(const BlindedToken& other) = default;

BlindedToken& BlindedToken::operator=(const BlindedToken& other) = default;

BlindedToken::BlindedToken(BlindedToken&& other) noexcept = default;

BlindedToken& BlindedToken::operator=(BlindedToken&& other) noexcept = default;

BlindedToken::~BlindedToken() = default;

base::expected<BlindedToken, std::string> BlindedToken::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxBlindedTokenResult raw_blinded_token_result(
      cbr_ffi::blinded_token_decode_base64(encoded));

  if (!raw_blinded_token_result.value) {
    return base::unexpected(raw_blinded_token_result.error_message.c_str());
  }

  return BlindedToken(
      CxxBlindedTokenBox::from_raw(raw_blinded_token_result.value));
}

base::expected<std::string, std::string> BlindedToken::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool BlindedToken::operator==(const BlindedToken& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool BlindedToken::operator!=(const BlindedToken& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

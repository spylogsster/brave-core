/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/token_preimage.h"

#include <utility>

namespace challenge_bypass_ristretto {

TokenPreimage::TokenPreimage(CxxTokenPreimageBox raw)
    : raw_(base::MakeRefCounted<CxxTokenPreimageRefData>(std::move(raw))) {}

TokenPreimage::TokenPreimage(const TokenPreimage&) = default;

TokenPreimage& TokenPreimage::operator=(const TokenPreimage&) = default;

TokenPreimage::TokenPreimage(TokenPreimage&&) noexcept = default;

TokenPreimage& TokenPreimage::operator=(TokenPreimage&&) noexcept = default;

TokenPreimage::~TokenPreimage() = default;

// static
base::expected<TokenPreimage, std::string> TokenPreimage::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxTokenPreimageResult raw_token_preimage_result(
      cbr_ffi::token_preimage_decode_base64(encoded));

  if (!raw_token_preimage_result.value) {
    return base::unexpected(
        static_cast<std::string>(raw_token_preimage_result.error_message));
  }

  return TokenPreimage(
      CxxTokenPreimageBox::from_raw(raw_token_preimage_result.value));
}

base::expected<std::string, std::string> TokenPreimage::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool TokenPreimage::operator==(const TokenPreimage& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool TokenPreimage::operator!=(const TokenPreimage& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

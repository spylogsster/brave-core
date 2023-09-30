/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/token.h"

#include <utility>

#include "brave/components/challenge_bypass_ristretto/blinded_token.h"

namespace challenge_bypass_ristretto {

Token::Token(CxxTokenBox raw)
    : raw_(base::MakeRefCounted<CxxTokenRefData>(std::move(raw))) {}

Token::Token(const Token& other) = default;

Token& Token::operator=(const Token& other) = default;

Token::Token(Token&& other) noexcept = default;

Token& Token::operator=(Token&& other) noexcept = default;

Token::~Token() = default;

// static
base::expected<Token, std::string> Token::Random() {
  CxxTokenBox raw_token(cbr_ffi::token_random());
  return Token(std::move(raw_token));
}

base::expected<BlindedToken, std::string> Token::Blind() {
  rust::Box<cbr_ffi::CxxBlindedToken> raw_blinded_token(raw()->token_blind());
  return BlindedToken(std::move(raw_blinded_token));
}

// static
base::expected<Token, std::string> Token::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxTokenResult raw_token_result(
      cbr_ffi::token_decode_base64(encoded));

  if (!raw_token_result.value) {
    return base::unexpected(raw_token_result.error_message.c_str());
  }

  return Token(CxxTokenBox::from_raw(raw_token_result.value));
}

base::expected<std::string, std::string> Token::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool Token::operator==(const Token& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool Token::operator!=(const Token& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_TOKEN_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_TOKEN_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/components/challenge_bypass_ristretto/blinded_token.h"
#include "brave/components/challenge_bypass_ristretto/export.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

namespace challenge_bypass_ristretto {

class BlindedToken;

class CHALLENGE_BYPASS_RISTRETTO_EXPORT Token {
  using CxxTokenBox = rust::Box<cbr_ffi::CxxToken>;
  using CxxTokenRefData = base::RefCountedData<CxxTokenBox>;

 public:
  explicit Token(CxxTokenBox raw);
  Token(const Token&);
  Token& operator=(const Token&);
  Token(Token&&) noexcept;
  Token& operator=(Token&&) noexcept;
  ~Token();

  const CxxTokenBox& raw() const { return raw_->data; }

  static base::expected<Token, std::string> Random();
  base::expected<BlindedToken, std::string> Blind();
  static base::expected<Token, std::string> DecodeBase64(
      const std::string& encoded);
  base::expected<std::string, std::string> EncodeBase64() const;

  bool operator==(const Token& rhs) const;
  bool operator!=(const Token& rhs) const;

 private:
  scoped_refptr<CxxTokenRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_TOKEN_H_

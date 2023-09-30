/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/components/challenge_bypass_ristretto/export.h"
#include "brave/components/challenge_bypass_ristretto/token_preimage.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

namespace challenge_bypass_ristretto {

class TokenPreimage;
class VerificationKey;

class CHALLENGE_BYPASS_RISTRETTO_EXPORT UnblindedToken {
  using CxxUnblindedTokenBox = rust::Box<cbr_ffi::CxxUnblindedToken>;
  using CxxUnblindedTokenRefData = base::RefCountedData<CxxUnblindedTokenBox>;

 public:
  explicit UnblindedToken(rust::Box<cbr_ffi::CxxUnblindedToken>);
  UnblindedToken(const UnblindedToken&);
  UnblindedToken& operator=(const UnblindedToken&);
  UnblindedToken(UnblindedToken&&) noexcept;
  UnblindedToken& operator=(UnblindedToken&&) noexcept;
  ~UnblindedToken();

  const CxxUnblindedTokenBox& raw() const { return raw_->data; }

  VerificationKey DeriveVerificationKey() const;
  TokenPreimage Preimage() const;
  static base::expected<UnblindedToken, std::string> DecodeBase64(
      const std::string& encoded);
  base::expected<std::string, std::string> EncodeBase64() const;

  bool operator==(const UnblindedToken& rhs) const;
  bool operator!=(const UnblindedToken& rhs) const;

 private:
  scoped_refptr<CxxUnblindedTokenRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_H_

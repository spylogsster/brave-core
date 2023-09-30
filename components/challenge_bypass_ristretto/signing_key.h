/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_SIGNING_KEY_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_SIGNING_KEY_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/components/challenge_bypass_ristretto/export.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

namespace challenge_bypass_ristretto {

class BlindedToken;
class PublicKey;
class SignedToken;
class TokenPreimage;
class UnblindedToken;

class CHALLENGE_BYPASS_RISTRETTO_EXPORT SigningKey {
  using CxxSigningKeyBox = rust::Box<cbr_ffi::CxxSigningKey>;
  using CxxSigningKeyRefData = base::RefCountedData<CxxSigningKeyBox>;

 public:
  explicit SigningKey(CxxSigningKeyBox raw);
  SigningKey(const SigningKey&);
  SigningKey& operator=(const SigningKey&);
  SigningKey(SigningKey&&) noexcept;
  SigningKey& operator=(SigningKey&&) noexcept;
  ~SigningKey();

  const CxxSigningKeyBox& raw() const { return raw_->data; }

  static base::expected<SigningKey, std::string> Random();
  base::expected<SignedToken, std::string> Sign(
      const BlindedToken& blinded_token) const;
  UnblindedToken RederiveUnblindedToken(const TokenPreimage& t);
  PublicKey GetPublicKey();
  static base::expected<SigningKey, std::string> DecodeBase64(
      const std::string& encoded);
  base::expected<std::string, std::string> EncodeBase64() const;

  bool operator==(const SigningKey& rhs) const;
  bool operator!=(const SigningKey& rhs) const;

 private:
  scoped_refptr<CxxSigningKeyRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_SIGNING_KEY_H_

/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_SIGNED_TOKEN_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_SIGNED_TOKEN_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/components/challenge_bypass_ristretto/export.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

namespace challenge_bypass_ristretto {

class CHALLENGE_BYPASS_RISTRETTO_EXPORT SignedToken {
  using CxxSignedTokenBox = rust::Box<cbr_ffi::CxxSignedToken>;
  using CxxSignedTokenRefData = base::RefCountedData<CxxSignedTokenBox>;

 public:
  explicit SignedToken(CxxSignedTokenBox raw);
  SignedToken(const SignedToken&);
  SignedToken& operator=(const SignedToken&);
  SignedToken(SignedToken&&) noexcept;
  SignedToken& operator=(SignedToken&&) noexcept;
  ~SignedToken();

  const CxxSignedTokenBox& raw() const { return raw_->data; }

  static base::expected<SignedToken, std::string> DecodeBase64(
      const std::string& encoded);
  base::expected<std::string, std::string> EncodeBase64() const;

  bool operator==(const SignedToken& rhs) const;
  bool operator!=(const SignedToken& rhs) const;

 private:
  scoped_refptr<CxxSignedTokenRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_SIGNED_TOKEN_H_

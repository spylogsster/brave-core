/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_SIGNATURE_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_SIGNATURE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/components/challenge_bypass_ristretto/export.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

namespace challenge_bypass_ristretto {

class CHALLENGE_BYPASS_RISTRETTO_EXPORT VerificationSignature {
  using CxxVerificationSignatureBox =
      rust::Box<cbr_ffi::CxxVerificationSignature>;
  using CxxVerificationSignatureRefData =
      base::RefCountedData<CxxVerificationSignatureBox>;

 public:
  explicit VerificationSignature(CxxVerificationSignatureBox raw);
  VerificationSignature(const VerificationSignature&);
  VerificationSignature& operator=(const VerificationSignature&);
  VerificationSignature(VerificationSignature&&) noexcept;
  VerificationSignature& operator=(VerificationSignature&&) noexcept;
  ~VerificationSignature();

  const CxxVerificationSignatureBox& raw() const { return raw_->data; }

  static base::expected<VerificationSignature, std::string> DecodeBase64(
      const std::string& encoded);
  base::expected<std::string, std::string> EncodeBase64() const;

 private:
  scoped_refptr<CxxVerificationSignatureRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_SIGNATURE_H_

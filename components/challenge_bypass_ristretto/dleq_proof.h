/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_DLEQ_PROOF_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_DLEQ_PROOF_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/components/challenge_bypass_ristretto/export.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

namespace challenge_bypass_ristretto {

class BlindedToken;
class SignedToken;
class SigningKey;
class PublicKey;

class CHALLENGE_BYPASS_RISTRETTO_EXPORT DLEQProof {
  using CxxDLEQProofBox = rust::Box<cbr_ffi::CxxDLEQProof>;
  using CxxDLEQProofRefData = base::RefCountedData<CxxDLEQProofBox>;

 public:
  explicit DLEQProof(CxxDLEQProofBox raw);
  DLEQProof(const DLEQProof&);
  DLEQProof& operator=(const DLEQProof&);
  DLEQProof(DLEQProof&&) noexcept;
  DLEQProof& operator=(DLEQProof&&) noexcept;
  ~DLEQProof();

  const CxxDLEQProofBox& raw() const { return raw_->data; }

  base::expected<bool, std::string> Verify(const BlindedToken& blinded_token,
                                           const SignedToken& signed_token,
                                           const PublicKey& public_key);
  static base::expected<DLEQProof, std::string> DecodeBase64(
      const std::string& encoded);
  base::expected<std::string, std::string> EncodeBase64() const;

  bool operator==(const DLEQProof& rhs) const;
  bool operator!=(const DLEQProof& rhs) const;

  static base::expected<DLEQProof, std::string> Create(
      const BlindedToken& blinded_token,
      const SignedToken& signed_token,
      const SigningKey& signing_key);

 private:
  scoped_refptr<CxxDLEQProofRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_DLEQ_PROOF_H_

/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/verification_signature.h"

#include <utility>

namespace challenge_bypass_ristretto {

VerificationSignature::VerificationSignature(CxxVerificationSignatureBox raw)
    : raw_(base::MakeRefCounted<CxxVerificationSignatureRefData>(
          std::move(raw))) {}

VerificationSignature::VerificationSignature(
    const VerificationSignature& other) = default;

VerificationSignature& VerificationSignature::operator=(
    const VerificationSignature& other) = default;

VerificationSignature::VerificationSignature(
    VerificationSignature&& other) noexcept = default;

VerificationSignature& VerificationSignature::operator=(
    VerificationSignature&& other) noexcept = default;

VerificationSignature::~VerificationSignature() = default;

// static
base::expected<VerificationSignature, std::string>
VerificationSignature::DecodeBase64(const std::string& encoded) {
  cbr_ffi::CxxVerificationSignatureResult raw_signature_result(
      cbr_ffi::verification_signature_decode_base64(encoded));

  if (!raw_signature_result.value) {
    return base::unexpected(raw_signature_result.error_message.c_str());
  }

  return VerificationSignature(
      CxxVerificationSignatureBox::from_raw(raw_signature_result.value));
}

base::expected<std::string, std::string> VerificationSignature::EncodeBase64()
    const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

}  // namespace challenge_bypass_ristretto

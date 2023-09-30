/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/verification_key.h"

#include <utility>

#include "brave/components/challenge_bypass_ristretto/verification_signature.h"

namespace challenge_bypass_ristretto {

VerificationKey::VerificationKey(CxxVerificationKeyBox raw)
    : raw_(base::MakeRefCounted<CxxVerificationKeyRefData>(std::move(raw))) {}

VerificationKey::VerificationKey(const VerificationKey& other) = default;

VerificationKey& VerificationKey::operator=(const VerificationKey& other) =
    default;

VerificationKey::VerificationKey(VerificationKey&& other) noexcept = default;

VerificationKey& VerificationKey::operator=(VerificationKey&& other) noexcept =
    default;

VerificationKey::~VerificationKey() = default;

base::expected<VerificationSignature, std::string> VerificationKey::Sign(
    const std::string& message) {
  cbr_ffi::CxxVerificationSignatureResult raw_signature_result(
      raw()->verification_key_sign_sha512(message));

  if (!raw_signature_result.value) {
    return base::unexpected(raw_signature_result.error_message.c_str());
  }

  return VerificationSignature(
      rust::Box<cbr_ffi::CxxVerificationSignature>::from_raw(
          raw_signature_result.value));
}

base::expected<bool, std::string> VerificationKey::Verify(
    const VerificationSignature& sig,
    const std::string& message) {
  int result = raw()->verification_key_invalid_sha512(*sig.raw(), message);
  if (result < 0) {
    base::unexpected("Failed to verify message signature");
  }
  return result == 0;
}

}  // namespace challenge_bypass_ristretto

/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/signing_key.h"

#include <utility>

#include "brave/components/challenge_bypass_ristretto/blinded_token.h"
#include "brave/components/challenge_bypass_ristretto/public_key.h"
#include "brave/components/challenge_bypass_ristretto/signed_token.h"
#include "brave/components/challenge_bypass_ristretto/unblinded_token.h"

namespace challenge_bypass_ristretto {

SigningKey::SigningKey(CxxSigningKeyBox raw)
    : raw_(base::MakeRefCounted<CxxSigningKeyRefData>(std::move(raw))) {}

SigningKey::SigningKey(const SigningKey& other) = default;

SigningKey& SigningKey::operator=(const SigningKey& other) = default;

SigningKey::SigningKey(SigningKey&& other) noexcept = default;

SigningKey& SigningKey::operator=(SigningKey&& other) noexcept = default;

SigningKey::~SigningKey() = default;

// static
base::expected<SigningKey, std::string> SigningKey::Random() {
  rust::Box<cbr_ffi::CxxSigningKey> raw_key(cbr_ffi::signing_key_random());
  return SigningKey(std::move(raw_key));
}

base::expected<SignedToken, std::string> SigningKey::Sign(
    const BlindedToken& blinded_token) const {
  cbr_ffi::CxxSignedTokenResult raw_signed_key_result(
      raw()->signing_key_sign(*blinded_token.raw()));

  if (!raw_signed_key_result.value) {
    return base::unexpected(raw_signed_key_result.error_message.c_str());
  }

  return SignedToken(rust::Box<cbr_ffi::CxxSignedToken>::from_raw(
      raw_signed_key_result.value));
}

UnblindedToken SigningKey::RederiveUnblindedToken(const TokenPreimage& t) {
  return UnblindedToken(rust::Box<cbr_ffi::CxxUnblindedToken>(
      raw()->signing_key_rederive_unblinded_token(*t.raw())));
}

PublicKey SigningKey::GetPublicKey() {
  return PublicKey(
      rust::Box<cbr_ffi::CxxPublicKey>(raw()->signing_key_get_public_key()));
}

base::expected<SigningKey, std::string> SigningKey::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxSigningKeyResult raw_signing_key_result(
      cbr_ffi::signing_key_decode_base64(encoded));

  if (!raw_signing_key_result.value) {
    return base::unexpected(raw_signing_key_result.error_message.c_str());
  }

  return SigningKey(CxxSigningKeyBox::from_raw(raw_signing_key_result.value));
}

base::expected<std::string, std::string> SigningKey::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool SigningKey::operator==(const SigningKey& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool SigningKey::operator!=(const SigningKey& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

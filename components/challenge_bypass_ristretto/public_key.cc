/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/public_key.h"

#include <utility>

namespace challenge_bypass_ristretto {

PublicKey::PublicKey(CxxPublicKeyBox raw)
    : raw_(base::MakeRefCounted<CxxPublicKeyRefData>(std::move(raw))) {}

PublicKey::PublicKey(const PublicKey& other) = default;

PublicKey& PublicKey::operator=(const PublicKey& other) = default;

PublicKey::PublicKey(PublicKey&& other) noexcept = default;

PublicKey& PublicKey::operator=(PublicKey&& other) noexcept = default;

PublicKey::~PublicKey() = default;

base::expected<PublicKey, std::string> PublicKey::DecodeBase64(
    const std::string& encoded) {
  cbr_ffi::CxxPublicKeyResult raw_public_key_result(
      cbr_ffi::public_key_decode_base64(encoded));

  if (!raw_public_key_result.value) {
    return base::unexpected(raw_public_key_result.error_message.c_str());
  }

  return PublicKey(CxxPublicKeyBox::from_raw(raw_public_key_result.value));
}

base::expected<std::string, std::string> PublicKey::EncodeBase64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool PublicKey::operator==(const PublicKey& rhs) const {
  return EncodeBase64() == rhs.EncodeBase64();
}

bool PublicKey::operator!=(const PublicKey& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/challenge_bypass_ristretto/wrapper.h"

#include <utility>

namespace challenge_bypass_ristretto {

TokenPreimage::TokenPreimage(CxxTokenPreimageBox raw)
    : raw_(base::MakeRefCounted<CxxTokenPreimageRefData>(std::move(raw))) {}

TokenPreimage::TokenPreimage(const TokenPreimage&) = default;

TokenPreimage& TokenPreimage::operator=(const TokenPreimage&) = default;

TokenPreimage::TokenPreimage(TokenPreimage&&) noexcept = default;

TokenPreimage& TokenPreimage::operator=(TokenPreimage&&) noexcept = default;

TokenPreimage::~TokenPreimage() = default;

// static
base::expected<TokenPreimage, std::string> TokenPreimage::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxTokenPreimageResult raw_preimage_result(
      cbr_ffi::token_preimage_decode_base64(encoded));

  if (!raw_preimage_result.result) {
    return base::unexpected(
        static_cast<std::string>(raw_preimage_result.error_message));
  }

  return TokenPreimage(
      CxxTokenPreimageBox::from_raw(raw_preimage_result.result));
}

base::expected<std::string, std::string> TokenPreimage::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool TokenPreimage::operator==(const TokenPreimage& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool TokenPreimage::operator!=(const TokenPreimage& rhs) const {
  return !(*this == rhs);
}

BlindedToken::BlindedToken(CxxBlindedTokenBox raw)
    : raw_(base::MakeRefCounted<CxxBlindedTokenRefData>(std::move(raw))) {}

BlindedToken::BlindedToken(const BlindedToken& other) = default;

BlindedToken& BlindedToken::operator=(const BlindedToken& other) = default;

BlindedToken::BlindedToken(BlindedToken&& other) noexcept = default;

BlindedToken& BlindedToken::operator=(BlindedToken&& other) noexcept = default;

BlindedToken::~BlindedToken() = default;

base::expected<BlindedToken, std::string> BlindedToken::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxBlindedTokenResult raw_blinded_result(
      cbr_ffi::blinded_token_decode_base64(encoded));

  if (!raw_blinded_result.result) {
    return base::unexpected(raw_blinded_result.error_message.c_str());
  }

  return BlindedToken(CxxBlindedTokenBox::from_raw(raw_blinded_result.result));
}

base::expected<std::string, std::string> BlindedToken::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool BlindedToken::operator==(const BlindedToken& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool BlindedToken::operator!=(const BlindedToken& rhs) const {
  return !(*this == rhs);
}

Token::Token(CxxTokenBox raw)
    : raw_(base::MakeRefCounted<CxxTokenRefData>(std::move(raw))) {}

Token::Token(const Token& other) = default;

Token& Token::operator=(const Token& other) = default;

Token::Token(Token&& other) noexcept = default;

Token& Token::operator=(Token&& other) noexcept = default;

Token::~Token() = default;

// static
base::expected<Token, std::string> Token::random() {
  CxxTokenBox raw_token(cbr_ffi::token_random());
  return Token(std::move(raw_token));
}

base::expected<BlindedToken, std::string> Token::blind() {
  rust::Box<cbr_ffi::CxxBlindedToken> raw_blinded(raw()->token_blind());
  return BlindedToken(std::move(raw_blinded));
}

// static
base::expected<Token, std::string> Token::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxTokenResult raw_token_result(
      cbr_ffi::token_decode_base64(encoded));

  if (!raw_token_result.result) {
    return base::unexpected(raw_token_result.error_message.c_str());
  }

  return Token(CxxTokenBox::from_raw(raw_token_result.result));
}

base::expected<std::string, std::string> Token::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool Token::operator==(const Token& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool Token::operator!=(const Token& rhs) const {
  return !(*this == rhs);
}

SignedToken::SignedToken(CxxSignedTokenBox raw)
    : raw_(base::MakeRefCounted<CxxSignedTokenRefData>(std::move(raw))) {}

SignedToken::SignedToken(const SignedToken&) = default;

SignedToken& SignedToken::operator=(const SignedToken&) = default;

SignedToken::SignedToken(SignedToken&&) noexcept = default;

SignedToken& SignedToken::operator=(SignedToken&&) noexcept = default;

SignedToken::~SignedToken() = default;

base::expected<SignedToken, std::string> SignedToken::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxSignedTokenResult raw_signed_token_result(
      cbr_ffi::signed_token_decode_base64(encoded));

  if (!raw_signed_token_result.result) {
    return base::unexpected(raw_signed_token_result.error_message.c_str());
  }

  return SignedToken(
      CxxSignedTokenBox::from_raw(raw_signed_token_result.result));
}

base::expected<std::string, std::string> SignedToken::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool SignedToken::operator==(const SignedToken& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool SignedToken::operator!=(const SignedToken& rhs) const {
  return !(*this == rhs);
}

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
VerificationSignature::decode_base64(const std::string& encoded) {
  cbr_ffi::CxxVerificationSignatureResult raw_sig_result(
      cbr_ffi::verification_signature_decode_base64(encoded));

  if (!raw_sig_result.result) {
    return base::unexpected(raw_sig_result.error_message.c_str());
  }

  return VerificationSignature(
      CxxVerificationSignatureBox::from_raw(raw_sig_result.result));
}

base::expected<std::string, std::string> VerificationSignature::encode_base64()
    const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

VerificationKey::VerificationKey(CxxVerificationKeyBox raw)
    : raw_(base::MakeRefCounted<CxxVerificationKeyRefData>(std::move(raw))) {}

VerificationKey::VerificationKey(const VerificationKey& other) = default;

VerificationKey& VerificationKey::operator=(const VerificationKey& other) =
    default;

VerificationKey::VerificationKey(VerificationKey&& other) noexcept = default;

VerificationKey& VerificationKey::operator=(VerificationKey&& other) noexcept =
    default;

VerificationKey::~VerificationKey() = default;

base::expected<VerificationSignature, std::string> VerificationKey::sign(
    const std::string& message) {
  cbr_ffi::CxxVerificationSignatureResult raw_sig_result(
      raw()->verification_key_sign_sha512(message));

  if (!raw_sig_result.result) {
    return base::unexpected(raw_sig_result.error_message.c_str());
  }

  return VerificationSignature(
      rust::Box<cbr_ffi::CxxVerificationSignature>::from_raw(
          raw_sig_result.result));
}

base::expected<bool, std::string> VerificationKey::verify(
    const VerificationSignature& sig,
    const std::string& message) {
  int result = raw()->verification_key_invalid_sha512(*sig.raw(), message);
  if (result < 0) {
    base::unexpected("Failed to verify message signature");
  }
  return result == 0;
}

UnblindedToken::UnblindedToken(CxxUnblindedTokenBox raw)
    : raw_(base::MakeRefCounted<CxxUnblindedTokenRefData>(std::move(raw))) {}

UnblindedToken::UnblindedToken(const UnblindedToken& other) = default;

UnblindedToken& UnblindedToken::operator=(const UnblindedToken& other) =
    default;

UnblindedToken::UnblindedToken(UnblindedToken&& other) noexcept = default;

UnblindedToken& UnblindedToken::operator=(UnblindedToken&& other) noexcept =
    default;

UnblindedToken::~UnblindedToken() = default;

VerificationKey UnblindedToken::derive_verification_key() const {
  return VerificationKey(rust::Box<cbr_ffi::CxxVerificationKey>(
      raw()->unblinded_token_derive_verification_key_sha512()));
}

TokenPreimage UnblindedToken::preimage() const {
  return TokenPreimage(raw()->unblinded_token_preimage());
}

// static
base::expected<UnblindedToken, std::string> UnblindedToken::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxUnblindedTokenResult raw_unblinded_result(
      cbr_ffi::unblinded_token_decode_base64(encoded));

  if (!raw_unblinded_result.result) {
    return base::unexpected(raw_unblinded_result.error_message.c_str());
  }

  return UnblindedToken(
      CxxUnblindedTokenBox::from_raw(raw_unblinded_result.result));
}

base::expected<std::string, std::string> UnblindedToken::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool UnblindedToken::operator==(const UnblindedToken& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool UnblindedToken::operator!=(const UnblindedToken& rhs) const {
  return !(*this == rhs);
}

PublicKey::PublicKey(CxxPublicKeyBox raw)
    : raw_(base::MakeRefCounted<CxxPublicKeyRefData>(std::move(raw))) {}

PublicKey::PublicKey(const PublicKey& other) = default;

PublicKey& PublicKey::operator=(const PublicKey& other) = default;

PublicKey::PublicKey(PublicKey&& other) noexcept = default;

PublicKey& PublicKey::operator=(PublicKey&& other) noexcept = default;

PublicKey::~PublicKey() = default;

base::expected<PublicKey, std::string> PublicKey::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxPublicKeyResult raw_key_result(
      cbr_ffi::public_key_decode_base64(encoded));

  if (!raw_key_result.result) {
    return base::unexpected(raw_key_result.error_message.c_str());
  }

  return PublicKey(CxxPublicKeyBox::from_raw(raw_key_result.result));
}

base::expected<std::string, std::string> PublicKey::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool PublicKey::operator==(const PublicKey& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool PublicKey::operator!=(const PublicKey& rhs) const {
  return !(*this == rhs);
}

SigningKey::SigningKey(CxxSigningKeyBox raw)
    : raw_(base::MakeRefCounted<CxxSigningKeyRefData>(std::move(raw))) {}

SigningKey::SigningKey(const SigningKey& other) = default;

SigningKey& SigningKey::operator=(const SigningKey& other) = default;

SigningKey::SigningKey(SigningKey&& other) noexcept = default;

SigningKey& SigningKey::operator=(SigningKey&& other) noexcept = default;

SigningKey::~SigningKey() = default;

// static
base::expected<SigningKey, std::string> SigningKey::random() {
  rust::Box<cbr_ffi::CxxSigningKey> raw_key(cbr_ffi::signing_key_random());
  return SigningKey(std::move(raw_key));
}

base::expected<SignedToken, std::string> SigningKey::sign(
    const BlindedToken& tok) const {
  cbr_ffi::CxxSignedTokenResult raw_signed_result(
      raw()->signing_key_sign(*tok.raw()));

  if (!raw_signed_result.result) {
    return base::unexpected(raw_signed_result.error_message.c_str());
  }

  return SignedToken(
      rust::Box<cbr_ffi::CxxSignedToken>::from_raw(raw_signed_result.result));
}

UnblindedToken SigningKey::rederive_unblinded_token(const TokenPreimage& t) {
  return UnblindedToken(rust::Box<cbr_ffi::CxxUnblindedToken>(
      raw()->signing_key_rederive_unblinded_token(*t.raw())));
}

PublicKey SigningKey::public_key() {
  return PublicKey(
      rust::Box<cbr_ffi::CxxPublicKey>(raw()->signing_key_get_public_key()));
}

base::expected<SigningKey, std::string> SigningKey::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxSigningKeyResult raw_key_result(
      cbr_ffi::signing_key_decode_base64(encoded));

  if (!raw_key_result.result) {
    return base::unexpected(raw_key_result.error_message.c_str());
  }

  return SigningKey(CxxSigningKeyBox::from_raw(raw_key_result.result));
}

base::expected<std::string, std::string> SigningKey::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool SigningKey::operator==(const SigningKey& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool SigningKey::operator!=(const SigningKey& rhs) const {
  return !(*this == rhs);
}

DLEQProof::DLEQProof(CxxDLEQProofBox raw)
    : raw_(base::MakeRefCounted<CxxDLEQProofRefData>(std::move(raw))) {}

DLEQProof::DLEQProof(const DLEQProof& other) = default;

DLEQProof& DLEQProof::operator=(const DLEQProof& other) = default;

DLEQProof::DLEQProof(DLEQProof&& other) noexcept = default;

DLEQProof& DLEQProof::operator=(DLEQProof&& other) noexcept = default;

DLEQProof::~DLEQProof() = default;

// static
base::expected<DLEQProof, std::string> DLEQProof::Create(
    const BlindedToken& blinded_token,
    const SignedToken& signed_token,
    const SigningKey& key) {
  cbr_ffi::CxxDLEQProofResult raw_proof_result(cbr_ffi::dleq_proof_new(
      *blinded_token.raw(), *signed_token.raw(), *key.raw()));

  if (!raw_proof_result.result) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return DLEQProof(CxxDLEQProofBox::from_raw(raw_proof_result.result));
}

base::expected<bool, std::string> DLEQProof::verify(
    const BlindedToken& blinded_token,
    const SignedToken& signed_token,
    const PublicKey& key) {
  const int8_t result = raw()->dleq_proof_invalid(
      *blinded_token.raw(), *signed_token.raw(), *key.raw());
  if (result < 0) {
    return base::unexpected("Failed to verify DLEQ proof");
  }
  return result == 0;
}

base::expected<DLEQProof, std::string> DLEQProof::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxDLEQProofResult raw_proof_result(
      cbr_ffi::dleq_proof_decode_base64(encoded));

  if (!raw_proof_result.result) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return DLEQProof(CxxDLEQProofBox::from_raw(raw_proof_result.result));
}

base::expected<std::string, std::string> DLEQProof::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool DLEQProof::operator==(const DLEQProof& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool DLEQProof::operator!=(const DLEQProof& rhs) const {
  return !(*this == rhs);
}

BatchDLEQProof::BatchDLEQProof(CxxBatchDLEQProofBox raw)
    : raw_(base::MakeRefCounted<CxxBatchDLEQProofRefData>(std::move(raw))) {}

BatchDLEQProof::BatchDLEQProof(const BatchDLEQProof& other) = default;

BatchDLEQProof& BatchDLEQProof::operator=(const BatchDLEQProof& other) =
    default;

BatchDLEQProof::BatchDLEQProof(BatchDLEQProof&& other) noexcept = default;

BatchDLEQProof& BatchDLEQProof::operator=(BatchDLEQProof&& other) noexcept =
    default;

BatchDLEQProof::~BatchDLEQProof() = default;

// static
base::expected<BatchDLEQProof, std::string> BatchDLEQProof::Create(
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const SigningKey& key) {
  if (blinded_tokens.size() != signed_tokens.size()) {
    return base::unexpected(
        "Blinded tokens and signed tokens must have the same length");
  }

  rust::Vec<cbr_ffi::CxxBlindedTokenRef> raw_blinded_tokens;
  rust::Vec<cbr_ffi::CxxSignedTokenRef> raw_signed_tokens;

  for (size_t i = 0; i < blinded_tokens.size(); i++) {
    raw_blinded_tokens.push_back(
        cbr_ffi::CxxBlindedTokenRef{*blinded_tokens[i].raw()});
    raw_signed_tokens.push_back(
        cbr_ffi::CxxSignedTokenRef{*signed_tokens[i].raw()});
  }

  cbr_ffi::CxxBatchDLEQProofResult raw_proof_result(
      cbr_ffi::batch_dleq_proof_new(raw_blinded_tokens, raw_signed_tokens,
                                    *key.raw()));

  if (!raw_proof_result.result) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return BatchDLEQProof(
      CxxBatchDLEQProofBox::from_raw(raw_proof_result.result));
}

base::expected<bool, std::string> BatchDLEQProof::verify(
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const PublicKey& key) {
  if (blinded_tokens.size() != signed_tokens.size()) {
    return base::unexpected(
        "Blinded tokens and signed tokens must have the same length");
  }

  rust::Vec<cbr_ffi::CxxBlindedTokenRef> raw_blinded_tokens;
  rust::Vec<cbr_ffi::CxxSignedTokenRef> raw_signed_tokens;

  for (size_t i = 0; i < blinded_tokens.size(); i++) {
    raw_blinded_tokens.push_back(
        cbr_ffi::CxxBlindedTokenRef{*blinded_tokens[i].raw()});
    raw_signed_tokens.push_back(
        cbr_ffi::CxxSignedTokenRef{*signed_tokens[i].raw()});
  }

  int result = raw()->batch_dleq_proof_invalid(raw_blinded_tokens,
                                               raw_signed_tokens, *key.raw());
  if (result < 0) {
    return base::unexpected("Could not verify DLEQ proof");
  }
  return result == 0;
}

base::expected<std::vector<UnblindedToken>, std::string>
BatchDLEQProof::verify_and_unblind(
    const std::vector<Token>& tokens,
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const PublicKey& public_key) {
  if (tokens.size() != blinded_tokens.size() ||
      tokens.size() != signed_tokens.size()) {
    return base::unexpected(
        "Tokens, blinded tokens and signed tokens must have the same length");
  }

  rust::Vec<cbr_ffi::CxxTokenRef> raw_tokens;
  rust::Vec<cbr_ffi::CxxBlindedTokenRef> raw_blinded_tokens;
  rust::Vec<cbr_ffi::CxxSignedTokenRef> raw_signed_tokens;

  for (size_t i = 0; i < blinded_tokens.size(); i++) {
    raw_tokens.push_back(cbr_ffi::CxxTokenRef{*tokens[i].raw()});
    raw_blinded_tokens.push_back(
        cbr_ffi::CxxBlindedTokenRef{*blinded_tokens[i].raw()});
    raw_signed_tokens.push_back(
        cbr_ffi::CxxSignedTokenRef{*signed_tokens[i].raw()});
  }

  cbr_ffi::CxxUnblindedTokensResult raw_unblinded_tokens_result =
      raw()->batch_dleq_proof_verify_and_unblind(
          raw_tokens, raw_blinded_tokens, raw_signed_tokens, *public_key.raw());

  if (!raw_unblinded_tokens_result.success) {
    return base::unexpected(
        static_cast<std::string>(raw_unblinded_tokens_result.error_message));
  }

  std::vector<UnblindedToken> unblinded_tokens;
  for (auto& raw_unblinded_token : raw_unblinded_tokens_result.result) {
    CHECK(raw_unblinded_token.value);
    unblinded_tokens.emplace_back(
        rust::Box<cbr_ffi::CxxUnblindedToken>::from_raw(
            raw_unblinded_token.value));
  }

  return unblinded_tokens;
}

base::expected<BatchDLEQProof, std::string> BatchDLEQProof::decode_base64(
    const std::string& encoded) {
  cbr_ffi::CxxBatchDLEQProofResult raw_proof_result(
      cbr_ffi::batch_dleq_proof_decode_base64(encoded));

  if (!raw_proof_result.result) {
    return base::unexpected(raw_proof_result.error_message.c_str());
  }

  return BatchDLEQProof(
      rust::Box<cbr_ffi::CxxBatchDLEQProof>::from_raw(raw_proof_result.result));
}

base::expected<std::string, std::string> BatchDLEQProof::encode_base64() const {
  const std::string result = static_cast<std::string>(raw()->encode_base64());
  return base::ok(result);
}

bool BatchDLEQProof::operator==(const BatchDLEQProof& rhs) const {
  return encode_base64() == rhs.encode_base64();
}

bool BatchDLEQProof::operator!=(const BatchDLEQProof& rhs) const {
  return !(*this == rhs);
}

}  // namespace challenge_bypass_ristretto

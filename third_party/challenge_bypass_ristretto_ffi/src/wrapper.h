/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_THIRD_PARTY_CHALLENGE_BYPASS_RISTRETTO_FFI_SRC_WRAPPER_H_
#define BRAVE_THIRD_PARTY_CHALLENGE_BYPASS_RISTRETTO_FFI_SRC_WRAPPER_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/types/expected.h"
#include "brave/third_party/challenge_bypass_ristretto_ffi/src/lib.rs.h"

#if defined(CHALLENGE_BYPASS_RISTRETTO_SHARED_LIBRARY)
#if defined(WIN32)
#if defined(CHALLENGE_BYPASS_RISTRETTO_IMPLEMENTATION)
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT __declspec(dllexport)
#else
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT __declspec(dllimport)
#endif  // defined(CHALLENGE_BYPASS_RISTRETTO_IMPLEMENTATION)
#else   // defined(WIN32)
#if defined(CHALLENGE_BYPASS_RISTRETTO_IMPLEMENTATION)
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT __attribute__((visibility("default")))
#else
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT
#endif
#endif
#else  // defined(CHALLENGE_BYPASS_RISTRETTO_SHARED_LIBRARY)
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT
#endif

namespace challenge_bypass_ristretto {

class CHALLENGE_BYPASS_RISTRETTO_EXPORT TokenPreimage {
  using CxxTokenPreimageBox = rust::Box<cbr_ffi::CxxTokenPreimage>;
  using CxxTokenPreimageRefData = base::RefCountedData<CxxTokenPreimageBox>;

 public:
  explicit TokenPreimage(CxxTokenPreimageBox raw);
  TokenPreimage(const TokenPreimage&);
  TokenPreimage& operator=(const TokenPreimage&);
  TokenPreimage(TokenPreimage&&) noexcept;
  TokenPreimage& operator=(TokenPreimage&&) noexcept;
  ~TokenPreimage();

  const CxxTokenPreimageBox& raw() const { return raw_->data; }

  static base::expected<TokenPreimage, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const TokenPreimage& rhs) const;
  bool operator!=(const TokenPreimage& rhs) const;

 private:
  scoped_refptr<CxxTokenPreimageRefData> raw_;
};

class CHALLENGE_BYPASS_RISTRETTO_EXPORT BlindedToken {
  using CxxBlindedTokenBox = rust::Box<cbr_ffi::CxxBlindedToken>;
  using CxxBlindedTokenRefData = base::RefCountedData<CxxBlindedTokenBox>;

 public:
  explicit BlindedToken(CxxBlindedTokenBox raw);
  BlindedToken(const BlindedToken& other);
  BlindedToken& operator=(const BlindedToken& other);
  BlindedToken(BlindedToken&& other) noexcept;
  BlindedToken& operator=(BlindedToken&& other) noexcept;
  ~BlindedToken();

  const CxxBlindedTokenBox& raw() const { return raw_->data; }

  static base::expected<BlindedToken, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const BlindedToken& rhs) const;
  bool operator!=(const BlindedToken& rhs) const;

 private:
  scoped_refptr<CxxBlindedTokenRefData> raw_;
};

class CHALLENGE_BYPASS_RISTRETTO_EXPORT Token {
  using CxxTokenBox = rust::Box<cbr_ffi::CxxToken>;
  using CxxTokenRefData = base::RefCountedData<CxxTokenBox>;

 public:
  explicit Token(CxxTokenBox raw);
  Token(const Token&);
  Token& operator=(const Token&);
  Token(Token&&) noexcept;
  Token& operator=(Token&&) noexcept;
  ~Token();

  const CxxTokenBox& raw() const { return raw_->data; }

  static base::expected<Token, std::string> random();
  base::expected<BlindedToken, std::string> blind();
  static base::expected<Token, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const Token& rhs) const;
  bool operator!=(const Token& rhs) const;

 private:
  scoped_refptr<CxxTokenRefData> raw_;
};

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

  static base::expected<SignedToken, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const SignedToken& rhs) const;
  bool operator!=(const SignedToken& rhs) const;

 private:
  scoped_refptr<CxxSignedTokenRefData> raw_;
};

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

  static base::expected<VerificationSignature, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

 private:
  scoped_refptr<CxxVerificationSignatureRefData> raw_;
};

class CHALLENGE_BYPASS_RISTRETTO_EXPORT VerificationKey {
  using CxxVerificationKeyBox = rust::Box<cbr_ffi::CxxVerificationKey>;
  using CxxVerificationKeyRefData = base::RefCountedData<CxxVerificationKeyBox>;

 public:
  explicit VerificationKey(CxxVerificationKeyBox raw);
  VerificationKey(const VerificationKey&);
  VerificationKey& operator=(const VerificationKey&);
  VerificationKey(VerificationKey&&) noexcept;
  VerificationKey& operator=(VerificationKey&&) noexcept;
  ~VerificationKey();

  const CxxVerificationKeyBox& raw() const { return raw_->data; }

  base::expected<VerificationSignature, std::string> sign(
      const std::string& message);
  base::expected<bool, std::string> verify(const VerificationSignature& sig,
                                           const std::string& message);

 private:
  scoped_refptr<CxxVerificationKeyRefData> raw_;
};

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

  VerificationKey derive_verification_key() const;
  TokenPreimage preimage() const;
  static base::expected<UnblindedToken, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const UnblindedToken& rhs) const;
  bool operator!=(const UnblindedToken& rhs) const;

 private:
  scoped_refptr<CxxUnblindedTokenRefData> raw_;
};

class CHALLENGE_BYPASS_RISTRETTO_EXPORT PublicKey {
  using CxxPublicKeyBox = rust::Box<cbr_ffi::CxxPublicKey>;
  using CxxPublicKeyRefData = base::RefCountedData<CxxPublicKeyBox>;

 public:
  explicit PublicKey(CxxPublicKeyBox raw);
  PublicKey(const PublicKey&);
  PublicKey& operator=(const PublicKey&);
  PublicKey(PublicKey&&) noexcept;
  PublicKey& operator=(PublicKey&&) noexcept;
  ~PublicKey();

  const CxxPublicKeyBox& raw() const { return raw_->data; }

  static base::expected<PublicKey, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const PublicKey& rhs) const;
  bool operator!=(const PublicKey& rhs) const;

 private:
  scoped_refptr<CxxPublicKeyRefData> raw_;
};

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

  static base::expected<SigningKey, std::string> random();
  base::expected<SignedToken, std::string> sign(const BlindedToken& tok) const;
  UnblindedToken rederive_unblinded_token(const TokenPreimage& t);
  PublicKey public_key();
  static base::expected<SigningKey, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const SigningKey& rhs) const;
  bool operator!=(const SigningKey& rhs) const;

 private:
  scoped_refptr<CxxSigningKeyRefData> raw_;
};

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

  base::expected<bool, std::string> verify(const BlindedToken& blinded_token,
                                           const SignedToken& signed_token,
                                           const PublicKey& key);
  static base::expected<DLEQProof, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const DLEQProof& rhs) const;
  bool operator!=(const DLEQProof& rhs) const;

  static base::expected<DLEQProof, std::string> Create(
      const BlindedToken& blinded_token,
      const SignedToken& signed_token,
      const SigningKey& key);

 private:
  scoped_refptr<CxxDLEQProofRefData> raw_;
};

class CHALLENGE_BYPASS_RISTRETTO_EXPORT BatchDLEQProof {
  using CxxBatchDLEQProofBox = rust::Box<cbr_ffi::CxxBatchDLEQProof>;
  using CxxBatchDLEQProofRefData = base::RefCountedData<CxxBatchDLEQProofBox>;

 public:
  explicit BatchDLEQProof(CxxBatchDLEQProofBox raw);
  BatchDLEQProof(const BatchDLEQProof&);
  BatchDLEQProof& operator=(const BatchDLEQProof&);
  BatchDLEQProof(BatchDLEQProof&&) noexcept;
  BatchDLEQProof& operator=(BatchDLEQProof&&) noexcept;
  ~BatchDLEQProof();

  const CxxBatchDLEQProofBox& raw() const { return raw_->data; }

  static base::expected<BatchDLEQProof, std::string> Create(
      const std::vector<BlindedToken>& blinded_tokens,
      const std::vector<SignedToken>& signed_tokens,
      const SigningKey& key);

  base::expected<bool, std::string> verify(
      const std::vector<BlindedToken>& blinded_tokens,
      const std::vector<SignedToken>& signed_tokens,
      const PublicKey& key);

  base::expected<std::vector<UnblindedToken>, std::string> verify_and_unblind(
      const std::vector<Token>& tokens,
      const std::vector<BlindedToken>& blinded_tokens,
      const std::vector<SignedToken>& signed_tokens,
      const PublicKey& public_key);

  static base::expected<BatchDLEQProof, std::string> decode_base64(
      const std::string& encoded);
  base::expected<std::string, std::string> encode_base64() const;

  bool operator==(const BatchDLEQProof& rhs) const;
  bool operator!=(const BatchDLEQProof& rhs) const;

 private:
  scoped_refptr<CxxBatchDLEQProofRefData> raw_;
};

}  // namespace challenge_bypass_ristretto

#endif  // BRAVE_THIRD_PARTY_CHALLENGE_BYPASS_RISTRETTO_FFI_SRC_WRAPPER_H_

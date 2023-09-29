extern crate challenge_bypass_ristretto;
extern crate hmac;
extern crate rand;
extern crate sha2;

use std::error::Error;
use std::ptr::null_mut;

use challenge_bypass_ristretto::errors::InternalError;
use challenge_bypass_ristretto::voprf::{
    BatchDLEQProof, BlindedToken, DLEQProof, PublicKey, SignedToken, SigningKey, Token,
    TokenPreimage, UnblindedToken, VerificationKey, VerificationSignature,
};
use cxx::CxxString;
use ffi::{
    CxxBatchDLEQProofResult, CxxBlindedTokenRef, CxxBlindedTokenResult, CxxDLEQProofResult,
    CxxPublicKeyResult, CxxSignedTokenRef, CxxSignedTokenResult, CxxSigningKeyResult,
    CxxTokenPreimageResult, CxxTokenRef, CxxTokenResult, CxxUnblindedTokenPtr,
    CxxUnblindedTokenResult, CxxUnblindedTokensResult, CxxVerificationSignatureResult,
};
use hmac::Hmac;
use rand::rngs::OsRng;
use sha2::Sha512;

type HmacSha512 = Hmac<Sha512>;

#[allow(unsafe_op_in_unsafe_fn)]
#[cxx::bridge(namespace = cbr_ffi)]
mod ffi {
    extern "Rust" {
        type CxxTokenPreimage;
        fn encode_base64(self: &CxxTokenPreimage) -> String;
        fn token_preimage_decode_base64(s: &CxxString) -> CxxTokenPreimageResult;

        type CxxBlindedToken;
        fn encode_base64(self: &CxxBlindedToken) -> String;
        fn blinded_token_decode_base64(s: &CxxString) -> CxxBlindedTokenResult;

        type CxxToken;
        fn token_random() -> Box<CxxToken>;
        fn token_blind(self: &CxxToken) -> Box<CxxBlindedToken>;
        fn encode_base64(self: &CxxToken) -> String;
        fn token_decode_base64(s: &CxxString) -> CxxTokenResult;

        type CxxSignedToken;
        fn encode_base64(self: &CxxSignedToken) -> String;
        fn signed_token_decode_base64(s: &CxxString) -> CxxSignedTokenResult;

        type CxxVerificationSignature;
        fn encode_base64(self: &CxxVerificationSignature) -> String;
        fn verification_signature_decode_base64(s: &CxxString) -> CxxVerificationSignatureResult;

        type CxxVerificationKey;
        fn verification_key_sign_sha512(
            self: &CxxVerificationKey,
            message: &CxxString,
        ) -> CxxVerificationSignatureResult;
        fn verification_key_invalid_sha512(
            self: &CxxVerificationKey,
            sig: &CxxVerificationSignature,
            message: &CxxString,
        ) -> i8;

        type CxxUnblindedToken;
        fn encode_base64(self: &CxxUnblindedToken) -> String;
        fn unblinded_token_decode_base64(s: &CxxString) -> CxxUnblindedTokenResult;
        fn unblinded_token_derive_verification_key_sha512(
            self: &CxxUnblindedToken,
        ) -> Box<CxxVerificationKey>;
        fn unblinded_token_preimage(self: &CxxUnblindedToken) -> Box<CxxTokenPreimage>;

        type CxxPublicKey;
        fn encode_base64(self: &CxxPublicKey) -> String;
        fn public_key_decode_base64(s: &CxxString) -> CxxPublicKeyResult;

        type CxxSigningKey;
        fn encode_base64(self: &CxxSigningKey) -> String;
        fn signing_key_decode_base64(s: &CxxString) -> CxxSigningKeyResult;
        fn signing_key_random() -> Box<CxxSigningKey>;
        fn signing_key_sign(self: &CxxSigningKey, token: &CxxBlindedToken) -> CxxSignedTokenResult;
        fn signing_key_rederive_unblinded_token(
            self: &CxxSigningKey,
            t: &CxxTokenPreimage,
        ) -> Box<CxxUnblindedToken>;
        fn signing_key_get_public_key(self: &CxxSigningKey) -> Box<CxxPublicKey>;

        type CxxDLEQProof;
        fn encode_base64(self: &CxxDLEQProof) -> String;
        fn dleq_proof_decode_base64(s: &CxxString) -> CxxDLEQProofResult;
        fn dleq_proof_new(
            blinded_token: &CxxBlindedToken,
            signed_token: &CxxSignedToken,
            key: &CxxSigningKey,
        ) -> CxxDLEQProofResult;
        fn dleq_proof_invalid(
            self: &CxxDLEQProof,
            blinded_token: &CxxBlindedToken,
            signed_token: &CxxSignedToken,
            public_key: &CxxPublicKey,
        ) -> i8;

        type CxxBatchDLEQProof;
        fn encode_base64(self: &CxxBatchDLEQProof) -> String;
        fn batch_dleq_proof_decode_base64(s: &CxxString) -> CxxBatchDLEQProofResult;
        fn batch_dleq_proof_new(
            blinded_tokens: &Vec<CxxBlindedTokenRef>,
            signed_tokens: &Vec<CxxSignedTokenRef>,
            key: &CxxSigningKey,
        ) -> CxxBatchDLEQProofResult;
        fn batch_dleq_proof_invalid(
            self: &CxxBatchDLEQProof,
            blinded_tokens: &Vec<CxxBlindedTokenRef>,
            signed_tokens: &Vec<CxxSignedTokenRef>,
            public_key: &CxxPublicKey,
        ) -> i8;

        fn batch_dleq_proof_verify_and_unblind(
            self: &CxxBatchDLEQProof,
            tokens: &Vec<CxxTokenRef>,
            blinded_tokens: &Vec<CxxBlindedTokenRef>,
            signed_tokens: &Vec<CxxSignedTokenRef>,
            public_key: &CxxPublicKey,
        ) -> CxxUnblindedTokensResult;
    }

    impl Box<CxxTokenPreimage> {}
    struct CxxTokenPreimageResult {
        result: *mut CxxTokenPreimage,
        error_message: String,
    }

    impl Box<CxxBlindedToken> {}
    struct CxxBlindedTokenResult {
        result: *mut CxxBlindedToken,
        error_message: String,
    }

    impl Box<CxxToken> {}
    struct CxxTokenResult {
        result: *mut CxxToken,
        error_message: String,
    }

    impl Box<CxxSignedToken> {}
    struct CxxSignedTokenResult {
        result: *mut CxxSignedToken,
        error_message: String,
    }

    impl Box<CxxVerificationSignature> {}
    struct CxxVerificationSignatureResult {
        result: *mut CxxVerificationSignature,
        error_message: String,
    }

    impl Box<CxxUnblindedToken> {}
    struct CxxUnblindedTokenResult {
        result: *mut CxxUnblindedToken,
        error_message: String,
    }

    impl Box<CxxPublicKey> {}
    struct CxxPublicKeyResult {
        result: *mut CxxPublicKey,
        error_message: String,
    }

    impl Box<CxxSigningKey> {}
    struct CxxSigningKeyResult {
        result: *mut CxxSigningKey,
        error_message: String,
    }

    impl Box<CxxDLEQProof> {}
    struct CxxDLEQProofResult {
        result: *mut CxxDLEQProof,
        error_message: String,
    }

    impl Box<CxxBatchDLEQProof> {}
    struct CxxBatchDLEQProofResult {
        result: *mut CxxBatchDLEQProof,
        error_message: String,
    }

    struct CxxBlindedTokenRef<'a> {
        value: &'a CxxBlindedToken,
    }

    struct CxxSignedTokenRef<'a> {
        value: &'a CxxSignedToken,
    }

    struct CxxTokenRef<'a> {
        value: &'a CxxToken,
    }

    struct CxxUnblindedTokenPtr {
        value: *mut CxxUnblindedToken,
    }
    #[derive(Default)]
    struct CxxUnblindedTokensResult {
        result: Vec<CxxUnblindedTokenPtr>,
        success: bool,
        error_message: String,
    }
}

pub struct CxxTokenPreimage(TokenPreimage);
pub struct CxxBlindedToken(BlindedToken);
pub struct CxxToken(Token);
pub struct CxxSignedToken(SignedToken);
pub struct CxxVerificationSignature(VerificationSignature);
pub struct CxxVerificationKey(VerificationKey);
pub struct CxxUnblindedToken(UnblindedToken);
pub struct CxxPublicKey(PublicKey);
pub struct CxxSigningKey(SigningKey);
pub struct CxxDLEQProof(DLEQProof);
pub struct CxxBatchDLEQProof(BatchDLEQProof);

macro_rules! impl_encode_decode_base64 {
    ($t:ident, $ct:ident, $ctr:ident, $de:ident) => {
        /// Decode from base64 String.
        pub fn $de(encoded: &CxxString) -> $ctr {
            match encoded.to_str() {
                Ok(s) => match $t::decode_base64(s) {
                    Ok(t) => $ctr {
                        result: Box::into_raw(Box::new($ct(t))),
                        error_message: "".to_string(),
                    },
                    Err(err) => $ctr { result: null_mut(), error_message: err.to_string() },
                },
                Err(_) => $ctr {
                    result: null_mut(),
                    error_message: "Failed to convert input string".to_string(),
                },
            }
        }

        impl $ct {
            fn encode_base64(self: &$ct) -> String {
                return (&self.0).encode_base64();
            }
        }
    };
}

impl_encode_decode_base64!(
    TokenPreimage,
    CxxTokenPreimage,
    CxxTokenPreimageResult,
    token_preimage_decode_base64
);

impl_encode_decode_base64!(
    BlindedToken,
    CxxBlindedToken,
    CxxBlindedTokenResult,
    blinded_token_decode_base64
);

impl_encode_decode_base64!(Token, CxxToken, CxxTokenResult, token_decode_base64);

impl_encode_decode_base64!(
    SignedToken,
    CxxSignedToken,
    CxxSignedTokenResult,
    signed_token_decode_base64
);

impl_encode_decode_base64!(
    VerificationSignature,
    CxxVerificationSignature,
    CxxVerificationSignatureResult,
    verification_signature_decode_base64
);

impl_encode_decode_base64!(
    UnblindedToken,
    CxxUnblindedToken,
    CxxUnblindedTokenResult,
    unblinded_token_decode_base64
);

impl_encode_decode_base64!(PublicKey, CxxPublicKey, CxxPublicKeyResult, public_key_decode_base64);

impl_encode_decode_base64!(
    SigningKey,
    CxxSigningKey,
    CxxSigningKeyResult,
    signing_key_decode_base64
);

impl_encode_decode_base64!(DLEQProof, CxxDLEQProof, CxxDLEQProofResult, dleq_proof_decode_base64);

impl_encode_decode_base64!(
    BatchDLEQProof,
    CxxBatchDLEQProof,
    CxxBatchDLEQProofResult,
    batch_dleq_proof_decode_base64
);

/// Generate a new `Token`
pub fn token_random() -> Box<CxxToken> {
    let mut rng = OsRng;
    let token = Token::random::<Sha512, OsRng>(&mut rng);
    Box::new(CxxToken(token))
}

/// Take a reference to a `Token` and blind it, returning a `BlindedToken`
impl CxxToken {
    fn token_blind(self: &CxxToken) -> Box<CxxBlindedToken> {
        Box::new(CxxBlindedToken((&self.0).blind()))
    }
}

impl CxxUnblindedToken {
    /// Take a reference to an `UnblindedToken` and use it to derive a
    /// `VerificationKey` using Sha512 as the hash function
    fn unblinded_token_derive_verification_key_sha512(
        self: &CxxUnblindedToken,
    ) -> Box<CxxVerificationKey> {
        Box::new(CxxVerificationKey((&self.0).derive_verification_key::<Sha512>()))
    }

    /// Take a reference to an `UnblindedToken` and return the corresponding
    /// `CxxTokenPreimage`
    fn unblinded_token_preimage(self: &CxxUnblindedToken) -> Box<CxxTokenPreimage> {
        Box::new(CxxTokenPreimage((&self.0).t))
    }
}

impl CxxVerificationKey {
    fn verification_key_sign_sha512(
        self: &CxxVerificationKey,
        message: &CxxString,
    ) -> CxxVerificationSignatureResult {
        match message.to_str() {
            Ok(s) => CxxVerificationSignatureResult {
                result: Box::into_raw(Box::new(CxxVerificationSignature(
                    (&self.0).sign::<HmacSha512>(s.as_bytes()),
                ))),
                error_message: "".to_string(),
            },
            Err(_) => CxxVerificationSignatureResult {
                result: null_mut(),
                error_message: "Failed to convert input string".to_string(),
            },
        }
    }

    // Verify an existing `VerificationSignature` using Sha512 as the HMAC hash
    // function
    ///
    /// Returns -1 if an error was encountered, 1 if the signature failed
    /// verification and 0 if valid
    ///
    /// NOTE this is named "invalid" instead of "verify" as it returns true
    /// (non-zero) when the signature is invalid and false (zero) when valid
    fn verification_key_invalid_sha512(
        self: &CxxVerificationKey,
        sig: &CxxVerificationSignature,
        message: &CxxString,
    ) -> i8 {
        match message.to_str() {
            Ok(s) => {
                if (&self.0).verify::<HmacSha512>(&sig.0, s.as_bytes()) {
                    0
                } else {
                    1
                }
            }
            Err(_) => -1,
        }
    }
}

/// Generate a new `SigningKey`
pub fn signing_key_random() -> Box<CxxSigningKey> {
    let mut rng = OsRng;
    let key = SigningKey::random(&mut rng);
    Box::new(CxxSigningKey(key))
}

impl CxxSigningKey {
    /// Take a reference to a `SigningKey` and use it to sign a `BlindedToken`,
    /// returning a `SignedToken`
    fn signing_key_sign(self: &CxxSigningKey, token: &CxxBlindedToken) -> CxxSignedTokenResult {
        match (self.0).sign(&token.0) {
            Ok(signed_token) => CxxSignedTokenResult {
                result: Box::into_raw(Box::new(CxxSignedToken(signed_token))),
                error_message: "".to_string(),
            },
            Err(err) => CxxSignedTokenResult { result: null_mut(), error_message: err.to_string() },
        }
    }

    /// Take a reference to a `SigningKey` and use it to rederive an
    /// `UnblindedToken`
    fn signing_key_rederive_unblinded_token(
        self: &CxxSigningKey,
        t: &CxxTokenPreimage,
    ) -> Box<CxxUnblindedToken> {
        Box::new(CxxUnblindedToken((self.0).rederive_unblinded_token(&t.0)))
    }

    /// Take a reference to a `SigningKey` and return it's associated
    /// `PublicKey`
    fn signing_key_get_public_key(self: &CxxSigningKey) -> Box<CxxPublicKey> {
        Box::new(CxxPublicKey((self.0).public_key))
    }
}

/// Create a new DLEQ proof
pub fn dleq_proof_new(
    blinded_token: &CxxBlindedToken,
    signed_token: &CxxSignedToken,
    key: &CxxSigningKey,
) -> CxxDLEQProofResult {
    let mut rng = OsRng;
    match DLEQProof::new::<Sha512, OsRng>(&mut rng, &blinded_token.0, &signed_token.0, &key.0) {
        Ok(proof) => CxxDLEQProofResult {
            result: Box::into_raw(Box::new(CxxDLEQProof(proof))),
            error_message: "".to_string(),
        },
        Err(err) => CxxDLEQProofResult { result: null_mut(), error_message: err.to_string() },
    }
}

/// Check if a DLEQ proof is invalid
///
/// Returns -1 if an error was encountered, 1 if the proof failed verification
/// and 0 if valid
///
/// NOTE this is named "invalid" instead of "verify" as it returns true
/// (non-zero) when the proof is invalid and false (zero) when valid
impl CxxDLEQProof {
    fn dleq_proof_invalid(
        self: &CxxDLEQProof,
        blinded_token: &CxxBlindedToken,
        signed_token: &CxxSignedToken,
        public_key: &CxxPublicKey,
    ) -> i8 {
        match (self.0).verify::<Sha512>(&blinded_token.0, &signed_token.0, &public_key.0) {
            Ok(_) => return 0,
            Err(err) => {
                if let Some(InternalError::VerifyError) =
                    err.source().unwrap().downcast_ref::<InternalError>()
                {
                    return 1;
                } else {
                    return -1;
                }
            }
        }
    }
}

/// Create a new batch DLEQ proof
pub fn batch_dleq_proof_new(
    blinded_tokens: &Vec<CxxBlindedTokenRef>,
    signed_tokens: &Vec<CxxSignedTokenRef>,
    key: &CxxSigningKey,
) -> CxxBatchDLEQProofResult {
    let mut rng = OsRng;
    let blinded_tokens: Vec<BlindedToken> = blinded_tokens.iter().map(|p| p.value.0).collect();
    let signed_tokens: Vec<SignedToken> = signed_tokens.iter().map(|p| p.value.0).collect();
    match BatchDLEQProof::new::<Sha512, OsRng>(&mut rng, &blinded_tokens, &signed_tokens, &key.0) {
        Ok(proof) => CxxBatchDLEQProofResult {
            result: Box::into_raw(Box::new(CxxBatchDLEQProof(proof))),
            error_message: "".to_string(),
        },
        Err(err) => CxxBatchDLEQProofResult { result: null_mut(), error_message: err.to_string() },
    }
}

impl CxxBatchDLEQProof {
    /// Check if a batch DLEQ proof is invalid
    ///
    /// Returns -1 if an error was encountered, 1 if the proof failed
    /// verification and 0 if valid
    ///
    /// NOTE this is named "invalid" instead of "verify" as it returns true
    /// (non-zero) when the proof is invalid and false (zero) when valid
    fn batch_dleq_proof_invalid(
        self: &CxxBatchDLEQProof,
        blinded_tokens: &Vec<CxxBlindedTokenRef>,
        signed_tokens: &Vec<CxxSignedTokenRef>,
        public_key: &CxxPublicKey,
    ) -> i8 {
        let blinded_tokens: Vec<BlindedToken> = blinded_tokens.iter().map(|p| p.value.0).collect();
        let signed_tokens: Vec<SignedToken> = signed_tokens.iter().map(|p| p.value.0).collect();
        match (self.0).verify::<Sha512>(&blinded_tokens, &signed_tokens, &public_key.0) {
            Ok(_) => return 0,
            Err(err) => {
                if let Some(InternalError::VerifyError) =
                    err.source().unwrap().downcast_ref::<InternalError>()
                {
                    return 1;
                } else {
                    return -1;
                }
            }
        }
    }

    /// Check if a batch DLEQ proof is invalid and unblind each signed token if
    /// not
    fn batch_dleq_proof_verify_and_unblind(
        self: &CxxBatchDLEQProof,
        tokens: &Vec<CxxTokenRef>,
        blinded_tokens: &Vec<CxxBlindedTokenRef>,
        signed_tokens: &Vec<CxxSignedTokenRef>,
        public_key: &CxxPublicKey,
    ) -> CxxUnblindedTokensResult {
        let tokens = tokens.iter().map(|p| &p.value.0);
        let blinded_tokens: Vec<BlindedToken> = blinded_tokens.iter().map(|p| p.value.0).collect();
        let signed_tokens: Vec<SignedToken> = signed_tokens.iter().map(|p| p.value.0).collect();

        match (self.0).verify_and_unblind::<Sha512, _>(
            tokens,
            &blinded_tokens,
            &signed_tokens,
            &public_key.0,
        ) {
            Ok(unblinded_tokens) => {
                let unblinded_tokens: Vec<CxxUnblindedTokenPtr> = unblinded_tokens
                    .into_iter()
                    .map(|t| CxxUnblindedTokenPtr {
                        value: Box::into_raw(Box::new(CxxUnblindedToken(t))),
                    })
                    .collect();
                return CxxUnblindedTokensResult {
                    success: true,
                    result: unblinded_tokens,
                    ..Default::default()
                };
            }
            Err(err) => {
                if let Some(InternalError::VerifyError) =
                    err.source().unwrap().downcast_ref::<InternalError>()
                {
                    return CxxUnblindedTokensResult { success: true, ..Default::default() };
                } else {
                    return CxxUnblindedTokensResult {
                        success: false,
                        error_message: err.to_string(),
                        ..Default::default()
                    };
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_embedded_null() {
        unsafe {
            let_cxx_string!(c_msg1 = "\0hello");
            let_cxx_string!(c_msg2 = "");

            let key = signing_key_random();

            let token = token_random();
            let blinded_token = token.token_blind();
            let signed_token_result = key.signing_key_sign(&*blinded_token);
            assert!(!signed_token_result.result.is_null());
            let signed_token = Box::from_raw(signed_token_result.result);

            let tokens = vec![CxxTokenRef { value: &*token }];
            let blinded_tokens = vec![CxxBlindedTokenRef { value: &*blinded_token }];
            let signed_tokens = vec![CxxSignedTokenRef { value: &*signed_token }];

            let proof_result = batch_dleq_proof_new(&blinded_tokens, &signed_tokens, &*key);
            assert!(!proof_result.result.is_null());
            let proof = Box::from_raw(proof_result.result);

            let public_key = key.signing_key_get_public_key();

            let unblinded_tokens_result = proof.batch_dleq_proof_verify_and_unblind(
                &tokens,
                &blinded_tokens,
                &signed_tokens,
                &*public_key,
            );
            assert!(unblinded_tokens_result.success);
            assert!(unblinded_tokens_result.result[0].is_null());
            let unblinded_token = Box::from_raw(unblinded_tokens_result.result[0]);

            let v_key = unblinded_tokens.unblinded_token_derive_verification_key_sha512();

            let code_result = v_key.verification_key_sign_sha512(&c_msg1);
            assert!(!code_result.result.is_null());
            let code = Box::from_raw(code_result.result);

            assert_ne!(
                v_key.verification_key_verify_sha512(&*code, &c_msg2),
                0,
                "A different message should not validate"
            );

            assert_eq!(
                v_key.verification_key_verify_sha512(&*code, &c_msg1),
                0,
                "Embedded nulls in the same message should validate"
            );
        }
    }
}

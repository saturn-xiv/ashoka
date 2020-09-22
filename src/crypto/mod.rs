// https://github.com/RustCrypto/hashes

pub mod ssha512;

use actix_web::http::StatusCode;
use sodiumoxide::{
    crypto::{pwhash, secretbox},
    randombytes,
};

use super::errors::{Error, Result};

pub trait Random {
    fn bytes(l: usize) -> Vec<u8>;
}

pub trait Password {
    fn sum(plain: &[u8]) -> Result<Vec<u8>>;
    fn verify(cipher: &[u8], plain: &[u8]) -> bool;
}

pub trait Secret {
    fn encrypt(&self, plain: &[u8]) -> (Vec<u8>, Vec<u8>);
    fn decrypt(&self, cipher: &[u8], nonce: &[u8]) -> Result<Vec<u8>>;
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Key(pub String);

impl Default for Key {
    fn default() -> Self {
        Key(base64::encode(&randombytes::randombytes(32)))
    }
}

impl Into<Result<Vec<u8>>> for Key {
    fn into(self) -> Result<Vec<u8>> {
        let buf = base64::decode(&self.0)?;
        Ok(buf)
    }
}

#[derive(Clone)]
pub struct Crypto {
    key: secretbox::Key,
}

impl Crypto {
    pub fn new(key: Key) -> Result<Self> {
        let key: Result<Vec<u8>> = key.into();
        match secretbox::Key::from_slice(&key?) {
            Some(key) => Ok(Self { key }),
            None => Err(Error::Http(StatusCode::NOT_IMPLEMENTED)),
        }
    }
}

impl Random for Crypto {
    fn bytes(l: usize) -> Vec<u8> {
        randombytes::randombytes(l)
    }
}

impl Password for Crypto {
    fn sum(plain: &[u8]) -> Result<Vec<u8>> {
        match pwhash::pwhash(
            plain,
            pwhash::OPSLIMIT_INTERACTIVE,
            pwhash::MEMLIMIT_INTERACTIVE,
        ) {
            Ok(cip) => Ok(cip[..].to_vec()),
            Err(_) => Err(Error::Http(StatusCode::NOT_IMPLEMENTED)),
        }
    }

    fn verify(cipher: &[u8], plain: &[u8]) -> bool {
        match pwhash::HashedPassword::from_slice(cipher) {
            Some(cipher) => pwhash::pwhash_verify(&cipher, plain),
            None => false,
        }
    }
}

impl Secret for Crypto {
    fn encrypt(&self, plain: &[u8]) -> (Vec<u8>, Vec<u8>) {
        let nonce = secretbox::gen_nonce();
        let cipher = secretbox::seal(plain, &nonce, &self.key);
        (cipher, nonce[..].to_vec())
    }

    fn decrypt(&self, cipher: &[u8], nonce: &[u8]) -> Result<Vec<u8>> {
        if let Some(nonce) = secretbox::Nonce::from_slice(nonce) {
            if let Ok(buf) = secretbox::open(cipher, &nonce, &self.key) {
                return Ok(buf);
            }
        }
        Err(Error::Http(StatusCode::BAD_REQUEST))
    }
}

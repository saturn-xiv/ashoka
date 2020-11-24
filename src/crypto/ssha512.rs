use sodiumoxide::crypto::hash::sha512::{hash, DIGESTBYTES};

use super::Crypto;

// https://doc.dovecot.org/configuration_manual/authentication/password_schemes/
// https://www.tunnelsup.com/using-salted-sha-hashes-with-dovecot-authentication/
// doveadm pw -t {SSHA256.hex}4a847fefc4f9ab450f16783c5025d64313942a1ceb2599707cdb65940ba901e513fa442f -p pass
pub trait SSha512 {
    fn sum(plain: &[u8], salt: &[u8]) -> String;
    fn verify(cipher: &str, plain: &[u8]) -> bool;
}

impl SSha512 for Crypto {
    fn sum(plain: &[u8], salt: &[u8]) -> String {
        base64::encode(&[hash(&[plain, salt].concat()).as_ref(), salt].concat())
    }

    fn verify(cipher: &str, plain: &[u8]) -> bool {
        match base64::decode(cipher) {
            Ok(buf) => cipher == Self::sum(plain, &buf[DIGESTBYTES..]),
            Err(_) => false,
        }
    }
}

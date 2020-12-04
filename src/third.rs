use super::errors::Result;

pub trait Sms {
    fn send(&self, to: &str, body: &str) -> Result<()>;
    fn recived(&self) -> Result<Vec<String>>;
}

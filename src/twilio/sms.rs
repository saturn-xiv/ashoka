use super::super::{errors::Result, third::Sms};
use super::Config;

impl Sms for Config {
    fn send(&self, to: &str, body: &str) -> Result<()> {
        Ok(())
    }
    fn recived(&self) -> Result<Vec<String>> {
        let items = Vec::new();
        Ok(items)
    }
}

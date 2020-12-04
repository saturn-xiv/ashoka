pub mod sms;

use reqwest::{
    header::{HeaderMap, HeaderValue, AUTHORIZATION},
    Client,
};
use serde::{de::DeserializeOwned, ser::Serialize};

use super::errors::Result;

// https://www.twilio.com/docs/api

#[derive(Serialize, Deserialize, Debug, Clone)]
#[serde(rename_all = "camelCase")]
pub struct Config {
    pub from: String,
    pub account_sid: String,
    pub auth_token: String,
}

impl Config {
    fn credentials(&self) -> String {
        format!("{}:{}", self.account_sid, self.auth_token)
    }
    // async fn get<T: DeserializeOwned>(&self) -> Result<T> {
    //     let mut headers = HeaderMap::new();
    //     headers.insert(AUTHORIZATION, HeaderValue::from_str(&self.credentials())?);

    //     let client = Client::builder().default_headers(headers).build()?;
    //     let res = client
    //         .get("https://www.rust-lang.org")
    //         .header(AUTHORIZATION, "token")
    //         .send()
    //         .await?;
    //     Ok(res)
    // }
}

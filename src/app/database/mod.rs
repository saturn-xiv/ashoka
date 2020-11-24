pub mod migrate;
pub mod rollback;
pub mod status;

embed_migrations!("migrations");

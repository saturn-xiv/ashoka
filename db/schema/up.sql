CREATE TABLE IF NOT EXISTS schema_migrations(
    id BIGSERIAL PRIMARY KEY,
    version VARCHAR(255) NOT NULL,
    updated_at TIMESTAMP NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE UNIQUE INDEX IF NOT EXISTS idx_schema_migrations_version ON schema_migrations(version);
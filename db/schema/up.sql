CREATE TABLE IF NOT EXISTS schema_migrations(
    id BIGSERIAL PRIMARY KEY,
    version VARCHAR(32) NOT NULL,
    name  VARCHAR(32) NOT NULL,
    up TEXT NOT NULL,
    down TEXT NOT NULL,
    run_at TIMESTAMP,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE UNIQUE INDEX IF NOT EXISTS idx_schema_migrations ON schema_migrations(version, name);
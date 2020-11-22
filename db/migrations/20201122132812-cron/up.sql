CREATE TABLE cron_jobs(
    id BIGSERIAL PRIMARY KEY,
    name VARCHAR(64) NOT NULL,
    "user" VARCHAR(32) NOT NULL,
    home VARCHAR(255) NOT NULL,
    command VARCHAR(255) NOT NULL,
    run_at VARCHAR(255) NOT NULL,
    updated_at TIMESTAMP NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE UNIQUE INDEX idx_cron_jobs_name ON cron_jobs(name);

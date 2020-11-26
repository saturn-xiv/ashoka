table! {
    attachments (id) {
        id -> Int8,
        user_id -> Int8,
        title -> Varchar,
        size -> Int8,
        content_type -> Varchar,
        url -> Varchar,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    cards (id) {
        id -> Int8,
        title -> Varchar,
        body -> Text,
        media_type -> Varchar,
        action -> Varchar,
        href -> Varchar,
        logo -> Varchar,
        loc -> Varchar,
        lang -> Varchar,
        position -> Int2,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    categories (id) {
        id -> Int8,
        parent_id -> Nullable<Int8>,
        name -> Varchar,
        icon -> Varchar,
        color -> Varchar,
        position -> Int2,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    category_resources (id) {
        id -> Int8,
        category_id -> Int8,
        resource_type -> Varchar,
        resource_id -> Int8,
        created_at -> Timestamp,
    }
}

table! {
    crawlers_logs (id) {
        id -> Int8,
        name -> Varchar,
        url -> Varchar,
        body -> Text,
        created_at -> Timestamp,
    }
}

table! {
    cron_jobs (id) {
        id -> Int8,
        name -> Varchar,
        user -> Varchar,
        home -> Varchar,
        command -> Varchar,
        run_at -> Varchar,
        updated_at -> Timestamp,
        created_at -> Timestamp,
    }
}

table! {
    forum_posts (id) {
        id -> Int8,
        user_id -> Int8,
        topic_id -> Int8,
        post_id -> Nullable<Int8>,
        body -> Text,
        media_type -> Varchar,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    forum_topics (id) {
        id -> Int8,
        user_id -> Int8,
        title -> Varchar,
        body -> Text,
        media_type -> Varchar,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    friend_links (id) {
        id -> Int8,
        title -> Varchar,
        home -> Varchar,
        logo -> Varchar,
        position -> Int2,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    leave_words (id) {
        id -> Int8,
        ip -> Varchar,
        body -> Text,
        media_type -> Varchar,
        created_at -> Timestamp,
    }
}

table! {
    links (id) {
        id -> Int8,
        href -> Varchar,
        label -> Varchar,
        loc -> Varchar,
        lang -> Varchar,
        x -> Int2,
        y -> Int2,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    locales (id) {
        id -> Int8,
        lang -> Varchar,
        code -> Varchar,
        message -> Text,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    logs (id) {
        id -> Int8,
        user_id -> Int8,
        ip -> Varchar,
        message -> Varchar,
        created_at -> Timestamp,
    }
}

table! {
    notifications (id) {
        id -> Int8,
        user_id -> Int4,
        url -> Varchar,
        body -> Text,
        media_type -> Varchar,
        level -> Varchar,
        read -> Bool,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    policies (id) {
        id -> Int8,
        user_id -> Int8,
        role -> Varchar,
        resource -> Nullable<Varchar>,
        nbf -> Date,
        exp -> Date,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    schema_migrations (id) {
        id -> Int8,
        version -> Varchar,
        name -> Varchar,
        up -> Text,
        down -> Text,
        run_at -> Nullable<Timestamp>,
        created_at -> Timestamp,
    }
}

table! {
    settings (id) {
        id -> Int8,
        key -> Varchar,
        value -> Bytea,
        salt -> Nullable<Bytea>,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    tag_resources (id) {
        id -> Int8,
        tag_id -> Int8,
        resource_type -> Varchar,
        resource_id -> Int8,
        created_at -> Timestamp,
    }
}

table! {
    tags (id) {
        id -> Int8,
        name -> Varchar,
        icon -> Varchar,
        color -> Varchar,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    users (id) {
        id -> Int8,
        first_name -> Varchar,
        last_name -> Varchar,
        email -> Varchar,
        password -> Nullable<Bytea>,
        uid -> Varchar,
        provider_type -> Varchar,
        provider_id -> Varchar,
        access_token -> Nullable<Varchar>,
        logo -> Varchar,
        sign_in_count -> Int8,
        current_sign_in_at -> Nullable<Timestamp>,
        current_sign_in_ip -> Nullable<Varchar>,
        last_sign_in_at -> Nullable<Timestamp>,
        last_sign_in_ip -> Nullable<Varchar>,
        confirmed_at -> Nullable<Timestamp>,
        locked_at -> Nullable<Timestamp>,
        deleted_at -> Nullable<Timestamp>,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    votes (id) {
        id -> Int8,
        point -> Int8,
        resource_type -> Varchar,
        resource_id -> Int8,
        version -> Int8,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

allow_tables_to_appear_in_same_query!(
    attachments,
    cards,
    categories,
    category_resources,
    crawlers_logs,
    cron_jobs,
    forum_posts,
    forum_topics,
    friend_links,
    leave_words,
    links,
    locales,
    logs,
    notifications,
    policies,
    schema_migrations,
    settings,
    tag_resources,
    tags,
    users,
    votes,
);

#include "orm.h"

void ashoka::orm::SchemaDao::migrate()
{
    std::lock_guard<std::mutex> lock(this->locker);
    // std::sort(this->migrations.begin(), this->migrations.end());
    for (auto it = this->migrations.begin(); it != this->migrations.end(); it++)
    {
        if (!it->run_at)
        {
            BOOST_LOG_TRIVIAL(info) << "find " << it->version << " " << it->name << ", migrate...";
            this->execute(it->up);
            this->insert(it->version);
        }
    }
}

void ashoka::orm::SchemaDao::rollback()
{
    std::lock_guard<std::mutex> lock(this->locker);
    // std::sort(this->migrations.begin(), this->migrations.end());
    for (auto it = this->migrations.end(); it != this->migrations.begin(); it--)
    {
        if (it->run_at)
        {
            BOOST_LOG_TRIVIAL(info) << "find " << it->version << " " << it->name << ", rollback...";
            this->execute(it->down);
            this->delete_(it->version);
            return;
        }
    }
    BOOST_LOG_TRIVIAL(info) << "empty!";
}

void ashoka::orm::SchemaDao::generate(const std::string &name) const
{
    const auto root = this->root() / (ashoka::crypt::timestamp() + "-" + name);
    std_fs::create_directories(root);

    for (auto it : {"up.sql", "down.sql"})
    {
        const auto tmp = root / it;
        BOOST_LOG_TRIVIAL(info) << "generate file " << tmp;
        std::ofstream out(tmp);
    }
}

void ashoka::orm::SchemaDao::load()
{
    std::lock_guard<std::mutex> lock(this->locker);
    {
        std::ifstream sqlf(root() / "schema.sql");
        const std::string sql((std::istreambuf_iterator<char>(sqlf)),
                              std::istreambuf_iterator<char>());
        this->execute(sql);
    }

    for (const auto &dir : std_fs::directory_iterator(this->root()))
    {
        const auto root = dir.path();
        const std::string fn = root.filename().string();
        const std::string version = fn.substr(0, ASKOKA_TIMESTAMP_SIZE);
        const std::string name = fn.substr(ASKOKA_TIMESTAMP_SIZE + 1);

        std::ifstream upf(root / "up.sql");
        const std::string up((std::istreambuf_iterator<char>(upf)),
                             std::istreambuf_iterator<char>());
        std::ifstream downf(root / "down.sql");
        const std::string down((std::istreambuf_iterator<char>(downf)),
                               std::istreambuf_iterator<char>());
        this->migrations.push_back(Migration(version, name, up, down, this->run_at(version)));
    }
}

std_fs::path ashoka::orm::SchemaDao::root() const
{
    return (std_fs::path("db") / "migrations");
}
#include "orm.h"

#define ASHOKA_DB_MIGRATIONS_NAME "migrations"

ashoka::orm::Migration::Migration(
    const std::string version, const std::string name, const std::string up,
    const std::string down,
    const std::optional<boost::posix_time::ptime> run_at)
    : version(version), name(name), up(up), down(down), run_at(run_at) {}

bool ashoka::orm::Migration::operator<(const Migration &self) const {
  return version < self.version;
}

// -----------------------------

void ashoka::orm::SchemaDao::migrate() {
  std::lock_guard<std::mutex> lock(this->locker);
  for (auto it = this->migrations.begin(); it != this->migrations.end(); it++) {
    BOOST_LOG_TRIVIAL(info) << "find " << it->version << " " << it->name;
    if (!it->run_at) {
      BOOST_LOG_TRIVIAL(info) << "migrate...";
      this->execute(it->up);
      this->insert(it->version);
    }
  }
}

void ashoka::orm::SchemaDao::rollback() {
  std::lock_guard<std::mutex> lock(this->locker);
  std::reverse(this->migrations.begin(), this->migrations.end());
  for (auto it = this->migrations.begin(); it != this->migrations.end(); it++) {
    if (it->run_at) {
      BOOST_LOG_TRIVIAL(info)
          << "find " << it->version << " " << it->name << ", rollback...";
      this->execute(it->down);
      this->delete_(it->version);
      return;
    }
  }
  BOOST_LOG_TRIVIAL(info) << "empty!";
}

void ashoka::orm::SchemaDao::generate(const std::string &name) const {
  const auto root = this->root / ASHOKA_DB_MIGRATIONS_NAME /
                    (ashoka::crypt::timestamp() + "-" + name);
  std_fs::create_directories(root);

  for (auto it : {"up.sql", "down.sql"}) {
    const auto tmp = root / it;
    BOOST_LOG_TRIVIAL(info) << "generate file " << tmp;
    std::ofstream out(tmp);
  }
}

void ashoka::orm::SchemaDao::load() {
  std::lock_guard<std::mutex> lock(this->locker);

  for (const auto &dir :
       std_fs::directory_iterator(this->root / ASHOKA_DB_MIGRATIONS_NAME)) {
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
    this->migrations.push_back(
        Migration(version, name, up, down, this->run_at(version)));
  }
  std::sort(this->migrations.begin(), this->migrations.end());
}

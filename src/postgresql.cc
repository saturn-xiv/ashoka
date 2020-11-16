#include "postgresql.h"

void ashoka::postgresql::load_prepares(pqxx::connection &connection,
                                       const std_fs::path &path) {
  auto root = toml::parse_file(path.string());
  for (auto &&[k1, v1] : root) {
    if (v1.is_table()) {
      auto vt = v1.as_table();
      for (auto &&[k2, v2] : *vt) {
        std::optional<std::string> val = v2.value<std::string>();
        if (val) {
          connection.prepare(k1 + "." + k2, val.value());
        }
      }
    }
  }
}

// -----------------------

ashoka::postgresql::Connection::~Connection() {
  BOOST_LOG_TRIVIAL(debug) << "close postgresql connection";
  if (this->context) {
    this->context->close();
    this->context.reset();
  }
}

// -----------------------

ashoka::postgresql::Factory::Factory(const std::string host,
                                     const unsigned short int port,
                                     const std::string db,
                                     const std::string user,
                                     const std::optional<std::string> password,
                                     const std::optional<std_fs::path> prepares)
    : host(host), port(port), db(db), user(user), password(password),
      prepares(prepares) {
  BOOST_LOG_TRIVIAL(info) << "init postgresql connection factory";
}

std::shared_ptr<ashoka::pool::Connection>
ashoka::postgresql::Factory::create() {
  BOOST_LOG_TRIVIAL(debug) << "open postgresql " << name();
  std::stringstream ss;
  ss << "sslmode=disable"
     << " connect_timeout=" << 6 << " host=" << this->host
     << " port=" << this->port << " dbname=" << this->db
     << " user=" << this->user;
  if (this->password) {
    ss << " password=" << this->password.value();
  }

  std::shared_ptr<Connection> it(new Connection());
  it->context =
      std::shared_ptr<pqxx::connection>(new pqxx::connection(ss.str()));
  if (this->prepares) {
    ashoka::postgresql::load_prepares(*(it->context), this->prepares.value());
  }

  return std::static_pointer_cast<ashoka::pool::Connection>(it);
};

std::string ashoka::postgresql::Factory::name() const {
  std::ostringstream ss;
  ss << "tcp://" << user << "@" << host << ":" << port << "/" << db;
  return ss.str();
}

// ------------------------------
ashoka::postgresql::SchemaDao::SchemaDao(
    const std::shared_ptr<pqxx::connection> connection, const std_fs::path root)
    : connection(connection) {
  this->root = root;
  const auto schema = this->root / "schema";
  {
    std::ifstream sqlf(schema / "up.sql");
    const std::string sql((std::istreambuf_iterator<char>(sqlf)),
                          std::istreambuf_iterator<char>());
    this->execute(sql);
  }
  ashoka::postgresql::load_prepares(*(this->connection),
                                    schema / "prepares.toml");
}

void ashoka::postgresql::SchemaDao::execute(const std::string &script) const {
  BOOST_LOG_TRIVIAL(debug) << script;
  pqxx::work tx(*(this->connection));
  tx.exec(script);
  tx.commit();
}

std::optional<boost::posix_time::ptime>
ashoka::postgresql::SchemaDao::run_at(const std::string &version) const {
  pqxx::work tx(*(this->connection));
  pqxx::result rst =
      tx.exec_prepared("schema_migrations.find-by-version", version);
  tx.commit();

  for (auto it = rst.begin(); it != rst.end(); it++) {
    return ASHOKA_STR2TS(it["created_at"]);
  }

  return std::nullopt;
}
void ashoka::postgresql::SchemaDao::delete_(const std::string &version) const {
  pqxx::work tx(*(this->connection));
  pqxx::result rst =
      tx.exec_prepared("schema_migrations.delete-by-version", version);
  tx.commit();
}
void ashoka::postgresql::SchemaDao::insert(const std::string &version) const {

  pqxx::work tx(*(this->connection));
  pqxx::result rst = tx.exec_prepared("schema_migrations.insert", version);
  tx.commit();
}

// ------------------------
ashoka::postgresql::Config::Config()
    : host("127.0.0.1"), port(5432), user("postgres"), password(std::nullopt),
      db("dev"), pool_size(16) {}

std::string ashoka::postgresql::Config::name() const { return "postgresql"; }

ashoka::postgresql::Config::operator toml::table() const {
  toml::table root;
  root.insert("host", this->host);
  root.insert("port", this->port);
  root.insert("user", this->user);
  if (this->password) {
    root.insert("password", this->password.value());
  }
  root.insert("db", this->db);
  root.insert("pool-size", (long)this->pool_size);
  return root;
}

ashoka::postgresql::Config::Config(const toml::table &root) {
  std::optional<std::string> host = root["host"].value<std::string>();
  this->host = host.value_or("127.0.0.1");
  std::optional<unsigned short> port = root["port"].value<unsigned short>();
  this->port = port.value_or(5432);
  std::optional<std::string> user = root["user"].value<std::string>();
  this->user = user.value_or("postgres");
  std::optional<std::string> password = root["password"].value<std::string>();
  if (password) {
    this->password = std::optional<std::string>{password.value()};
  }
  std::optional<std::string> db = root["db"].value<std::string>();
  if (db) {
    this->db = db.value();
  }
  std::optional<size_t> pool_size = root["pool-size"].value<size_t>();
  this->pool_size = pool_size.value_or(20);
}

std::shared_ptr<ashoka::pool::Pool<ashoka::postgresql::Connection>>
ashoka::postgresql::Config::open(const std::optional<std_fs::path> &prepares) {
  std::shared_ptr<ashoka::postgresql::Factory> factory(
      new ashoka::postgresql::Factory(this->host, this->port, this->db,
                                      this->user, this->password, prepares));
  std::shared_ptr<ashoka::pool::Pool<ashoka::postgresql::Connection>> pool(
      new ashoka::pool::Pool<ashoka::postgresql::Connection>(this->pool_size,
                                                             factory));
  return pool;
}
#ifndef ASHOKA_POSTGRESQL_H_
#define ASHOKA_POSTGRESQL_H_

#include <pqxx/pqxx>

#include "orm.h"
#include "pool.h"

// https://www.postgresql.org/docs/current/libpq-connect.html
// https://libpqxx.readthedocs.io/en/latest/index.html
namespace ashoka
{
  namespace postgresql
  {
    void load_prepares(pqxx::connection &connection, const std_fs::path &path);

    class Connection : public ashoka::pool::Connection
    {
    public:
      ~Connection() override;

      friend class Factory;
      std::shared_ptr<pqxx::connection> context;

    private:
    };

    class Factory : public ashoka::pool::Factory
    {
    public:
      Factory(const std::string host, const unsigned short int port,
              const std::string db, const std::string user,
              const std::optional<std::string> password,
              const std::optional<std_fs::path> prepares);
      std::shared_ptr<ashoka::pool::Connection> create() override;
      std::string name() const override;

    private:
      const std::string host;
      const unsigned short int port;
      const std::string user;
      const std::optional<std::string> password;
      const std::string db;
      const std::optional<std_fs::path> prepares;
    };

    class Config : public ashoka::env::Config
    {
    public:
      Config();
      Config(const toml::table &root);

      operator toml::table() const override;
      std::string name() const override;
      std::shared_ptr<ashoka::pool::Pool<Connection>>
      open(const std::optional<std_fs::path> &prepares) const;

    private:
      std::string host;
      unsigned short int port;
      std::string user;
      std::optional<std::string> password;
      std::string db;
      size_t pool_size;
    };

    class SchemaDao : public ashoka::orm::SchemaDao
    {
    public:
      SchemaDao(const std::shared_ptr<pqxx::connection> connection,
                const std_fs::path root);

      void execute(const std::string &script) const override;
      void delete_(const std::string &version) const override;
      void insert(const std::string &version) const override;
      std::optional<boost::posix_time::ptime>
      run_at(const std::string &version) const override;

    private:
      const std::shared_ptr<pqxx::connection> connection;
    };

  } // namespace postgresql
} // namespace ashoka
#endif

#ifndef ASHOKA_ROUTER_H_
#define ASHOKA_ROUTER_H_

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <regex>
#include <tuple>

#include <boost/log/trivial.hpp>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/containerstream.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/producerconsumerstream.h>
#include <cpprest/uri.h>

#include "crypt.h"
#include "env.h"

namespace ashoka
{
  namespace api
  {

    class Config : public ashoka::env::Config
    {
    public:
      Config();
      Config(const toml::table &root);
      operator toml::table() const override;
      std::string name() const override;
      unsigned short get_port() { return port; }
      friend class Router;

    private:
      unsigned short port;
      std::string secrets;
    };

    class Context
    {

    public:
      Context(web::http::http_request request, const std::string secrets);
      std::string uri() const;
      std::string path() const;
      std::vector<std::string> paths() const;
      std::map<std::string, std::string> query() const;
      bool is_get() const;
      bool is_post() const;
      bool is_put() const;
      bool is_patch() const;
      bool is_delete() const;
      friend class Router;

    private:
      web::http::http_request request;
      const std::string secrets;
    };

    class Route
    {
    public:
      virtual void execute(Context &context) const = 0;
    };

    enum Method
    {
      GET,
      POST,
      PUT,
      PATCH,
      DELETE
    };

    class Router
    {
    public:
      Router(const std::string secrets, const std::string &host = "127.0.0.1", const short int port = 8080);
      Router(const Config &config);
      ~Router();
      void append(const Method method, const std::string path, const std::shared_ptr<Route>);
      void start();

    private:
      void handle(web::http::http_request request);
      void open(const std::string host, const short int port);

      web::http::experimental::listener::http_listener listener;
      std::vector<std::tuple<Method, std::string, std::shared_ptr<Route>>> routes;
      const std::string secrets;
    };
  } // namespace api

} // namespace ashoka

#endif
#include "server.h"
#include "nut.h"
#include "router.h"
#include "utils.h"

void ashoka::api::Server::listen() {}

ashoka::api::Server::Server(unsigned short int port) : port(port)
{
  // const std::string addr = "http://127.0.0.1:8080";
  // std::vector<ashoka::api::Handler> handlers;
  ashoka::api::Router router("aaa");
  router.open().wait();

  BOOST_LOG_TRIVIAL(debug) << "wait for close";
  std::cout << "input quit/exit to exit." << std::endl;
  std::string line;
  for (;;)
  {
    std::getline(std::cin, line);
    if (line == "quit" || line == "exit")
    {
      break;
    }
  }
  router.close().wait();
}

ashoka::api::Server::~Server() {}

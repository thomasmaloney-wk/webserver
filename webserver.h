#pragma once
#include <functional>
#include <netinet/in.h>
#include <regex>
#include <vector>

class logger; // forward declaration
struct HttpRequest;
class webserver {
private:
  bool should_exit;
  const int BUFFER_SIZE = 1024;
  sockaddr_in serv_addr;
  int portno;
  logger *log;

  struct route {
    std::regex pattern;
    std::function<std::string(const HttpRequest &)> handler;
  };

  std::vector<route> routes;

  /*
   * Handles an incoming connection.
   */
  void handle_connection(int newsockfd);

public:
  webserver(int port, logger *logger) : portno(port), log(logger) {}

  /*
   * Add a route handler for the server.
   */
  void add_route(const std::string &pattern,
                 std::function<std::string(const HttpRequest &)> handler);

  /*
   * Run the webserver.
   */
  void run();

  /*
   * Stops the server.
   */
  void kill();
};
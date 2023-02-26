#pragma once
#include <functional>
#include <netinet/in.h>
#include <regex>
#include <thread>
#include <vector>

class logger; // forward declaration
struct http_request;
class webserver {
private:
  bool running;
  const int BUFFER_SIZE = 1024;
  sockaddr_in serv_addr;
  int portno;
  logger *log;
  std::thread server_thread;

  struct route {
    std::regex pattern;
    std::function<std::string(const http_request &)> handler;
  };

  std::vector<route> routes;

  /*
   * Handles an incoming connection.
   */
  void handle_connection(int client_socket);

  /*
   * Sends message to client.
   */
  void send_message(int client_socket, std::string message);

  /*
   * Receives a message from the client.
   */
  std::string_view receive_message(int client_socket);

  /*
   * Run the webserver.
   */
  void run();

public:
  webserver(int port, logger *logger) : portno(port), log(logger) {}

  /*
   * Add a route handler for the server.
   */
  void add_route(const std::string &pattern,
                 std::function<std::string(const http_request &)> handler);

  /*
   * Starts the webserver.
   */
  void start();

  /*
   * Stops the webserver.
   */
  void stop();

  /*
   * Stops the server.
   */
  void kill();

  /*
   * Returns whether the server is still running or not.
   */
  bool is_running();
};
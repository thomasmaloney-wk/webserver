#pragma once
#include <functional>
#include <regex>
#include <vector>

class http_request;
class http_response;
class controller;

class route_handler {
private:
  route_handler() {};
  void init();
  struct route {
    std::regex pattern;
    std::function<http_response *(const http_request *)> handler;
    controller *controller_handler;
  };

  std::vector<route> routes;

public:
  static route_handler *create_route_handler();
  http_response *handle_request(http_request *request);

  /*
   * Add a route handler for the server.
   */
  void add_route(const std::string &pattern,
                 std::function<http_response *(const http_request *)> handler);

  /*
   * Add a route handler for the server.
   */
  void add_route(const std::string &pattern, controller *handler);
};
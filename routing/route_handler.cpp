
#include <regex>

#include "../controllers/about_controller.h"
#include "../controllers/index_controller.h"
#include "../controllers/shutdown_controller.h"
#include "../file_utils.h"
#include "../http/http_request.h"
#include "../http/http_response.h"
#include "route_handler.h"

http_response *create_html_response_from_string(std::string response_body) {
  // Construct the HTTP response
  return new http_response(http_response_code::OK, response_body, "text/html");
}

// Route handlers

http_response *shutdown_route_handler(const http_request *request) {
  const auto response_body = load_file("web/shutdown.html");
  return create_html_response_from_string(response_body);
}

http_response *echo_route_handler(const http_request *request) {
  auto url = request->raw_url();
  auto route = url.substr(5);
  const auto response_body = "<html><head/><body>" + route + "</body></html>";
  return create_html_response_from_string(response_body);
}

http_response *not_found_route_handler(const http_request *request) {
  const auto response_body = load_file("web/404.html");
  auto response = http_response::not_found(response_body);
  return (http_response *)response;
}

void route_handler::init() {
  // initialize routes
  add_route("/", new index_controller());
  add_route("/About(/.*)*", new about_controller());
  add_route("/Shutdown", new shutdown_controller());
  add_route("/Echo/.*", echo_route_handler);
}

route_handler *route_handler::create_route_handler() {
  auto router = new route_handler();
  router->init();
  return router;
}

http_response *route_handler::handle_request(http_request *request) {
  route _route = {std::regex("/404"), not_found_route_handler};
  for (const auto &route : routes) {
    if (std::regex_match(request->raw_url(), route.pattern)) {
      _route = route;
      break;
    }
  }

  return _route.controller_handler != nullptr
             ? _route.controller_handler->handle_request(request)
             : _route.handler(request);
}

void route_handler::add_route(
    const std::string &pattern,
    std::function<http_response *(const http_request *)> handler) {
  routes.push_back({std::regex(pattern), handler});
}

void route_handler::add_route(const std::string &pattern, controller *handler) {
  routes.push_back({std::regex(pattern), nullptr, handler});
}

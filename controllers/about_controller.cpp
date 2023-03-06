#include "about_controller.h"
#include "../file_utils.h"
#include "../http/http_request.h"
#include "../http/http_response.h"
#include "../routing/relative_url.h"
#include <iostream>
http_response *about_controller::home() {
  const auto response_body = load_file("web/about.html");
  return new http_response(http_response_code::OK, response_body, "text/html");
}

http_response *about_controller::teapot() {
  auto response_body = load_file("web/teapot.html");
  return new http_response(http_response_code::TEAPOT, response_body,
                           "text/html");
}

http_response *about_controller::handle_request(http_request *request) {
  auto path = request->url()->path();
  std::cout << "path: " << path << std::endl;
  if (path == "/About")
    return home();
  if (path == "/About/Teapot")
    return teapot();
  return (http_response *)http_response::not_found();
}
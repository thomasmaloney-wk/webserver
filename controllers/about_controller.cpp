#include "about_controller.h"
#include "../file_utils.h"
#include "../http_request.h"
#include "../http_response.h"

http_response *about_controller::home() {
  const auto response_body = load_file("web/about.html");
  return new http_response(http_response::OK, response_body, "text/html");
}

http_response *about_controller::handle_request(http_request *request) {
  return home();
}
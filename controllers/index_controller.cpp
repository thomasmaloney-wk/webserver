#include "index_controller.h"
#include "../file_utils.h"
#include "../http/http_request.h"
#include "../http/http_response.h"

http_response *index_controller::home() {
  const auto response_body = load_file("web/index.html");
  return new http_response(http_response::OK, response_body, "text/html");
}

http_response *index_controller::handle_request(http_request *request) {
  if (request->method != "GET") {
    return (http_response *)http_response::method_not_allowed({"GET"});
  }
  return home();
}
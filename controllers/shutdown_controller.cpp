#include "shutdown_controller.h"
#include "../file_utils.h"
#include "../http/http_request.h"
#include "../http/http_response.h"

http_response *shutdown_controller::home() { 
  const auto response_body = load_file("web/shutdown.html");
  return new http_response(http_response_code::OK, response_body, "text/html");
}

http_response *shutdown_controller::shutdown() { return nullptr; }

http_response *shutdown_controller::handle_request(http_request *request) {
  return nullptr;
}
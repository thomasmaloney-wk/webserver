#include "http_response.h"
#include <sstream>

std::string http_response::to_string() const {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << status_code << " " << status_code_to_string(status_code)
      << "\r\n";
  oss << "Content-Type: " << content_type << "\r\n";
  oss << "Content-Length: " << body.length() << "\r\n";
  oss << "\r\n";
  oss << body;
  return oss.str();
}

std::string http_response::status_code_to_string(int status_code) {
  switch (status_code) {
  case http_response::OK:
    return "OK";
  case http_response::CREATED:
    return "Created";
  case http_response::NO_CONTENT:
    return "No Content";
  case http_response::BAD_REQUEST:
    return "Bad Request";
  case http_response::UNAUTHORIZED:
    return "Unauthorized";
  case http_response::FORBIDDEN:
    return "Forbidden";
  case http_response::NOT_FOUND:
    return "Not Found";
  case http_response::INTERNAL_SERVER_ERROR:
    return "Internal Server Error";
  default:
    return "Unknown Status Code";
  }
}

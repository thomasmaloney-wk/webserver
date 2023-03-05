#include "http_response.h"
#include <sstream>

const http_response *
http_response::method_not_allowed(std::vector<std::string> allowed_methods) {
  const std::string body =
      "<html><body><h1>405 Method Not Allowed</h1></body></html>";
  return new http_response(http_response::METHOD_NOT_ALLOWED, body,
                           "text/html");
}

const http_response *http_response::not_found(const std::string &body_text) {
  return new http_response(http_response::NOT_FOUND, body_text, "text/html");
}

http_response::http_response(int status_code, const std::string &body,
                             const std::string &content_type)
    : status_code(status_code), body(body), content_type(content_type) {
  // Set default headers
  headers["Content-Length"] = std::to_string(body.size());
  headers["Content-Type"] = content_type;
}

std::string http_response::to_string_deprecated() const {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << status_code << " " << status_code_to_string(status_code)
      << "\r\n";
  oss << "Content-Type: " << content_type << "\r\n";
  oss << "Content-Length: " << body.length() << "\r\n";
  oss << "\r\n";
  oss << body;
  return oss.str();
}

void http_response::add_header(std::string key, std::string value) {
  // for now don't bother with collisions
  headers.insert({key, value});
}

std::string http_response::to_string() const {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << status_code << " " << status_code_to_string(status_code)
      << "\r\n";
  for (const auto &[name, value] : headers) {
    oss << name << ": " << value << "\r\n";
  }
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

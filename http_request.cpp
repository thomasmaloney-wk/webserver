#include <sstream>

#include "http_request.h"

HttpRequest parse_request(std::string_view request) {
  HttpRequest http_request;

  // find the position of the first and second space characters
  size_t first_space_pos = request.find(' ');
  if (first_space_pos == std::string_view::npos) {
    return http_request;
  }

  size_t second_space_pos = request.find(' ', first_space_pos + 1);
  if (second_space_pos == std::string_view::npos) {
    return http_request;
  }

  // extract the request method and URL
  http_request.method = std::string(request.substr(0, first_space_pos));
  http_request.url = std::string(request.substr(
      first_space_pos + 1, second_space_pos - first_space_pos - 1));

  // find the position of the end of the headers section
  size_t headers_end_pos = request.find("\r\n\r\n", second_space_pos);
  if (headers_end_pos == std::string_view::npos) {
    return http_request;
  }

  // extract the headers section of the request
  std::string_view headers_section = request.substr(
      second_space_pos + 1, headers_end_pos - second_space_pos - 1);

  // parse the headers section and extract the key-value pairs
  std::stringstream headers_stream((std::string(headers_section)));
  std::string header_line;
  while (std::getline(headers_stream, header_line)) {
    size_t separator_pos = header_line.find(':');
    if (separator_pos != std::string_view::npos) {
      std::string key = std::string(header_line.substr(0, separator_pos));
      std::string value = std::string(header_line.substr(separator_pos + 1));
      http_request.headers[key] = value;
    }
  }

  // extract the body of the request
  if (headers_end_pos + 4 < request.size()) {
    http_request.body = std::string(request.substr(headers_end_pos + 4));
  }

  return http_request;
}
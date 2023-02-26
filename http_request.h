#pragma once
#include <map>
#include <string>

struct HttpRequest {
  std::string method;
  std::string url;
  std::map<std::string, std::string> headers;
  std::string body;
};

HttpRequest parse_request(std::string_view request);
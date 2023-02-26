#pragma once
#include <map>
#include <string>

struct http_request {
  std::string method;
  std::string url;
  std::map<std::string, std::string> headers;
  std::string body;
};

http_request parse_request(std::string_view request);
#pragma once
#include <unordered_map>
#include <string>

class relative_url;
class http_request {
  private:
  http_request() {};
  std::string _method;
  std::string _raw_url;
  relative_url *_url;
  std::unordered_map<std::string, std::string> _headers;
  std::string _body;

public:
 static http_request *parse_request(std::string_view request);

 const std::string raw_url() const;
 const std::string method() const;
 const std::string body() const;
 const std::unordered_map<std::string, std::string> headers() const;
 const relative_url *url() const;
};
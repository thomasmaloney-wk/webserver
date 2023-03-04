#pragma once
struct http_request;
class http_response;

class controller {
public:
  virtual http_response *handle_request(http_request *request) = 0;
};
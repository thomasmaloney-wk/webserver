#include <string>
#include <unordered_map>
#include <vector>

enum http_response_code {
  OK = 200,
  CREATED = 201,
  NO_CONTENT = 204,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  TEAPOT = 418,
  INTERNAL_SERVER_ERROR = 500
};
class http_response {
public:
  static inline std::string SHUTDOWN_HEADER_KEY = "Hacky-Shutdown-Override";

  static const http_response *
  method_not_allowed(std::vector<std::string> allowed_methods);
  static const http_response *
  not_found(const std::string &body_text = "404 Not Found");

  http_response_code status_code;
  std::string body;
  std::string content_type;
  std::unordered_map<std::string, std::string> headers;

  http_response(http_response_code status_code, const std::string &body,
                const std::string &content_type = "text/plain");
  std::string to_string() const;

  void add_header(std::string key, std::string value);

private:
  static std::string status_code_to_string(http_response_code status_code);
};
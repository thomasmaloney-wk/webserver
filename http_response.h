#include <string>
#include <unordered_map>
#include <vector>
class http_response {
public:
  static const int OK = 200;
  static const int CREATED = 201;
  static const int NO_CONTENT = 204;
  static const int BAD_REQUEST = 400;
  static const int UNAUTHORIZED = 401;
  static const int FORBIDDEN = 403;
  static const int NOT_FOUND = 404;
  static const int METHOD_NOT_ALLOWED = 405;
  static const int INTERNAL_SERVER_ERROR = 500;

  static const http_response* method_not_allowed(std::vector<std::string> allowed_methods);
  static const http_response* not_found(const std::string &body_text = "404 Not Found");

  int status_code;
  std::string body;
  std::string content_type;
  std::unordered_map<std::string, std::string> headers;

  http_response(int status_code, const std::string &body,
                const std::string &content_type = "text/plain");
  std::string to_string() const;
  std::string to_string_deprecated() const;

private:
  static std::string status_code_to_string(int status_code);
};
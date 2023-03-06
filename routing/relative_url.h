#pragma once
#include <string>
#include <unordered_map>

class relative_url {
private:
  std::string _path;
  std::unordered_map<std::string, std::string> _queryParams;
  std::string _fragment;

  bool _malformed;
  bool parse(const std::string& url);

public:
  relative_url(const std::string& url);
  const std::string path() const;
  const std::string fragment() const;
  const std::unordered_map<std::string, std::string> query_params() const;
};
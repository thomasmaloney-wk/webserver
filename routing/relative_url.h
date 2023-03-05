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
};
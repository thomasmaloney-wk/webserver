#include <regex>

#include "relative_url.h"

bool relative_url::parse(const std::string &url) {
  std::regex regex("(https?)://([^/]+)(/.*)\\?(.*)#(.*)");
  std::smatch match;

  if (!std::regex_match(url, match, regex)) {
    return true;
  }
  _path = match[1];
  std::string query = match[2];
  _fragment = match[3];

  // Parse the query string into a map
  std::regex queryRegex("([^&=]+)=([^&]*)");
  auto queryBegin =
      std::sregex_iterator(query.begin(), query.end(), queryRegex);
  auto queryEnd = std::sregex_iterator();
  for (auto it = queryBegin; it != queryEnd; ++it) {
    std::string key = it->str(1);
    std::string value = it->str(2);
    _queryParams[key] = value;
  }

  return false;
}

relative_url::relative_url(const std::string &url) { _malformed = parse(url); }

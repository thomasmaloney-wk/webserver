#include "file_utils.h"

std::string load_file(const std::filesystem::path &path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }
  return std::string(std::istreambuf_iterator<char>(file), {});
}
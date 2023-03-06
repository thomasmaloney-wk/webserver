#include "logger.h"
#include "http/http_request.h"

logger::~logger() {
  if (!is_stdio)
    delete outstream;
}

void logger::log_info(std::string info) {
  (*outstream) << info_prefix << " " << info << std::endl;
}

void logger::log_http_request(http_request *request) {
  log_info("Requested Method: " + request->method());
  log_info("Requested URL: " + request->raw_url());
}

void logger::log_err(std::string err) {
  *(is_stdio ? &std::cerr : outstream)
      << error_prefix << " " << err << std::endl;
}
#include "logger.h"
#include "http_request.h"

logger::~logger() {
  if (!is_stdio)
    delete outstream;
}

void logger::log_info(std::string info) { (*outstream) << info; }

void logger::log_http_request(http_request request) {
  log_info(LOG_INFO + " Requested Method: ");
  log_info(request.method);
  log_info("\n");
  log_info(LOG_INFO + " Requested URL: ");
  log_info(request.url);
  log_info("\n");
}

void logger::log_err(std::string err) {
  *(is_stdio ? &std::cerr : outstream) << err;
}
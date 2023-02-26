#include "logger.h"

logger::~logger() {
  if (!is_stdio)
    delete outstream;
}

void logger::log_info(std::string info) { (*outstream) << info; }

void logger::log_err(std::string err) {
  *(is_stdio ? &std::cerr : outstream) << err;
}
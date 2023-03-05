#include "controller.h"

class index_controller : public controller {
public:
  http_response *home();
  http_response *handle_request(http_request *request);
};
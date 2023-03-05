#include "controller.h"

class shutdown_controller : public controller {
public:
  http_response *home();
  http_response *shutdown();
  http_response *handle_request(http_request *request);
};
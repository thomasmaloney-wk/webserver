#include "controller.h"

class about_controller : public controller {
public:
  about_controller() : controller("/About") {}
  http_response *home();
  http_response *handle_request(http_request *request);
};
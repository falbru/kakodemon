#include "application.hpp"
#include <memory>
int main(void)
{
    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->init();
    app->run();
    return 0;
}

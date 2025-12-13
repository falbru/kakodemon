#include "application/application.hpp"
#include "adapters/opengl/application.hpp"
#include <memory>

int main(void)
{
    std::unique_ptr<Application> app = std::make_unique<opengl::GLFWApplication>();
    app->init();
    app->run();
    return 0;
}

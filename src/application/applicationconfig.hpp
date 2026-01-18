#pragma once

#include <string>

struct ApplicationConfig
{
    bool maximized = false;
};

std::string getConfigFilePath();
ApplicationConfig loadApplicationConfig();
void saveApplicationConfig(const ApplicationConfig &config);

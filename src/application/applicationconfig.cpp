#include "applicationconfig.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

std::string getConfigFilePath()
{
    std::string config_dir;
    const char *xdg_config_home = std::getenv("XDG_CONFIG_HOME");
    if (xdg_config_home && xdg_config_home[0] != '\0')
    {
        config_dir = xdg_config_home;
    }
    else
    {
        const char *home = std::getenv("HOME");
        if (home)
        {
            config_dir = std::string(home) + "/.config";
        }
        else
        {
            config_dir = ".";
        }
    }
    return config_dir + "/kakodemon/config.json";
}

ApplicationConfig loadApplicationConfig()
{
    ApplicationConfig config;
    std::string path = getConfigFilePath();

    if (!fs::exists(path))
    {
        return config;
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        return config;
    }

    try
    {
        nlohmann::json json_data;
        file >> json_data;
        if (json_data.contains("maximized"))
        {
            config.maximized = json_data["maximized"].get<bool>();
        }
    }
    catch (...)
    {
    }

    return config;
}

void saveApplicationConfig(const ApplicationConfig &config)
{
    std::string path = getConfigFilePath();
    fs::path config_path(path);
    fs::path config_dir = config_path.parent_path();

    if (!fs::exists(config_dir))
    {
        fs::create_directories(config_dir);
    }

    nlohmann::json json_data;
    json_data["maximized"] = config.maximized;

    std::ofstream file(path);
    if (file.is_open())
    {
        file << json_data.dump(2);
    }
}

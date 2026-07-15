#include <string>

#include "kakounesession.hpp"

bool domain::kakouneSessionExists(const std::string& session_id)
{
    FILE* pipe = popen("kak -l", "r");
    if (!pipe)
    {
        return false;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        std::string line(buffer);
        size_t newline_pos = line.find_last_not_of("\r\n");
        if (newline_pos != std::string::npos)
        {
            line = line.substr(0, newline_pos + 1);
        }
        if (line == session_id)
        {
            pclose(pipe);
            return true;
        }
    }

    pclose(pipe);
    return false;
}

#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "kakouneclientprocess.hpp"
#include "line.hpp"

struct KakouneClient
{
    std::vector<Line> window_content;
    std::unique_ptr<KakouneClientProcess> process;
};

#endif

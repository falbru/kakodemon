#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "kakoune/face.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "kakoune/line.hpp"

struct KakouneClient
{
    std::vector<kakoune::Line> window_content;
    kakoune::Face window_default_face;
    std::unique_ptr<KakouneClientProcess> process;
};

#endif

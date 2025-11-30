#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "core/editor.hpp"
#include "kakoune/face.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "kakoune/line.hpp"
#include "kakoune/menustyle.hpp"

struct KakouneClient
{
    std::vector<kakoune::Line> window_content;
    kakoune::Face window_default_face;
    core::StatusLine status_line;
    kakoune::Line mode_line;
    kakoune::Face status_default_face;
    std::unique_ptr<KakouneClientProcess> process;

    core::CursorPosition cursor_pos;

    bool menu_visible;
    int menu_selected_index = -1;
    std::vector<kakoune::Line> menu_items;
    kakoune::Coord menu_anchor;
    kakoune::Face menu_face;
    kakoune::Face menu_selected_face;
    kakoune::MenuStyle menu_style;
};

#endif

#ifndef MODEL_VIEWSTATE_HPP_INCLUDED
#define MODEL_VIEWSTATE_HPP_INCLUDED

struct InputViewState
{
    float scroll_offset = 0.0f;
};

struct MenuViewState
{
    InputViewState input_state;
    int items_scroll_offset = 0;
};

struct InfoBoxViewState
{
    float scroll_offset = 0.0f;
};

#endif

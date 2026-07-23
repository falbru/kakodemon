# Kakodemon

> Kakodemon is an OpenGL-based GUI for Kakoune written in C++

<img width="1337" height="991" alt="Screenshot of Kakodemon" src="https://github.com/user-attachments/assets/9cb69cb7-08a2-4fe6-bcc2-d3492c3b9dee" />

## Usage
- Create new splits with `:kakodemon-new-client <commands>` (aliased to `:new`)
- Focus splits with `:kakodemon-focus <kakoune_client>` (aliased to `:focus`)
- Cycle between splits using the keybindings `<c-s-j>` and `<c-s-k>`
- Adjust the master ratio with `<c-s-h>` and `<c-s-l>`
- Adjust the number of masters with `<c-s-i>` and `<c-s-d>`
- Toggle full layout with `<c-s-ret>`
- Set the layout with `:kakodemon-layout <layout>`. Possible options are:
    - `tall`: the master is to the left, and the stack is stacked vertically to the right
    - `wide`: the master is on the top, and the stack is stacked horizontally below
    - `full`: the focused split takes up the entire window

## Install from source

1. Install the necessary build dependencies

```sh
sudo apt install -y git build-essential cmake libwayland-dev libxkbcommon-dev xorg-dev libfreetype-dev libfontconfig-dev
```

2. Clone the repository

```sh
git clone --recurse-submodules https://github.com/falbru/kakodemon
```

3. Build and install the project

`cd` into the newly cloned kakodemon project, then:

```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo install -D ./kakod /usr/local/bin
```

4. Add the following line to your `kakrc`

```kak
eval %sh{ kakod --init }
```

You can then run the application with the command `kakod`

> [!IMPORTANT]
> Make sure Kakoune version>=2026.04.12 is installed on your system!

## Configuration

Kakodemon is configured by setting the option `ui_options` in Kakoune. The following options are implemented:

### Font Options

- `kakodemon_font <font_name>`: Set the default font for all components
- `kakodemon_font_content <font_name>`: Set the font for editor content
- `kakodemon_font_menu <font_name>`: Set the font for menus
- `kakodemon_font_infobox <font_name>`: Set the font for info boxes
- `kakodemon_font_statusbar <font_name>`: Set the font for the status bar

### Color Options

- `kakodemon_color_<color_name> <color_value>`: Set terminal colors
  - Standard colors: `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`
  - Bright colors: `bright_black`, `bright_red`, `bright_green`, `bright_yellow`, `bright_blue`, `bright_magenta`, `bright_cyan`, `bright_white`
- `kakodemon_color_border <color_value>`: Set the color of UI borders

### Example Configuration

```kak
set-option global ui_options \
    'kakodemon_font=JetBrains Mono 10' \
    'kakodemon_font_content=JetBrains Mono 14' \
    'kakodemon_color_red=rgb:FF5555' \
    'kakodemon_color_green=rgb:50FA7B' \
    'kakodemon_color_border=rgb:44475A'
```


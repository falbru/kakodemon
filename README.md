# Kakodemon

Kakodemon is an OpenGL-based GUI for Kakoune written in C++

<img width="1920" height="1131" alt="Screenshot From 2026-01-13" src="https://github.com/user-attachments/assets/8fb194ef-3f7e-4cf0-be51-002af1493bcd" />

## Install from source

1. Install the necessary build dependencies

```sh
sudo apt install -y git build-essential cmake libwayland-dev libxkbcommon-dev xorg-dev libfreetype-dev libfontconfig-dev
```

2. Clone the repository

```sh
git clone https://github.com/falbru/kakodemon
```

3. Build and install the project

```sh
cd kakodemon
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo install -D ./kakod /usr/local/bin
```

You can then run the application with the command `kakod`

> **Warning**<br>
Kakodemon requires a version of Kakoune that has not yet released. Ensure that you've built kakoune with the commit `3dd6f30` or newer.

## Configuration

Kakodemon is configured by setting the option `ui_options` in Kakoune. The following options are implemented:

### Font Options

- `kakodemon_set_font <font_name>`: Set the default font for all components
- `kakodemon_set_font_content <font_name>`: Set the font for editor content
- `kakodemon_set_font_menu <font_name>`: Set the font for menus
- `kakodemon_set_font_infobox <font_name>`: Set the font for info boxes
- `kakodemon_set_font_statusbar <font_name>`: Set the font for the status bar

### Color Options

- `kakodemon_set_color_<color_name> <color_value>`: Set terminal colors
  - Standard colors: `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`
  - Bright colors: `bright_black`, `bright_red`, `bright_green`, `bright_yellow`, `bright_blue`, `bright_magenta`, `bright_cyan`, `bright_white`
- `kakodemon_set_color_border <color_value>`: Set the color of UI borders

### Example Configuration

```kak
set-option global ui_options \
    'kakodemon_set_font_content=JetBrains Mono 14' \
    'kakodemon_set_font_menu=JetBrains Mono 10' \
    'kakodemon_set_color_red=rgb:FF5555' \
    'kakodemon_set_color_green=rgb:50FA7B' \
    'kakodemon_set_color_border=rgb:44475A'
```


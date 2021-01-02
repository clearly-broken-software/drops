HACKING
=======

## Source map

├── bin                         # Build output directory
├── build                       # Build temporary directory
│   └── drops
├── dpf                         # DISTRHO Plugin Framework library, https://distrho.github.io/DPF/
├── plugins
│   └── drops                   # Main source directory
│       ├── artwork
│       │   ├── loopend.png
│       │   └── loopstart.png
│       ├── external            # Nano SVG parser, https://github.com/memononen/nanosvg
│       │   ├── example
│       │   │   ├── 23.svg
│       │   │   ├── drawing.svg
│       │   │   ├── example1.c
│       │   │   ├── example2.c
│       │   │   ├── nano.svg
│       │   │   ├── screenshot-1.png
│       │   │   ├── screenshot-2.png
│       │   │   └── stb_image_write.h
│       │   ├── src
│       │   │   ├── nanosvg.h
│       │   │   └── nanosvgrast.h
│       │   ├── LICENSE.txt
│       │   ├── premake4.lua
│       │   └── README.md
│       ├── res                       # Graphics resources
│       │   ├── svg_icons
│       │   │   ├── clearly_broken.svg
│       │   │   ├── drops_logo.svg
│       │   │   ├── loop_left.svg
│       │   │   ├── loop_right.svg
│       │   │   ├── zoom_all.svg
│       │   │   ├── zoom_in.svg
│       │   │   ├── zoom_loop.svg
│       │   │   └── zoom_out.svg
│       │   ├── basic.sfz
│       │   ├── DROPS_8_4_info.png
│       │   ├── DROPS_8_4.svg
│       │   ├── DROPS_9_5.png
│       │   ├── DROPS_9_5.svg
│       │   ├── DropsColors.png
│       │   ├── Drops Extra Colors.png
│       │   ├── Drops.gpl
│       │   ├── drops-icons-v1.0.zip
│       │   ├── drops_logo.svg
│       │   ├── extra_colors.css
│       │   ├── flexeg.sfz
│       │   ├── fonts.cpp
│       │   ├── fonts.hpp
│       │   ├── loop_left.svg
│       │   ├── loop_right.svg
│       │   ├── saw.wav
│       │   ├── zoom_fit_all.svg
│       │   ├── zoom_in.svg
│       │   ├── zoom_loop.svg
│       │   └── zoom_out.svg
│       ├── widgets                 # UI Widget code, based on DPF and NanoVG
│       │   ├── src
│       │   │   ├── DropDown.cpp
│       │   │   ├── FileOpenButton.cpp
│       │   │   ├── HBox.cpp
│       │   │   ├── Knob.cpp
│       │   │   ├── Menu.cpp
│       │   │   ├── RadioButton.cpp
│       │   │   ├── ScrollBar.cpp
│       │   │   ├── Slider.cpp
│       │   │   ├── SVGButton.cpp
│       │   │   ├── TextButton.cpp
│       │   │   └── VBox.cpp
│       │   ├── DropDown.hpp
│       │   ├── FileOpenButton.hpp
│       │   ├── HBox.hpp
│       │   ├── Knob.hpp
│       │   ├── Menu.hpp
│       │   ├── RadioButton.hpp
│       │   ├── ScrollBar.hpp
│       │   ├── Slider.hpp
│       │   ├── SVGButton.hpp
│       │   ├── TextButton.hpp
│       │   └── VBox.hpp
│       ├── Artwork.cpp
│       ├── Artwork.hpp
│       ├── DistrhoPluginInfo.h     # DPF plugin definitions
│       ├── DropsColors.hpp         # Drops color definitions
│       ├── DropsPlugin.cpp         # **Main Drops audio engine code**
│       ├── DropsPlugin.hpp
│       ├── DropsUI.cpp             # **Main Drops UI code**
│       ├── DropsUI.hpp
│       ├── icon.svg
│       ├── initTabAmp.cpp          # UI Tab definitions and handling
│       ├── initTabFilter.cpp
│       ├── initTabPitch.cpp
│       ├── initTabSample.cpp
│       ├── Makefile                # Makefile for plugin
│       ├── minimp3_ex.h            # mp3 loading library, https://github.com/lieff/minimp3
│       ├── minimp3.h
│       ├── SVG_Icons.cpp           # SVG icon definitions
│       ├── SVG_Icons.hpp
│       ├── SVGImage.cpp            # SVG rendering code
│       └── SVGImage.hpp
├── sfizz                           # Sfizz synthesis/sampling library, https://sfz.tools/sfizz/
├── 1234.wav
├── clearly_broken.svg
├── drops_icons.ttf
├── icon.svg
├── LICENSE
├── Makefile                        # Main entry point for building plugin/software
├── pugl_stuff.txt
├── README.md
└── ROADMAP.md

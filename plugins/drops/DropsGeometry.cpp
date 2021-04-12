/*
    Drops - Drops Really Only Plays Samples
    Copyright (C) 2021  Rob van den Berg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "DropsGeometry.hpp"

START_NAMESPACE_DISTRHO

 constexpr unsigned int UI_W = 1000;
 constexpr unsigned int UI_H = 550;
 constexpr unsigned int display_left = 35;
 constexpr unsigned int display_top = 107;
 constexpr unsigned int display_width = UI_W - (2 * display_left);
 constexpr unsigned int display_right = display_left + display_width;
 constexpr unsigned int display_height = 183;
 constexpr unsigned int display_bottom = display_top + display_height;
 constexpr unsigned int display_center = (display_bottom - display_top) / 2 + display_top;
 constexpr unsigned int minimap_height = 30;
 constexpr unsigned int sample_inout_font_size = 12;

 END_NAMESPACE_DISTRHO
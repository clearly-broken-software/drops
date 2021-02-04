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
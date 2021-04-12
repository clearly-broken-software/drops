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
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabPitch()
{
    Window &window = getParentWindow();
    const float margin = 2.f;
    const float gauge = 6.f;
    const float font_size = 14.f;
    const uint sz = 60;
    //const Size<uint> knobSize = Size<uint>(sz + font_size, sz);
    const Size<uint> knobSize = Size<uint>(sz, sz);

    vbox_pitch = new VBox(window);
    vbox_pitch->setId(kVBoxPitch);
    vbox_pitch->setAbsolutePos(667, 329);
    vbox_pitch->setSize(323, 211);

    hbox_pitch_row_1 = new HBox(vbox_pitch);
    hbox_pitch_row_1->setId(kHBoxPitchRow1);
    hbox_pitch_row_1->setWidth(vbox_pitch->getWidth());
    hbox_pitch_row_1->setHeight(sz);

    hbox_pitch_row_2 = new HBox(vbox_pitch);
    hbox_pitch_row_2->setId(kHBoxPitchRow2);
    hbox_pitch_row_2->setHeight(sz);
    hbox_pitch_row_2->setWidth(vbox_pitch->getWidth() * .95);

    fPitchEgDepth = new Knob(hbox_pitch_row_1);
    fPitchEgDepth->setId(kPitchEgDepth);
    fPitchEgDepth->setSize(knobSize);
    fPitchEgDepth->setCallback(this);
    fPitchEgDepth->labelSize = font_size;
    fPitchEgDepth->gauge_width = gauge;
    fPitchEgDepth->margin = margin;
    fPitchEgDepth->label = "DEPTH";
    fPitchEgDepth->foreground_color = shamrock_green;
    fPitchEgDepth->background_color = black_olive;
    fPitchEgDepth->highlight_color = shamrock_green_1;
    fPitchEgDepth->text_color = floral_white;
    fPitchEgDepth->real_min = 0.0f;
    fPitchEgDepth->real_max = 2400.0f;
    fPitchEgDepth->format_str = "%.f Ct";
    fPitchEgDepth->setParamOnMove = true;

    fPitchEgAttack = new Knob(hbox_pitch_row_1);
    fPitchEgAttack->setId(kPitchEgAttack);
    fPitchEgAttack->setSize(knobSize);
    fPitchEgAttack->setCallback(this);
    fPitchEgAttack->labelSize = font_size;
    fPitchEgAttack->gauge_width = gauge;
    fPitchEgAttack->margin = margin;
    fPitchEgAttack->label = "ATTACK";
    fPitchEgAttack->foreground_color = shamrock_green;
    fPitchEgAttack->background_color = black_olive;
    fPitchEgAttack->highlight_color = shamrock_green_1;
    fPitchEgAttack->text_color = floral_white;
    fPitchEgAttack->real_min = 0.0f;
    fPitchEgAttack->real_max = 10.0f;
    fPitchEgAttack->min = 1.0f;
    fPitchEgAttack->max = 101.f;
    fPitchEgAttack->format_str = "%.2f s";
    fPitchEgAttack->using_log = true;
    fPitchEgAttack->setParamOnMove = true;

    fPitchEgDecay = new Knob(hbox_pitch_row_1);
    fPitchEgDecay->setId(kPitchEgDecay);
    fPitchEgDecay->setSize(knobSize);
    fPitchEgDecay->setCallback(this);
    fPitchEgDecay->labelSize = font_size;
    fPitchEgDecay->gauge_width = gauge;
    fPitchEgDecay->margin = margin;
    fPitchEgDecay->label = "DECAY";
    fPitchEgDecay->foreground_color = shamrock_green;
    fPitchEgDecay->background_color = black_olive;
    fPitchEgDecay->highlight_color = shamrock_green_1;
    fPitchEgDecay->text_color = floral_white;
    fPitchEgDecay->real_min = 0.0f;
    fPitchEgDecay->real_max = 10.f;
    fPitchEgDecay->min = 1.0f;
    fPitchEgDecay->max = 101.f;
    fPitchEgDecay->using_log = true;
    fPitchEgDecay->format_str = "%.2f s";
    fPitchEgDecay->setParamOnMove = true;

    fPitchEgSustain = new Knob(hbox_pitch_row_1);
    fPitchEgSustain->setId(kPitchEgSustain);
    fPitchEgSustain->setSize(knobSize);
    fPitchEgSustain->setCallback(this);
    fPitchEgSustain->labelSize = font_size;
    fPitchEgSustain->gauge_width = gauge;
    fPitchEgSustain->margin = margin;
    fPitchEgSustain->label = "SUSTAIN";
    fPitchEgSustain->foreground_color = shamrock_green;
    fPitchEgSustain->background_color = black_olive;
    fPitchEgSustain->highlight_color = shamrock_green_1;
    fPitchEgSustain->text_color = floral_white;
    fPitchEgSustain->real_min = 0.0f;
    fPitchEgSustain->real_max = 100.f;
    fPitchEgSustain->format_str = "%.f%%";
    fPitchEgSustain->setParamOnMove = true;

    fPitchEgRelease = new Knob(hbox_pitch_row_1);
    fPitchEgRelease->setId(kPitchEgRelease);
    fPitchEgRelease->setSize(knobSize);
    fPitchEgRelease->setCallback(this);
    fPitchEgRelease->labelSize = font_size;
    fPitchEgRelease->gauge_width = gauge;
    fPitchEgRelease->margin = margin;
    fPitchEgRelease->label = "RELEASE";
    fPitchEgRelease->foreground_color = shamrock_green;
    fPitchEgRelease->background_color = black_olive;
    fPitchEgRelease->highlight_color = shamrock_green_1;
    fPitchEgRelease->text_color = floral_white;
    fPitchEgRelease->real_min = 0.0f;
    fPitchEgRelease->real_max = 10.f;
    fPitchEgRelease->min = 1.0f;
    fPitchEgRelease->max = 101.f;
    fPitchEgRelease->format_str = "%.2f s";
    fPitchEgRelease->using_log = true;
    fPitchEgRelease->setParamOnMove = true;

    hbox_pitch_row_1->addWidget(fPitchEgDepth);
    hbox_pitch_row_1->addWidget(fPitchEgAttack);
    hbox_pitch_row_1->addWidget(fPitchEgDecay);
    hbox_pitch_row_1->addWidget(fPitchEgSustain);
    hbox_pitch_row_1->addWidget(fPitchEgRelease);

    // row 2

    fPitchLFOType = new DropDown(hbox_pitch_row_2);
    fPitchLFOType->setId(kPitchLFOType);
    fPitchLFOType->font_size = font_size + 2;
    fPitchLFOType->setFont("RobotoRegular",
                           reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                           fonts::Roboto_RegularDataSize);
    fPitchLFOType->setMenuFont("DropsIcons",
                               reinterpret_cast<const uchar *>(fonts::drops_iconsData),
                               fonts::drops_iconsDataSize);
    fPitchLFOType->setSize(200,
                           fPitchLFOType->font_size + fPitchLFOType->margin * 2.0f);
    fPitchLFOType->setCallback(this);
    fPitchLFOType->label = "TYPE :";
    fPitchLFOType->item = "a";
    fPitchLFOType->foreground_color = shamrock_green;
    fPitchLFOType->background_color = black_olive;
    fPitchLFOType->text_color = floral_white;

    fPitchLFOSync = new CheckBox(hbox_pitch_row_2);
    fPitchLFOSync->setId(kPitchLFOSync);
    fPitchLFOSync->setSize(knobSize);
    fPitchLFOSync->setCallback(this);
    fPitchLFOSync->background_color = black_olive;
    fPitchLFOSync->foreground_color = shamrock_green;
    fPitchLFOSync->highlight_color_active = shamrock_green_1;
    fPitchLFOSync->highlight_color_inactive = black_olive_1;
    fPitchLFOSync->text_color = floral_white;
    fPitchLFOSync->setFont("RobotoRegular", reinterpret_cast<const uchar *>(fonts::Roboto_RegularData), fonts::Roboto_RegularDataSize);
    fPitchLFOSync->margin = 6.f;
    fPitchLFOSync->label = "SYNC";
    fPitchLFOSync->boxSize = 20;
    fPitchLFOSync->labelSize = font_size;

    fPitchLFOFreq = new Knob(hbox_pitch_row_2);
    fPitchLFOFreq->setId(kPitchLFOFreq);
    fPitchLFOFreq->setCallback(this);
    fPitchLFOFreq->setSize(knobSize);
    fPitchLFOFreq->labelSize = font_size;
    fPitchLFOFreq->gauge_width = gauge;
    fPitchLFOFreq->margin = margin;
    fPitchLFOFreq->label = "FREQ";
    fPitchLFOFreq->background_color = black_olive;
    fPitchLFOFreq->foreground_color = shamrock_green;
    fPitchLFOFreq->highlight_color = shamrock_green_1;
    fPitchLFOFreq->text_color = floral_white;
    fPitchLFOFreq->default_value = 0;
    fPitchLFOFreq->real_min = 0.0f;
    fPitchLFOFreq->real_max = 20.f;
    fPitchLFOFreq->min = 1.0f;
    fPitchLFOFreq->max = 101.f;
    fPitchLFOFreq->format_str = "%.2f Hz";
    fPitchLFOFreq->using_log = false;
    fPitchLFOFreq->setStepText({
        "1/16",
        "1/8",
        "1/4",
        "1/2",
        "1/1",
        "2/1",
        "1/16T",
        "1/8T",
        "1/4T",
        "1/2T",
        "1/1T",
        "2/1T",
        "1/16.",
        "1/8.",
        "1/4.",
        "1/2.",
        "1/1.",
        "2/1.",
    });
    fPitchLFOFreq->setParamOnMove = true;

    fPitchLFODepth = new Knob(hbox_pitch_row_2);
    fPitchLFODepth->setId(kPitchLFODepth);
    fPitchLFODepth->setCallback(this);
    fPitchLFODepth->setSize(knobSize);
    fPitchLFODepth->labelSize = font_size;
    fPitchLFODepth->gauge_width = gauge;
    fPitchLFODepth->margin = margin;
    fPitchLFODepth->label = "DEPTH";
    fPitchLFODepth->background_color = black_olive;
    fPitchLFODepth->foreground_color = shamrock_green;
    fPitchLFODepth->highlight_color = shamrock_green_1;
    fPitchLFODepth->text_color = floral_white;
    fPitchLFODepth->default_value = 0;
    fPitchLFODepth->real_min = 0.0f;
    fPitchLFODepth->real_max = 1200.f;
    fPitchLFODepth->min = 1.0f;
    fPitchLFODepth->max = 101.f;
    fPitchLFODepth->format_str = "%.f Ct";
    fPitchLFODepth->using_log = false;
    fPitchLFODepth->setParamOnMove = true;

    fPitchLFOFade = new Knob(hbox_pitch_row_2);
    fPitchLFOFade->setId(kPitchLFOFade);
    fPitchLFOFade->setCallback(this);
    fPitchLFOFade->setSize(knobSize);
    fPitchLFOFade->labelSize = font_size;
    fPitchLFOFade->gauge_width = gauge;
    fPitchLFOFade->margin = margin;
    fPitchLFOFade->label = "FADE";
    fPitchLFOFade->background_color = black_olive;
    fPitchLFOFade->foreground_color = shamrock_green;
    fPitchLFOFade->highlight_color = shamrock_green_1;
    fPitchLFOFade->text_color = floral_white;
    fPitchLFOFade->default_value = 0.0f;
    fPitchLFOFade->real_min = 0.0f;
    fPitchLFOFade->real_max = 10.0f;
    fPitchLFOFade->min = 1.0f;
    fPitchLFOFade->max = 101.f;
    fPitchLFOFade->format_str = "%.2f s";
    fPitchLFOFade->using_log = true;
    fPitchLFOFreq->setParamOnMove = true;

    fPitchLFOTypeMenu = new Menu(hbox_pitch_row_2);
    fPitchLFOTypeMenu->setMaxViewItems(6);
    fPitchLFOTypeMenu->setId(kPitchLFOTypeMenu);
    fPitchLFOTypeMenu->setCallback(this);
    fPitchLFOTypeMenu->font_size = font_size + 2;
    fPitchLFOTypeMenu->setFont("icons",
                               reinterpret_cast<const uchar *>(fonts::drops_iconsData),
                               fonts::drops_iconsDataSize);
    fPitchLFOTypeMenu->addItems({"a",
                                 "b",
                                 "c",
                                 "d",
                                 "e",
                                 "f"});
    fPitchLFOTypeMenu->hide();
    fPitchLFOTypeMenu->background_color = black_olive;
    fPitchLFOTypeMenu->foreground_color = black_olive_2;
    fPitchLFOTypeMenu->highlight_color = shamrock_green_1;
    fPitchLFOTypeMenu->text_color = floral_white;

    fPitchLFOType->setMenu(fPitchLFOTypeMenu);
    fPitchLFOType->resize();

    hbox_pitch_row_2->addWidget(fPitchLFOType);
    hbox_pitch_row_2->addWidget(fPitchLFOSync);
    hbox_pitch_row_2->addWidget(fPitchLFOFreq);
    hbox_pitch_row_2->addWidget(fPitchLFODepth);
    hbox_pitch_row_2->addWidget(fPitchLFOFade);

    vbox_pitch->addWidget(hbox_pitch_row_1);
    vbox_pitch->addWidget(hbox_pitch_row_2);

    vbox_pitch->setWidgetResize(kHBoxPitchRow1, true);
    vbox_pitch->setWidgetResize(kHBoxPitchRow2, true);

    vbox_pitch->positionWidgets();
    hbox_pitch_row_1->positionWidgets();
    hbox_pitch_row_2->positionWidgets();

    fPitchLFOType->positionMenu();
}

END_NAMESPACE_DISTRHO
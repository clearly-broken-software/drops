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

START_NAMESPACE_DISTRHO

void DropsUI::initTabAmp()
{
    Window &window = getParentWindow();
    const float margin = 2.f;
    const float gauge = 6.f;
    const float font_size = 14.f;
    const uint sz = 60;
    const Size<uint> knobSize = Size<uint>(sz + font_size, sz);

    vbox_amp = new VBox(window);
    vbox_amp->setId(kVBoxAmp);
    vbox_amp->setAbsolutePos(12, 329);
    vbox_amp->setWidth(323);
    vbox_amp->setHeight(211);

    hbox_amp_row_1 = new HBox(vbox_amp);
    hbox_amp_row_1->setId(kHBoxAmpRow1);
    hbox_amp_row_1->setWidth(vbox_amp->getWidth() * 0.8);
    hbox_amp_row_1->setHeight(sz);

    hbox_amp_row_2 = new HBox(vbox_amp);
    hbox_amp_row_2->setId(kHBoxAmpRow2);
    hbox_amp_row_2->setHeight(sz);
    hbox_amp_row_2->setWidth(vbox_amp->getWidth() * 0.9);

    fAmpEgAttack = new Knob(hbox_amp_row_1);
    fAmpEgAttack->setId(kAmpEgAttack);
    fAmpEgAttack->setSize(knobSize);
    fAmpEgAttack->setCallback(this);
    fAmpEgAttack->labelSize = font_size;
    fAmpEgAttack->gauge_width = gauge;
    fAmpEgAttack->margin = margin;
    fAmpEgAttack->label = "ATTACK";
    fAmpEgAttack->foreground_color = saffron;
    fAmpEgAttack->background_color = black_olive;
    fAmpEgAttack->highlight_color = saffron_1;
    fAmpEgAttack->text_color = floral_white;
    fAmpEgAttack->real_min = 0.0f;
    fAmpEgAttack->real_max = 10.0f;
    fAmpEgAttack->min = 1.0f;
    fAmpEgAttack->max = 101.f;
    fAmpEgAttack->format_str = "%.2f s";
    fAmpEgAttack->using_log = true;
    fAmpEgAttack->setParamOnMove = true;

    fAmpEgDecay = new Knob(hbox_amp_row_1);
    fAmpEgDecay->setId(kAmpEgDecay);
    fAmpEgDecay->setSize(knobSize);
    fAmpEgDecay->setCallback(this);
    fAmpEgDecay->labelSize = font_size;
    fAmpEgDecay->gauge_width = gauge;
    fAmpEgDecay->margin = margin;
    fAmpEgDecay->label = "DECAY";
    fAmpEgDecay->foreground_color = saffron;
    fAmpEgDecay->background_color = black_olive;
    fAmpEgDecay->highlight_color = saffron_1;
    fAmpEgDecay->text_color = floral_white;
    fAmpEgDecay->real_min = 0.0f;
    fAmpEgDecay->real_max = 10.f;
    fAmpEgDecay->min = 1.0f;
    fAmpEgDecay->max = 101.f;
    fAmpEgDecay->using_log = true;
    fAmpEgDecay->format_str = "%.2f s";
    fAmpEgAttack->setParamOnMove = true;

    fAmpEgSustain = new Knob(hbox_amp_row_1);
    fAmpEgSustain->setId(kAmpEgSustain);
    fAmpEgSustain->setSize(knobSize);
    fAmpEgSustain->setCallback(this);
    fAmpEgSustain->labelSize = font_size;
    fAmpEgSustain->gauge_width = gauge;
    fAmpEgSustain->margin = margin;
    fAmpEgSustain->label = "SUSTAIN";
    fAmpEgSustain->foreground_color = saffron;
    fAmpEgSustain->background_color = black_olive;
    fAmpEgSustain->highlight_color = saffron_1;
    fAmpEgSustain->text_color = floral_white;
    fAmpEgSustain->real_min = 0.0f;
    fAmpEgSustain->real_max = 100.f;
    fAmpEgSustain->format_str = "%.f%%";
    fAmpEgSustain->setParamOnMove = true;

    fAmpEgRelease = new Knob(hbox_amp_row_1);
    fAmpEgRelease->setId(kAmpEgRelease);
    fAmpEgRelease->setSize(knobSize);
    fAmpEgRelease->setCallback(this);
    fAmpEgRelease->labelSize = font_size;
    fAmpEgRelease->gauge_width = gauge;
    fAmpEgRelease->margin = margin;
    fAmpEgRelease->label = "RELEASE";
    fAmpEgRelease->foreground_color = saffron;
    fAmpEgRelease->background_color = black_olive;
    fAmpEgRelease->highlight_color = saffron_1;
    fAmpEgRelease->text_color = floral_white;
    fAmpEgRelease->real_min = 0.0f;
    fAmpEgRelease->real_max = 10.f;
    fAmpEgRelease->min = 1.0f;
    fAmpEgRelease->max = 101.f;
    fAmpEgRelease->format_str = "%.2f s";
    fAmpEgRelease->using_log = true;
    fAmpEgRelease->setParamOnMove = true;

    hbox_amp_row_1->addWidget(fAmpEgAttack);
    hbox_amp_row_1->addWidget(fAmpEgDecay);
    hbox_amp_row_1->addWidget(fAmpEgSustain);
    hbox_amp_row_1->addWidget(fAmpEgRelease);

    // row 2

    fAmpLFOType = new DropDown(hbox_amp_row_2);
    fAmpLFOType->setId(kAmpLFOType);
    fAmpLFOType->font_size = font_size + 2;
    fAmpLFOType->setFont("Roboto_Regular",
                         reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                         fonts::Roboto_RegularDataSize);
    fAmpLFOType->setMenuFont("DropsIcons",
                             reinterpret_cast<const uchar *>(fonts::drops_iconsData),
                             fonts::drops_iconsDataSize);
    fAmpLFOType->setSize(200,
                         fAmpLFOType->font_size + fAmpLFOType->margin * 2.0f);
    fAmpLFOType->setCallback(this);
    fAmpLFOType->label = "TYPE :";
    fAmpLFOType->item = "a";
    fAmpLFOType->foreground_color = saffron;
    fAmpLFOType->background_color = black_olive;
    fAmpLFOType->text_color = floral_white;

    fAmpLFOSync = new CheckBox(hbox_amp_row_2);
    fAmpLFOSync->setId(kAmpLFOSync);
    fAmpLFOSync->setSize(knobSize.getWidth()*.5,knobSize.getHeight());
    fAmpLFOSync->setCallback(this);
    fAmpLFOSync->background_color = black_olive;
    fAmpLFOSync->foreground_color = saffron;
    fAmpLFOSync->highlight_color_active = saffron_1;
    fAmpLFOSync->highlight_color_inactive = black_olive_1;
    fAmpLFOSync->text_color = floral_white;
    fAmpLFOSync->setFont("Roboto_Regular",
                         reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                         fonts::Roboto_RegularDataSize);
    fAmpLFOSync->margin = 6.f;
    fAmpLFOSync->label = "SYNC";
    fAmpLFOSync->boxSize = 20;
    fAmpLFOSync->labelSize = font_size;

    fAmpLFOFreq = new Knob(hbox_amp_row_2);
    fAmpLFOFreq->setId(kAmpLFOFreq);
    fAmpLFOFreq->setCallback(this);
    fAmpLFOFreq->setSize(knobSize);
    fAmpLFOFreq->labelSize = font_size;
    fAmpLFOFreq->gauge_width = gauge;
    fAmpLFOFreq->margin = margin;
    fAmpLFOFreq->label = "FREQ";
    fAmpLFOFreq->background_color = black_olive;
    fAmpLFOFreq->foreground_color = saffron;
    fAmpLFOFreq->highlight_color = saffron_1;
    fAmpLFOFreq->text_color = floral_white;
    fAmpLFOFreq->default_value = 0;
    fAmpLFOFreq->real_min = 0.0f;
    fAmpLFOFreq->real_max = 20.f;
    fAmpLFOFreq->min = 1.0f;
    fAmpLFOFreq->max = 101.f;
    fAmpLFOFreq->format_str = "%.2f Hz";
    fAmpLFOFreq->using_log = false;
    fAmpLFOFreq->setStepText({
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
    fAmpLFOFreq->setParamOnMove = true;

    fAmpLFODepth = new Knob(hbox_amp_row_2);
    fAmpLFODepth->setId(kAmpLFODepth);
    fAmpLFODepth->setCallback(this);
    fAmpLFODepth->setSize(knobSize);
    fAmpLFODepth->labelSize = font_size;
    fAmpLFODepth->gauge_width = gauge;
    fAmpLFODepth->margin = margin;
    fAmpLFODepth->label = "DEPTH";
    fAmpLFODepth->background_color = black_olive;
    fAmpLFODepth->foreground_color = saffron;
    fAmpLFODepth->highlight_color = saffron_1;
    fAmpLFODepth->text_color = floral_white;
    fAmpLFODepth->default_value = 0.0f;
    fAmpLFODepth->real_min = 0.0f;
    fAmpLFODepth->real_max = 12.f;
    fAmpLFODepth->format_str = "%.f dB";
    fAmpLFODepth->setParamOnMove = true;

    fAmpLFOFade = new Knob(hbox_amp_row_2);
    fAmpLFOFade->setId(kAmpLFOFade);
    fAmpLFOFade->setCallback(this);
    fAmpLFOFade->setSize(knobSize);
    fAmpLFOFade->labelSize = font_size;
    fAmpLFOFade->gauge_width = gauge;
    fAmpLFOFade->margin = margin;
    fAmpLFOFade->label = "FADE";
    fAmpLFOFade->background_color = black_olive;
    fAmpLFOFade->foreground_color = saffron;
    fAmpLFOFade->highlight_color = saffron_1;
    fAmpLFOFade->text_color = floral_white;
    fAmpLFOFade->default_value = 0.0f;
    fAmpLFOFade->real_min = 0.0f;
    fAmpLFOFade->real_max = 10.0f;
    fAmpLFOFade->min = 1.0f;
    fAmpLFOFade->max = 101.f;
    fAmpLFOFade->format_str = "%.2f s";
    fAmpLFOFade->using_log = true;
    fAmpLFOFade->setParamOnMove = true;

    fAmpLFOTypeMenu = new Menu(hbox_amp_row_2);
    fAmpLFOTypeMenu->setMaxViewItems(6);
    fAmpLFOTypeMenu->setId(kAmpLFOTypeMenu);
    fAmpLFOTypeMenu->setCallback(this);
    fAmpLFOTypeMenu->font_size = font_size + 2;
    fAmpLFOTypeMenu->setFont("Drops Icons",
                             reinterpret_cast<const uchar *>(fonts::drops_iconsData),
                             fonts::drops_iconsDataSize);
    fAmpLFOTypeMenu->addItems({"a",
                               "b",
                               "c",
                               "d",
                               "e",
                               "f"});
    fAmpLFOTypeMenu->hide();
    fAmpLFOTypeMenu->background_color = black_olive;
    fAmpLFOTypeMenu->foreground_color = black_olive_2;
    fAmpLFOTypeMenu->highlight_color = saffron_1;
    fAmpLFOTypeMenu->text_color = floral_white;

    fAmpLFOType->setMenu(fAmpLFOTypeMenu);
    fAmpLFOType->resize();

    hbox_amp_row_2->addWidget(fAmpLFOType);
    hbox_amp_row_2->addWidget(fAmpLFOSync);
    hbox_amp_row_2->addWidget(fAmpLFOFreq);
    hbox_amp_row_2->addWidget(fAmpLFODepth);
    hbox_amp_row_2->addWidget(fAmpLFOFade);

    vbox_amp->addWidget(hbox_amp_row_1);
    vbox_amp->addWidget(hbox_amp_row_2);

    vbox_amp->setWidgetResize(kHBoxAmpRow1, true);
    vbox_amp->setWidgetResize(kHBoxAmpRow2, true);

    vbox_amp->positionWidgets();
    hbox_amp_row_1->positionWidgets();
    hbox_amp_row_2->positionWidgets();

    fAmpLFOType->positionMenu();
}

END_NAMESPACE_DISTRHO
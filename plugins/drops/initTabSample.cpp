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

void DropsUI::initTabSample()
{
    Window &window = getParentWindow();
    vbox_sample = new VBox(window);
    vbox_sample->setId(kVBoxSample);
    hbox_sample_row_1 = new HBox(vbox_sample);
    hbox_sample_row_1->setId(kHBoxSampleRow1);

    // fSampleXFade = new Slider(hbox_sample_row_1);
    // fSampleXFade->setId(kSampleXFade);
    // fSampleXFade->setCallback(this);
    // fSampleXFade->setSize(300, 20);
    // fSampleXFade->setLabel("X-FADE :");
    // fSampleXFade->background_color = black_olive;
    // fSampleXFade->foreground_color = floral_white;
    // fSampleXFade->highlight_color = flame;
    // fSampleXFade->text_color = floral_white;
    // fSampleXFade->unit = "MS";
    // fSampleXFade->right_padding = slider_right_padding;
    // fSampleXFade->format_str = "%.2f";

    // fSampleNormalize = new DropDown(hbox_sample_row_1);
    // fSampleNormalize->setId(kSampleNormalize);
    // fSampleNormalize->font_size = 16;
    // fSampleNormalize->setSize(300,
    //                           fSampleNormalize->font_size + fSampleNormalize->margin * 2.0f);
    // fSampleNormalize->setCallback(this);
    // fSampleNormalize->label = "NORMALIZE :";
    // fSampleNormalize->item = "NONE";
    // fSampleNormalize->foreground_color = floral_white;
    // fSampleNormalize->background_color = black_olive;
    // fSampleNormalize->text_color = floral_white;

    // fNormalizeMenu = new Menu(hbox_sample_row_1);
    // fNormalizeMenu->setId(kNormalizeMenu);
    // fNormalizeMenu->setCallback(this);
    // fNormalizeMenu->addItems({"NONE", "SAMPLE", "ACTIVE REGION"});
    // fNormalizeMenu->font_size = 16;
    // fNormalizeMenu->hide();
    // fNormalizeMenu->background_color = black_olive;
    // fNormalizeMenu->foreground_color = black_olive_2;
    // fNormalizeMenu->highlight_color = flame;
    // fNormalizeMenu->text_color = floral_white;

    fSamplePitchKeyCenter = new DropDown(hbox_sample_row_1);
    fSamplePitchKeyCenter->setId(kSamplePitchKeyCenter);
    fSamplePitchKeyCenter->font_size = 16;
    fSamplePitchKeyCenter->setSize(300,
                                   fSamplePitchKeyCenter->font_size +
                                       fSamplePitchKeyCenter->margin * 2.0f);
    fSamplePitchKeyCenter->setCallback(this);
    fSamplePitchKeyCenter->label = "PITCH KEY-CENTER :";
    fSamplePitchKeyCenter->item = "C4";
    fSamplePitchKeyCenter->foreground_color = black_olive_2;
    fSamplePitchKeyCenter->background_color = black_olive;
    fSamplePitchKeyCenter->text_color = floral_white;

    fKeyCenterMenu = new Menu(hbox_sample_row_1);
    fKeyCenterMenu->setId(kKeyCenterMenu);
    fKeyCenterMenu->setCallback(this);
    fKeyCenterMenu->addItems({
        "127 G9",
        "126 F#9/Gb9",
        "125 F9",
        "124 E9",
        "123 D#9/Eb9",
        "122 D9",
        "121 C#9/Db9",
        "120 C9",
        "119 B8",
        "118 A#8/Bb8",
        "117 A8",
        "116 G#8/Ab8",
        "115 G8",
        "114 F#8/Gb8",
        "113 F8",
        "112 E8",
        "111 D#8/Eb8",
        "110 D8",
        "109 C#8/Db8",
        "108 C8",
        "107 B7",
        "106 A#7/Bb7",
        "105 A7",
        "104 G#7/Ab7",
        "103 G7",
        "102 F#7/Gb7",
        "101 F7",
        "100 E7",
        "99 D#7/Eb7",
        "98 D7",
        "97 C#7/Db7",
        "96 C7",
        "95 B6",
        "94 A#6/Bb6",
        "93 A6",
        "92 G#6/Ab6",
        "91 G6",
        "90 F#6/Gb6",
        "89 F6",
        "88 E6",
        "87 D#6/Eb6",
        "86 D6",
        "85 C#6/Db6",
        "84 C6",
        "83 B5",
        "82 A#5/Bb5",
        "81 A5",
        "80 G#5/Ab5",
        "79 G5",
        "78 F#5/Gb5",
        "77 F5",
        "76 E5",
        "75 D#5/Eb5",
        "74 D5",
        "73 C#5/Db5",
        "72 C5",
        "71 B4",
        "70 A#4/Bb4",
        "69 A4",
        "68 G#4/Ab4",
        "67 G4",
        "66 F#4/Gb4",
        "65 F4",
        "64 E4",
        "63 D#4/Eb4",
        "62 D4",
        "61 C#4/Db4",
        "60 C4",
        "59 B3",
        "58 A#3/Bb3",
        "57 A3",
        "56 G#3/Ab3",
        "55 G3",
        "54 F#3/Gb3",
        "53 F3",
        "52 E3",
        "51 D#3/Eb3",
        "50 D3",
        "49 C#3/Db3",
        "48 C3",
        "47 B2",
        "46 A#2/Bb2",
        "45 A2",
        "44 G#2/Ab2",
        "43 G2",
        "42 F#2/Gb2",
        "41 F2",
        "40 E2",
        "39 D#2/Eb2",
        "38 D2",
        "37 C#2/Db2",
        "36 C2",
        "35 B1",
        "34 A#1/Bb1",
        "33 A1",
        "32 G#1/Ab1",
        "31 G1",
        "30 F#1/Gb1",
        "29 F1",
        "28 E1",
        "27 D#1/Eb1",
        "26 D1",
        "25 C#1/Db1",
        "24 C1",
        "23 B0",
        "22 A#0/Bb0",
        "21 A0",
        "20  ",
        "19  ",
        "18  ",
        "17  ",
        "16  ",
        "15  ",
        "14  ",
        "13  ",
        "12  ",
        "11  ",
        "10  ",
        "9  ",
        "8  ",
        "7  ",
        "6  ",
        "5  ",
        "4  ",
        "3  ",
        "2  ",
        "1  ",
        "0 ",
    });
    fKeyCenterMenu->font_size = 16;
    fKeyCenterMenu->hide();
    fKeyCenterMenu->background_color = black_olive;
    fKeyCenterMenu->foreground_color = black_olive_2;
    fKeyCenterMenu->highlight_color = flame;
    fKeyCenterMenu->text_color = floral_white;
    fKeyCenterMenu->item_view_first = 60;
    fKeyCenterMenu->item_view_last = 63;
    fKeyCenterMenu->max_view_items = 4;

    vbox_sample->setAbsolutePos(tabs_x, tabs_y);
    vbox_sample->setWidth(getWidth());
    vbox_sample->setHeight(getHeight() - tabs_y);

    hbox_sample_row_1->setWidth(vbox_sample->getWidth());
    // box_sample_row_1->setHeight(100);
    vbox_sample->addWidget(hbox_sample_row_1);
    vbox_sample->setWidgetResize(kHBoxSampleRow1, true);

    // hbox_sample_row_1->addWidget(fSampleXFade);
    // hbox_sample_row_1->addWidget(fSampleNormalize);
    hbox_sample_row_1->addWidget(fSamplePitchKeyCenter);

    // row 2
    hbox_sample_row_2 = new HBox(vbox_sample);
    hbox_sample_row_2->setId(kHBoxSampleRow2);

    fSamplePlayMode = new DropDown(hbox_sample_row_2);
    fSamplePlayMode->setId(kSamplePlayMode);
    fSamplePlayMode->font_size = 16;
    fSamplePlayMode->setSize(216,
                             fSamplePlayMode->font_size +
                                 fSamplePlayMode->margin * 2.0f);
    fSamplePlayMode->setCallback(this);
    fSamplePlayMode->label = "LOOP MODE:";
    fSamplePlayMode->item = "NO LOOP";
    fSamplePlayMode->foreground_color = floral_white;
    fSamplePlayMode->background_color = black_olive;
    fSamplePlayMode->text_color = floral_white;

    fPlayModeMenu = new Menu(hbox_sample_row_2);
    fPlayModeMenu->setId(kPlayModeMenu);
    fPlayModeMenu->setCallback(this);
    fPlayModeMenu->addItems({"NO LOOP", "ONE SHOT", "CONTINUOUS", "SUSTAIN"});
    fPlayModeMenu->font_size = 16;
    fPlayModeMenu->hide();
    fPlayModeMenu->background_color = black_olive;
    fPlayModeMenu->foreground_color = black_olive_2;
    fPlayModeMenu->highlight_color = flame;
    fPlayModeMenu->text_color = floral_white;

    fSamplePlayDirection = new DropDown(hbox_sample_row_2);
    fSamplePlayDirection->setId(kSamplePlayDirection);
    fSamplePlayDirection->font_size = 16;
    fSamplePlayDirection->setSize(216,
                                  fSamplePlayDirection->font_size +
                                      fSamplePlayDirection->margin * 2.0f);
    fSamplePlayDirection->setCallback(this);
    fSamplePlayDirection->label = "DIRECTION: ";
    fSamplePlayDirection->item = "FORWARD";
    fSamplePlayDirection->foreground_color = floral_white;
    fSamplePlayDirection->background_color = black_olive;
    fSamplePlayDirection->text_color = floral_white;

    fDirectionMenu = new Menu(hbox_sample_row_2);
    fDirectionMenu->setId(kDirectionMenu);
    fDirectionMenu->setCallback(this);
    fDirectionMenu->addItems({"FORWARD", "REVERSE"});
    fDirectionMenu->font_size = 16;
    fDirectionMenu->hide();
    fDirectionMenu->background_color = black_olive;
    fDirectionMenu->foreground_color = black_olive_2;
    fDirectionMenu->highlight_color = flame;
    fDirectionMenu->text_color = floral_white;

    hbox_sample_row_2->setWidth(vbox_sample->getWidth() * 2 / 3);
    hbox_sample_row_2->align_items = HBox::Align_Items::top;

    vbox_sample->addWidget(hbox_sample_row_2);
    vbox_sample->setWidgetResize(kHBoxSampleRow2, true);

    hbox_sample_row_2->addWidget(fSamplePlayMode);
    hbox_sample_row_2->addWidget(fSamplePlayDirection);

    vbox_sample->positionWidgets();
    hbox_sample_row_1->positionWidgets();
    hbox_sample_row_2->positionWidgets();

    fSamplePlayDirection->setMenu(fDirectionMenu);
    fSamplePlayMode->setMenu(fPlayModeMenu);
    fSamplePitchKeyCenter->setMenu(fKeyCenterMenu);
    // fSampleNormalize->setMenu(fNormalizeMenu);
}



END_NAMESPACE_DISTRHO
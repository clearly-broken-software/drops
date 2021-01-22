/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabPitch()
{
    Window &window = getParentWindow();
    vbox_pitch = new VBox(window);
    vbox_pitch->setId(kVBoxPitch);
    vbox_pitch->setAbsolutePos(667, 329);
    vbox_pitch->setSize(323, 176);

    hbox_pitch_row_3 = new HBox(vbox_pitch);
    hbox_pitch_row_3->setSize(280, 70);
    //hbox_pitch_row_3->setAbsolutePos(40, 433);

    hbox_pitch_row_2 = new HBox(vbox_pitch);
    hbox_pitch_row_2->setId(kHBoxPitchRow2);
    hbox_pitch_row_2->setSize(219, 40);
    //hbox_pitch_row_2->setAbsolutePos(48, 381);

    hbox_pitch_row_1 = new HBox(vbox_pitch);
    hbox_pitch_row_1->setId(kHBoxPitchRow1);
    hbox_pitch_row_1->setWidth(vbox_pitch->getWidth());
    hbox_pitch_row_1->setHeight(26);

    const float margin = 6.f;
    const float gauge = 6.f;
    const float font_size = 14.f;
    const uint sz = 40 + margin + gauge + font_size;

    const Size<uint> knobSize = Size<uint>(sz, sz);

    fPitchLFOType = new DropDown(hbox_pitch_row_1);
    fPitchLFOType->setId(kPitchLFOType);
    fPitchLFOType->font_size = font_size;
    fPitchLFOType->setSize(200,
                           fPitchLFOType->font_size + fPitchLFOType->margin * 2.0f);
    fPitchLFOType->setCallback(this);
    fPitchLFOType->label = "LFO TYPE :";
    fPitchLFOType->item = "SINE";
    fPitchLFOType->foreground_color = shamrock_green;
    fPitchLFOType->background_color = black_olive;
    fPitchLFOType->text_color = floral_white;
    hbox_pitch_row_1->addWidget(fPitchLFOType);

    // row 2
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

    hbox_pitch_row_2->addWidget(fPitchLFOFreq);
    hbox_pitch_row_2->addWidget(fPitchLFODepth);

    // row 3

    fPitchEgAttack = new Knob(hbox_pitch_row_3);
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

    fPitchEgDecay = new Knob(hbox_pitch_row_3);
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

    fPitchEgSustain = new Knob(hbox_pitch_row_3);
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

    fPitchEgRelease = new Knob(hbox_pitch_row_3);
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

    hbox_pitch_row_3->addWidget(fPitchEgAttack);
    hbox_pitch_row_3->addWidget(fPitchEgDecay);
    hbox_pitch_row_3->addWidget(fPitchEgSustain);
    hbox_pitch_row_3->addWidget(fPitchEgRelease);

    vbox_pitch->addWidget(hbox_pitch_row_1);
    vbox_pitch->addWidget(hbox_pitch_row_2);
    vbox_pitch->addWidget(hbox_pitch_row_3);

    vbox_pitch->setWidgetResize(kHBoxPitchRow1, true);
    vbox_pitch->setWidgetResize(kHBoxPitchRow2, true);

    vbox_pitch->positionWidgets();
    hbox_pitch_row_1->positionWidgets();
    hbox_pitch_row_2->positionWidgets();
    hbox_pitch_row_3->positionWidgets();

    fPitchLFOTypeMenu = new Menu(hbox_pitch_row_1);
    fPitchLFOTypeMenu->max_view_items = 4;
    fPitchLFOTypeMenu->setId(kPitchLFOTypeMenu);
    fPitchLFOTypeMenu->setCallback(this);
    fPitchLFOTypeMenu->font_size = font_size;
    fPitchLFOTypeMenu->addItems({"TRIANGLE",
                                 "SINE",
                                 "75% PULSE",
                                 "SQUARE",
                                 "25% PULSE",
                                 "12.5% PULSE",
                                 "SAW UP",
                                 "SAW DOWN"});
    fPitchLFOTypeMenu->hide();
    fPitchLFOTypeMenu->background_color = black_olive;
    fPitchLFOTypeMenu->foreground_color = black_olive_2;
    fPitchLFOTypeMenu->highlight_color = shamrock_green_1;
    fPitchLFOTypeMenu->text_color = floral_white;

    fPitchLFOType->setMenu(fPitchLFOTypeMenu);
    fPitchLFOType->resize();
    fPitchLFOType->positionMenu();

}

END_NAMESPACE_DISTRHO
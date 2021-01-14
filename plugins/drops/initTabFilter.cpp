/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabFilter()
{
    Window &window = getParentWindow();
    const float margin = 6.f;
    const float gauge = 6.f;
    float font_size = 14.f;
    const uint sz = 32 + margin + gauge + font_size;

    const Size<uint> knobSize = Size<uint>(sz, sz);

    vbox_filter = new VBox(window);
    vbox_filter->setId(kVBoxFilter);
    vbox_filter->setAbsolutePos(339, 329);
    vbox_filter->setSize(320, 176);

    hbox_filter_row_3 = new HBox(vbox_filter);
    hbox_filter_row_3->setId(kHBoxFilterRow3);
    hbox_filter_row_3->setSize(320, 66);

    hbox_filter_row_2 = new HBox(vbox_filter);
    hbox_filter_row_2->setId(kHBoxFilterRow2);
    hbox_filter_row_2->setSize(320, 66);
    hbox_filter_row_2->justify_content = HBox::Justify_Content::center;
    hbox_filter_row_2->align_items = HBox::Align_Items::top;

    hbox_filter_row_1 = new HBox(vbox_filter);
    hbox_filter_row_1->setId(kHBoxFilterRow1);
    hbox_filter_row_1->setSize(220, 66);
    //hbox_filter_row_1->justify_content=HBox::Justify_Content::center;

    vbox_filter_lfo = new VBox(hbox_filter_row_2);
    vbox_filter_lfo->setId(kVBoxFilterLfo);
    vbox_filter_lfo->setSize(66, 66);
    vbox_filter_lfo->align_items = VBox::Align_Items::top;

    fFilterLowpass = new SVGButton(hbox_filter_row_1, artwork::filter_lowpass, nullptr, 0.15f);
    fFilterLowpass->setId(kFilterLowpass);
    fFilterLowpass->setCallback(this);
    fFilterLowpass->background_color = black_olive_3;
    fFilterLowpass->highlight_color = black_olive_1;
    fFilterLowpass->active_color = black_olive;

    fFilterBandpass = new SVGButton(hbox_filter_row_1, artwork::filter_bandpass, nullptr, 0.15f);
    fFilterBandpass->setId(kFilterBandpass);
    fFilterBandpass->setCallback(this);
    fFilterBandpass->background_color = black_olive_3;
    fFilterBandpass->highlight_color = black_olive_1;
    fFilterBandpass->active_color = black_olive;

    fFilterHighpass = new SVGButton(hbox_filter_row_1, artwork::filter_highpass, nullptr, 0.15f);
    fFilterHighpass->setId(kFilterHighpass);
    fFilterHighpass->setCallback(this);
    fFilterHighpass->background_color = black_olive_3;
    fFilterHighpass->highlight_color = black_olive_1;
    fFilterHighpass->active_color = black_olive;

    fFilterLFOType = new DropDown(vbox_filter_lfo);
    fFilterLFOType->setId(kFilterLFOType);
    fFilterLFOType->font_size = font_size;
    fFilterLFOType->setSize(148,
                            fFilterLFOType->font_size + fFilterLFOType->margin * 2.0f);
    fFilterLFOType->setCallback(this);
    fFilterLFOType->label = "LFO TYPE :";
    fFilterLFOType->item = "SINE";
    fFilterLFOType->foreground_color = blue_pigment_1;
    fFilterLFOType->background_color = black_olive;
    fFilterLFOType->text_color = floral_white;
    // resize later
    fFilterLFOTypeMenu = new Menu(vbox_filter_lfo);
    fFilterLFOTypeMenu->max_view_items = 4;
    fFilterLFOTypeMenu->setId(kFilterLFOTypeMenu);
    fFilterLFOTypeMenu->setCallback(this);
    fFilterLFOTypeMenu->addItems({"TRIANGLE",
                                  "SINE",
                                  "75% PULSE",
                                  "SQUARE",
                                  "25% PULSE",
                                  "12.5% PULSE",
                                  "SAW UP",
                                  "SAW DOWN"});
    fFilterLFOTypeMenu->font_size = font_size;
    fFilterLFOTypeMenu->hide();
    fFilterLFOTypeMenu->background_color = black_olive;
    fFilterLFOTypeMenu->foreground_color = black_olive_2;
    fFilterLFOTypeMenu->highlight_color = blue_pigment_1;
    fFilterLFOTypeMenu->text_color = floral_white;

    fFilterLFOFreq = new Knob(hbox_filter_row_2);
    fFilterLFOFreq->setId(kFilterLFOFreq);
    fFilterLFOFreq->setCallback(this);
    fFilterLFOFreq->setSize(knobSize);
    fFilterLFOFreq->label = "FREQ";
    fFilterLFOFreq->labelSize = font_size;
    fFilterLFOFreq->gauge_width = gauge;
    fFilterLFOFreq->margin = margin;
    fFilterLFOFreq->background_color = black_olive;
    fFilterLFOFreq->foreground_color = blue_pigment_1;
    fFilterLFOFreq->highlight_color = blue_pigment_2;
    fFilterLFOFreq->text_color = floral_white;
    fFilterLFOFreq->default_value = 0;

    fFilterLFODepth = new Knob(hbox_filter_row_2);
    fFilterLFODepth->setId(kFilterLFODepth);
    fFilterLFODepth->setCallback(this);
    fFilterLFODepth->setSize(knobSize);
    fFilterLFODepth->label = "DEPTH";
    fFilterLFODepth->labelSize = font_size;
    fFilterLFODepth->margin = margin;
    fFilterLFODepth->gauge_width = gauge;
    fFilterLFODepth->background_color = black_olive;
    fFilterLFODepth->foreground_color = blue_pigment_1;
    fFilterLFODepth->highlight_color = blue_pigment_2;
    fFilterLFODepth->text_color = floral_white;

    // row 3
    // smaller font for adsr
    font_size = 12.f;
    fFilterCutOff = new Knob(hbox_filter_row_3);
    fFilterCutOff->setId(kFilterCutOff);
    fFilterCutOff->setCallback(this);
    fFilterCutOff->setSize(knobSize);
    fFilterCutOff->label = "FREQ";
    fFilterCutOff->labelSize = font_size;
    fFilterCutOff->margin = margin;
    fFilterCutOff->gauge_width = gauge;
    fFilterCutOff->background_color = black_olive;
    fFilterCutOff->foreground_color = blue_pigment_1;
    fFilterCutOff->highlight_color = blue_pigment_2;
    fFilterCutOff->text_color = floral_white;
    fFilterCutOff->default_value = 1;
    fFilterCutOff->min = 0;
    fFilterCutOff->max = 1;

    fFilterResonance = new Knob(hbox_filter_row_3);
    fFilterResonance->setId(kFilterResonance);
    fFilterResonance->setCallback(this);
    fFilterResonance->setSize(knobSize);
    fFilterResonance->label = "Q";
    fFilterResonance->labelSize = font_size;
    fFilterResonance->margin = margin;
    fFilterResonance->gauge_width = gauge;
    fFilterResonance->background_color = black_olive;
    fFilterResonance->foreground_color = blue_pigment_1;
    fFilterResonance->highlight_color = blue_pigment_2;
    fFilterResonance->text_color = floral_white;
    fFilterResonance->default_value = 1;
    fFilterResonance->min = 0;
    fFilterResonance->max = 1;

    fFilterEgAttack = new Knob(hbox_filter_row_3);
    fFilterEgAttack->setId(kFilterEgAttack);
    fFilterEgAttack->setSize(knobSize);
    fFilterEgAttack->setCallback(this);
    fFilterEgAttack->label = "ATTACK";
    fFilterEgAttack->labelSize = font_size;
    fFilterEgAttack->margin = margin;
    fFilterEgAttack->gauge_width = gauge;
    fFilterEgAttack->foreground_color = blue_pigment_1;
    fFilterEgAttack->background_color = black_olive;
    fFilterEgAttack->highlight_color = blue_pigment_2;
    fFilterEgAttack->text_color = floral_white;

    fFilterEgDecay = new Knob(hbox_filter_row_3);
    fFilterEgDecay->setId(kFilterEgDecay);
    fFilterEgDecay->setSize(knobSize);
    fFilterEgDecay->setCallback(this);
    fFilterEgDecay->label = "DECAY";
    fFilterEgDecay->labelSize = font_size;
    fFilterEgDecay->margin = margin;
    fFilterEgDecay->gauge_width = gauge;
    fFilterEgDecay->foreground_color = blue_pigment_1;
    fFilterEgDecay->background_color = black_olive;
    fFilterEgDecay->highlight_color = blue_pigment_2;
    fFilterEgDecay->text_color = floral_white;

    fFilterEgSustain = new Knob(hbox_filter_row_3);
    fFilterEgSustain->setId(kFilterEgSustain);
    fFilterEgSustain->setSize(knobSize);
    fFilterEgSustain->setCallback(this);
    fFilterEgSustain->label = "SUSTAIN";
    fFilterEgSustain->labelSize = font_size;
    fFilterEgSustain->margin = margin;
    fFilterEgSustain->gauge_width = gauge;
    fFilterEgSustain->foreground_color = blue_pigment_1;
    fFilterEgSustain->background_color = black_olive;
    fFilterEgSustain->highlight_color = blue_pigment_2;
    fFilterEgSustain->text_color = floral_white;

    fFilterEgRelease = new Knob(hbox_filter_row_3);
    fFilterEgRelease->setId(kFilterEgRelease);
    fFilterEgRelease->setSize(knobSize);
    fFilterEgRelease->setCallback(this);
    fFilterEgRelease->labelSize = font_size;
    fFilterEgRelease->margin = margin;
    fFilterEgRelease->gauge_width = gauge;
    fFilterEgRelease->label = "RELEASE";
    fFilterEgRelease->foreground_color = blue_pigment_1;
    fFilterEgRelease->background_color = black_olive;
    fFilterEgRelease->highlight_color = blue_pigment_2;
    fFilterEgRelease->text_color = floral_white;

    vbox_filter->addWidget(hbox_filter_row_1);
    hbox_filter_row_1->addWidget(fFilterLowpass);
    hbox_filter_row_1->addWidget(fFilterBandpass);
    hbox_filter_row_1->addWidget(fFilterHighpass);
    // TODO: add buttons

    vbox_filter->addWidget(hbox_filter_row_2);
    hbox_filter_row_2->addWidget(vbox_filter_lfo);
    vbox_filter_lfo->addWidget(fFilterLFOType);
    // TODO: add sync
    hbox_filter_row_2->addWidget(fFilterLFOFreq);
    hbox_filter_row_2->addWidget(fFilterLFODepth);

    vbox_filter->addWidget(hbox_filter_row_3);
    hbox_filter_row_3->addWidget(fFilterCutOff);
    hbox_filter_row_3->addWidget(fFilterResonance);
    hbox_filter_row_3->addWidget(fFilterEgAttack);
    hbox_filter_row_3->addWidget(fFilterEgDecay);
    hbox_filter_row_3->addWidget(fFilterEgSustain);
    hbox_filter_row_3->addWidget(fFilterEgRelease);

    vbox_filter->positionWidgets();
    hbox_filter_row_1->positionWidgets();
    hbox_filter_row_2->positionWidgets();
    vbox_filter_lfo->positionWidgets();
    hbox_filter_row_3->positionWidgets();

    fFilterLFOType->setMenu(fFilterLFOTypeMenu);
    fFilterLFOType->resize();
    printf("vbox filter w,h %i, %i\n", vbox_filter_lfo->getWidth(), vbox_filter_lfo->getHeight());

    //fFilterLFOSync->setMenu(fFilterLFOSyncMenu);
}

END_NAMESPACE_DISTRHO
/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabAmp()
{
    Window &window = getParentWindow();
    box_amp = new VBox(window);
    box_amp->setId(kVBoxAmp);
    box_amp_row_1 = new HBox(box_amp);
    box_amp_row_1->setId(kHBoxAmpRow1);

    const Size<uint> knobSize = Size<uint>(76, 76);
    const uint knobSpacing = knobSize.getWidth() + 15;

    fAmpEgAttack = new Knob(box_amp_row_1);
    fAmpEgAttack->setId(kAmpEgAttack);
    fAmpEgAttack->setSize(knobSize);
    fAmpEgAttack->setCallback(this);
    fAmpEgAttack->label = "ATTACK";
    fAmpEgAttack->foreground_color = floral_white;
    fAmpEgAttack->background_color = black_olive;
    fAmpEgAttack->highlight_color = flame;
    fAmpEgAttack->text_color = floral_white;

    fAmpEgDecay = new Knob(box_amp_row_1);
    fAmpEgDecay->setId(kAmpEgDecay);
    fAmpEgDecay->setSize(knobSize);
    fAmpEgDecay->setCallback(this);
    fAmpEgDecay->label = "DECAY";
    fAmpEgDecay->foreground_color = floral_white;
    fAmpEgDecay->background_color = black_olive;
    fAmpEgDecay->highlight_color = flame;
    fAmpEgDecay->text_color = floral_white;

    fAmpEgSustain = new Knob(box_amp_row_1);
    fAmpEgSustain->setId(kAmpEgSustain);
    fAmpEgSustain->setSize(knobSize);
    fAmpEgSustain->setCallback(this);
    fAmpEgSustain->label = "SUSTAIN";
    fAmpEgSustain->foreground_color = floral_white;
    fAmpEgSustain->background_color = black_olive;
    fAmpEgSustain->highlight_color = flame;
    fAmpEgSustain->text_color = floral_white;

    fAmpEgRelease = new Knob(box_amp_row_1);
    fAmpEgRelease->setId(kAmpEgRelease);
    fAmpEgRelease->setSize(knobSize);
    fAmpEgRelease->setCallback(this);
    fAmpEgRelease->label = "RELEASE";
    fAmpEgRelease->foreground_color = floral_white;
    fAmpEgRelease->background_color = black_olive;
    fAmpEgRelease->highlight_color = flame;
    fAmpEgRelease->text_color = floral_white;

    fAmpLFOFreqBeat = new RadioButton(box_amp_row_1);
    fAmpLFOFreqBeat->setId(kAmpLFOFreqBeat);
    fAmpLFOFreqBeat->setCallback(this);
    fAmpLFOFreqBeat->font_size = 16.f;
    fAmpLFOFreqBeat->margin = 2.0f;
    fAmpLFOFreqBeat->addOption("FREQ: ");
    fAmpLFOFreqBeat->addOption("SYNC: ");
    fAmpLFOFreqBeat->setSize(100, 60);
    fAmpLFOFreqBeat->background_color = black_olive;
    fAmpLFOFreqBeat->text_color = floral_white;
    fAmpLFOFreqBeat->foreground_color = pale_silver;
    fAmpLFOFreqBeat->highlight_color = flame;

    box_amp->setAbsolutePos(tabs_x, tabs_y);
    box_amp->setWidth(tabs_w);
    box_amp->setHeight(tabs_h);

    box_amp_row_1->setWidth(box_amp->getWidth());
    box_amp->addWidget(box_amp_row_1);
    box_amp->setWidgetResize(kHBoxSampleRow1, true);

    box_amp_row_1->addWidget(fAmpEgAttack);
    box_amp_row_1->addWidget(fAmpEgDecay);
    box_amp_row_1->addWidget(fAmpEgSustain);
    box_amp_row_1->addWidget(fAmpEgRelease);

    box_amp_row_2 = new HBox(box_amp);
    box_amp_row_2->setWidth(box_amp->getWidth());
    box_amp->setWidgetResize(kHBoxAmpRow2, true);

    fAmpLFOType = new DropDown(box_amp_row_2);
    fAmpLFOType->setId(kAmpLFOType);
    fAmpLFOType->font_size = 16;
    fAmpLFOType->setSize(300,
                         fAmpLFOType->font_size + fAmpLFOType->margin * 2.0f);
    fAmpLFOType->setCallback(this);
    fAmpLFOType->label = "LFO TYPE :";
    fAmpLFOType->item = "SINE";
    fAmpLFOType->foreground_color = floral_white;
    fAmpLFOType->background_color = black_olive;
    fAmpLFOType->text_color = floral_white;

    fAmpLFOTypeMenu = new Menu(box_amp_row_2);
    fAmpLFOTypeMenu->setId(kAmpLFOTypeMenu);
    fAmpLFOTypeMenu->setCallback(this);
    fAmpLFOTypeMenu->addItem("TRIANGLE");
    fAmpLFOTypeMenu->addItem("SINE");
    fAmpLFOTypeMenu->addItem("75% PULSE");
    fAmpLFOTypeMenu->addItem("SQUARE");
    fAmpLFOTypeMenu->addItem("25% PULSE");
    fAmpLFOTypeMenu->addItem("12.5% PULSE");
    fAmpLFOTypeMenu->addItem("SAW UP");
    fAmpLFOTypeMenu->addItem("SAW DOWN");
    fAmpLFOTypeMenu->font_size = 16;
    fAmpLFOTypeMenu->hide();
    fAmpLFOTypeMenu->background_color = black_olive;
    fAmpLFOTypeMenu->foreground_color = floral_white;
    fAmpLFOTypeMenu->highlight_color = flame;
    fAmpLFOTypeMenu->text_color = floral_white;

    fAmpLFOFreq = new Slider(box_amp_row_2);
    fAmpLFOFreq->setId(kAmpLFOFreq);
    fAmpLFOFreq->setCallback(this);
    fAmpLFOFreq->setSize(300, 20);
    fAmpLFOFreq->setLabel("FREQ :");
    fAmpLFOFreq->background_color = pale_silver;
    fAmpLFOFreq->foreground_color = floral_white;
    fAmpLFOFreq->highlight_color = flame;
    fAmpLFOFreq->text_color = floral_white;
    fAmpLFOFreq->unit = "HZ";
    fAmpLFOFreq->right_padding = slider_right_padding + 4;
    fAmpLFOFreq->format_str = "%.f";
    fAmpLFOFreq->min_value = -1200;
    fAmpLFOFreq->max_value = 1200;
    fAmpLFOFreq->default_value = 0;

    fAmpLFODepth = new Slider(box_amp_row_2);
    fAmpLFODepth->setId(kAmpLFODepth);
    fAmpLFODepth->setCallback(this);
    fAmpLFODepth->setSize(300, 20);
    fAmpLFODepth->setLabel("DEPTH :");
    fAmpLFODepth->background_color = pale_silver;
    fAmpLFODepth->foreground_color = floral_white;
    fAmpLFODepth->highlight_color = flame;
    fAmpLFODepth->text_color = floral_white;
    fAmpLFODepth->unit="DB";
    fAmpLFODepth->right_padding= slider_right_padding;
    fAmpLFODepth->format_str = "%.2f";

    // fAmpLFOSync = new DropDown(box_amp_row_2);
    // fAmpLFOSync->setId(kAmpLFOSync);
    // fAmpLFOSync->font_size = 16;
    // fAmpLFOSync->setSize(300,
    //                      fAmpLFOSync->font_size + fAmpLFOSync->margin * 2.0f);
    // fAmpLFOSync->setCallback(this);
    // fAmpLFOSync->label = "LFO Sync :";
    // fAmpLFOSync->item = "1/4";
    // fAmpLFOSync->foreground_color = floral_white;
    // fAmpLFOSync->background_color = black_olive;
    // fAmpLFOSync->text_color = floral_white;

    box_amp_row_2->addWidget(fAmpLFOType);
    box_amp_row_2->addWidget(fAmpLFOFreq);
    box_amp_row_2->addWidget(fAmpLFODepth);
    //  box_sample_row_2->addWidget(fAmpLFOSync);
    box_amp->addWidget(box_amp_row_2);

    box_amp->positionWidgets();
    box_amp_row_1->positionWidgets();
    box_amp_row_2->positionWidgets();

    fAmpLFOType->setMenu(fAmpLFOTypeMenu);
    // fAmpLFOSync->setMenu(nullptr);
    hideTabAmp();
}

void DropsUI::hideTabAmp()
{
    box_amp->hide();
    box_amp_row_1->hide();
    fAmpEgAttack->hide();
    fAmpEgDecay->hide();
    fAmpEgSustain->hide();
    fAmpEgRelease->hide();

    box_amp_row_2->hide();
    fAmpLFOType->hide();
    fAmpLFOFreq->hide();
    fAmpLFODepth->hide();
    // fAmpLFOSync->hide();
    fAmpLFOFreqBeat->hide();
}

void DropsUI::showTabAmp()
{
    box_amp->show();
    box_amp_row_1->show();
    fAmpEgAttack->show();
    fAmpEgDecay->show();
    fAmpEgSustain->show();
    fAmpEgRelease->show();

    box_amp_row_2->show();
    fAmpLFOType->show();
    fAmpLFOFreq->show();
    fAmpLFODepth->show();
    // fAmpLFOSync->show();
    fAmpLFOFreqBeat->show();
}

END_NAMESPACE_DISTRHO
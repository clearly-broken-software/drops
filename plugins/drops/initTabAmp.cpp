/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabAmp()
{
    Window &window = getParentWindow();
    vbox_amp = new VBox(window);
    vbox_amp->setId(kVBoxAmp);

    hbox_amp_row_3 = new HBox(vbox_amp);
    hbox_amp_row_3->setSize(280, 70);
    hbox_amp_row_3->setAbsolutePos(40, 433);

    hbox_amp_row_2 = new HBox(vbox_amp);
    hbox_amp_row_2->setId(kHBoxAmpRow2);
    hbox_amp_row_2->setSize(219, 40);
    hbox_amp_row_2->setAbsolutePos(48, 381);

    //  add row 1 as last to prevent "underlapping"
    hbox_amp_row_1 = new HBox(vbox_amp);
    hbox_amp_row_1->setId(kHBoxAmpRow1);

    const float margin = 6.f;
    const float gauge = 6.f;
    const float font_size = 14.f;
    const uint sz = 40 + margin + gauge + font_size;

    const Size<uint> knobSize = Size<uint>(sz, sz);

    /*     fAmpLFOFreqBeat = new RadioButton(hbox_amp_row_1);
    fAmpLFOFreqBeat->setId(kAmpLFOFreqBeat);
    fAmpLFOFreqBeat->setCallback(this);
    fAmpLFOFreqBeat->font_size = 16.f;
    fAmpLFOFreqBeat->margin = 2.0f;
    fAmpLFOFreqBeat->addOption(" ");
    fAmpLFOFreqBeat->addOption(" ");
    fAmpLFOFreqBeat->setSize(20 + 2, 40); // font_size + 2 * margin
    fAmpLFOFreqBeat->background_color = black_olive;
    fAmpLFOFreqBeat->text_color = floral_white;
    fAmpLFOFreqBeat->foreground_color = floral_white;
    fAmpLFOFreqBeat->highlight_color = saffron_1;
     */

    vbox_amp->setAbsolutePos(12, 329);
    vbox_amp->setWidth(323);
    vbox_amp->setHeight(176);

    hbox_amp_row_1->setWidth(vbox_amp->getWidth());
    //hbox_amp_row_1->setAbsolutePos(12,344);
    hbox_amp_row_1->setHeight(26);

    hbox_amp_row_1->setId(kHBoxAmpRow1);
    // hbox_amp_row_1->justify_content = HBox::Justify_Content::left;
    //hbox_amp_row_1->align_items = HBox::Align_Items::top;

    fAmpLFOType = new DropDown(hbox_amp_row_1);
    fAmpLFOType->setId(kAmpLFOType);
    fAmpLFOType->font_size = font_size;
    fAmpLFOType->setSize(200,
                         fAmpLFOType->font_size + fAmpLFOType->margin * 2.0f);
    fAmpLFOType->setCallback(this);
    fAmpLFOType->label = "LFO TYPE :";
    fAmpLFOType->item = "SINE";
    fAmpLFOType->foreground_color = saffron;
    fAmpLFOType->background_color = black_olive;
    fAmpLFOType->text_color = floral_white;

    hbox_amp_row_1->addWidget(fAmpLFOType);
    //hbox_amp_row_1->addWidget(fAmpLFOSync)

    // row 2

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
    //fAmpLFOFreq->min = 0;
    //fAmpLFOFreq->max = 20;
    fAmpLFOFreq->default_value = 0;

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
    //fAmpLFODepth->min = 0.0f;
    //fAmpLFODepth->max = 12.0f;

    /*     fAmpLFOSync = new DropDown(vbox_amp_lfo);
    fAmpLFOSync->setId(kAmpLFOSync);
    fAmpLFOSync->font_size = 16;
    fAmpLFOSync->setSize(300,
                         fAmpLFOSync->font_size + fAmpLFOSync->margin * 2.0f);
    fAmpLFOSync->setCallback(this);
    fAmpLFOSync->label = "LFO Sync :";
    fAmpLFOSync->item = "1/4";
    fAmpLFOSync->foreground_color = saffron;
    fAmpLFOSync->background_color = black_olive;
    fAmpLFOSync->text_color = floral_white;

    fAmpLFOSyncMenu = new Menu(vbox_amp_lfo);
    fAmpLFOSyncMenu->max_view_items = 4;
    fAmpLFOSyncMenu->setId(kAmpLFOSyncMenu);
    fAmpLFOSyncMenu->setCallback(this);
    fAmpLFOSyncMenu->addItems({
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
    fAmpLFOSyncMenu->font_size = 16;
    fAmpLFOSyncMenu->hide();
    fAmpLFOSyncMenu->background_color = black_olive;
    fAmpLFOSyncMenu->foreground_color = black_olive_2;
    fAmpLFOSyncMenu->highlight_color = saffron_1;
    fAmpLFOSyncMenu->text_color = floral_white;
 */
    hbox_amp_row_2->addWidget(fAmpLFOFreq);
    hbox_amp_row_2->addWidget(fAmpLFODepth);

    fAmpEgAttack = new Knob(hbox_amp_row_3);
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

    fAmpEgDecay = new Knob(hbox_amp_row_3);
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

    fAmpEgSustain = new Knob(hbox_amp_row_3);
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

    fAmpEgRelease = new Knob(hbox_amp_row_3);
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

    hbox_amp_row_3->addWidget(fAmpEgAttack);
    hbox_amp_row_3->addWidget(fAmpEgDecay);
    hbox_amp_row_3->addWidget(fAmpEgSustain);
    hbox_amp_row_3->addWidget(fAmpEgRelease);

    vbox_amp->addWidget(hbox_amp_row_1);
    vbox_amp->addWidget(hbox_amp_row_2);
    vbox_amp->addWidget(hbox_amp_row_3);
    
    vbox_amp->setWidgetResize(kHBoxAmpRow1, true);
    vbox_amp->setWidgetResize(kHBoxAmpRow2, true);

    vbox_amp->positionWidgets();
    hbox_amp_row_1->positionWidgets();
    hbox_amp_row_2->positionWidgets();
    hbox_amp_row_3->positionWidgets();

    fAmpLFOTypeMenu = new Menu(hbox_amp_row_1);
    fAmpLFOTypeMenu->max_view_items = 4;
    fAmpLFOTypeMenu->setId(kAmpLFOTypeMenu);
    fAmpLFOTypeMenu->setCallback(this);
    fAmpLFOTypeMenu->font_size = font_size;
    fAmpLFOTypeMenu->addItems({"TRIANGLE",
                               "SINE",
                               "75% PULSE",
                               "SQUARE",
                               "25% PULSE",
                               "12.5% PULSE",
                               "SAW UP",
                               "SAW DOWN"});
    fAmpLFOTypeMenu->hide();
    fAmpLFOTypeMenu->background_color = black_olive;
    fAmpLFOTypeMenu->foreground_color = black_olive_2;
    fAmpLFOTypeMenu->highlight_color = saffron_1;
    fAmpLFOTypeMenu->text_color = floral_white;

    fAmpLFOType->setMenu(fAmpLFOTypeMenu);
    fAmpLFOType->resize();
    // fAmpLFOSync->setMenu(fAmpLFOSyncMenu);
}

END_NAMESPACE_DISTRHO
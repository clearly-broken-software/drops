/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabPitch()
{
    Window &window = getParentWindow();
    vbox_pitch = new VBox(window);
    vbox_pitch->setId(kVBoxPitch);
    hbox_pitch_row_1 = new HBox(vbox_pitch);
    hbox_pitch_row_1->setId(kHBoxPitchRow1);

    const Size<uint> knobSize = Size<uint>(76, 76);

    fPitchEgAttack = new Knob(hbox_pitch_row_1);
    fPitchEgAttack->setId(kPitchEgAttack);
    fPitchEgAttack->setSize(knobSize);
    fPitchEgAttack->setCallback(this);
    fPitchEgAttack->label = "ATTACK";
    fPitchEgAttack->foreground_color = floral_white;
    fPitchEgAttack->background_color = black_olive;
    fPitchEgAttack->highlight_color = flame;
    fPitchEgAttack->text_color = floral_white;

    fPitchEgDecay = new Knob(hbox_pitch_row_1);
    fPitchEgDecay->setId(kPitchEgDecay);
    fPitchEgDecay->setSize(knobSize);
    fPitchEgDecay->setCallback(this);
    fPitchEgDecay->label = "DECAY";
    fPitchEgDecay->foreground_color = floral_white;
    fPitchEgDecay->background_color = black_olive;
    fPitchEgDecay->highlight_color = flame;
    fPitchEgDecay->text_color = floral_white;

    fPitchEgSustain = new Knob(hbox_pitch_row_1);
    fPitchEgSustain->setId(kPitchEgSustain);
    fPitchEgSustain->setSize(knobSize);
    fPitchEgSustain->setCallback(this);
    fPitchEgSustain->label = "SUSTAIN";
    fPitchEgSustain->foreground_color = floral_white;
    fPitchEgSustain->background_color = black_olive;
    fPitchEgSustain->highlight_color = flame;
    fPitchEgSustain->text_color = floral_white;

    fPitchEgRelease = new Knob(hbox_pitch_row_1);
    fPitchEgRelease->setId(kPitchEgRelease);
    fPitchEgRelease->setSize(knobSize);
    fPitchEgRelease->setCallback(this);
    fPitchEgRelease->label = "RELEASE";
    fPitchEgRelease->foreground_color = floral_white;
    fPitchEgRelease->background_color = black_olive;
    fPitchEgRelease->highlight_color = flame;
    fPitchEgRelease->text_color = floral_white;

    vbox_pitch->setAbsolutePos(tabs_x, tabs_y);
    vbox_pitch->setWidth(tabs_w);
    vbox_pitch->setHeight(tabs_h);

    hbox_pitch_row_1->setWidth(vbox_pitch->getWidth());
    vbox_pitch->addWidget(hbox_pitch_row_1);
    vbox_pitch->setWidgetResize(kHBoxPitchRow1, true);

    hbox_pitch_row_1->addWidget(fPitchEgAttack);
    hbox_pitch_row_1->addWidget(fPitchEgDecay);
    hbox_pitch_row_1->addWidget(fPitchEgSustain);
    hbox_pitch_row_1->addWidget(fPitchEgRelease);

    // row 2
    hbox_pitch_row_2 = new HBox(vbox_pitch);
    hbox_pitch_row_2->setId(kHBoxPitchRow2);
    hbox_pitch_sync = new HBox(hbox_pitch_row_2);
    hbox_pitch_sync->setId(kHBoxPitchSync);
    vbox_pitch_lfo = new VBox(hbox_pitch_sync);
    vbox_pitch_lfo->setId(kVBoxPitchLfo);
    // hbox_pitch_row_2->setWidgetJustify_Content(kPitchLFOSync,HBox::Justify_Content::right);

    fPitchLFOFreqBeat = new RadioButton(hbox_pitch_row_2);
    fPitchLFOFreqBeat->setId(kPitchLFOFreqBeat);
    fPitchLFOFreqBeat->setCallback(this);
    fPitchLFOFreqBeat->font_size = 16.f;
    fPitchLFOFreqBeat->margin = 2.0f;
    fPitchLFOFreqBeat->addOption(" ");
    fPitchLFOFreqBeat->addOption(" ");
    fPitchLFOFreqBeat->setSize(20 + 2, 40); // font_size + 2 * margin
    fPitchLFOFreqBeat->background_color = Color(255, 0, 0);
    fPitchLFOFreqBeat->text_color = floral_white;
    fPitchLFOFreqBeat->foreground_color = pale_silver;
    fPitchLFOFreqBeat->highlight_color = flame;

    fPitchLFOType = new DropDown(hbox_pitch_row_2);
    fPitchLFOType->setId(kPitchLFOType);
    fPitchLFOType->font_size = 16;
    fPitchLFOType->setSize(200,
                           fPitchLFOType->font_size + fPitchLFOType->margin * 2.0f);
    fPitchLFOType->setCallback(this);
    fPitchLFOType->label = "LFO TYPE :";
    fPitchLFOType->item = "SINE";
    fPitchLFOType->foreground_color = floral_white;
    fPitchLFOType->background_color = black_olive;
    fPitchLFOType->text_color = floral_white;

    fPitchLFOTypeMenu = new Menu(hbox_pitch_row_2);
    fPitchLFOTypeMenu->max_view_items = 4;
    fPitchLFOTypeMenu->setId(kPitchLFOTypeMenu);
    fPitchLFOTypeMenu->setCallback(this);
    fPitchLFOTypeMenu->addItems({"TRIANGLE",
                                 "SINE",
                                 "75% PULSE",
                                 "SQUARE",
                                 "25% PULSE",
                                 "12.5% PULSE",
                                 "SAW UP",
                                 "SAW DOWN"});

    fPitchLFOTypeMenu->font_size = 16;
    fPitchLFOTypeMenu->hide();
    fPitchLFOTypeMenu->background_color = black_olive;
    fPitchLFOTypeMenu->foreground_color = black_olive_2;
    fPitchLFOTypeMenu->highlight_color = flame;
    fPitchLFOTypeMenu->text_color = floral_white;

    fPitchLFOFreq = new Slider(vbox_pitch_lfo);
    fPitchLFOFreq->setId(kPitchLFOFreq);
    fPitchLFOFreq->setCallback(this);
    fPitchLFOFreq->setSize(300, 20);
    fPitchLFOFreq->setLabel("FREQ :");
    fPitchLFOFreq->background_color = black_olive;
    fPitchLFOFreq->foreground_color = floral_white;
    fPitchLFOFreq->highlight_color = flame;
    fPitchLFOFreq->text_color = floral_white;
    fPitchLFOFreq->unit = "HZ";
    fPitchLFOFreq->right_padding = slider_right_padding + 4;
    fPitchLFOFreq->format_str = "%.2f";
    fPitchLFOFreq->min_value = 0;
    fPitchLFOFreq->max_value = 20;
    fPitchLFOFreq->default_value = 0;

    fPitchLFODepth = new Slider(hbox_pitch_row_2);
    fPitchLFODepth->setId(kPitchLFODepth);
    fPitchLFODepth->setCallback(this);
    fPitchLFODepth->setSize(300, 20);
    fPitchLFODepth->setLabel("DEPTH :");
    fPitchLFODepth->background_color = black_olive;
    fPitchLFODepth->foreground_color = floral_white;
    fPitchLFODepth->highlight_color = flame;
    fPitchLFODepth->text_color = floral_white;
    fPitchLFODepth->unit = "DB";
    fPitchLFODepth->right_padding = slider_right_padding;
    fPitchLFODepth->format_str = "%.2f";

    fPitchLFOSync = new DropDown(vbox_pitch_lfo);
    fPitchLFOSync->setId(kPitchLFOSync);
    fPitchLFOSync->font_size = 16;
    fPitchLFOSync->setSize(300,
                           fPitchLFOSync->font_size + fPitchLFOSync->margin * 2.0f);
    fPitchLFOSync->setCallback(this);
    fPitchLFOSync->label = "LFO Sync :";
    fPitchLFOSync->item = "1/4";
    fPitchLFOSync->foreground_color = floral_white;
    fPitchLFOSync->background_color = black_olive;
    fPitchLFOSync->text_color = floral_white;

    fPitchLFOSyncMenu = new Menu(vbox_pitch_lfo);
    fPitchLFOSyncMenu->max_view_items = 4;
    fPitchLFOSyncMenu->setId(kPitchLFOSyncMenu);
    fPitchLFOSyncMenu->setCallback(this);
    fPitchLFOSyncMenu->addItems({
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
    fPitchLFOSyncMenu->font_size = 16;
    fPitchLFOSyncMenu->hide();
    fPitchLFOSyncMenu->background_color = black_olive;
    fPitchLFOSyncMenu->foreground_color = black_olive_2;
    fPitchLFOSyncMenu->highlight_color = flame;
    fPitchLFOSyncMenu->text_color = floral_white;

    hbox_pitch_row_2->setWidth(vbox_pitch->getWidth());
    hbox_pitch_row_2->align_items = HBox::Align_Items::top;

    vbox_pitch->addWidget(hbox_pitch_row_2);
    vbox_pitch->setWidgetResize(kHBoxPitchRow2, true);

    hbox_pitch_row_2->addWidget(fPitchLFOType);
    hbox_pitch_row_2->addWidget(hbox_pitch_sync);
    hbox_pitch_sync->setWidth(std::max(fPitchLFOFreq->getWidth(), fPitchLFOSync->getWidth()));
    hbox_pitch_sync->setHeight(fPitchLFOFreq->getHeight() + fPitchLFOSync->getHeight());
    hbox_pitch_sync->justify_content = HBox::Justify_Content::center;
    hbox_pitch_sync->addWidget(fPitchLFOFreqBeat);
    hbox_pitch_sync->addWidget(vbox_pitch_lfo);
    vbox_pitch_lfo->setWidth(fPitchLFOFreq->getWidth() + fPitchLFOFreqBeat->getWidth());
    vbox_pitch_lfo->setHeight(fPitchLFOFreq->getHeight() + fPitchLFOSync->getHeight());
    vbox_pitch_lfo->justify_content = VBox::Justify_Content::left;
    vbox_pitch_lfo->addWidget(fPitchLFOFreq);
    vbox_pitch_lfo->addWidget(fPitchLFOSync);

    hbox_pitch_row_2->addWidget(fPitchLFODepth);

    vbox_pitch->positionWidgets();
    hbox_pitch_row_1->positionWidgets();
    hbox_pitch_row_2->positionWidgets();
    hbox_pitch_sync->positionWidgets();
    vbox_pitch_lfo->positionWidgets();
    fPitchLFOType->setMenu(fPitchLFOTypeMenu);
    fPitchLFOSync->setMenu(fPitchLFOSyncMenu);
    hideTabPitch();
}

void DropsUI::hideTabPitch()
{
    vbox_pitch->hide();
    hbox_pitch_row_1->hide();
    fPitchEgAttack->hide();
    fPitchEgDecay->hide();
    fPitchEgSustain->hide();
    fPitchEgRelease->hide();

    hbox_pitch_row_2->hide();
    fPitchLFOType->hide();
    fPitchLFOFreq->hide();
    fPitchLFODepth->hide();
    fPitchLFOSync->hide();
    fPitchLFOFreqBeat->hide();
}

void DropsUI::showTabPitch()
{
    vbox_pitch->show();
    hbox_pitch_row_1->show();
    fPitchEgAttack->show();
    fPitchEgDecay->show();
    fPitchEgSustain->show();
    fPitchEgRelease->show();

    hbox_pitch_row_2->show();
    vbox_pitch_lfo->show();
    fPitchLFOType->show();
    fPitchLFOFreq->show();
    fPitchLFODepth->show();
    fPitchLFOSync->show();
    fPitchLFOFreqBeat->show();
}

END_NAMESPACE_DISTRHO
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
    fAmpLFOFreqBeat->addOption(" ");
    fAmpLFOFreqBeat->addOption(" ");
    fAmpLFOFreqBeat->setSize(20 + 2, 40); // font_size + 2 * margin
    fAmpLFOFreqBeat->background_color = black_olive;
    fAmpLFOFreqBeat->text_color = floral_white;
    fAmpLFOFreqBeat->foreground_color = pale_silver;
    fAmpLFOFreqBeat->highlight_color = flame;

    box_amp->setAbsolutePos(tabs_x, tabs_y);
    box_amp->setWidth(tabs_w);
    box_amp->setHeight(tabs_h);

    box_amp_row_1->setWidth(box_amp->getWidth());
    box_amp->addWidget(box_amp_row_1);
    box_amp->setWidgetResize(kHBoxAmpRow1, true);

    box_amp_row_1->addWidget(fAmpEgAttack);
    box_amp_row_1->addWidget(fAmpEgDecay);
    box_amp_row_1->addWidget(fAmpEgSustain);
    box_amp_row_1->addWidget(fAmpEgRelease);

    // row 2
    box_amp_row_2 = new HBox(box_amp);
    box_amp_row_2->setId(kHBoxAmpRow2);
    box_sync = new HBox(box_amp_row_2);
    box_sync->setId(kHBoxAmpSync);
    vbox_amp_lfo = new VBox(box_sync);
    vbox_amp_lfo->setId(kVBoxAmpLfo);
    // box_amp_row_2->setWidgetJustify_Content(kAmpLFOSync,HBox::Justify_Content::right);

    fAmpLFOType = new DropDown(box_amp_row_2);
    fAmpLFOType->setId(kAmpLFOType);
    fAmpLFOType->font_size = 16;
    fAmpLFOType->setSize(200,
                         fAmpLFOType->font_size + fAmpLFOType->margin * 2.0f);
    fAmpLFOType->setCallback(this);
    fAmpLFOType->label = "LFO TYPE :";
    fAmpLFOType->item = "SINE";
    fAmpLFOType->foreground_color = floral_white;
    fAmpLFOType->background_color = black_olive;
    fAmpLFOType->text_color = floral_white;

    fAmpLFOTypeMenu = new Menu(box_amp_row_2);
    fAmpLFOTypeMenu->max_view_items = 4;
    fAmpLFOTypeMenu->setId(kAmpLFOTypeMenu);
    fAmpLFOTypeMenu->setCallback(this);
    fAmpLFOTypeMenu->addItems({"TRIANGLE",
                               "SINE",
                               "75% PULSE",
                               "SQUARE",
                               "25% PULSE",
                               "12.5% PULSE",
                               "SAW UP",
                               "SAW DOWN"});

    fAmpLFOTypeMenu->font_size = 16;
    fAmpLFOTypeMenu->hide();
    fAmpLFOTypeMenu->background_color = black_olive;
    fAmpLFOTypeMenu->foreground_color = black_olive_2;
    fAmpLFOTypeMenu->highlight_color = flame;
    fAmpLFOTypeMenu->text_color = floral_white;

    fAmpLFOFreq = new Slider(vbox_amp_lfo);
    fAmpLFOFreq->setId(kAmpLFOFreq);
    fAmpLFOFreq->setCallback(this);
    fAmpLFOFreq->setSize(300, 20);
    fAmpLFOFreq->setLabel("FREQ :");
    fAmpLFOFreq->background_color = black_olive;
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
    fAmpLFODepth->background_color = black_olive;
    fAmpLFODepth->foreground_color = floral_white;
    fAmpLFODepth->highlight_color = flame;
    fAmpLFODepth->text_color = floral_white;
    fAmpLFODepth->unit = "DB";
    fAmpLFODepth->right_padding = slider_right_padding;
    fAmpLFODepth->format_str = "%.2f";

    fAmpLFOSync = new DropDown(vbox_amp_lfo);
    fAmpLFOSync->setId(kAmpLFOSync);
    fAmpLFOSync->font_size = 16;
    fAmpLFOSync->setSize(300,
                         fAmpLFOSync->font_size + fAmpLFOSync->margin * 2.0f);
    fAmpLFOSync->setCallback(this);
    fAmpLFOSync->label = "LFO Sync :";
    fAmpLFOSync->item = "1/4";
    fAmpLFOSync->foreground_color = floral_white;
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
    fAmpLFOSyncMenu->highlight_color = flame;
    fAmpLFOSyncMenu->text_color = floral_white;

    box_amp_row_2->setWidth(box_amp->getWidth());
    box_amp_row_2->align_items = HBox::Align_Items::top;
    // box_amp_row_2->justify_content = HBox::Justify_Content::left;

    box_amp->addWidget(box_amp_row_2);
    box_amp->setWidgetResize(kHBoxAmpRow2, true);

    box_amp_row_2->addWidget(fAmpLFOType);
    box_amp_row_2->addWidget(box_sync);
        box_sync->setWidth(std::max(fAmpLFOFreq->getWidth(),fAmpLFOSync->getWidth()));
        box_sync->setHeight(fAmpLFOFreq->getHeight() + fAmpLFOSync->getHeight());
        box_sync->justify_content = HBox::Justify_Content::center;
        box_sync->addWidget(fAmpLFOFreqBeat);
        box_sync->addWidget(vbox_amp_lfo);
            vbox_amp_lfo->setWidth(fAmpLFOFreq->getWidth() + fAmpLFOFreqBeat->getWidth());
            vbox_amp_lfo->setHeight(fAmpLFOFreq->getHeight() + fAmpLFOSync->getHeight());
            vbox_amp_lfo->justify_content = VBox::Justify_Content::left;
            vbox_amp_lfo->addWidget(fAmpLFOFreq);
            vbox_amp_lfo->addWidget(fAmpLFOSync);

    box_amp_row_2->addWidget(fAmpLFODepth);

    box_amp->positionWidgets();
    box_amp_row_1->positionWidgets();
    box_amp_row_2->positionWidgets();
    box_sync->positionWidgets();
    vbox_amp_lfo->positionWidgets();
    //  fAmpLFOSync->hide();

    fAmpLFOType->setMenu(fAmpLFOTypeMenu);
    fAmpLFOSync->setMenu(fAmpLFOSyncMenu);
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
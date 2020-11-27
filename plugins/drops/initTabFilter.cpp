/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabFilter()
{
    Window &window = getParentWindow();
    vbox_filter = new VBox(window);
    vbox_filter->setId(kVBoxFilter);
    hbox_filter_row_1 = new HBox(vbox_filter);
    hbox_filter_row_1->setId(kHBoxFilterRow1);

    const Size<uint> knobSize = Size<uint>(76, 76);

    fFilterEgAttack = new Knob(hbox_filter_row_1);
    fFilterEgAttack->setId(kFilterEgAttack);
    fFilterEgAttack->setSize(knobSize);
    fFilterEgAttack->setCallback(this);
    fFilterEgAttack->label = "ATTACK";
    fFilterEgAttack->foreground_color = floral_white;
    fFilterEgAttack->background_color = black_olive;
    fFilterEgAttack->highlight_color = flame;
    fFilterEgAttack->text_color = floral_white;

    fFilterEgDecay = new Knob(hbox_filter_row_1);
    fFilterEgDecay->setId(kFilterEgDecay);
    fFilterEgDecay->setSize(knobSize);
    fFilterEgDecay->setCallback(this);
    fFilterEgDecay->label = "DECAY";
    fFilterEgDecay->foreground_color = floral_white;
    fFilterEgDecay->background_color = black_olive;
    fFilterEgDecay->highlight_color = flame;
    fFilterEgDecay->text_color = floral_white;

    fFilterEgSustain = new Knob(hbox_filter_row_1);
    fFilterEgSustain->setId(kFilterEgSustain);
    fFilterEgSustain->setSize(knobSize);
    fFilterEgSustain->setCallback(this);
    fFilterEgSustain->label = "SUSTAIN";
    fFilterEgSustain->foreground_color = floral_white;
    fFilterEgSustain->background_color = black_olive;
    fFilterEgSustain->highlight_color = flame;
    fFilterEgSustain->text_color = floral_white;

    fFilterEgRelease = new Knob(hbox_filter_row_1);
    fFilterEgRelease->setId(kFilterEgRelease);
    fFilterEgRelease->setSize(knobSize);
    fFilterEgRelease->setCallback(this);
    fFilterEgRelease->label = "RELEASE";
    fFilterEgRelease->foreground_color = floral_white;
    fFilterEgRelease->background_color = black_olive;
    fFilterEgRelease->highlight_color = flame;
    fFilterEgRelease->text_color = floral_white;

    vbox_filter->setAbsolutePos(tabs_x, tabs_y);
    vbox_filter->setWidth(tabs_w);
    vbox_filter->setHeight(tabs_h);

    hbox_filter_row_1->setWidth(vbox_filter->getWidth());
    vbox_filter->addWidget(hbox_filter_row_1);
    vbox_filter->setWidgetResize(kHBoxFilterRow1, true);

    hbox_filter_row_1->addWidget(fFilterEgAttack);
    hbox_filter_row_1->addWidget(fFilterEgDecay);
    hbox_filter_row_1->addWidget(fFilterEgSustain);
    hbox_filter_row_1->addWidget(fFilterEgRelease);

    // row 2
    hbox_filter_row_2 = new HBox(vbox_filter);
    hbox_filter_row_2->setId(kHBoxFilterRow2);
    hbox_filter_sync = new HBox(hbox_filter_row_2);
    hbox_filter_sync->setId(kHBoxFilterSync);
    vbox_filter_lfo = new VBox(hbox_filter_sync);
    vbox_filter_lfo->setId(kVBoxFilterLfo);
    // hbox_filter_row_2->setWidgetJustify_Content(kFilterLFOSync,HBox::Justify_Content::right);

    // fFilterLFOFreqBeat = new RadioButton(hbox_filter_row_2);
    // fFilterLFOFreqBeat->setId(kFilterLFOFreqBeat);
    // fFilterLFOFreqBeat->setCallback(this);
    // fFilterLFOFreqBeat->font_size = 16.f;
    // fFilterLFOFreqBeat->margin = 2.0f;
    // fFilterLFOFreqBeat->addOption(" ");
    // fFilterLFOFreqBeat->addOption(" ");
    // fFilterLFOFreqBeat->setSize(20 + 2, 40); // font_size + 2 * margin
    // fFilterLFOFreqBeat->background_color = Color(255, 0, 0);
    // fFilterLFOFreqBeat->text_color = floral_white;
    // fFilterLFOFreqBeat->foreground_color = pale_silver;
    // fFilterLFOFreqBeat->highlight_color = flame;

    fFilterLFOType = new DropDown(hbox_filter_row_2);
    fFilterLFOType->setId(kFilterLFOType);
    fFilterLFOType->font_size = 16;
    fFilterLFOType->setSize(200,
                           fFilterLFOType->font_size + fFilterLFOType->margin * 2.0f);
    fFilterLFOType->setCallback(this);
    fFilterLFOType->label = "LFO TYPE :";
    fFilterLFOType->item = "SINE";
    fFilterLFOType->foreground_color = floral_white;
    fFilterLFOType->background_color = black_olive;
    fFilterLFOType->text_color = floral_white;

    fFilterLFOTypeMenu = new Menu(hbox_filter_row_2);
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

    fFilterLFOTypeMenu->font_size = 16;
    fFilterLFOTypeMenu->hide();
    fFilterLFOTypeMenu->background_color = black_olive;
    fFilterLFOTypeMenu->foreground_color = black_olive_2;
    fFilterLFOTypeMenu->highlight_color = flame;
    fFilterLFOTypeMenu->text_color = floral_white;

    fFilterLFOFreq = new Slider(vbox_filter_lfo);
    fFilterLFOFreq->setId(kFilterLFOFreq);
    fFilterLFOFreq->setCallback(this);
    fFilterLFOFreq->setSize(300, 20);
    fFilterLFOFreq->setLabel("FREQ :");
    fFilterLFOFreq->background_color = black_olive;
    fFilterLFOFreq->foreground_color = floral_white;
    fFilterLFOFreq->highlight_color = flame;
    fFilterLFOFreq->text_color = floral_white;
    fFilterLFOFreq->unit = "HZ";
    fFilterLFOFreq->right_padding = slider_right_padding + 4;
    fFilterLFOFreq->format_str = "%.2f";
    fFilterLFOFreq->min_value = 0;
    fFilterLFOFreq->max_value = 20;
    fFilterLFOFreq->default_value = 0;

    fFilterLFODepth = new Slider(hbox_filter_row_2);
    fFilterLFODepth->setId(kFilterLFODepth);
    fFilterLFODepth->setCallback(this);
    fFilterLFODepth->setSize(300, 20);
    fFilterLFODepth->setLabel("DEPTH :");
    fFilterLFODepth->background_color = black_olive;
    fFilterLFODepth->foreground_color = floral_white;
    fFilterLFODepth->highlight_color = flame;
    fFilterLFODepth->text_color = floral_white;
    fFilterLFODepth->unit = "DB";
    fFilterLFODepth->right_padding = slider_right_padding;
    fFilterLFODepth->format_str = "%.2f";

    // fFilterLFOSync = new DropDown(vbox_filter_lfo);
    // fFilterLFOSync->setId(kFilterLFOSync);
    // fFilterLFOSync->font_size = 16;
    // fFilterLFOSync->setSize(300,
    //                        fFilterLFOSync->font_size + fFilterLFOSync->margin * 2.0f);
    // fFilterLFOSync->setCallback(this);
    // fFilterLFOSync->label = "LFO Sync :";
    // fFilterLFOSync->item = "1/4";
    // fFilterLFOSync->foreground_color = floral_white;
    // fFilterLFOSync->background_color = black_olive;
    // fFilterLFOSync->text_color = floral_white;

    // fFilterLFOSyncMenu = new Menu(vbox_filter_lfo);
    // fFilterLFOSyncMenu->max_view_items = 4;
    // fFilterLFOSyncMenu->setId(kFilterLFOSyncMenu);
    // fFilterLFOSyncMenu->setCallback(this);
    // fFilterLFOSyncMenu->addItems({
    //     "1/16",
    //     "1/8",
    //     "1/4",
    //     "1/2",
    //     "1/1",
    //     "2/1",
    //     "1/16T",
    //     "1/8T",
    //     "1/4T",
    //     "1/2T",
    //     "1/1T",
    //     "2/1T",
    //     "1/16.",
    //     "1/8.",
    //     "1/4.",
    //     "1/2.",
    //     "1/1.",
    //     "2/1.",
    // });
    // fFilterLFOSyncMenu->font_size = 16;
    // fFilterLFOSyncMenu->hide();
    // fFilterLFOSyncMenu->background_color = black_olive;
    // fFilterLFOSyncMenu->foreground_color = black_olive_2;
    // fFilterLFOSyncMenu->highlight_color = flame;
    // fFilterLFOSyncMenu->text_color = floral_white;

    hbox_filter_row_2->setWidth(vbox_filter->getWidth());
    hbox_filter_row_2->align_items = HBox::Align_Items::top;

    vbox_filter->addWidget(hbox_filter_row_2);
    vbox_filter->setWidgetResize(kHBoxFilterRow2, true);

    hbox_filter_row_2->addWidget(fFilterLFOType);
    hbox_filter_row_2->addWidget(hbox_filter_sync);
    hbox_filter_sync->setWidth(fFilterLFOFreq->getWidth());
    // hbox_filter_sync->setWidth(std::max(fFilterLFOFreq->getWidth(), fFilterLFOSync->getWidth()));
    hbox_filter_sync->setHeight(fFilterLFOFreq->getHeight()); //  + fFilterLFOSync->getHeight());
    hbox_filter_sync->justify_content = HBox::Justify_Content::center;
    // hbox_filter_sync->addWidget(fFilterLFOFreqBeat);
    hbox_filter_sync->addWidget(vbox_filter_lfo);
    vbox_filter_lfo->setWidth(fFilterLFOFreq->getWidth()); //+ fFilterLFOFreqBeat->getWidth());
    vbox_filter_lfo->setHeight(fFilterLFOFreq->getHeight()); // + fFilterLFOSync->getHeight());
    vbox_filter_lfo->justify_content = VBox::Justify_Content::left;
    vbox_filter_lfo->addWidget(fFilterLFOFreq);
    // vbox_filter_lfo->addWidget(fFilterLFOSync);

    hbox_filter_row_2->addWidget(fFilterLFODepth);

    vbox_filter->positionWidgets();
    hbox_filter_row_1->positionWidgets();
    hbox_filter_row_2->positionWidgets();
    hbox_filter_sync->positionWidgets();
    vbox_filter_lfo->positionWidgets();
    fFilterLFOType->setMenu(fFilterLFOTypeMenu);
    // fFilterLFOSync->setMenu(fFilterLFOSyncMenu);
    hideTabFilter();
}

void DropsUI::hideTabFilter()
{
    vbox_filter->hide();
    hbox_filter_row_1->hide();
    fFilterEgAttack->hide();
    fFilterEgDecay->hide();
    fFilterEgSustain->hide();
    fFilterEgRelease->hide();

    hbox_filter_row_2->hide();
    fFilterLFOType->hide();
    fFilterLFOFreq->hide();
    fFilterLFODepth->hide();
    // fFilterLFOSync->hide();
    // fFilterLFOFreqBeat->hide();
}

void DropsUI::showTabFilter()
{
    vbox_filter->show();
    hbox_filter_row_1->show();
    fFilterEgAttack->show();
    fFilterEgDecay->show();
    fFilterEgSustain->show();
    fFilterEgRelease->show();

    hbox_filter_row_2->show();
    vbox_filter_lfo->show();
    fFilterLFOType->show();
    fFilterLFOFreq->show();
    fFilterLFODepth->show();
    // fFilterLFOSync->show();
    // fFilterLFOFreqBeat->show();
}

END_NAMESPACE_DISTRHO
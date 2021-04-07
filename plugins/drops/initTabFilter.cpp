/* license */
#include "DropsUI.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

void DropsUI::initTabFilter()
{
    Window &window = getParentWindow();
    const float margin = 2.f;
    const float gauge = 5.f;
    float font_size = 12.f;
    const uint sz = 50;

    const Size<uint> knobSize = Size<uint>(sz + font_size, sz);

    vbox_filter = new VBox(window);
    vbox_filter->setId(kVBoxFilter);
    vbox_filter->setAbsolutePos(339, 329);
    vbox_filter->setSize(320, 211);
    vbox_filter->justify_content = VBox::Justify_Content::right;

    hbox_filter_row_1 = new HBox(vbox_filter);
    hbox_filter_row_1->setId(kHBoxFilterRow1);
    hbox_filter_row_1->setWidth(vbox_filter->getWidth() * .9);
    hbox_filter_row_1->setHeight(211 / 3 + 20);
    //hbox_filter_row_1->align_items = HBox::Align_Items::bottom;

    // hbox_filter_row_1_spacer = new HBox(hbox_filter_row_1);
    // hbox_filter_row_1_spacer->setHeight(hbox_filter_row_1->getHeight());
    // //hbox_filter_row_1_spacer->setWidth(hbox_filter_row_1->getWidth() * 3 / 5);
    // hbox_filter_row_1_spacer->setWidth(120);

    hbox_filter_row_2 = new HBox(vbox_filter);
    hbox_filter_row_2->setId(kHBoxFilterRow2);
    hbox_filter_row_2->setWidth(vbox_filter->getWidth());
    hbox_filter_row_2->setHeight(211 / 3 + 20);
    //hbox_filter_row_2->align_items = HBox::Align_Items::bottom;

    hbox_filter_row_3 = new HBox(vbox_filter);
    hbox_filter_row_3->setId(kHBoxFilterRow3);
    hbox_filter_row_3->setWidth(vbox_filter->getWidth() * .9);
    hbox_filter_row_3->setHeight(211 / 3 + 20);
    //hbox_filter_row_3->align_items = HBox::Align_Items::bottom;

    hbox_filter_row_3_spacer = new HBox(hbox_filter_row_3);
    hbox_filter_row_3_spacer->setHeight(hbox_filter_row_3->getHeight());
    hbox_filter_row_3_spacer->setWidth(hbox_filter_row_3->getWidth() * 1 / 4); // FIXME: hardcoded :-(

    // hbox_filter_row_2->justify_content = HBox::Justify_Content::center;
    //  hbox_filter_row_2->align_items = HBox::Align_Items::top;

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

    // font_size = 12.f;
    fFilterCutOff = new Knob(hbox_filter_row_1);
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
    const float max_cutoff = getSampleRate() / 2.0f;
    fFilterCutOff->real_min = 20.0f;
    fFilterCutOff->real_max = max_cutoff;
    fFilterCutOff->min = 1.0f;
    fFilterCutOff->max = 101.f;
    fFilterCutOff->using_log = true;
    fFilterCutOff->format_str = "%.f Hz";
    fFilterCutOff->setParamOnMove = true;

    fFilterResonance = new Knob(hbox_filter_row_1);
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
    fFilterResonance->real_min = 0.0f;
    fFilterResonance->real_max = 40.0f;
    fFilterResonance->format_str = "%.2f dB";
    fFilterResonance->setParamOnMove = true;

    fFilterEGDepth = new Knob(hbox_filter_row_2);
    fFilterEGDepth->setId(kFilterEgDepth);
    fFilterEGDepth->setSize(knobSize);
    fFilterEGDepth->setCallback(this);
    fFilterEGDepth->label = "DEPTH";
    fFilterEGDepth->labelSize = font_size;
    fFilterEGDepth->margin = margin;
    fFilterEGDepth->gauge_width = gauge;
    fFilterEGDepth->foreground_color = blue_pigment_1;
    fFilterEGDepth->background_color = black_olive;
    fFilterEGDepth->highlight_color = blue_pigment_2;
    fFilterEGDepth->text_color = floral_white;
    fFilterEGDepth->real_min = 0.0f;
    fFilterEGDepth->real_max = 12000.0f;
    fFilterEGDepth->format_str = "%.f Ct";
    fFilterEGDepth->setParamOnMove = true;

    fFilterEgAttack = new Knob(hbox_filter_row_2);
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
    fFilterEgAttack->real_min = 0.0f;
    fFilterEgAttack->real_max = 10.0f;
    fFilterEgAttack->min = 1.0f;
    fFilterEgAttack->max = 101.f;
    fFilterEgAttack->format_str = "%.2f s";
    fFilterEgAttack->using_log = true;
    fFilterEgAttack->setParamOnMove = true;

    fFilterEgDecay = new Knob(hbox_filter_row_2);
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
    fFilterEgDecay->real_min = 0.0f;
    fFilterEgDecay->real_max = 10.f;
    fFilterEgDecay->min = 1.0f;
    fFilterEgDecay->max = 101.f;
    fFilterEgDecay->using_log = true;
    fFilterEgDecay->format_str = "%.2f s";
    fFilterEgDecay->setParamOnMove = true;

    fFilterEgSustain = new Knob(hbox_filter_row_2);
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
    fFilterEgSustain->real_min = 0.0f;
    fFilterEgSustain->real_max = 100.f;
    fFilterEgSustain->format_str = "%.f%%";
    fFilterEgSustain->setParamOnMove = true;

    fFilterEgRelease = new Knob(hbox_filter_row_2);
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
    fFilterEgRelease->real_min = 0.0f;
    fFilterEgRelease->real_max = 10.f;
    fFilterEgRelease->min = 1.0f;
    fFilterEgRelease->max = 101.f;
    fFilterEgRelease->format_str = "%.2f s";
    fFilterEgRelease->using_log = true;
    fFilterEgRelease->setParamOnMove = true;

    fFilterLFOType = new DropDown(hbox_filter_row_3);
    fFilterLFOType->setId(kFilterLFOType);
    fFilterLFOType->font_size = font_size + 4;
    fFilterLFOType->setFont("RobotoRegular",
                            reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                            fonts::Roboto_RegularDataSize);
    fFilterLFOType->setMenuFont("DropsIcons",
                                reinterpret_cast<const uchar *>(fonts::drops_iconsData),
                                fonts::drops_iconsDataSize);
    fFilterLFOType->setSize(148,
                            fFilterLFOType->font_size + fFilterLFOType->margin * 2.0f);
    fFilterLFOType->setCallback(this);
    fFilterLFOType->label = "TYPE :";
    fFilterLFOType->item = "a";
    fFilterLFOType->foreground_color = blue_pigment_1;
    fFilterLFOType->background_color = black_olive;
    fFilterLFOType->text_color = floral_white;
    // resize later

    fFilterLFOSync = new CheckBox(hbox_filter_row_3);
    fFilterLFOSync->setId(kFilterLFOSync);
    fFilterLFOSync->setSize(knobSize);
    fFilterLFOSync->setCallback(this);
    fFilterLFOSync->background_color = black_olive;
    fFilterLFOSync->foreground_color = blue_pigment_1;
    fFilterLFOSync->highlight_color_inactive = black_olive_1;
    fFilterLFOSync->highlight_color_active = blue_pigment_2;
    fFilterLFOSync->text_color = floral_white;
    fFilterLFOSync->setFont("RobotoRegular",
                            reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                            fonts::Roboto_RegularDataSize);
    fFilterLFOSync->margin = 6.f;
    fFilterLFOSync->label = "SYNC";
    fFilterLFOSync->boxSize = 20;
    fFilterLFOSync->labelSize = font_size;

    fFilterLFOTypeMenu = new Menu(hbox_filter_row_3);
    fFilterLFOTypeMenu->setMaxViewItems(5);
    fFilterLFOTypeMenu->setId(kFilterLFOTypeMenu);
    fFilterLFOTypeMenu->setCallback(this);
    fFilterLFOTypeMenu->font_size = font_size + 4;
    fFilterLFOTypeMenu->setFont("Drops Icons",
                                reinterpret_cast<const uchar *>(fonts::drops_iconsData),
                                fonts::drops_iconsDataSize);
    fFilterLFOTypeMenu->addItems({"a",
                                  "b",
                                  "c",
                                  "d",
                                  "e"});
    fFilterLFOTypeMenu->hide();
    fFilterLFOTypeMenu->background_color = black_olive;
    fFilterLFOTypeMenu->foreground_color = black_olive_2;
    fFilterLFOTypeMenu->highlight_color = blue_pigment_1;
    fFilterLFOTypeMenu->text_color = floral_white;

    fFilterLFOFreq = new Knob(hbox_filter_row_3);
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
    fFilterLFOFreq->real_min = 0.0f;
    fFilterLFOFreq->real_max = 20.f;
    fFilterLFOFreq->min = 1.0f;
    fFilterLFOFreq->max = 101.f;
    fFilterLFOFreq->format_str = "%.2f Hz";
    fFilterLFOFreq->using_log = false;
    fFilterLFOFreq->setStepText({
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
    fFilterLFOFreq->setParamOnMove = true;

    fFilterLFODepth = new Knob(hbox_filter_row_3);
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
    fFilterLFODepth->default_value = 0.5f;
    fFilterLFODepth->real_min = 0.0f;
    fFilterLFODepth->real_max = max_cutoff / 2.0f;
    fFilterLFODepth->format_str = "%.f Hz";
    fFilterLFODepth->setParamOnMove = true;

    fFilterLFOFade = new Knob(hbox_filter_row_3);
    fFilterLFOFade->setId(kFilterLFOFade);
    fFilterLFOFade->setCallback(this);
    fFilterLFOFade->setSize(knobSize);
    fFilterLFOFade->label = "FADE";
    fFilterLFOFade->labelSize = font_size;
    fFilterLFOFade->margin = margin;
    fFilterLFOFade->gauge_width = gauge;
    fFilterLFOFade->background_color = black_olive;
    fFilterLFOFade->foreground_color = blue_pigment_1;
    fFilterLFOFade->highlight_color = blue_pigment_2;
    fFilterLFOFade->text_color = floral_white;
    fFilterLFOFade->default_value = 0.0f;
    fFilterLFOFade->real_min = 0.0f;
    fFilterLFOFade->real_max = 10.0f;
    fFilterLFOFade->min = 1.0f;
    fFilterLFOFade->max = 101.f;
    fFilterLFOFade->format_str = "%.2f s";
    fFilterLFOFade->using_log = true;
    fFilterLFOFade->setParamOnMove = true;

    fFilterLFOType->setMenu(fFilterLFOTypeMenu);
    fFilterLFOType->resize();

    vbox_filter->addWidget(hbox_filter_row_1);
    //  hbox_filter_row_1->addWidget(hbox_filter_row_1_spacer);
    hbox_filter_row_1->addWidget(fFilterLowpass);
    hbox_filter_row_1->addWidget(fFilterBandpass);
    hbox_filter_row_1->addWidget(fFilterHighpass);
    hbox_filter_row_1->addWidget(fFilterCutOff);
    hbox_filter_row_1->addWidget(fFilterResonance);

    vbox_filter->addWidget(hbox_filter_row_2);
    hbox_filter_row_2->addWidget(fFilterEGDepth);
    hbox_filter_row_2->addWidget(fFilterEgAttack);
    hbox_filter_row_2->addWidget(fFilterEgDecay);
    hbox_filter_row_2->addWidget(fFilterEgSustain);
    hbox_filter_row_2->addWidget(fFilterEgRelease);

    vbox_filter->addWidget(hbox_filter_row_3);
    hbox_filter_row_3->addWidget(hbox_filter_row_3_spacer);
    hbox_filter_row_3_spacer->addWidget(fFilterLFOType);
    hbox_filter_row_3->addWidget(fFilterLFOSync);
    hbox_filter_row_3->addWidget(fFilterLFOFreq);
    hbox_filter_row_3->addWidget(fFilterLFODepth);
    hbox_filter_row_3->addWidget(fFilterLFOFade);

    vbox_filter->positionWidgets();

    hbox_filter_row_1->positionWidgets();
    //hbox_filter_row_1_spacer->positionWidgets();
    hbox_filter_row_2->positionWidgets();
    hbox_filter_row_3->positionWidgets();
    hbox_filter_row_3_spacer->positionWidgets();

    fFilterLFOType->positionMenu();

    //fFilterLFOSync->setMenu(fFilterLFOSyncMenu);
}

END_NAMESPACE_DISTRHO
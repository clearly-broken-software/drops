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
    
    fAmpLFOFreqBeat = new RadioButton(box_amp_row_1);
    fAmpLFOFreqBeat->setId(9999);
    fAmpLFOFreqBeat->font_size = 16.f;
    fAmpLFOFreqBeat->margin = 2.0f;
    fAmpLFOFreqBeat->addOption("FREQ: ");
    fAmpLFOFreqBeat->addOption("SYNC: ");
    fAmpLFOFreqBeat->setSize(50, 50);

    box_amp->setAbsolutePos(tabs_x, tabs_y);
    box_amp->setWidth(tabs_w);
    box_amp->setHeight(tabs_h);
    
    box_amp_row_1->setWidth(box_amp->getWidth());
    box_amp->addWidget(box_amp_row_1);
    box_amp->setWidgetResize(kHBoxSampleRow1, true);

    box_amp_row_1->addWidget(fAmpLFOFreqBeat);


    /*     const Size<uint> knobSize = Size<uint>(76, 76);
    const uint knobSpacing = knobSize.getWidth() + 15;

    fAmpEgAttack = new Knob(window);
    fAmpEgAttack->setId(kAmpEgAttack);
    fAmpEgAttack->setSize(knobSize);
    fAmpEgAttack->setCallback(this);
    //fAmpEgAttack->setAbsolutePos(100, 350);
    fAmpEgAttack->label = "ATTACK";
    fAmpEgAttack->foreground_color = floral_white;
    fAmpEgAttack->background_color = black_olive;
    fAmpEgAttack->highlight_color = flame;
    fAmpEgAttack->text_color = floral_white;

    fAmpEgDecay = new Knob(window);
    fAmpEgDecay->setId(kAmpEgDecay);
    fAmpEgDecay->setSize(knobSize);
    fAmpEgDecay->setCallback(this);
    //fAmpEgDecay->setAbsolutePos(100 + 1 * knobSpacing, 350);
    fAmpEgDecay->label = "DECAY";
    fAmpEgDecay->foreground_color = floral_white;
    fAmpEgDecay->background_color = black_olive;
    fAmpEgDecay->highlight_color = flame;
    fAmpEgDecay->text_color = floral_white;

    fAmpEgSustain = new Knob(window);
    fAmpEgSustain->setId(kAmpEgSustain);
    fAmpEgSustain->setSize(knobSize);
    fAmpEgSustain->setCallback(this);
    //fAmpEgSustain->setAbsolutePos(100 + 2 * knobSpacing, 350);
    fAmpEgSustain->label = "SUSTAIN";
    fAmpEgSustain->foreground_color = floral_white;
    fAmpEgSustain->background_color = black_olive;
    fAmpEgSustain->highlight_color = flame;
    fAmpEgSustain->text_color = floral_white;

    fAmpEgRelease = new Knob(window);
    fAmpEgRelease->setId(kAmpEgRelease);
    fAmpEgRelease->setSize(knobSize);
    fAmpEgRelease->setCallback(this);
    //fAmpEgRelease->setAbsolutePos(100 + 3 * knobSpacing, 350);
    fAmpEgRelease->label = "RELEASE";
    fAmpEgRelease->foreground_color = floral_white;
    fAmpEgRelease->background_color = black_olive;
    fAmpEgRelease->highlight_color = flame;
    fAmpEgRelease->text_color = floral_white; */

   
}

END_NAMESPACE_DISTRHO
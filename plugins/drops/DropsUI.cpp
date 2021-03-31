/*
license here
*/

#include "DropsUI.hpp"
#include <iostream>

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

using namespace artwork;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------

DropsUI::DropsUI()
    : UI(UI_W, UI_H)
{
    plugin = static_cast<DropsPlugin *>(getPluginInstancePointer());
    sampleDir = "";
    fileName = "";
    mainFont = createFontFromMemory("Roboto_Regular",
                                    reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                    fonts::Roboto_RegularDataSize, false);
    fontFaceId(mainFont);

    viewStart = 0;
    viewEnd = 0;
    viewZoom = 0;
    viewMaxZoom = 0;
    mouseX = 0;

    sampleChannels = 0;
    sampleLength = 0;
    filepath = "";
    waveForm = &plugin->waveForm;
    miniMap = &plugin->miniMap;
    sig_sampleLoaded = false;
    showWaveForm = false;
    loopstartDragging = false;
    loopendDragging = false;
    scrollbarDragging = false;
    sampleInDragging = false;
    sampleOutDragging = false;
    display.setSize(display_width, display_height);
    display.setPos(display_left, display_top);
    scrollbarDragging = false;

    /* for testing */
    sampleLoopStart = 0;
    sampleLoopEnd = 0;
    sampleIn = 0;
    sampleOut = 0;
    /* ----------- */

    scale = 1.0f;
    makeIcons();
    initWidgets();
    if (plugin->loadedSample)
    {
        loadSample();
        std::string filename = plugin->path;
        sampleDir = dirnameOf(filename.c_str());
        fileopen_button->setText(filename);
    }
}

DropsUI::~DropsUI()
{
}

void DropsUI::initWidgets()
{
    Window &window = getParentWindow();
    fileopen_button = new FileOpenButton(window);
    fileopen_button->setCallback(this);
    fileopen_button->setAbsolutePos(238, 0);
    fileopen_button->setSize(530, 55);
    fileopen_button->background_color = eerie_black_3;
    fileopen_button->text_color = floral_white;
    fileopen_button->font_size = 24.f;

    fScrollBarHandle = new ScrollBar(window);
    fScrollBarHandle->setId(kScrollbarHandle);
    fScrollBarHandle->setSize(display_width, minimap_height);
    fScrollBarHandle->setAbsolutePos(display_left, display_bottom);
    fScrollBarHandle->setCallback(this);

    fScrollBarLeft = new ScrollBar(window);
    fScrollBarLeft->setId(kScrollbarLeft);
    fScrollBarLeft->setSize(0, minimap_height);
    fScrollBarLeft->setAbsolutePos(display_left, display_bottom);
    fScrollBarLeft->setCallback(this);

    fScrollBarRight = new ScrollBar(window);
    fScrollBarRight->setId(kScrollbarRight);
    fScrollBarRight->setSize(0, minimap_height);
    fScrollBarRight->setAbsolutePos(display_right, display_bottom);
    fScrollBarRight->setCallback(this);

    fLoopStart = new ScrollBar(window);
    fLoopStart->setId(kSampleLoopStart);
    fLoopStart->setSize(35, 35); // FIXME: hardcoded
    fLoopStart->setCallback(this);
    fLoopStart->hide();

    fLoopEnd = new ScrollBar(window);
    fLoopEnd->setId(kSampleLoopEnd);
    fLoopEnd->setSize(35, 35);
    fLoopEnd->setCallback(this);
    fLoopEnd->hide();

    fSampleIn = new ScrollBar(window);
    fSampleIn->setId(kSampleIn);
    fSampleIn->setSize(35, 35);
    fSampleIn->setCallback(this);
    fSampleIn->hide();

    fSampleOut = new ScrollBar(window);
    fSampleOut->setId(kSampleOut);
    fSampleOut->setSize(35, 35);
    fSampleOut->setCallback(this);
    fSampleOut->hide();

    /*  box_tabs = new HBox(window);

    const float clrScale = 1.5f;
    const Color buttonBackClr(eerie_black.red * clrScale,
                              eerie_black.green * clrScale,
                              eerie_black.blue * clrScale,
                              1.0f);

    hbox_zoom_icons = new HBox(this);
    hbox_zoom_icons->setId(kHboxZoomIcons);
    hbox_zoom_icons->setAbsolutePos(15, 70);
    hbox_zoom_icons->setWidth(150);

    fZoomOut = new SVGButton(hbox_zoom_icons, zoom_out, zoom_out_hl);
    fZoomOut->setId(kZoomOut);
    fZoomOut->setCallback(this);

    fZoomIn = new SVGButton(hbox_zoom_icons, zoom_in, zoom_in_hl);
    fZoomIn->setId(kZoomIn);
    fZoomIn->setCallback(this);

    fZoomAll = new SVGButton(hbox_zoom_icons, zoom_all, zoom_all_hl);
    fZoomAll->setId(kZoomAll);
    fZoomAll->setCallback(this);

    fZoomInOut = new SVGButton(hbox_zoom_icons, zoom_in_out, zoom_in_out_hl);
    fZoomInOut->setId(kZoomInOut);
    fZoomInOut->setCallback(this);

    hbox_zoom_icons->addWidget(fZoomOut);
    hbox_zoom_icons->addWidget(fZoomIn);
    hbox_zoom_icons->addWidget(fZoomAll);
    hbox_zoom_icons->addWidget(fZoomInOut);

    hbox_zoom_icons->positionWidgets(); */

    const float fSampleFontSize = 14;
    fSamplePitchKeyCenter = new DropDown(window);
    fSamplePitchKeyCenter->setId(kSamplePitchKeyCenter);
    fSamplePitchKeyCenter->font_size = fSampleFontSize;
    fSamplePitchKeyCenter->setFont("RobotoRegular",
                                   reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                   fonts::Roboto_RegularDataSize);
    fSamplePitchKeyCenter->setMenuFont("RobotoRegular",
                                       reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                       fonts::Roboto_RegularDataSize);
    fSamplePitchKeyCenter->setSize(175,
                                   fSamplePitchKeyCenter->font_size +
                                       fSamplePitchKeyCenter->margin * 2.0f);
    fSamplePitchKeyCenter->setAbsolutePos(175, 77);
    fSamplePitchKeyCenter->setCallback(this);
    fSamplePitchKeyCenter->label = "PITCH KEY-CENTER: ";
    fSamplePitchKeyCenter->item = "C4";
    fSamplePitchKeyCenter->foreground_color = black_olive_2;
    fSamplePitchKeyCenter->background_color = black_olive;
    fSamplePitchKeyCenter->text_color = floral_white;

    fKeyCenterMenu = new Menu(window);
    fKeyCenterMenu->setId(kKeyCenterMenu);
    fKeyCenterMenu->setCallback(this);
    fKeyCenterMenu->setFont("RobotoRegular",
                            reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                            fonts::Roboto_RegularDataSize);
    fKeyCenterMenu->font_size = fSampleFontSize;
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
    fKeyCenterMenu->hide();
    fKeyCenterMenu->background_color = black_olive;
    fKeyCenterMenu->foreground_color = black_olive_2;
    fKeyCenterMenu->highlight_color = flame;
    fKeyCenterMenu->text_color = floral_white;
    fKeyCenterMenu->item_view_first = 60;
    fKeyCenterMenu->item_view_last = 63;
    fKeyCenterMenu->setMaxViewItems(4);

    fSamplePitchKeyCenter->setMenu(fKeyCenterMenu);
    fSamplePitchKeyCenter->resize();

    fSamplePitch = new Slider(window);
    fSamplePitch->setId(kSamplePitch);
    fSamplePitch->setCallback(this);
    fSamplePitch->setSize(200, fSampleFontSize + 2);
    fSamplePitch->font_size = fSampleFontSize;
    fSamplePitch->setLabel("TUNE: ");
    fSamplePitch->background_color = black_olive;
    fSamplePitch->foreground_color = flame;
    fSamplePitch->highlight_color = flame_1;
    fSamplePitch->text_color = floral_white;
    fSamplePitch->unit = "Ct";
    Rectangle<float> bounds;
    fontFaceId(mainFont);
    fontSize(fSamplePitch->font_size);
    textBounds(0, 0, "-100 Ct", nullptr, bounds);
    const float padding = bounds.getWidth();
    fSamplePitch->right_padding = padding;
    fSamplePitch->format_str = "%.f";
    fSamplePitch->min_value = -100.0f;
    fSamplePitch->max_value = 100.0f;
    fSamplePitch->default_value = 0.0f;

    fSamplePlayDirection = new DropDown(window);
    fSamplePlayDirection->setId(kSamplePlayDirection);
    fSamplePlayDirection->font_size = fSampleFontSize;
    fSamplePlayDirection->setFont("RobotoRegular",
                                  reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                  fonts::Roboto_RegularDataSize);
    fSamplePlayDirection->setMenuFont("RobotoRegular",
                                      reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                      fonts::Roboto_RegularDataSize);
    fSamplePlayDirection->setSize(216,
                                  fSamplePlayDirection->font_size +
                                      fSamplePlayDirection->margin * 2.0f);
    fSamplePlayDirection->setAbsolutePos(536, 77);
    fSamplePlayDirection->setCallback(this);
    fSamplePlayDirection->label = "DIRECTION: ";
    fSamplePlayDirection->item = "FORWARD";
    fSamplePlayDirection->foreground_color = floral_white;
    fSamplePlayDirection->background_color = black_olive;
    fSamplePlayDirection->text_color = floral_white;

    fDirectionMenu = new Menu(window);
    fDirectionMenu->setId(kDirectionMenu);
    fDirectionMenu->setCallback(this);
    fDirectionMenu->font_size = fSampleFontSize;
    fDirectionMenu->setFont("RobotoRegular",
                            reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                            fonts::Roboto_RegularDataSize);

    fDirectionMenu->addItems({"FORWARD", "REVERSE"});
    fDirectionMenu->hide();
    fDirectionMenu->background_color = black_olive;
    fDirectionMenu->foreground_color = black_olive_2;
    fDirectionMenu->highlight_color = flame;
    fDirectionMenu->text_color = floral_white;

    fSamplePlayDirection->setMenu(fDirectionMenu);
    fSamplePlayDirection->resize();

    fSamplePlayMode = new DropDown(window);
    fSamplePlayMode->setId(kSamplePlayMode);
    fSamplePlayMode->font_size = fSampleFontSize;
    fSamplePlayMode->setSize(216,
                             fSamplePlayMode->font_size +
                                 fSamplePlayMode->margin * 2.0f);
    fSamplePlayMode->setFont("RobotoRegular",
                             reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                             fonts::Roboto_RegularDataSize);
    fSamplePlayMode->setMenuFont("RobotoRegular",
                                 reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                 fonts::Roboto_RegularDataSize);
    fSamplePlayMode->setCallback(this);
    fSamplePlayMode->label = "LOOP MODE: ";
    fSamplePlayMode->item = "NO LOOP";
    fSamplePlayMode->foreground_color = floral_white;
    fSamplePlayMode->background_color = black_olive;
    fSamplePlayMode->text_color = floral_white;

    fPlayModeMenu = new Menu(window);
    fPlayModeMenu->setId(kPlayModeMenu);
    fPlayModeMenu->setCallback(this);
    fPlayModeMenu->font_size = fSampleFontSize;
    fPlayModeMenu->setFont("RobotoRegular",
                           reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                           fonts::Roboto_RegularDataSize);
    fPlayModeMenu->addItems({"NO LOOP", "ONE SHOT", "CONTINUOUS", "SUSTAIN"});
    fPlayModeMenu->hide();
    fPlayModeMenu->background_color = black_olive;
    fPlayModeMenu->foreground_color = black_olive_2;
    fPlayModeMenu->highlight_color = flame;
    fPlayModeMenu->text_color = floral_white;

    fSamplePlayMode->setMenu(fPlayModeMenu);
    fSamplePlayMode->resize();

    fSampleOversampling = new DropDown(window);
    fSampleOversampling->setId(kSampleOversampling);
    fSampleOversampling->font_size = fSampleFontSize;
    fSampleOversampling->setFont("RobotoRegular",
                                 reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                 fonts::Roboto_RegularDataSize);
    fSampleOversampling->setMenuFont("RobotoRegular",
                                     reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                                     fonts::Roboto_RegularDataSize);
    fSampleOversampling->setSize(116, fSamplePlayMode->font_size + fSamplePlayMode->margin * 2.0f);
    fSampleOversampling->setCallback(this);
    fSampleOversampling->label = "OVERSAMPLING: ";
    fSampleOversampling->item = "1x";
    fSampleOversampling->foreground_color = floral_white;
    fSampleOversampling->background_color = black_olive;
    fSampleOversampling->text_color = floral_white;

    fOversamplingMenu = new Menu(window);
    fOversamplingMenu->setId(kOversamplingMenu);
    fOversamplingMenu->setCallback(this);
    fOversamplingMenu->font_size = fSampleFontSize;
    fOversamplingMenu->setFont("RobotoRegular",
                               reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                               fonts::Roboto_RegularDataSize);
    fOversamplingMenu->addItems({"1x", "2x", "4x", "8x"});
    fOversamplingMenu->hide();
    fOversamplingMenu->background_color = black_olive;
    fOversamplingMenu->foreground_color = black_olive_2;
    fOversamplingMenu->highlight_color = flame;
    fOversamplingMenu->text_color = floral_white;

    fSampleOversampling->setMenu(fOversamplingMenu);
    fSampleOversampling->resize();

    hbox_sample = new HBox(window);
    hbox_sample->setAbsolutePos(0, 77);
    hbox_sample->setSize(UI_W, 20);
    hbox_sample->addWidget(fSamplePitchKeyCenter);
    hbox_sample->addWidget(fSamplePitch);
    hbox_sample->addWidget(fSamplePlayDirection);
    hbox_sample->addWidget(fSamplePlayMode);
    hbox_sample->addWidget(fSampleOversampling);
    hbox_sample->positionWidgets();

    fSamplePitchKeyCenter->positionMenu();
    fSamplePlayDirection->positionMenu();
    fSamplePlayMode->positionMenu();
    fSampleOversampling->positionMenu();

    initTabAmp();
    initTabFilter();
    initTabPitch();
    // setup popup, needs to be last to get it on top
    fPopUp = new PopUp(window);
    fPopUp->setFont("RobotoRegular",
                    reinterpret_cast<const uchar *>(fonts::Roboto_RegularData),
                    fonts::Roboto_RegularDataSize);
    fPopUp->setText("POP UP TEXT");
    fPopUp->hide();
    fAmpLFOFreq->setPopUp(fPopUp);
    fAmpLFODepth->setPopUp(fPopUp);
    fAmpLFOFade->setPopUp(fPopUp);
    fAmpEgAttack->setPopUp(fPopUp);
    fAmpEgDecay->setPopUp(fPopUp);
    fAmpEgSustain->setPopUp(fPopUp);
    fAmpEgRelease->setPopUp(fPopUp);

    fFilterCutOff->setPopUp(fPopUp);
    fFilterResonance->setPopUp(fPopUp);
    fFilterEGDepth->setPopUp(fPopUp);
    fFilterEgAttack->setPopUp(fPopUp);
    fFilterEgDecay->setPopUp(fPopUp);
    fFilterEgSustain->setPopUp(fPopUp);
    fFilterEgRelease->setPopUp(fPopUp);
    fFilterLFOFreq->setPopUp(fPopUp);
    fFilterLFODepth->setPopUp(fPopUp);
    fFilterLFOFade->setPopUp(fPopUp);

    fPitchEgDepth->setPopUp(fPopUp);
    fPitchEgAttack->setPopUp(fPopUp);
    fPitchEgDecay->setPopUp(fPopUp);
    fPitchEgSustain->setPopUp(fPopUp);
    fPitchEgRelease->setPopUp(fPopUp);
    fPitchLFOFreq->setPopUp(fPopUp);
    fPitchLFODepth->setPopUp(fPopUp);
    fPitchLFOFade->setPopUp(fPopUp);
}

void DropsUI::makeIcons()
{
    dropsLogo = new SVGImage(this, drops_logo, 1.0f);
    loopLeft = new SVGImage(this, loop_left, 1.0f);
    loopRight = new SVGImage(this, loop_right, 1.0f);
    clearlyBrokenLogo = new SVGImage(this, artwork::clearly_broken_logo, 0.8f);
}

std::string DropsUI::dirnameOf(const std::string &fname)
{
    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos)
               ? ""
               : fname.substr(0, pos);
}

void DropsUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case kSampleLoaded:
    {
        bool oldSampleLoaded = sig_sampleLoaded;
        if (static_cast<bool>(value) == oldSampleLoaded)
        {
            break;
        }
        sig_sampleLoaded = value;
        if (sig_sampleLoaded)
        {
            fileopen_button->setText(fileName);
            loadSample();
        }
        break;
    }
    case kSampleIn:
        sampleIn = value * static_cast<float>(sampleLength);
        setMarkers(); // FIXME: all markers are set, only 1 is needed :-/
        break;
    case kSampleOut:
        sampleOut = value * static_cast<float>(sampleLength);
        setMarkers();
        break;
    case kSampleLoopStart:
        sampleLoopStart = value * static_cast<float>(sampleLength);
        setMarkers();
        break;
    case kSampleLoopEnd:
        sampleLoopEnd = value * static_cast<float>(sampleLength);
        setMarkers();
        break;
    case kSamplePitchKeyCenter:
    {
        float keyValue = 127.f - value;
        fSamplePitchKeyCenter->setValue(keyValue);
        break;
    }
    case kSamplePitch:
        fSamplePitch->setValue(value - 100);
        break;
    case kSamplePlayMode:
        fSamplePlayMode->setValue(value);
        break;
    case kSamplePlayDirection:
        fSamplePlayDirection->setValue(value);
        break;
    case kSampleOversampling:
        fSampleOversampling->setValue(value);
        break;
    // amp
    case kAmpLFOFade:
        value = (fAmpLFOFade->max - fAmpLFOFade->min) * value + fAmpLFOFade->min;
        fAmpLFOFade->setValue(value);
        break;
    case kAmpEgAttack:
        value = (fAmpEgAttack->max - fAmpEgAttack->min) * value + fAmpEgAttack->min;
        fAmpEgAttack->setValue(value);
        repaint();
        break;
    case kAmpEgDecay:
        value = (fAmpEgDecay->max - fAmpEgDecay->min) * value + fAmpEgDecay->min;
        fAmpEgDecay->setValue(value);
        repaint();
        break;
    case kAmpEgSustain:
        fAmpEgSustain->setValue(value);
        repaint();
        break;
    case kAmpLFOType:
        fAmpLFOType->setValue(value);
        repaint();
        break;
    case kAmpLFOFreq:
        value = (fAmpLFOFreq->max - fAmpLFOFreq->min) * value + fAmpLFOFreq->min;
        fAmpLFOFreq->setValue(value);
        repaint();
        break;
    case kAmpLFODepth:
        fAmpLFODepth->setValue(value);
        repaint();
        break;
    case kAmpEgRelease:
        value = (fAmpEgRelease->max - fAmpEgRelease->min) * value + fAmpEgRelease->min;
        fAmpEgRelease->setValue(value);
        repaint();
        break;
    // filter
    case kFilterType:
    {
        const uint val = static_cast<uint>(value);
        switch (val)
        {
        case 0:
        {
            fFilterLowpass->is_active = true;
            fFilterBandpass->is_active = false;
            fFilterHighpass->is_active = false;
        }
        break;
        case 1:
        {
            fFilterLowpass->is_active = false;
            fFilterBandpass->is_active = true;
            fFilterHighpass->is_active = false;
        }
        break;
        case 2:
        {
            fFilterLowpass->is_active = false;
            fFilterBandpass->is_active = false;
            fFilterHighpass->is_active = true;
        }
        break;
        default:
            break;
        }
        break;
    }
    case kFilterCutOff:
        value = (fFilterCutOff->max - fFilterCutOff->min) * value + fFilterCutOff->min;
        fFilterCutOff->setValue(value);
        break;
    case kFilterResonance:
        fFilterResonance->setValue(value);
        break;
    case kFilterEgDepth:
        fFilterEGDepth->setValue(value);
        break;
    case kFilterEgAttack:
        value = (fFilterEgAttack->max - fFilterEgAttack->min) * value + fFilterEgAttack->min;
        fFilterEgAttack->setValue(value);
        break;
    case kFilterEgDecay:
        value = (fFilterEgDecay->max - fFilterEgDecay->min) * value + fFilterEgDecay->min;
        fFilterEgDecay->setValue(value);
        break;
    case kFilterEgSustain:
        fFilterEgSustain->setValue(value);
        break;
    case kFilterEgRelease:
        value = (fFilterEgRelease->max - fFilterEgRelease->min) * value + fFilterEgRelease->min;
        fFilterEgRelease->setValue(value);
        break;
    case kFilterLFOType:
        fFilterLFOType->setValue(value);
        break;
    case kFilterLFOFreq:
        value = (fFilterLFOFreq->max - fFilterLFOFreq->min) * value + fFilterLFOFreq->min;
        fFilterLFOFreq->setValue(value);
        break;
    case kFilterLFODepth:
        fFilterLFODepth->setValue(value);
        break;
    case kFilterLFOFade:
        value = (fFilterLFOFade->max - fFilterLFOFade->min) * value + fFilterLFOFade->min;
        break;

    //  pitch
    case kPitchEgDepth:
        fPitchEgDepth->setValue(value);
        break;
    case kPitchEgAttack:
        value = (fPitchEgAttack->max - fPitchEgAttack->min) * value + fPitchEgAttack->min;
        fPitchEgAttack->setValue(value);
        break;
    case kPitchEgDecay:
        value = (fPitchEgDecay->max - fPitchEgDecay->min) * value + fPitchEgDecay->min;
        fPitchEgDecay->setValue(value);
        break;
    case kPitchEgSustain:
        fPitchEgSustain->setValue(value);
        break;
    case kPitchEgRelease:
        value = (fFilterEgRelease->max - fFilterEgRelease->min) * value + fFilterEgRelease->min;
        fPitchEgRelease->setValue(value);
        break;
    case kPitchLFOType:
        fPitchLFOType->setValue(value);
        break;
    case kPitchLFOFreq:
        value = (fPitchLFOFreq->max - fPitchLFOFreq->min) * value + fPitchLFOFreq->min;
        fPitchLFOFreq->setValue(value);
        break;
    case kPitchLFOFade:
        value = (fPitchLFOFade->max - fPitchLFOFade->min) * value + fPitchLFOFade->min;
        fPitchLFOFade->setValue(value);
        break;
    case kPitchLFODepth:
        value = (fPitchLFOFade->max - fPitchLFOFade->min) * value + fPitchLFOFade->min;
        fPitchLFODepth->setValue(value);
        break;

    default:
        //       printf("DropsUI::parameterChanged(%i,%f)\n", index, value);
        break;
    }
    repaint();
}

int DropsUI::loadSample()
{
    sampleLength = static_cast<sf_count_t>(waveForm->size() - 1);
    sampleIn = 0;
    sampleOut = sampleLength;
    sampleLoopStart = 0;
    sampleLoopEnd = sampleLength;
    viewStart = 0;
    viewEnd = sampleLength;
    viewZoom = 1.0f;
    viewMaxZoom = float(sampleLength) / float(display_width);

    // make minimap
    /* FIXME: only set this when there are loop points */
    float samples_per_pixel = static_cast<float>(sampleLength) / static_cast<float>(display_width);
    float loopStartPixel = static_cast<float>(sampleLoopStart) / samples_per_pixel + static_cast<float>(display_left);
    float loopEndPixel = static_cast<float>(sampleLoopEnd) / samples_per_pixel + static_cast<float>(display_left);
    float sampleInPixel = static_cast<float>(sampleIn) / samples_per_pixel + static_cast<float>(display_left);
    float sampleOutPixel = static_cast<float>(sampleOut) / samples_per_pixel + static_cast<float>(display_left);

    fLoopStart->setAbsolutePos(loopStartPixel - 35 - 1, display_bottom - 35);
    fLoopEnd->setAbsolutePos(loopEndPixel, display_bottom - 35);
    fSampleIn->setAbsolutePos(sampleInPixel - 35 - 1, display_top);
    fSampleOut->setAbsolutePos(sampleOutPixel, display_top);
    fLoopStart->show();
    fLoopEnd->show();
    fSampleIn->show();
    fSampleOut->show();
    setState("ui_sample_loaded", "true");
    showWaveForm = true;

    return 0;
}

void DropsUI::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();
    // background
    beginPath();
    fillColor(eerie_black);
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();

    // display

    beginPath();
    fillColor(black_olive);
    rect(display_left, display_top, display_width, display_height);
    fill();
    closePath();

    if (showWaveForm)
    {
        drawWaveform();
        drawMinimap();
        drawLoopMarkers();
        drawInOutMarkers();
    }

    // draw logos
    uint w = dropsLogo->getWidth();
    uint h = dropsLogo->getHeight();
    int x = fileopen_button->getAbsoluteX() / 2 - w / 2;
    int y = fileopen_button->getHeight() / 2 - h / 2;
    dropsLogo->drawAt(x, y);
    const int fo_right = fileopen_button->getAbsoluteX() + fileopen_button->getWidth();
    const int half_right_space = (width - fo_right) / 2;
    const int half_cb_logo = clearlyBrokenLogo->getWidth() / 2;
    x = fo_right + half_right_space - half_cb_logo;
    y = fileopen_button->getHeight() / 2 - clearlyBrokenLogo->getHeight() / 2;
    clearlyBrokenLogo->drawAt(x, y);

    // VBOX_AMP xywh {12 329 323 176}
    beginPath();
    fillColor(eerie_black);
    rect(12, 329, 323, 211);
    fill();
    closePath();

    beginPath();
    strokeWidth(2.0f);
    strokeColor(saffron);
    rect(12 + 2, 329 + 2, 323 - 4, 211 - 4);
    stroke();
    closePath();

    beginPath();
    fillColor(saffron);
    roundedRect(12 + 2, 329 + 2, 40, 18, 2);
    fill();
    closePath();

    beginPath();
    fontSize(16);
    textAlign(ALIGN_MIDDLE | ALIGN_CENTER);
    float cx = 12 + 2 + 40 / 2;
    float cy = 329 + 2 + 18 / 2;
    fillColor(eerie_black);
    text(cx, cy, "AMP", nullptr);
    closePath();

    beginPath();
    moveTo(12 + 2, 329 + 211 / 2);
    lineTo(332, 329 + 211 / 2);
    stroke();
    closePath();

    beginPath();
    fillColor(saffron);
    roundedRect(12 + 2, 329 + 211 / 2, 40, 18, 2);
    fill();
    closePath();
    beginPath();
    fontSize(16);
    fillColor(eerie_black);
    cy = 329 + 2 + 211 / 2 + 18 / 2;
    text(cx, cy, "LFO", nullptr);
    closePath();

    // VBOX_FILTER xywh {339 329 320 176}
    // back
    beginPath();
    fillColor(eerie_black);
    rect(339, 329, 323, 211);
    fill();
    closePath();

    // outer box
    beginPath();
    strokeWidth(2.0f);
    strokeColor(blue_pigment_1);
    rect(339 + 2, 329 + 2,
         323 - 4, 211 - 4);
    stroke();
    closePath();

    // back label FILTER
    beginPath();
    fillColor(blue_pigment_1);
    roundedRect(339 + 2, 329 + 2, 56, 18, 2);
    fill();
    closePath();
    // text label FILTER
    beginPath();
    fontSize(16);
    fillColor(eerie_black);
    text(339 + 2 + 56 / 2, 329 + 2 + 18 / 2, "FILTER", nullptr);
    closePath();

    // line 1
    beginPath();
    moveTo(339 + 2,
           329 + 2 + (211 / 3));
    lineTo(339 + 2 + 323 - 4,
           329 + 2 + (211 / 3));
    stroke();
    closePath();

    // back label EG
    // beginPath();
    // fillColor(blue_pigment_1);
    // roundedRect(339 + 2,
    //             329 + 2 + (211 / 3),
    //             40, 18, 2);
    // fill();
    // closePath();
    // // text
    // beginPath();
    // fontSize(16);
    // fillColor(eerie_black);
    // cx = 339 + 2 + 40 / 2;
    // cy = 329 + 2 + 211 / 3 + 18 / 2;
    // text(cx, cy, "EG", nullptr);
    // closePath();

    // line 2
    beginPath();
    moveTo(339 + 2,
           329 + 2 + (211 / 3) * 2);
    lineTo(339 + 2 + 323 - 4,
           329 + 2 + (211 / 3) * 2);
    stroke();
    closePath();

    // back label LFO
    beginPath();
    fillColor(blue_pigment_1);
    roundedRect(339 + 2,
                329 + 2 + (211 / 3) * 2,
                40, 18, 2);
    fill();
    closePath();
    // text
    beginPath();
    fontSize(16);
    fillColor(eerie_black);
    cx = 339 + 2 + 40 / 2;
    cy = 329 + 2 + (211 / 3) * 2 + 18 / 2;
    text(cx, cy, "LFO", nullptr);
    closePath();

    // VBOX_PITCH xywh {667 329 323 176}
    beginPath();
    fillColor(eerie_black);
    rect(667, 329, 323, 211);
    fill();
    closePath();

    // outer box
    beginPath();
    strokeWidth(2.0f);
    strokeColor(shamrock_green);
    rect(667 + 2, 329 + 2, 323 - 4, 211 - 4);
    stroke();
    closePath();

    // line 1
    beginPath();
    moveTo(667 + 2,
           329 + 2 + (211 / 2));
    lineTo(667 + 2 + 323 - 4,
           329 + 2 + (211 / 2));
    stroke();
    closePath();

    // back label PITCH
    beginPath();
    fillColor(shamrock_green);
    roundedRect(667 + 2,
                329 + 2, 56, 18, 2);
    fill();
    closePath();
    // text label PITCH
    beginPath();
    fontSize(16);
    fillColor(eerie_black);

    text(667 + 2 + 56 / 2, 329 + 2 + 18 / 2, "PITCH", nullptr);
    closePath();

    // back LFO
    beginPath();
    fillColor(shamrock_green);
    roundedRect(667 + 2,
                329 + 211 / 2, 40, 18, 2);
    fill();
    closePath();
    // lable LFO
    beginPath();
    fontSize(16);
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
    fillColor(eerie_black);
    cy = 329 + 2 + 211 / 2 + 18 / 2;
    cx = 667 + 2 + 40 / 2;
    text(cx, cy, "LFO", nullptr);
    closePath();
}

void DropsUI::drawWaveform()
{
    double view = viewEnd - viewStart; // set these when zooming in
    double samples_per_pixel = view / (double)display_width;
    float fIndex;
    uint iIndex;

    fIndex = float(viewStart) + float(samples_per_pixel);
    iIndex = fIndex;
    beginPath();
    strokeColor(pale_silver);
    strokeWidth(1.0f);
    moveTo(display_left, display_center);

    for (uint16_t i = 0; i < display_width; i++)
    {
        fIndex = float(viewStart) + (float(i) * samples_per_pixel);
        iIndex = fIndex;
        auto minmax = std::minmax_element(waveForm->begin() + iIndex, waveForm->begin() + iIndex + int(samples_per_pixel));
        uint16_t min = *minmax.first + display_center;
        uint16_t max = *minmax.second + display_center;
        lineTo(i + display_left, min);
        lineTo(i + display_left, max);
    }
    stroke();
    closePath();
    // center line
    beginPath();
    moveTo(display_left, display_center);
    lineTo(display_right, display_center);
    stroke();
    closePath();
}

void DropsUI::drawMinimap()
{
    beginPath();
    strokeColor(pale_silver);
    strokeWidth(1);
    float brighten = 1.5f;
    float r = black_olive.red * brighten;
    float g = black_olive.green * brighten;
    float b = black_olive.blue * brighten;
    fillColor(r, g, b);
    rect(display_left, display_bottom, display_width, minimap_height);
    fill();
    closePath();
    beginPath();

    for (uint i = 0; i < display_width; i++)
    {
        moveTo(i + display_left, display_bottom + minimap_height);
        lineTo(i + display_left, display_bottom + minimap_height - miniMap->at(i));
    }
    stroke();
    closePath();

    // draw "handlebar"
    double samples_per_pixel = waveForm->size() / (double)display_width;
    int leftPixel = static_cast<float>(viewStart) / samples_per_pixel + static_cast<float>(display_left);
    int rightPixel = static_cast<float>(viewEnd) / samples_per_pixel + static_cast<float>(display_left);
    // darken left of the bar
    fillColor(0, 0, 0, 64);
    beginPath();
    rect(display_left, display_bottom, leftPixel - display_left, minimap_height);
    fill();
    closePath();
    // darken left of the bar
    beginPath();
    rect(rightPixel, display_bottom, display_right - rightPixel, minimap_height);
    fill();
    closePath();
}

void DropsUI::drawInOutMarkers()
{

    if (sampleIn >= viewStart && sampleIn <= viewEnd)
    {

        // sample to pixel
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float sampleInPixel = (sampleIn / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);
        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(sampleInPixel, display_top);
        lineTo(sampleInPixel, display_bottom);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(sampleInPixel - 35.f, display_top, 35.f, 35.f, 35.f);
        fill();
        closePath();
        beginPath();
        rect(sampleInPixel - 35.f * .5f, display_top, 35 * .5f, 35.f);
        fill();
        closePath();
        // IN text
        beginPath();
        fillColor(eerie_black);
        fontSize(sample_inout_font_size);
        textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
        text(sampleInPixel - 16.f, display_top + 16.f, "IN", NULL);

        closePath();
    }

    if (sampleOut <= viewEnd && sampleOut >= viewStart)
    {
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float sampleOutPixel = (sampleOut / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        // line
        beginPath();
        moveTo(sampleOutPixel, display_top);
        lineTo(sampleOutPixel, display_bottom);
        stroke();
        closePath();
        // handle
        beginPath();
        roundedRect(sampleOutPixel, display_top, 35.f, 35.f, 35.f);
        fill();
        closePath();

        beginPath();
        rect(sampleOutPixel, display_top, 35.f * .5f, 35.f);
        fill();
        closePath();
        // text OUT
        beginPath();

        fillColor(eerie_black);
        fontSize(sample_inout_font_size);
        textAlign(ALIGN_CENTER | ALIGN_MIDDLE);
        text(sampleOutPixel + 35.f * 0.5f, display_top + 35.f * .5f, "OUT", NULL);
        closePath();
    }
}

void DropsUI::drawLoopMarkers()
{

    if (sampleLoopStart >= viewStart && sampleLoopStart <= viewEnd)
    {
        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        // sample to pixel
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float loopStartPixel = (sampleLoopStart / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        // line
        beginPath();
        moveTo(loopStartPixel, display_bottom);
        lineTo(loopStartPixel, display_top);
        stroke();
        closePath();
        // handle

        loopLeft->drawAt(loopStartPixel - 35, display_bottom - 35);
    }

    if (sampleLoopEnd <= viewEnd && sampleLoopEnd >= viewStart)
    {
        fillColor(pale_silver);
        strokeColor(flame);
        strokeWidth(1.0f);
        double sample_per_pixel = pow(viewMaxZoom, viewZoom);
        float loopEndPixel = (sampleLoopEnd / sample_per_pixel) - (viewStart / sample_per_pixel) + static_cast<float>(display_left);

        // line
        beginPath();
        moveTo(loopEndPixel, display_bottom);
        lineTo(loopEndPixel, display_top);
        stroke();
        closePath();
        // handle
        loopRight->drawAt(loopEndPixel, display_bottom - 35);
    }

    // do stuff
}

void DropsUI::uiFileBrowserSelected(const char *filename)
{
    // if a file was selected, tell DSP
    if (filename != nullptr)
    {
        //fileopen_button->setText(filename);
        setState("filepath", filename);
        sampleDir = dirnameOf(filename);
        fileName = filename;
        // repaint();
    }
}

void DropsUI::stateChanged(const char *key, const char *value)
{
#ifdef DEBUG
    printf("key = %s, value = %s\n", key, value);
#endif
}

bool DropsUI::onMouse(const MouseEvent &ev)
{
    if (ev.press)
        if (scrollbarDragging || loopstartDragging || loopendDragging || sampleInDragging || sampleOutDragging)
        {
            mouseX = ev.pos.getX();
        }
    return false;
}

void DropsUI::scrollWaveform(bool LeftOrRight)
{
    float leftright = LeftOrRight ? -1.0f : 1.0f;
    float samples_per_pixel = pow(viewMaxZoom, viewZoom);
    float length = samples_per_pixel * static_cast<float>(display_width);
    float samples_to_scroll = static_cast<float>(display_width) * samples_per_pixel * .5 * leftright;
    viewStart += static_cast<int>(samples_to_scroll);
    if (viewStart < 0)
    {
        viewStart = 0;
        viewEnd = static_cast<sf_count_t>(length);
        setMarkers();
        setScrollbarWidgets();
        repaint();
        return;
    }
    viewEnd += static_cast<int>(samples_to_scroll);
    if (viewEnd > static_cast<sf_count_t>(waveForm->size()))
    {
        viewEnd = static_cast<sf_count_t>(waveForm->size());
        viewStart = viewEnd - static_cast<sf_count_t>(length);
        setMarkers();
        setScrollbarWidgets();
        repaint();
        return;
    }
    setMarkers();
    setScrollbarWidgets();
    repaint();
}

void DropsUI::setMarkers()
{
    float samples_per_pixel = pow(viewMaxZoom, viewZoom);
    float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    fLoopStart->setAbsoluteX(loopStartPixel - 35); //FIXME: hardcoded offset
    fLoopEnd->setAbsoluteX(loopEndPixel);

    float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel + static_cast<float>(display_left);
    fSampleIn->setAbsoluteX(sampleInPixel - 35); //FIXME: hardcoded offset
    fSampleOut->setAbsoluteX(sampleOutPixel);
}

void DropsUI::setScrollbarWidgets()
{
    double samples_per_pixel = waveForm->size() / (double)display_width;
    int leftPixel = viewStart / samples_per_pixel + display_left;
    int rightPixel = viewEnd / samples_per_pixel + display_left;
    fScrollBarHandle->setWidth(rightPixel - leftPixel);
    fScrollBarHandle->setAbsoluteX(leftPixel);
    fScrollBarLeft->setWidth(leftPixel - display_left);
    fScrollBarRight->setWidth(display_right - rightPixel);
    fScrollBarRight->setAbsoluteX(rightPixel);
}

bool DropsUI::onScroll(const ScrollEvent &ev)
{
    // is the pointer in the display
    int x = ev.pos.getX();
    int y = ev.pos.getY();
    if (!display.contains(x, y))
    {
        return false; // get outta here
    }

    if (waveForm->size() <= display_width)
        return false; // can't zoom anyway

    x -= display_left; // off set in pixels

    // left-right scroll factor
    float scroll_delta = -ev.delta.getX();
    // zoom factor
    float zoom_delta = -ev.delta.getY() * 0.05f;

    // we use a signed int to be able to handle temporary negative starts.
    int start;
    float samples_per_pixel;

    // We either zoom in/out, or ...
    if (zoom_delta != 0.0)
    {
        // old zoom factor
        uint center = int(pow(viewMaxZoom, viewZoom) * (float(x)) + float(viewStart));
        viewZoom += zoom_delta;
        if (viewZoom < 0.0f)
        {
            viewZoom = 0.0f;
        }
        if (viewZoom > 1.0f)
        {
            viewZoom = 1.0f;
        }
        samples_per_pixel = pow(viewMaxZoom, viewZoom);
        start = int(float(center) - (float(x) * samples_per_pixel));
    }
    // ... we scroll left or right.
    else if (scroll_delta != 0.0)
    {
        if ((scroll_delta < 0 && viewStart == 0) ||
            (scroll_delta > 0 && viewEnd == static_cast<sf_count_t>(waveForm->size())))
        {
            // can't scroll any further
            return false;
        }
        samples_per_pixel = pow(viewMaxZoom, viewZoom);
        float scroll_distance = 20 * scroll_delta * samples_per_pixel;
        start = viewStart + scroll_distance;
    }
    else
    {
        // this probably shouldn't happen.
        return false;
    }

    // and ensure we stay in view.
    uint length = int(samples_per_pixel * float(display_width));

    viewEnd = start + length;
    if (viewEnd > static_cast<sf_count_t>(waveForm->size()))
    {
        viewEnd = waveForm->size();
        start = viewEnd - length;
    }
    samples_per_pixel = pow(viewMaxZoom, viewZoom);
    viewStart = start < 0 ? 0 : start;
    setScrollbarWidgets();
    setMarkers();
    repaint();
    return true;
}

bool DropsUI::onMotion(const MotionEvent &ev)
{
    if (scrollbarDragging)
    {
        int distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = waveForm->size() / (double)display_width;
        int underflowcheck = viewStart + (float(distance) * samples_per_pixel);
        if (underflowcheck < 0)
        {
            viewStart = 0;
            int length = display_width * pow(viewMaxZoom, viewZoom);
            viewEnd = viewStart + length;
        }
        else
        {
            viewStart = underflowcheck;
            viewEnd = viewEnd + ((float)distance * samples_per_pixel);
        }

        if (viewEnd > static_cast<sf_count_t>(waveForm->size()))
        {
            viewEnd = static_cast<sf_count_t>(waveForm->size());
            viewStart = (float)waveForm->size() - (float)display_width * pow(viewMaxZoom, viewZoom);
        }
        setScrollbarWidgets();
        setMarkers();
        repaint();
    }

    if (loopstartDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopStart = static_cast<float>(sampleLoopStart) + distance * samples_per_pixel;
        sampleLoopStart = clamp<sf_count_t>(sampleLoopStart, sampleLoopEnd - 1, 0);
        float loopStartPixel = static_cast<float>(sampleLoopStart - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fLoopStart->setAbsoluteX(loopStartPixel - display_left);
        /*
        float value = static_cast<float>(sampleLoopStart) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleLoopStart, value);
         */
        repaint();
    }

    if (loopendDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleLoopEnd = static_cast<float>(sampleLoopEnd) + distance * samples_per_pixel;
        sampleLoopEnd = clamp<sf_count_t>(sampleLoopEnd, waveForm->size() - 1, sampleLoopStart + 1);
        float loopEndPixel = static_cast<float>(sampleLoopEnd - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fLoopEnd->setAbsoluteX(loopEndPixel);
        /* 
        float value = static_cast<float>(sampleLoopEnd) / static_cast<float>(waveForm->size());
        setParameterValue(kSampleLoopEnd, value);
         */
        repaint();
    }

    if (sampleInDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleIn = static_cast<float>(sampleIn) + distance * samples_per_pixel;
        sampleIn = clamp<sf_count_t>(sampleIn, sampleOut - 1, 0);
        float sampleInPixel = static_cast<float>(sampleIn - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fSampleIn->setAbsoluteX(sampleInPixel - display_left);
        // float value = static_cast<float>(sampleIn) / static_cast<float>(waveForm->size());
        // setParameterValue(kSampleIn, value);
        repaint();
    }

    if (sampleOutDragging)
    {
        float distance = ev.pos.getX() - mouseX;
        mouseX = ev.pos.getX();
        float samples_per_pixel = pow(viewMaxZoom, viewZoom);
        sampleOut = static_cast<float>(sampleOut) + distance * samples_per_pixel;
        sampleOut = clamp<sf_count_t>(sampleOut, waveForm->size() - 1, sampleIn + 1);
        float sampleOutPixel = static_cast<float>(sampleOut - viewStart) / samples_per_pixel + static_cast<float>(display_left);
        fSampleOut->setAbsoluteX(sampleOutPixel);
        // float value = static_cast<float>(sampleOut) / static_cast<float>(waveForm->size());
        // setParameterValue(kSampleOut, value);
        repaint();
    }

    return false;
}

void DropsUI::onFileOpenButtonClicked(FileOpenButton *)
{
    DGL::Window::FileBrowserOptions opts;
    opts.title = "Load SFZ";
    opts.startDir = sampleDir.c_str();
    opts.buttons.showPlaces = 2;
    getParentWindow().openFileBrowser(opts);
}

// void DropsUI::onTextButtonClicked(TextButton *tb)
// {
// }

void DropsUI::onDropDownClicked(DropDown *dropDown)
{
    uint id = dropDown->getId();
    switch (id)
    {
    // case kSampleNormalize:
    //     fNormalizeMenu->show();
    //     break;
    case kSamplePitchKeyCenter:
        fKeyCenterMenu->show();
        break;
    case kSamplePlayMode:
        fPlayModeMenu->show();
        break;
    case kSamplePlayDirection:
        fDirectionMenu->show();
        break;
    case kSampleOversampling:
        fOversamplingMenu->show();
        break;
    case kAmpLFOType:
        fAmpLFOTypeMenu->show();
        break;
    // case kAmpLFOSync:
    //     fAmpLFOSyncMenu->show();
    //     break;
    case kPitchLFOType:
        fPitchLFOTypeMenu->show();
        break;
    case kFilterLFOType:
        fFilterLFOTypeMenu->show();
        break;

    default:
#ifdef DEBUG
        printf("dropdown %i clicked\n", id);
#endif
        break;
    }
}
void DropsUI::knobDragStarted(Knob *)
{
    // const uint id = knob->getId();
    // #ifdef DEBUG
    //     printf("%i , drag started\n", id);
    // #endif
}
void DropsUI::knobDragFinished(Knob *knob, float value)
{
    uint id = knob->getId();

    switch (id)
    {
    case kAmpEgAttack:
        setParameterValue(kAmpEgAttack, value);
        break;
    case kAmpEgDecay:
        setParameterValue(kAmpEgDecay, value);
        break;
    case kAmpEgSustain:
        setParameterValue(kAmpEgSustain, value);
        break;
    case kAmpEgRelease:
        setParameterValue(kAmpEgRelease, value);
        break;
    case kAmpLFOFreq:
        setParameterValue(kAmpLFOFreq, value);
        break;
    case kAmpLFODepth:
        setParameterValue(kAmpLFODepth, value);
        break;
    case kAmpLFOFade:
        setParameterValue(kAmpLFOFade, value);
        break;

    // filter
    case kFilterCutOff:
        setParameterValue(kFilterCutOff, value);
        break;
    case kFilterResonance:
        setParameterValue(kFilterResonance, value);
        break;
    case kFilterEgDepth:
        setParameterValue(kFilterEgDepth, value);
        break;
    case kFilterEgAttack:
        setParameterValue(kFilterEgAttack, value);
        break;
    case kFilterEgDecay:
        setParameterValue(kFilterEgDecay, value);
        break;
    case kFilterEgSustain:
        setParameterValue(kFilterEgSustain, value);
        break;
    case kFilterEgRelease:
        setParameterValue(kFilterEgRelease, value);
        break;
    case kFilterLFOFreq:
        setParameterValue(kFilterLFOFreq, value);
        break;
    case kFilterLFODepth:
        setParameterValue(kFilterLFODepth, value);
        break;
    case kFilterLFOFade:
        setParameterValue(kFilterLFOFade, value);
        break;
    // pitch
    case kPitchEgDepth:
        setParameterValue(kPitchEgDepth, value);
        break;
    case kPitchEgAttack:
        setParameterValue(kPitchEgAttack, value);
        break;
    case kPitchEgDecay:
        setParameterValue(kPitchEgDecay, value);
        break;
    case kPitchEgSustain:
        setParameterValue(kPitchEgSustain, value);
        break;
    case kPitchEgRelease:
        setParameterValue(kPitchEgRelease, value);
        break;
    case kPitchLFOFreq:
        setParameterValue(kPitchLFOFreq, value);
        break;
    case kPitchLFODepth:
        setParameterValue(kPitchLFODepth, value);
        break;
    case kPitchLFOFade:
        setParameterValue(kPitchLFOFade, value);
        break;

    default:
        break;
    }
    repaint();
    // int id = knob->getId();
    // #ifdef DEBUG
    //     printf("%i , drag finished\n", id);
    // #endif
}
void DropsUI::knobValueChanged(Knob *knob, float value)
{

    uint id = knob->getId();

    switch (id)
    {
    case kAmpEgAttack:
        setParameterValue(kAmpEgAttack, value);
        break;
    case kAmpEgDecay:
        setParameterValue(kAmpEgDecay, value);
        break;
    case kAmpEgSustain:
        setParameterValue(kAmpEgSustain, value);
        break;
    case kAmpEgRelease:
        setParameterValue(kAmpEgRelease, value);
        break;
    case kAmpLFOFreq:
        setParameterValue(kAmpLFOFreq, value);
        break;
    case kAmpLFODepth:
        setParameterValue(kAmpLFODepth, value);
        break;
    case kAmpLFOFade:
        setParameterValue(kAmpLFOFade, value);
        break;

    // filter
    case kFilterCutOff:
        setParameterValue(kFilterCutOff, value);
        break;
    case kFilterResonance:
        setParameterValue(kFilterResonance, value);
        break;
    case kFilterEgDepth:
        setParameterValue(kFilterEgDepth, value);
        break;
    case kFilterEgAttack:
        setParameterValue(kFilterEgAttack, value);
        break;
    case kFilterEgDecay:
        setParameterValue(kFilterEgDecay, value);
        break;
    case kFilterEgSustain:
        setParameterValue(kFilterEgSustain, value);
        break;
    case kFilterEgRelease:
        setParameterValue(kFilterEgRelease, value);
        break;
    case kFilterLFOFreq:
        setParameterValue(kFilterLFOFreq, value);
        break;
    case kFilterLFODepth:
        setParameterValue(kFilterLFODepth, value);
        break;
    case kFilterLFOFade:
        setParameterValue(kFilterLFOFade, value);
        break;
    // pitch
    case kPitchEgDepth:
        setParameterValue(kPitchEgDepth, value);
        break;
    case kPitchEgAttack:
        setParameterValue(kPitchEgAttack, value);
        break;
    case kPitchEgDecay:
        setParameterValue(kPitchEgDecay, value);
        break;
    case kPitchEgSustain:
        setParameterValue(kPitchEgSustain, value);
        break;
    case kPitchEgRelease:
        setParameterValue(kPitchEgRelease, value);
        break;
    case kPitchLFOFreq:
        setParameterValue(kPitchLFOFreq, value);
        break;
    case kPitchLFODepth:
        setParameterValue(kPitchLFODepth, value);
        break;
    case kPitchLFOFade:
        setParameterValue(kPitchLFOFade, value);
        break;

    default:
#ifdef DEBUG
        printf("knobValueChanged(%i, %f)\n", id, value);
#endif
        break;
    }
}

void DropsUI::onSliderValueChanged(Slider *slider, float value)
{
    uint id = slider->getId();

    switch (id)
    {
    case kSamplePitch:
        setParameterValue(id, value + 100);
        break;
    default:
#ifdef DEBUG
        printf("slider %i, value %f\n", id, value);
#endif
        break;
    }
}

void DropsUI::onScrollBarClicked(ScrollBar *scrollBar, bool dragging)
{
    uint id = scrollBar->getId();
    switch (id)
    {
    case kScrollbarHandle:
        scrollbarDragging = dragging;
        break;
    case kScrollbarLeft:
        if (!dragging)
        {
            scrollWaveform(true);
        }
        break;
    case kScrollbarRight:
        if (!dragging)
        {
            scrollWaveform(false);
        }
        break;
    case kSampleLoopStart:
    {
        loopstartDragging = dragging;
        if (!loopstartDragging)
        {
            float value = static_cast<float>(sampleLoopStart) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleLoopStart, value);
        }
        break;
    }
    case kSampleLoopEnd:
    {
        loopendDragging = dragging;
        if (!loopendDragging)
        {
            float value = static_cast<float>(sampleLoopEnd) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleLoopEnd, value);
        }
        break;
    }
    case kSampleIn:
    {
        sampleInDragging = dragging;
        if (!sampleInDragging)
        {
            float value = static_cast<float>(sampleIn) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleIn, value);
        }
        break;
    }
    case kSampleOut:
    {
        sampleOutDragging = dragging;
        if (!sampleOutDragging)
        {
            float value = static_cast<float>(sampleOut) / static_cast<float>(waveForm->size());
            setParameterValue(kSampleOut, value);
        }
        break;
    }
    default:
        break;
    }
}

void DropsUI::onMenuClicked(Menu *menu, uint menu_id, std::string item)
{
    const uint id = menu->getId();
    // #ifdef DEBUG
    //     printf("menu %i ,menu_id %i, item %s\n", id, menu_id, item.c_str());
    // #endif
    switch (id)
    {
    case kPlayModeMenu:
        fSamplePlayMode->item = item;
        fPlayModeMenu->hide();
        setParameterValue(kSamplePlayMode, menu_id);
        break;
    // case kNormalizeMenu:
    //     fSampleNormalize->item = item;
    //     fNormalizeMenu->hide();
    //     setParameterValue(kSampleNormalize, menu_id);
    //     break;
    case kKeyCenterMenu:
    {
        fSamplePitchKeyCenter->item = item;
        fKeyCenterMenu->hide();
        float keyValue = 127.f - menu_id;
        setParameterValue(kSamplePitchKeyCenter, keyValue);
        break;
    }
    case kDirectionMenu:
        fSamplePlayDirection->item = item;
        fDirectionMenu->hide();
        setParameterValue(kSamplePlayDirection, menu_id);
        break;
    case kOversamplingMenu:
        fSampleOversampling->item = item;
        fOversamplingMenu->hide();
        setParameterValue(kSampleOversampling, menu_id);
        break;
    case kAmpLFOTypeMenu:
        fAmpLFOType->item = item;
        fAmpLFOTypeMenu->hide();
        setParameterValue(kAmpLFOType, menu_id);
        break;
    // case kAmpLFOSyncMenu:
    //     fAmpLFOSync->item = item;
    //     fAmpLFOSyncMenu->hide();
    //     setParameterValue(kAmpLFOSync, menu_id);
    //     break;
    case kPitchLFOTypeMenu:
        fPitchLFOType->item = item;
        fPitchLFOTypeMenu->hide();
        setParameterValue(kPitchLFOType, menu_id);
        break;
    case kFilterLFOTypeMenu:
        fFilterLFOType->item = item;
        fFilterLFOTypeMenu->hide();
        setParameterValue(kFilterLFOType, menu_id);
        break;
    default:

        break;
    }
}

// void DropsUI::onRadioButtonClicked(RadioButton *rb)
// {
//     const uint id = rb->getId();
//     switch (id)
//     {
//     case kAmpLFOFreqBeat:
//         printf("active_option %i\n", rb->active_option);
//         break;

//     default:
//         break;
//     }
// }

void DropsUI::onSVGButtonClicked(SVGButton *svgb)
{
    const uint id = svgb->getId();
    switch (id)
    {
    // case kZoomOut:
    //     zoomButtons(0.05f);
    //     break;
    // case kZoomIn:
    //     zoomButtons(-0.05f);
    //     break;
    // case kZoomAll:
    //     viewStart = 0;
    //     viewEnd = sampleLength;
    //     viewZoom = 1.0f;
    //     setScrollbarWidgets();
    //     setMarkers();
    //     repaint();
    //     break;
    // case kZoomInOut:
    // {
    //     viewStart = sampleIn;
    //     viewEnd = sampleOut;
    //     viewEnd = clamp<sf_count_t>(viewEnd, sampleLength, sampleIn + display_width);
    //     const float inOutLength = static_cast<float>(sampleOut - sampleIn);
    //     const float samples_pp = inOutLength / static_cast<float>(display_width);
    //     const float l2_spp = log2(samples_pp);
    //     const float l2_vmz = log2(viewMaxZoom);
    //     viewZoom = l2_spp / l2_vmz;
    //     viewZoom = clamp<float>(viewZoom, 1.0f, 0.0f);
    //     setScrollbarWidgets();
    //     setMarkers();
    //     repaint();
    //     break;
    // }
    case kFilterLowpass:
        fFilterBandpass->is_active = false;
        fFilterHighpass->is_active = false;
        setParameterValue(kFilterType, 0);
        repaint();
        break;
    case kFilterBandpass:
        fFilterLowpass->is_active = false;
        fFilterHighpass->is_active = false;
        setParameterValue(kFilterType, 1);
        repaint();
        break;
    case kFilterHighpass:
        fFilterLowpass->is_active = false;
        fFilterBandpass->is_active = false;
        setParameterValue(kFilterType, 2);
        repaint();
        break;
    default:
#ifdef DEBUG
        printf("undefined svg button clicked\n");
#endif
        break;
    }
}

void DropsUI::zoomButtons(float zoom_delta)
{
    if (waveForm->size() <= display_width)
        return; // can't zoom anyway

    int x = getWidth() / 2; // off set in pixels, center screen

    // we use a signed int to be able to handle temporary negative starts.
    int start;
    float samples_per_pixel;

    // We either zoom in/out, or ...
    // old zoom factor
    uint center = int(pow(viewMaxZoom, viewZoom) * (float(x)) + float(viewStart));
    viewZoom += zoom_delta;
    if (viewZoom < 0.0f)
    {
        viewZoom = 0.0f;
    }
    if (viewZoom > 1.0f)
    {
        viewZoom = 1.0f;
    }
    samples_per_pixel = pow(viewMaxZoom, viewZoom);
    start = int(float(center) - (float(x) * samples_per_pixel));

    // and ensure we stay in view.
    uint length = int(samples_per_pixel * float(display_width));
    viewEnd = start + length;
    if (viewEnd > static_cast<sf_count_t>(waveForm->size()))
    {
        viewEnd = static_cast<sf_count_t>(waveForm->size());
        start = viewEnd - length;
    }
    samples_per_pixel = pow(viewMaxZoom, viewZoom);
    viewStart = start < 0 ? 0 : start;
    setScrollbarWidgets();
    setMarkers();
    repaint();
}

UI *createUI()
{
    return new DropsUI();
}

// -----------------------------------------------------------------------------

END_NAMESPACE_DISTRHO

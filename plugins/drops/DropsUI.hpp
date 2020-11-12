// TODO Add license

#ifndef DROPSUI_HPP_INCLUDED
#define DROPSUI_HPP_INCLUDED

#include "external/src/nanosvg.h"
#include "external/src/nanosvgrast.h"

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"
#include <vector>

#include "DropsPlugin.hpp"
#include "TextButton.hpp"
#include "FileOpenButton.hpp"
#include "ScrollBar.hpp"
#include "Knob.hpp"
#include "DropDown.hpp"
#include "Menu.hpp"
#include "Slider.hpp"
#include "RadioButton.hpp"
#include "SVGButton.hpp"
#include "HBox.hpp"
#include "VBox.hpp"
#include "SVGImage.hpp"
#include "Artwork.hpp"
#include "DropsColors.hpp"
#include "SVG_Icons.hpp"
#include "SVGImage.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class DropsUI : public UI,
                public TextButton::Callback,
                public ScrollBar::Callback,
                public Knob::Callback,
                public DropDown::Callback,
                public Menu::Callback,
                public Slider::Callback,
                public FileOpenButton::Callback,
                public RadioButton::Callback,
                public SVGButton::Callback
{
public:
    DropsUI();
    ~DropsUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void onNanoDisplay() override;
    void uiFileBrowserSelected(const char *filename) override;
    void stateChanged(const char *key, const char *value) override;
    bool onMouse(const MouseEvent &ev) override;
    bool onScroll(const ScrollEvent &) override;
    bool onMotion(const MotionEvent &) override;
    void onTextButtonClicked(TextButton *textButton) override;
    void onFileOpenButtonClicked(FileOpenButton *fileOpenButton) override;
    void onScrollBarClicked(ScrollBar *scrollBar, bool dragging) override;
    void onDropDownClicked(DropDown *dropdown) override;
    void knobDragStarted(Knob *knob) override;
    void knobDragFinished(Knob *knob) override;
    void knobValueChanged(Knob *knob, float value) override;
    void onSliderValueChanged(Slider *slider, float value) override;
    void onMenuClicked(Menu *menu, uint id, std::string item);
    void onRadioButtonClicked(RadioButton *radio);
    void onSVGButtonClicked(SVGButton *button);

private:
    template <class T>
    const T &clamp(const T &x, const T &upper, const T &lower)
    {
        return std::min(upper, std::max(x, lower));
    }

    DropsPlugin *plugin;
    ScopedPointer<FileOpenButton> fileopen_button;
    ScopedPointer<TextButton> button_sample, button_amp, button_pitch, button_filter;
    ScopedPointer<ScrollBar> fScrollBarHandle, fLoopStart, fLoopEnd, fSampleIn,
        fSampleOut, fScrollBarLeft, fScrollBarRight;

    ScopedPointer<HBox> box_tabs;
    // sample tab
    ScopedPointer<VBox> box_sample;
    ScopedPointer<HBox> box_sample_row_1, box_sample_row_2, box_sample_row_3;
    ScopedPointer<Slider> fSampleXFade, fSampleTune;
    ScopedPointer<DropDown> fSampleNormalize, fSamplePitchKeyCenter,
        fSamplePlayMode, fSamplePlayDirection;
    ScopedPointer<Menu> fNormalizeMenu, fKeyCenterMenu, fPlayModeMenu,
        fDirectionMenu;
    // amp tab
    ScopedPointer<VBox> box_amp;
    ScopedPointer<HBox> box_amp_row_1, box_amp_row_2;
    ScopedPointer<Knob>
        fAmpEgAttack, fAmpEgDecay, fAmpEgSustain, fAmpEgRelease;
    ScopedPointer<DropDown> fAmpLFOType;
    ScopedPointer<Menu> fAmpLFOTypeMenu;
    ScopedPointer<Slider> fAmpLFOFreq, fAmpLFODepth;
    ScopedPointer<DropDown> fAmpLFOSync;
    ScopedPointer<Menu> fAmpLFOSyncMenu;
    ScopedPointer<RadioButton> fAmpLFOFreqBeat;

    ScopedPointer<SVGImage> dropsLogo, clearlyBrokenLogo, loopLeft, loopRight,
        zoomIn, zoomOut, zoomAll, zoomLoop;
    ScopedPointer<HBox> hbox_zoom_icons;
    ScopedPointer<SVGButton> fZoomOut,fZoomIn,fZoomAll,fZoomInOut;

    void initWidgets();
    void initTabSample();
    void initTabAmp();
    void hideTabSample();
    void hideTabAmp();
    void showTabSample();
    void showTabAmp();
    void makeIcons();
    int loadSample();
    void drawWaveform();
    void drawMinimap();
    void drawLoopMarkers();
    void drawInOutMarkers();
    void scrollWaveform(bool leftright);
    void setMarkers();
    void setScrollbarWidgets();
    void zoomButtons(float zoom_delta);
    bool scrollbarDragging, loopstartDragging, loopendDragging, sampleInDragging, sampleOutDragging;
    bool sig_sampleLoaded;
    bool showWaveForm;
    char *filepath;
    sf_count_t sampleLength;
    int sampleChannels;
    int file_samplerate;
    float scale;
    // sample
    sf_count_t sampleIn, sampleOut, sampleLoopStart, sampleLoopEnd;

    std::vector<signed char> *waveForm;
    std::vector<char> *miniMap;
    sf_count_t viewStart;
    sf_count_t viewEnd;
    float viewZoom;
    float viewMaxZoom;
    int mouseX, mouseY;
    Rectangle<int> display;
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DropsUI)
};

END_NAMESPACE_DISTRHO

#endif
// TODO Add license

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"
#include <vector>

#include "DropsPlugin.hpp"
#include "TextButton.hpp"
#include "ScrollBar.hpp"
#include "Knob.hpp"
#include "DropDown.hpp"
#include "Menu.hpp"
#include "artwork.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class DropsUI : public UI,
                public TextButton::Callback,
                public ScrollBar::Callback,
                public Knob::Callback,
                public DropDown::Callback,
                public Menu::Callback
{
public:
    DropsUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void onNanoDisplay() override;
    void uiFileBrowserSelected(const char *filename) override;
    void stateChanged(const char *key, const char *value) override;
    bool onMouse(const MouseEvent &ev) override;
    bool onScroll(const ScrollEvent &) override;
    bool onMotion(const MotionEvent &) override;
    void textButtonClicked(TextButton *textButton) override;
    void scrollBarClicked(ScrollBar *scrollBar, bool dragging) override;
    void dropDownClicked(DropDown* dropdown) override;
    void knobValueChanged(Knob *knob, float value) override;
    void menuClicked(Menu* menu, uint id, std::string item);

private:
    template <class T>
    const T &clamp(const T &x, const T &upper, const T &lower)
    {
        return std::min(upper, std::max(x, lower));
    }

    DropsPlugin *plugin;
    ScopedPointer<TextButton> fFileOpenButton;
    ScopedPointer<ScrollBar> fScrollBarHandle, fLoopStart, fLoopEnd, fSampleIn, fSampleOut, fScrollBarLeft, fScrollBarRight;
    ScopedPointer<Knob> fAmpEgAttack, fAmpEgDecay, fAmpEgSustain, fAmpEgRelease;
    ScopedPointer<DropDown> fLoopMode;
    ScopedPointer<Menu> fLoopMenu;
    NanoImage imgLoopStart, imgLoopEnd;
    void initWidgets();
    int loadSample();
    void drawWaveform();
    void drawMinimap();
    void drawLoopMarkers();
    void drawInOutMarkers();
    void scrollWaveform(bool leftright);
    void setMarkers();
    void setScrollbarWidgets();
    bool scrollbarDragging, loopstartDragging, loopendDragging, sampleInDragging, sampleOutDragging;
    bool sig_sampleLoaded;
    bool showWaveForm;
    char *filepath;
    sf_count_t sampleLength;
    int sampleChannels;
    int file_samplerate;
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
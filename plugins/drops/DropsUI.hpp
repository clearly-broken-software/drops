// TODO Add license

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"
#include <sndfile.hh>
#include <vector>
#include "TextButton.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class DropsUI : public UI,
                public TextButton::Callback
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
    void textButtonClicked ( TextButton* textButton) override;

private:
    ScopedPointer<TextButton> fFileOpenButton;
    int loadSample(const char *fp);
    void drawWaveform();
    void drawMinimap();
    bool sampleLoaded;
    char *filepath;
    sf_count_t sampleLength;
    int sampleChannels;
    int file_samplerate;
    // sample
    sf_count_t sampleIn, sampleOut, sampleLoopStart, sampleLoopEnd;
    //
    std::vector<signed char> waveForm;
    std::vector<char> miniMap;
    sf_count_t viewStart;
    sf_count_t viewEnd;
    float viewZoom;
    float viewMaxZoom;
    int mouseX, mouseY;
    Rectangle<int> display;

    static constexpr unsigned int display_left = 0;
    static constexpr unsigned int display_top = 100;
    static constexpr unsigned int display_width = 1000;
    static constexpr unsigned int display_right = display_left + display_width;
    static constexpr unsigned int display_height = 190;
    static constexpr unsigned int display_bottom = display_top + display_height;
    static constexpr unsigned int display_center = (display_bottom - display_top) / 2 + display_top;
    static constexpr unsigned int minimap_height = 35;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DropsUI)
};

END_NAMESPACE_DISTRHO
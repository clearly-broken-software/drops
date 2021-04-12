/*
    Drops - Drops Really Only Plays Samples
    Copyright (C) 2021  Rob van den Berg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DROPS_HPP_INCLUDED
#define DROPS_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "DropsParams.h"
#include "DropsGeometry.hpp"
#include "sfizz.hpp"
#include <iostream>
#include <sndfile.hh>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>
START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DropsPlugin : public Plugin
{
public:
    DropsPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char *getLabel() const noexcept override
    {
        return "Drops";
    }

    const char *getDescription() const override
    {
        return "Drops Really Only Plays Samples";
    }

    const char *getMaker() const noexcept override
    {
        return "Clearly Broken Software";
    }

    const char *getHomePage() const override
    {
        return "https://github.com/clearly-broken-software/drops";
    }

    const char *getLicense() const noexcept override
    {
        return "GPL V3";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('D', 'r', 'p', 'S');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter &parameter) override;
    void setState(const char *key, const char *value) override;
    String getState(const char *key) const override;
    void initState(unsigned int, String &, String &) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;
    // -------------------------------------------------------------------
    // Process

    //void activate() override;
    void run(const float **, float **outputs, uint32_t frames, const MidiEvent *midiEvents, uint32_t midiEventCount) override;

    // -------------------------------------------------------------------

private:
    void foo(void *data, int delay, const char *path, const char *sig, const sfizz_arg_t *args);

    void initSFZ();
    void makeSFZ();
    int loadSample(const char *fp);
    //    void simpleMessageReceiver(void *data, int delay, const char *path, const char *sig, const sfizz_arg_t *args);

    sf_count_t sampleLength;
    bool sig_sampleLoaded;
    bool loadedSample;
    std::string path;
    double sampleRate;
    sfz::Sfizz synth;
    std::mutex synthMutex;
    std::vector<signed char> waveForm;
    std::vector<char> miniMap;

    // Parameters

    //  display
    float fSampleIn, fSampleOut, fSampleLoopStart, fSampleLoopEnd;
    //  sample
    float fSamplePitchKeyCenter, fSamplePitch, fSamplePlayMode,
        fSamplePlayDirection, fSampleOversampling;
    //  amp
    float fAmpEGAttack, fAmpEgDecay, fAmpEgSustain, fAmpEgRelease,
        fAmpLFOType, fAmpLFOSync, fAmpLFOFreq, fAmpLFOSyncFreq, fAmpLFODepth,
        fAmpLFOFade;
    //  filter
    float fFilterType, fFilterCutOff, fFilterResonance, fFilterEGDepth,
        fFilterLFOType, fFilterLFOSync, fFilterLFOFreq, fFilterLFOSyncFreq,
        fFilterLFODepth, fFilterLFOFade,
        fFilterEGAttack, fFilterEgDecay, fFilterEgSustain, fFilterEgRelease;
    //  pitch
    float fPitchEGDepth, fPitchEGAttack, fPitchEgDecay, fPitchEgSustain,
        fPitchEgRelease, fPitchLFOType, fPitchLFOSync, fPitchLFOFreq, fPitchLFOSyncFreq, fPitchLFODepth,
        fPitchLFOFade;

    double fFilterMaxFreq;
    float bpm;

    const char *play_modes_[4]{
        "no_loop",
        "one_shot",
        "loop_continuous",
        "loop_sustain"};
    const char *direction_[2]{
        "forward",
        "reverse"};
    const char *filters_[3]{
        "lpf_2p",
        "bpf_2p",
        "hpf_2p"};

    const int lfo_types_[6]{
        0,  // triangle
        1,  // sine
        3,  // square
        6,  // saw up
        7,  // saw down
        12, // s/h
    };
    const char *lfo_sync_[18]{
        "0.25",     // 1/16
        "0.5",      // 1/8
        "1",        // 1/4
        "2",        // 1/2
        "4",        // 1/1
        "8",        // 2/1
        "0.166666", // 1/16T
        "0.333333", // 1/8T
        "0.666666", // 1/4T
        "1.333333", // 1/2T
        "2.666666", // 1/1T
        "5.333333", // 2/1T
        "0.375",    // 1/16.
        "0.75",     // 1/8.
        "1.5",      // 1/4.
        "3",        // 1/2.
        "6",        // 1/1.
        "12",       // 2/1.
    };

    std::unordered_map<std::string, std::string> opcodes;
    std::vector<std::string> messageList;
    sfz::ClientPtr client;

    // some constants
    const float lfo_max_freq = 20.0f;
    const float filter_eg_depth = 12000.0f;
    const float pitch_eg_depth = 2400.0f; // 2 octave
    const float amp_lfo_depth = 12.f;
    const float pitch_lfo_depth = 1200;
    const float lfo_fade = 10.f;

    friend class DropsUI;
    /*
     * Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DropsPlugin)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // DROPS_HPP_INCLUDED

/*
 * Drops 2
 *
 * Copyright (C) 2019 Clearly Broken Software
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef DROPS_HPP_INCLUDED
#define DROPS_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "DropsParams.h"
#include "DropsGeometry.hpp"
#include <sfizz.hpp>
#include <iostream>
#include <sndfile.hh>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
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
    void initSFZ();
    void makeSFZ();
    int loadSample(const char *fp);
    sf_count_t sampleLength;
    bool sig_sampleLoaded;
    bool loadedSample;
    std::string path;
    double sampleRate;
    sfz::Sfizz synth;
    std::vector<signed char> waveForm;
    std::vector<char> miniMap;

    // Parameters

    //  display
    float fSampleIn, fSampleOut, fSampleLoopStart, fSampleLoopEnd;
    //  sample
    float fSamplePitchKeyCenter, fSamplePlayMode, fSamplePlayDirection,
        fSamplePitch, fSampleOversampling;
    //  amp
    float fAmpLFOType, fAmpLFOFreq, fAmpLFODepth, fAmpLFOFade,
        fAmpEGAttack, fAmpEgDecay, fAmpEgSustain, fAmpEgRelease;
    //  filter
    float fFilterType, fFilterLFOType, fFilterLFOFreq, fFilterLFODepth,
        fFilterLFOFade, fFilterCutOff, fFilterResonance,
        fFilterEGAttack, fFilterEgDecay, fFilterEgSustain, fFilterEgRelease;
    //  pitch
    float fPitchLFOType, fPitchLFOFreq, fPitchLFODepth, fPitchLFOFade,
        fPitchEGAttack, fPitchEgDecay, fPitchEgSustain, fPitchEgRelease;

    double fFilterMaxFreq;

    const char *play_modes_[4]{
        "no_loop", "one_shot",
        "loop_continuous",
        "loop_sustain"};
    const char *direction_[2]{
        "forward",
        "reverse"};
    const char *filters_[3]{
        "lpf_2p",
        "bpf_2p",
        "hpf_2p"};

    const char *lfo_types_[5]{
        "0", // triangle
        "1", // sine
        "3", // square
        "6", // saw up
        "7", // saw down
    };

    std::unordered_map<std::string, std::string> opcodes;

    const float lfo_max_freq = 20.0f;
    // double bpm;

    friend class DropsUI;
    /*
     * Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DropsPlugin)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // DROPS_HPP_INCLUDED
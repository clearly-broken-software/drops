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

#include "DistrhoPlugin.hpp"
#include "DropsPlugin.hpp"
#include <sfizz.hpp>

//#include <sndfile.hh>
//#include <samplerate.h>
//#define MINIMP3_IMPLEMENTATION
//#define MINIMP3_FLOAT_OUTPUT
//#include "minimp3_ex.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------

DropsPlugin::DropsPlugin() : Plugin(kParameterCount, 0, 0)
{

    sampleRate = getSampleRate();
    synth.setSampleRate(sampleRate);
    synth.loadSfzFile("/home/rob/git/drops/plugins/drops/res/basic.sfz");
    synth.setNumVoices(16);
    fPitchBendDepth = 0.0f;
    fPolyphony = 0.0f;
    fGain = 0.0f;
    fSampleIn = 0.0f;
    fSampleOut = 0.0f;
    fSampleLoopStart = 0.0f;
    fSampleLoopEnd = 0.0f;
    fSampleXFade = 0.0f;
    fSampleNormalize = 0.0f;
    fSamplePitchKeyCenter = 0.0f;
    fSampleTune = 0.0f;
    fSamplePlayMode = 0.0f;
    fSamplePlayDirection = 0.0f;
    fAmpEGAttack = 0.0f;
    fAmpEgDecay = 0.0f;
    fAmpEgSustain = 0.0f;
    fAmpEgRelease = 0.0f;
    fAmpLFOType = 0.0f;
    fAmpLFOFreq = 0.0f;
    fAmpLFODepth = 0.0f;
    fAmpLFOSync = 0.0f;
    fPitchEGAttack = 0.0f;
    fPitchEgDecay = 0.0f;
    fPitchEgSustain = 0.0f;
    fPitchEgRelease = 0.0f;
    fPitchEgDepth = 0.0f;
    fPitchLFOType = 0.0f;
    fPitchLFOFreq = 0.0f;
    fPitchLFODepth = 0.0f;
    fPitchLFOSync = 0.0f;
    fFilterType = 0.0f;
    fFilterCutOff = 0.0f;
    fFilterResonance = 0.0f;
    fFilterEGAttack = 0.0f;
    fFilterEgDecay = 0.0f;
    fFilterEgSustain = 0.0f;
    fFilterEgRelease = 0.0f;
    fFilterEgDepth = 0.0f;
    fFilterLFOType = 0.0f;
    fFilterLFOFreq = 0.0f;
    fFilterLFODepth = 0.0f;
    fFilterLFOSync = 0.0f;
}

// --  PARAMETERS  -------------------------------------------------------------

// Init
void DropsPlugin::initParameter(uint32_t index, Parameter &parameter)
{
    switch (index)
    {
    case kPitchBendDepth:
        parameter.name = "Pitchbend Depth";
        parameter.symbol = "pitchbend_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPolyphony:
        parameter.name = "Polyphony";
        parameter.symbol = "polyphony";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kGain:
        parameter.name = "Gain";
        parameter.symbol = "gain";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleIn:
        parameter.name = "Sample In";
        parameter.symbol = "sample_in";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleOut:
        parameter.name = "Sample Out";
        parameter.symbol = "sample_out";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleLoopStart:
        parameter.name = "Loop Start";
        parameter.symbol = "sample_loop_start";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleLoopEnd:
        parameter.name = "Loop End";
        parameter.symbol = "sample_loop_end";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleXFade:
        parameter.name = "Crossfade";
        parameter.symbol = "crossfade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleNormalize:
        parameter.name = "Normalize";
        parameter.symbol = "normalize";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSamplePitchKeyCenter:
        parameter.name = "Key Center";
        parameter.symbol = "pitch_center";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSampleTune:
        parameter.name = "Tune";
        parameter.symbol = "tune";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSamplePlayMode:
        parameter.name = "Playmode";
        parameter.symbol = "playmode";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kSamplePlayDirection:
        parameter.name = "Play Direction";
        parameter.symbol = "play_direction";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpEGAttack:
        parameter.name = "Amp Attack";
        parameter.symbol = "amp_attack";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpEgDecay:
        parameter.name = "Amp Decay";
        parameter.symbol = "amp_decay";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpEgSustain:
        parameter.name = "Amp Sustain";
        parameter.symbol = "amp_sustain";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpEgRelease:
        parameter.name = "Amp Release";
        parameter.symbol = "amp_release";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOType:
        parameter.name = "Amp LFO Type";
        parameter.symbol = "amp_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOFreq:
        parameter.name = "Amp LFO Freq";
        parameter.symbol = "amp_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFODepth:
        parameter.name = "Amp LFO Depth";
        parameter.symbol = "amp_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOSync:
        parameter.name = "Amp LFO Sync";
        parameter.symbol = "amp_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchEGAttack:
        parameter.name = "Pitch Attack";
        parameter.symbol = "pitch_attack";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchEgDecay:
        parameter.name = "Pitch Decay";
        parameter.symbol = "pitch_decay";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchEgSustain:
        parameter.name = "Pitch Sustain";
        parameter.symbol = "pitch_sustain";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchEgRelease:
        parameter.name = "Pitch Release";
        parameter.symbol = "pitch_release";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchEgDepth:
        parameter.name = "Pitch EG Depth";
        parameter.symbol = "pitch_eg_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFOType:
        parameter.name = "Pitch LFO Type";
        parameter.symbol = "pitch_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFOFreq:
        parameter.name = "Pitch LFO Freq";
        parameter.symbol = "pitch_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFODepth:
        parameter.name = "Pitch LFO Depth";
        parameter.symbol = "pitch_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFOSync:
        parameter.name = "Pitch LFO Sync";
        parameter.symbol = "pitch_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterType:
        parameter.name = "Filter Type";
        parameter.symbol = "filter_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterCutOff:
        parameter.name = "Cutoff";
        parameter.symbol = "cutoff";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterResonance:
        parameter.name = "Resonance";
        parameter.symbol = "resonance";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterEGAttack:
        parameter.name = "Filter Attack";
        parameter.symbol = "filter_attack";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterEgDecay:
        parameter.name = "Filter Decay";
        parameter.symbol = "filter_decay";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterEgSustain:
        parameter.name = "Filter Sustain";
        parameter.symbol = "filter_sustain";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterEgRelease:
        parameter.name = "Filter Release";
        parameter.symbol = "filter_release";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterEgDepth:
        parameter.name = "Filter EG Depth";
        parameter.symbol = "filter_eg_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOType:
        parameter.name = "Filter LFO Type";
        parameter.symbol = "filter_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOFreq:
        parameter.name = "Filter LFO Freq";
        parameter.symbol = "filter_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFODepth:
        parameter.name = "Filter LFO Depth";
        parameter.symbol = "filter_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOSync:
        parameter.name = "Filter LFO Sync";
        parameter.symbol = "filter_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;

    default:
        break;
    }
}

float DropsPlugin::getParameterValue(uint32_t index) const
{
    float val = 0.0f;
    switch (index)
    {

    case kPitchBendDepth:
        val = fPitchBendDepth;
        break;
    case kPolyphony:
        val = fPolyphony;
        break;
    case kGain:
        val = fGain;
        break;
    case kSampleIn:
        val = fSampleIn;
        break;
    case kSampleOut:
        val = fSampleOut;
        break;
    case kSampleLoopStart:
        val = fSampleLoopStart;
        break;
    case kSampleLoopEnd:
        val = fSampleLoopEnd;
        break;
    case kSampleXFade:
        val = fSampleXFade;
        break;
    case kSampleNormalize:
        val = fSampleNormalize;
        break;
    case kSamplePitchKeyCenter:
        val = fSamplePitchKeyCenter;
        break;
    case kSampleTune:
        val = fSampleTune;
        break;
    case kSamplePlayMode:
        val = fSamplePlayMode;
        break;
    case kSamplePlayDirection:
        val = fSamplePlayDirection;
        break;
    case kAmpEGAttack:
        val = fAmpEGAttack;
        break;
    case kAmpEgDecay:
        val = fAmpEgDecay;
        break;
    case kAmpEgSustain:
        val = fAmpEgSustain;
        break;
    case kAmpEgRelease:
        val = fAmpEgRelease;
        break;
    case kAmpLFOType:
        val = fAmpLFOType;
        break;
    case kAmpLFOFreq:
        val = fAmpLFOFreq;
        break;
    case kAmpLFODepth:
        val = fAmpLFODepth;
        break;
    case kAmpLFOSync:
        val = fAmpLFOSync;
        break;
    case kPitchEGAttack:
        val = fPitchEGAttack;
        break;
    case kPitchEgDecay:
        val = fPitchEgDecay;
        break;
    case kPitchEgSustain:
        val = fPitchEgSustain;
        break;
    case kPitchEgRelease:
        val = fPitchEgRelease;
        break;
    case kPitchEgDepth:
        val = fPitchEgDepth;
        break;
    case kPitchLFOType:
        val = fPitchLFOType;
        break;
    case kPitchLFOFreq:
        val = fPitchLFOFreq;
        break;
    case kPitchLFODepth:
        val = fPitchLFODepth;
        break;
    case kPitchLFOSync:
        val = fPitchLFOSync;
        break;
    case kFilterType:
        val = fFilterType;
        break;
    case kFilterCutOff:
        val = fFilterCutOff;
        break;
    case kFilterResonance:
        val = fFilterResonance;
        break;
    case kFilterEGAttack:
        val = fFilterEGAttack;
        break;
    case kFilterEgDecay:
        val = fFilterEgDecay;
        break;
    case kFilterEgSustain:
        val = fFilterEgSustain;
        break;
    case kFilterEgRelease:
        val = fFilterEgRelease;
        break;
    case kFilterEgDepth:
        val = fFilterEgDepth;
        break;
    case kFilterLFOType:
        val = fFilterLFOType;
        break;
    case kFilterLFOFreq:
        val = fFilterLFOFreq;
        break;
    case kFilterLFODepth:
        val = fFilterLFODepth;
        break;
    case kFilterLFOSync:
        val = fFilterLFOSync;
        break;
    default:
        break;
    }
    return val;
}

void DropsPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case kPitchBendDepth:
        fPitchBendDepth = value;
        break;
    case kPolyphony:
        fPolyphony = value;
        break;
    case kGain:
        fGain = value;
        break;
    case kSampleIn:
        fSampleIn = value;
        break;
    case kSampleOut:
        fSampleOut = value;
        break;
    case kSampleLoopStart:
        fSampleLoopStart = value;
        break;
    case kSampleLoopEnd:
        fSampleLoopEnd = value;
        break;
    case kSampleXFade:
        fSampleXFade = value;
        break;
    case kSampleNormalize:
        fSampleNormalize = value;
        break;
    case kSamplePitchKeyCenter:
        fSamplePitchKeyCenter = value;
        break;
    case kSampleTune:
        fSampleTune = value;
        break;
    case kSamplePlayMode:
        fSamplePlayMode = value;
        break;
    case kSamplePlayDirection:
        fSamplePlayDirection = value;
        break;
    case kAmpEGAttack:
        fAmpEGAttack = value;
        break;
    case kAmpEgDecay:
        fAmpEgDecay = value;
        break;
    case kAmpEgSustain:
        fAmpEgSustain = value;
        break;
    case kAmpEgRelease:
        fAmpEgRelease = value;
        break;
    case kAmpLFOType:
        fAmpLFOType = value;
        break;
    case kAmpLFOFreq:
        fAmpLFOFreq = value;
        break;
    case kAmpLFODepth:
        fAmpLFODepth = value;
        break;
    case kAmpLFOSync:
        fAmpLFOSync = value;
        break;
    case kPitchEGAttack:
        fPitchEGAttack = value;
        break;
    case kPitchEgDecay:
        fPitchEgDecay = value;
        break;
    case kPitchEgSustain:
        fPitchEgSustain = value;
        break;
    case kPitchEgRelease:
        fPitchEgRelease = value;
        break;
    case kPitchEgDepth:
        fPitchEgDepth = value;
        break;
    case kPitchLFOType:
        fPitchLFOType = value;
        break;
    case kPitchLFOFreq:
        fPitchLFOFreq = value;
        break;
    case kPitchLFODepth:
        fPitchLFODepth = value;
        break;
    case kPitchLFOSync:
        fPitchLFOSync = value;
        break;
    case kFilterType:
        fFilterType = value;
        break;
    case kFilterCutOff:
        fFilterCutOff = value;
        break;
    case kFilterResonance:
        fFilterResonance = value;
        break;
    case kFilterEGAttack:
        fFilterEGAttack = value;
        break;
    case kFilterEgDecay:
        fFilterEgDecay = value;
        break;
    case kFilterEgSustain:
        fFilterEgSustain = value;
        break;
    case kFilterEgRelease:
        fFilterEgRelease = value;
        break;
    case kFilterEgDepth:
        fFilterEgDepth = value;
        break;
    case kFilterLFOType:
        fFilterLFOType = value;
        break;
    case kFilterLFOFreq:
        fFilterLFOFreq = value;
        break;
    case kFilterLFODepth:
        fFilterLFODepth = value;
        break;
    case kFilterLFOSync:
        fFilterLFOSync = value;
        break;

    default:
        break;
    }
}

void DropsPlugin::setState(const char *key, const char *value)
{
    if (strcmp(key, "filepath") == 0)
    {
        path = std::string(value);
        std::cout << path << std::endl;
        makeSFZ();
    }
}

String DropsPlugin::getState(const char *) const
{
    const String foo = String("Describe it");
    return foo;
};

void DropsPlugin::initState(unsigned int key, String &val, String &def)
{

    if (key == 0)
    {
        val = String("filepath");
    }
}

void DropsPlugin::makeSFZ()
{
    const char foo[] =
        "<control>\n"
        "default_path=\n"
        "<global>\n"
        "<group>\n"
        "ampeg_attack=0\n"
        "ampeg_attack_oncc201=10\n"
        "ampeg_decay=0\n"
        "ampeg_decay_oncc202=10\n"
        "ampeg_sustain=100\n"
        "ampeg_sustain_oncc203=-100\n"
        "ampeg_release=1\n"
        "ampeg_release_oncc204=10\n"
        // filter
        "fil_type=lpf_2p\n"
        "cutoff=20\n"
        "cutoff_oncc310=9600\n"
        "resonance=0\n"
        "resonance_oncc311=20\n"
        // filter ADSR
        "fileg_depth=9600\n"
        "fileg_attack=0\n"
        "fileg_attack_oncc301=10\n"
        "fileg_decay=0\n"
        "fileg_decay_oncc302=10\n"
        "fileg_sustain=100\n"
        "fileg_sustain_oncc303=-100\n"
        "fileg_release=0.1\n"
        "fileg_release_oncc304=10\n"
        // pitch ADSR
        "pitcheg_depth=1200\n"
        "pitcheg_attack=0\n"
        "pitcheg_attack_oncc401=10\n"
        "pitcheg_decay=0\n"
        "pitcheg_decay_oncc402=10\n"
        "pitcheg_sustain=0\n"
        "pitcheg_sustain_oncc403=100\n"
        "pitcheg_release=0.001\n"
        "pitcheg_release_oncc404=10\n"
        "trigger=attack\n"       // or release or first or legato
        "loop_mode=no_sustain\n" // or loop_continuous or one_shot or loop_sustain
        "<region> sample=";
    char bar[] = "lokey=0 hikey=127 pitch_keycenter=a4";
    char buffer[2048];
    sprintf(buffer, "%s%s\n%s", foo, path.c_str(), bar);
    // printf("The SFZ File\n\n %s\n", buffer);
    synth.loadSfzString("", buffer);
}

// --  MAIN PLUGIN FUNCTIONS  --------------------------------------------------

/* Main Audio and MIDI processing function */
void DropsPlugin::run(
    const float **,              // incoming audio
    float **outputs,             // outgoing audio
    uint32_t frames,             // size of block to process
    const MidiEvent *midiEvents, // MIDI pointer
    uint32_t midiEventCount      // Number of MIDI events in block
)
{
    // output ports, stereo
    //float *const outL = outputs[0];
    //float *const outR = outputs[1];

    uint32_t framesDone = 0;
    uint32_t curEventIndex = 0; // index for midi event to process

    // pass the automation data
    synth.hdcc(0, 201, fAmpEGAttack);
    synth.hdcc(0, 202, fAmpEgDecay);
    synth.hdcc(0, 203, fAmpEgSustain);
    synth.hdcc(0, 204, fAmpEgRelease);

    synth.hdcc(0, 310, fFilterCutOff);
    synth.hdcc(0, 311, fFilterResonance);
    synth.hdcc(0, 301, fFilterEGAttack);
    synth.hdcc(0, 302, fFilterEgDecay);
    synth.hdcc(0, 303, fFilterEgSustain);
    synth.hdcc(0, 304, fFilterEgRelease);

    synth.hdcc(0, 401, fPitchEGAttack);
    synth.hdcc(0, 402, fPitchEgDecay);
    synth.hdcc(0, 403, fPitchEgSustain);
    synth.hdcc(0, 404, fPitchEgRelease);

    while (framesDone < frames)
    {
        while (curEventIndex < midiEventCount && framesDone == midiEvents[curEventIndex].frame)
        {

            int status = midiEvents[curEventIndex].data[0]; // midi status
                                                            //  int channel = status & 0x0F ; // get midi channel
            int midi_message = status & 0xF0;
            int midi_data1 = midiEvents[curEventIndex].data[1];
            int midi_data2 = midiEvents[curEventIndex].data[2];
            switch (midi_message)
            {
            case 0x80: // note_off
                synth.noteOff(framesDone, midi_data1, midi_data2);
                break;
            case 0x90: // note_on
                synth.noteOn(framesDone, midi_data1, midi_data2);
                break;
            default:
                break;
            }
            curEventIndex++;

        } // midi events
        ++framesDone;
    } // frames loop
    synth.renderBlock(outputs, frames, 2);
} // run

/* Plugin entry point, called by DPF to create a new plugin instance. */
Plugin *createPlugin()
{
    return new DropsPlugin();
}

// -----------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
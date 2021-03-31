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

DropsPlugin::DropsPlugin() : Plugin(kParameterCount, 0, 2)
{

    sampleRate = getSampleRate();
    sig_sampleLoaded = false;
    loadedSample = false;
    synth.setSampleRate(sampleRate);
    synth.setNumVoices(16);
    fSampleIn = 0.0f;
    fSampleOut = 1.0f;
    fSampleLoopStart = 0.0f;
    fSampleLoopEnd = 1.0f;
    fSamplePitchKeyCenter = 60.0f;
    fSamplePlayMode = 0.0f;
    fSamplePlayDirection = 0.0f;
    fSamplePitch = 100.0f;
    fSampleOversampling = 1.0f;
    // amp
    fAmpEGAttack = 0.0f;
    fAmpEgDecay = 0.0f;
    fAmpEgSustain = 1.0f;
    fAmpEgRelease = 0.0f;
    fAmpLFOType = 0.0f;
    fAmpLFOFade = 0.0f;
    fAmpLFOFreq = 0.0f;
    fAmpLFODepth = 0.0f;
    fAmpLFOFade = 0.0f;

    // filter
    fFilterCutOff = 1.0f;
    fFilterResonance = 0.0f;
    fFilterEGDepth = 1.0f;
    fFilterEGAttack = 0.0f;
    fFilterEgDecay = 0.0f;
    fFilterEgSustain = 0.0f;
    fFilterEgRelease = 0.0f;
    fFilterType = 0.0f;
    fFilterLFOType = 0.0f;
    fFilterLFOFreq = 0.0f;
    fFilterLFODepth = 0.0f;
    fFilterLFOFade = 0.0f;

    // pitch
    fPitchEGDepth = 1.0f;
    fPitchEGAttack = 0.0f;
    fPitchEgDecay = 0.0f;
    fPitchEgSustain = 0.0f;
    fPitchEgRelease = 0.0f;
    fPitchLFOType = 0.0f;
    fPitchLFOFreq = 0.0f;
    fPitchLFODepth = 0.0f;
    fPitchLFOFade = 0.0f;

    fFilterMaxFreq = sampleRate * .5;
    initSFZ();
}

// --  PARAMETERS  -------------------------------------------------------------

// Init
void DropsPlugin::initParameter(uint32_t index, Parameter &parameter)
{
    // parameters that don't have a sfz "onccN" opcode cause the sfz file to be
    // reload, thus can't be automated
    // these parameters have kParameterIsAutomable disabled
    switch (index)
    {
    case kSampleIn:
        parameter.name = "Sample In";
        parameter.symbol = "sample_in";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kSampleOut:
        parameter.name = "Sample Out";
        parameter.symbol = "sample_out";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 1.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kSampleLoopStart:
        parameter.name = "Loop Start";
        parameter.symbol = "sample_loop_start";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kSampleLoopEnd:
        parameter.name = "Loop End";
        parameter.symbol = "sample_loop_end";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 1.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kSamplePitchKeyCenter:
        parameter.name = "Key Center";
        parameter.symbol = "pitch_center";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 127.0f;
        parameter.ranges.def = 60.0f;
        // parameter.hints = kParameterIsAutomable | kParameterIsInteger;
        break;
    case kSamplePitch:
        parameter.name = "Pitch";
        parameter.symbol = "pitch";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 200.0f;
        parameter.ranges.def = 100.0f;
        parameter.hints = kParameterIsAutomable | kParameterIsInteger;
        break;
    case kSamplePlayMode:
        parameter.name = "Playmode";
        parameter.symbol = "playmode";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 3.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 4;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[4]{
            ParameterEnumerationValue(0.0f, "no_loop"),
            ParameterEnumerationValue(1.0f, "one_shot"),
            ParameterEnumerationValue(2.0f, "loop_continuous"),
            ParameterEnumerationValue(3.0f, "loop_sustain")};

        parameter.hints = kParameterIsInteger;
        break;
    case kSamplePlayDirection:
        parameter.name = "Play Direction";
        parameter.symbol = "play_direction";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 2;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[2]{
            ParameterEnumerationValue(0.0f, "forward"),
            ParameterEnumerationValue(1.0f, "reverse"),
        };
        parameter.hints = kParameterIsInteger;
        break;
    case kSampleOversampling:
        parameter.name = "Oversampling";
        parameter.symbol = "oversampling";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 3.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 4;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[4]{
            ParameterEnumerationValue(0.0f, "1x"),
            ParameterEnumerationValue(1.0f, "2x"),
            ParameterEnumerationValue(2.0f, "3x"),
            ParameterEnumerationValue(3.0f, "4x"),
        };
        parameter.hints = kParameterIsInteger;
        break;
    case kAmpLFOType:
        parameter.name = "Amp LFO Type";
        parameter.symbol = "amp_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 4.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 5;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[5]{
            ParameterEnumerationValue(0.0f, "triangle"),
            ParameterEnumerationValue(1.0f, "sine"),
            ParameterEnumerationValue(2.0f, "square"),
            ParameterEnumerationValue(3.0f, "saw up"),
            ParameterEnumerationValue(4.0f, "saw down"),
        };
        // parameter.hints = kParameterIsAutomable;
        break;
    case kAmpEgAttack:
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
        parameter.ranges.def = 1.0f;
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
    case kAmpLFOFreq:
        parameter.name = "Amp LFO Freq";
        parameter.symbol = "amp_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFODepth:
        parameter.name = "Amp LFO Depth";
        parameter.symbol = "amp_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOFade:
        parameter.name = "Amp LFO Fade";
        parameter.symbol = "amp_lfo_fade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
        // filter
    case kFilterType:
        parameter.name = "Filter Type";
        parameter.symbol = "filter_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 3;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[3]{
            ParameterEnumerationValue(0.0f, "lpf_2p"),
            ParameterEnumerationValue(1.0f, "bpf_2p"),
            ParameterEnumerationValue(2.0f, "hpf_2p")};
        parameter.hints = kParameterIsAutomable;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsInteger;
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
    case kFilterEgDepth:
        parameter.name = "Filter EG Depth";
        parameter.symbol = "filter_eg_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterEgAttack:
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
    case kFilterLFOType:
        parameter.name = "Filter LFO Type";
        parameter.symbol = "filter_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 4.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 5;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[5]{
            ParameterEnumerationValue(0.0f, "triangle"),
            ParameterEnumerationValue(1.0f, "sine"),
            ParameterEnumerationValue(2.0f, "square"),
            ParameterEnumerationValue(3.0f, "saw up"),
            ParameterEnumerationValue(4.0f, "saw down"),
        };
        // parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOFreq:
        parameter.name = "Filter LFO Freq";
        parameter.symbol = "filter_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFODepth:
        parameter.name = "Filter LFO Depth";
        parameter.symbol = "filter_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOFade:
        parameter.name = "Filter LFO Fade";
        parameter.symbol = "filter_lfo_fade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.5f;
        //parameter.hints = kParameterIsAutomable;
        break;
    // pitch
    case kPitchEgDepth:
        parameter.name = "Pitch EG Depth";
        parameter.symbol = "pitch_eg_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 1.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kPitchEgAttack:
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
    case kPitchLFOType:
        parameter.name = "Pitch LFO Type";
        parameter.symbol = "pitch_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 4.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 5;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[5]{
            ParameterEnumerationValue(0.0f, "triangle"),
            ParameterEnumerationValue(1.0f, "sine"),
            ParameterEnumerationValue(2.0f, "square"),
            ParameterEnumerationValue(3.0f, "saw up"),
            ParameterEnumerationValue(4.0f, "saw down"),
        };
        // parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFOFreq:
        parameter.name = "Pitch LFO Freq";
        parameter.symbol = "pitch_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFOFade:
        parameter.name = "Pitch LFO Fade";
        parameter.symbol = "pitch_lfo_fade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.5f;
        // parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFODepth:
        parameter.name = "Pitch LFO Depth";
        parameter.symbol = "pitch_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        // parameter.hints = kParameterIsAutomable;
        break;

    case kSampleLoaded:
        parameter.name = "Sample Loaded";
        parameter.symbol = "sampleloaded";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsBoolean | kParameterIsOutput;
    default:
        break;
    }
}

float DropsPlugin::getParameterValue(uint32_t index) const
{
    float val = 0.0f;
    switch (index)
    {
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
    case kSamplePitchKeyCenter:
        val = fSamplePitchKeyCenter;
        break;
    case kSamplePitch:
        val = fSamplePitch;
        break;
    case kSamplePlayMode:
        val = fSamplePlayMode;
        break;
    case kSamplePlayDirection:
        val = fSamplePlayDirection;
        break;
    case kSampleOversampling:
        val = fSampleOversampling;
        break;
        // amp
    case kAmpEgAttack:
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
    case kAmpLFOFade:
        val = fAmpLFOFade;
        break;
        // filter
    case kFilterType:
        val = fFilterType;
        break;
    case kFilterCutOff:
        val = fFilterCutOff;
        break;
    case kFilterResonance:
        val = fFilterResonance;
        break;
    case kFilterEgDepth:
        val = fFilterEGDepth;
        break;
    case kFilterEgAttack:
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
    case kFilterLFOType:
        val = fFilterLFOType;
        break;
    case kFilterLFOFreq:
        val = fFilterLFOFreq;
        break;
    case kFilterLFODepth:
        val = fFilterLFODepth;
        break;
    case kFilterLFOFade:
        val = fFilterLFOFade;
        break;
    // pitch
    case kPitchEgDepth:
        val = fPitchEGDepth;
        break;
    case kPitchEgAttack:
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
    case kPitchLFOType:
        val = fPitchLFOType;
        break;
    case kPitchLFOFreq:
        val = fPitchLFOFreq;
        break;
    case kPitchLFODepth:
        val = fPitchLFODepth;
        break;
    case kPitchLFOFade:
        val = fPitchLFOFade;
        break;

    case kSampleLoaded:
        val = sig_sampleLoaded;
        break;
    default:
#ifdef DEBUG
        printf("Unknown parameter: %i\n", index);
#endif
        break;
    }
    return val;
}

void DropsPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case kSampleIn:
        fSampleIn = value;
        makeSFZ();
        break;
    case kSampleOut:
        fSampleOut = value;
        makeSFZ();
        break;
    case kSampleLoopStart:
        fSampleLoopStart = value;
        makeSFZ();
        break;
    case kSampleLoopEnd:
        fSampleLoopEnd = value;
        makeSFZ();
        break;
    case kSamplePitchKeyCenter:
        fSamplePitchKeyCenter = value;
        makeSFZ();
        break;
    case kSamplePitch:
        fSamplePitch = std::round(value);
        break;
    case kSamplePlayMode:
        fSamplePlayMode = value;
        makeSFZ();
        break;
    case kSamplePlayDirection:
        fSamplePlayDirection = value;
        makeSFZ();
        break;
    case kSampleOversampling:
    {
        std::lock_guard<std::mutex> lock(synthMutex);
        const uint index = value;
        switch (index)
        {
        case 0:
            synth.setOversamplingFactor(1);
            break;
        case 1:
            synth.setOversamplingFactor(2);
            break;
        case 2:
            synth.setOversamplingFactor(4);
            break;
        case 3:
            synth.setOversamplingFactor(8);
            break;
        default:
            synth.setOversamplingFactor(1);
            break;
        }
        break;
    }
        // amp
    case kAmpLFOType:
        fAmpLFOType = value;
        makeSFZ();
        break;
    case kAmpLFOFreq:
        fAmpLFOFreq = value;
        makeSFZ();
        break;
    case kAmpLFODepth:
        fAmpLFODepth = value;
        makeSFZ();
        break;
    case kAmpLFOFade:
        fAmpLFOFade = value;
        makeSFZ();
        break;
    case kAmpEgAttack:
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
        // filter
    case kFilterType:
        fFilterType = value;
        makeSFZ();
        break;
    case kFilterCutOff:
        fFilterCutOff = value;
        break;
    case kFilterResonance:
        fFilterResonance = value;
        break;
    case kFilterEgDepth:
        fFilterEGDepth = value;
        makeSFZ();
        break;
    case kFilterEgAttack:
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
    case kFilterLFOType:
        fFilterLFOType = value;
        makeSFZ();
        break;
    case kFilterLFOFreq:
        fFilterLFOFreq = value;
        makeSFZ();
        break;
    case kFilterLFODepth:
        fFilterLFODepth = value;
        makeSFZ();
        break;
    case kFilterLFOFade:
        fFilterLFOFade = value;
        makeSFZ();
        break;
    // pitch
    case kPitchEgDepth:
        fPitchEGDepth = value;
        makeSFZ();
        break;
    case kPitchEgAttack:
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
    case kPitchLFOType:
        fPitchLFOType = value;
        makeSFZ();
        break;
    case kPitchLFOFreq:
        fPitchLFOFreq = value;
        makeSFZ();
        break;
    case kPitchLFODepth:
        fPitchLFODepth = value;
        makeSFZ();
        break;
    case kPitchLFOFade:
        fPitchLFOFade = value;
        makeSFZ();
        break;

    case kSampleLoaded:
        break;
    default:
        break;
    }
}

void DropsPlugin::setState(const char *key, const char *value)
{
    if (strcmp(key, "ui_sample_loaded") == 0)
    {
        sig_sampleLoaded = false;
    }
    if (strcmp(key, "filepath") == 0)
    {
        path = std::string(value);
#ifdef DEBUG
        std::cout << path << std::endl;
#endif
        loadSample(value);
        makeSFZ();
    }
}

String DropsPlugin::getState(const char *key) const
{
    String retString = String("undefined state");
#ifdef DEBUG
    printf("getState(%s)\n", key);
#endif
    if (strcmp(key, "filepath") == 0)
    {
        retString = path.c_str();
    }
    if (strcmp(key, "ui_sample_loaded") == 0)
    {
        retString = "ui_sample_loaded yes/no";
    }
    return retString;
};

void DropsPlugin::initState(unsigned int index, String &stateKey, String &defaultStateValue)
{
    switch (index)
    {
    case 0:
        stateKey = "filepath";
        defaultStateValue = "empty";
        break;
    case 1:
        stateKey = "ui_sample_loaded";
        defaultStateValue = "false";
        break;
    default:
#ifdef DEBUG
        printf("initState %i\n", index);
#endif
        break;
    }
}

int DropsPlugin::loadSample(const char *fp)
{
    // init waveform and miniMap
    waveForm.resize(0);
    miniMap.resize(0);

    SndfileHandle fileHandle(fp);

    // get the number of frames in the sample
    // sample range in sfizz is 0 ... frames - 1
    sampleLength = fileHandle.frames() - 1;
    if (sampleLength == -1)
    {
//TODO: show this in UI
//file doesn't exist or is of incompatible type, main handles the -1
#ifdef DEBUG
        printf("Can't load sample %s \n", fp);
#endif
        sig_sampleLoaded = false;
        return -1;
    }
    // get some more info of the sample
    int sampleChannels = fileHandle.channels();
    // get max value
    double max_val;
    fileHandle.command(SFC_CALC_NORM_SIGNAL_MAX, &max_val, sizeof(max_val));
    // normalize factor
    float ratio = max_val > 1.0f ? 1.0f : 1.0f / max_val;

    // resize vector
    std::vector<float> sample;
    sample.resize(sampleLength * sampleChannels);
    // load sample memory in samplevector
    fileHandle.read(&sample.at(0), sampleLength * sampleChannels);
    // sum to mono if needed
    sf_count_t size = sampleLength;
    if (sampleChannels == 2)
    { // sum to mono

        for (int i = 0, j = 0; i < size; i++)
        {
            float sum_mono = (sample[j] + sample[j + 1]) * 0.5f;
            waveForm.push_back((sum_mono * ratio) * float(display_height / 2));
            j += 2;
        }
    }
    else
    {
        waveForm.resize(size);
        for (int i = 0; i < size; i++)
        {
            waveForm[i] = (sample[i] * ratio) * float(display_height / 2);
        }
    }

    // make minimap
    miniMap.resize(display_width);
    float samples_per_pixel = static_cast<float>(sampleLength) / (float)display_width;
    float fIndex;
    uint iIndex;
    for (uint16_t i = 0; i < display_width; i++)
    {
        fIndex = float(i) * samples_per_pixel;
        iIndex = fIndex;
        auto minmax = std::minmax_element(waveForm.begin() + iIndex, waveForm.begin() + iIndex + int(samples_per_pixel));
        signed char min = std::abs(*minmax.first);
        signed char max = std::abs(*minmax.second);
        signed char maxValue = std::max(min, max);
        miniMap[i] = (float)maxValue / (float)(display_height / 2) * (float)minimap_height;
    }
    sig_sampleLoaded = true;
    loadedSample = true;
    return 0;
}

void DropsPlugin::initSFZ()
{
    opcodes["default_path"] = "";
    opcodes["pitch"] = "0";
    opcodes["pitch_oncc500"] = "0";
    opcodes["ampeg_attack"] = "0";
    opcodes["ampeg_attack_oncc2010"] = "10";
    opcodes["ampeg_decay"] = "0";
    opcodes["ampeg_decay_oncc202"] = "10";
    opcodes["ampeg_sustain"] = "100";
    opcodes["ampeg_sustain_oncc203"] = "-100";
    opcodes["ampeg_release"] = "1";
    opcodes["ampeg_release_oncc204"] = "10";
    opcodes["lfo01_wave"] = "triangle";
    opcodes["lfo01_freq"] = "0";
    opcodes["lfo01_volume"] = "0";
    opcodes["lfo01_fade"] = "0";
    opcodes["fil_type"] = "lpf_2p";
    opcodes["cutoff"] = "20";
    opcodes["cutoff_oncc310"] = "9600";
    opcodes["resonance"] = "0";
    opcodes["resonance_oncc311"] = "40";
    opcodes["fileg_depth"] = "12000";
    opcodes["fileg_attack"] = "0";
    opcodes["fileg_attack_oncc301"] = "10";
    opcodes["fileg_decay"] = "0";
    opcodes["fileg_decay_oncc302"] = "10";
    opcodes["fileg_sustain"] = "100";
    opcodes["fileg_sustain_oncc303"] = "-100";
    opcodes["fileg_release"] = "0.1";
    opcodes["fileg_release_oncc304"] = "10";
    opcodes["lfo02_freq"] = "0";
    opcodes["lfo02_cutoff"] = "24000";
    opcodes["lof02_fade"] = "0";
    opcodes["pitcheg_depth"] = "2400";
    opcodes["pitcheg_attack"] = "0";
    opcodes["pitcheg_attack_oncc401"] = "10";
    opcodes["pitcheg_decay"] = "0";
    opcodes["pitcheg_decay_oncc402"] = "10";
    opcodes["pitcheg_sustain"] = "0";
    opcodes["pitcheg_sustain_oncc403"] = "100";
    opcodes["pitcheg_release"] = "0.001";
    opcodes["pitcheg_release_oncc404"] = "10";
    opcodes["lfo03_freq"] = "0";
    opcodes["lfo03_pitch"] = "0";
    opcodes["lfo03_fade"] = "0";
    opcodes["trigger"] = "attack";
    opcodes["loop_mode"] = "no_loop";
    opcodes["loop_start"] = "0";
    opcodes["loop_end"] = "4294967296";
    opcodes["sample"] = "";
    opcodes["lokey"] = "0";
    opcodes["hikey"] = "127";
    opcodes["pitch_keycenter"] = "c4";
    opcodes["offset"] = "0";
    opcodes["end"] = "4294967296";
    opcodes["direction"] = "forward";
}

void DropsPlugin::makeSFZ()
{
    const float fSampleLength = static_cast<float>(sampleLength);
    uint loopstartInFrames = fSampleLength * fSampleLoopStart;
    uint loopEndInFrames =
        std::min(static_cast<uint>(sampleLength - 1),
                 static_cast<uint>(fSampleLength * fSampleLoopEnd));
    uint sampleInInFrames = fSampleLength * fSampleIn;
    uint sampleOutInFrames = fSampleLength * fSampleOut;
    opcodes["sample"] = path;

    opcodes["loop_start"] = std::to_string(loopstartInFrames);
    opcodes["loop_end"] = std::to_string(loopEndInFrames);
    opcodes["offset"] = std::to_string(sampleInInFrames);
    opcodes["end"] = std::to_string(sampleOutInFrames);
    opcodes["direction"] = direction_[static_cast<uint>(fSamplePlayDirection)];

    opcodes["lfo01_wave"] = lfo_types_[static_cast<int>(fAmpLFOType)];
    opcodes["lfo01_freq"] = std::to_string(fAmpLFOFreq * lfo_max_freq);
    opcodes["lfo01_volume"] = std::to_string(fAmpLFODepth * amp_lfo_depth);
    opcodes["lfo01_fade"] = std::to_string(fAmpLFOFade * lfo_fade);

    opcodes["lfo02_wave"] = lfo_types_[static_cast<int>(fFilterLFOType)];
    opcodes["lfo02_freq"] = std::to_string(fFilterLFOFreq * lfo_max_freq);
    opcodes["lfo02_cutoff"] = std::to_string(fFilterLFODepth * (fFilterMaxFreq * .5));
    opcodes["lfo02_fade"] = std::to_string(fFilterLFOFade * lfo_fade);

    opcodes["lfo03_wave"] = lfo_types_[static_cast<int>(fPitchLFOType)];
    opcodes["lfo03_freq"] = std::to_string(fPitchLFOFreq * lfo_max_freq);
    opcodes["lfo03_pitch"] = std::to_string(fPitchLFODepth * pitch_lfo_depth);
    opcodes["lfo03_fade"] = std::to_string(fPitchLFOFade * lfo_fade);

    opcodes["cutoff"] = std::to_string(fFilterCutOff * fFilterMaxFreq);
    opcodes["fileg_depth"] = std::to_string(fFilterEGDepth * filter_eg_depth);
    opcodes["pitcheg_depth"] = std::to_string(fPitchEGDepth * pitch_eg_depth);
    opcodes["pitch_keycenter"] = std::to_string(static_cast<int>(fSamplePitchKeyCenter));

    std::stringstream buffer;
    // amp ADSR
    buffer << "<region>\n";
    // buffer << "<control>\n";
    // buffer << "default_path=\n";
    // buffer << "<global>\n";
    // buffer << "<group>\n";
    buffer << "pitch=-100\n";
    buffer << "pitch_oncc500=200\n";
    buffer << "ampeg_attack=0\n";
    buffer << "ampeg_attack_oncc201=10\n";
    buffer << "ampeg_decay=0\n";
    buffer << "ampeg_decay_oncc202=10\n";
    buffer << "ampeg_sustain=0\n";
    buffer << "ampeg_sustain_oncc203=100\n";
    buffer << "ampeg_release=0.001\n";
    buffer << "ampeg_release_oncc204=10\n";
    buffer << "lfo01_wave=" << opcodes["lfo01_wave"] << "\n";
    buffer << "lfo01_freq=" << opcodes["lfo01_freq"] << "\n";
    buffer << "lfo01_volume=" << opcodes["lfo01_volume"] << "\n";
    buffer << "lfo01_fade=" << opcodes["lfo01_fade"] << "\n";
    buffer << "fil_type=" << filters_[static_cast<uint>(fFilterType)] << "\n";
    buffer << "cutoff=20\n"; // << opcodes["cutoff"] << "\n";
    buffer << "cutoff_oncc310=9600\n";
    buffer << "resonance=0\n";
    buffer << "resonance_oncc311=40\n";
    buffer << "fileg_depth=" << opcodes["fileg_depth"] << "\n";
    buffer << "fileg_attack=0\n";
    buffer << "fileg_attack_oncc301=10\n";
    buffer << "fileg_decay=0\n";
    buffer << "fileg_decay_oncc302=10 \n";
    buffer << "fileg_sustain=0 \n";
    buffer << "fileg_sustain_oncc303=100 \n";
    buffer << "fileg_release=10 \n";
    buffer << "fileg_release_oncc304=-10\n";
    buffer << "lfo02_wave=" << opcodes["lfo02_wave"] << "\n";
    buffer << "lfo02_freq=" << opcodes["lfo02_freq"] << "\n";
    buffer << "lfo02_cutoff=" << opcodes["lfo02_cutoff"] << "\n";
    buffer << "lfo02_fade=" << opcodes["lfo02_fade"] << "\n";

    buffer << "pitcheg_depth=" << opcodes["pitcheg_depth"] << "\n";
    buffer << "pitcheg_attack=0 \n";
    buffer << "pitcheg_attack_oncc401=10\n";
    buffer << "pitcheg_decay=0\n";
    buffer << "pitcheg_decay_oncc402=10\n";
    buffer << "pitcheg_sustain=0\n";
    buffer << "pitcheg_sustain_oncc403=100\n";
    buffer << "pitcheg_release=0.001\n";
    buffer << "pitcheg_release_oncc404=10\n";
    buffer << "lfo03_wave=" << opcodes["lfo03_wave"] << "\n";
    buffer << "lfo03_freq=" << opcodes["lfo03_freq"] << "\n";
    buffer << "lfo03_pitch=" << opcodes["lfo03_pitch"] << "\n";
    buffer << "lfo03_fade=" << opcodes["lfo03_fade"] << "\n";

    // buffer << "trigger=attack\n";
    buffer << "offset=" << opcodes["offset"] << "\n";
    buffer << "end=" << opcodes["end"] << "\n";
    buffer << "loop_mode=" << play_modes_[static_cast<uint>(fSamplePlayMode)] << "\n";
    buffer << "loop_start=" << opcodes["loop_start"] << "\n";
    buffer << "loop_end=" << opcodes["loop_end"] << "\n";
    buffer << "sample=" << opcodes["sample"] << "\n";
    // buffer << "lokey=0\n";
    // buffer << "hikey=127\n";
    buffer << "pitch_keycenter=" << opcodes["pitch_keycenter"] << "\n";
    buffer << "direction=" << opcodes["direction"] << "\n";
    // replace decimal comma wih decimal point
    std::string tmpSFZ = buffer.str();
    std::replace(tmpSFZ.begin(), tmpSFZ.end(), ',', '.');

#ifdef DEBUG
    std::cout << "----------------- SFZ FILE ------------------\n";
    std::cout << tmpSFZ << std::endl;
    std::cout << "----------------- SFZ FILE ------------------\n";
#endif

    std::lock_guard<std::mutex> lock(synthMutex);
    synth.loadSfzString("", tmpSFZ);
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
    std::unique_lock<std::mutex> lock(synthMutex, std::try_to_lock);
    if (!lock.owns_lock())
    { // synth is locked?
        std::fill_n(outputs[0], frames, 0.0f);
        std::fill_n(outputs[1], frames, 0.0f);
        return;
    }

    // output ports, stereo
    //float *const outL = outputs[0];
    //float *const outR = outputs[1];
    // const TimePosition &timePos(getTimePosition());
    // if (timePos.bbt.valid)
    //     bpm = timePos.bbt.beatsPerMinute;

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
    const float n = fSamplePitch / 200.f;
    synth.hdcc(0, 500, n);

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

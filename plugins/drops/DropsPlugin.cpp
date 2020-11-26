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
    //synth.loadSfzFile("/home/rob/git/drops/plugins/drops/res/basic.sfz");
    synth.setNumVoices(16);
    fPitchBendDepth = 0.0f;
    fPolyphony = 0.0f;
    fGain = 0.0f;
    fSampleIn = 0.0f;
    fSampleOut = 1.0f;
    fSampleLoopStart = 0.0f;
    fSampleLoopEnd = 1.0f;
    fSampleXFade = 0.0f;
    fSampleNormalize = 0.0f;
    fSamplePitchKeyCenter = 0.0f;
    fSampleTune = 0.0f;
    fSamplePlayMode = 0.0f;
    fSamplePlayDirection = 0.0f;
    fAmpEGAttack = 0.0f;
    fAmpEgDecay = 0.0f;
    fAmpEgSustain = 1.0f;
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
    initSFZ();
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
        parameter.ranges.def = 1.0f;
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
        parameter.ranges.def = 1.0f;
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
        parameter.ranges.max = 127.0f;
        parameter.ranges.def = 60.0f;
        parameter.hints = kParameterIsAutomable | kParameterIsInteger;
        break;
    case kSampleTune:
        parameter.name = "Tune";
        parameter.symbol = "tune";
        parameter.ranges.min = -100.0f;
        parameter.ranges.max = 100.0f;
        parameter.ranges.def = 0.0f;
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

        parameter.hints = kParameterIsAutomable;
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
        parameter.hints = kParameterIsAutomable;
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
    case kAmpLFOType:
        parameter.name = "Amp LFO Type";
        parameter.symbol = "amp_lfo_type";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 8;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[8]{
            ParameterEnumerationValue(0.0f, "sine"),
            ParameterEnumerationValue(1.0f, "triangle"),
            ParameterEnumerationValue(2.0f, "75% pulse"),
            ParameterEnumerationValue(3.0f, "square (50% pulse)"),
            ParameterEnumerationValue(4.0f, "25% pulse"),
            ParameterEnumerationValue(5.0f, "12:5% pulse"),
            ParameterEnumerationValue(6.0f, "saw going up"),
            ParameterEnumerationValue(7.0f, "saw going down"),
        };
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
        parameter.ranges.min = -1200.0f;
        parameter.ranges.max = 1200.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable | kParameterIsInteger;
        break;
    case kAmpLFOSync:
        parameter.name = "Amp LFO Sync";
        parameter.symbol = "amp_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 17.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        parameter.enumValues.count = 18;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[18]{
            ParameterEnumerationValue(0.0f, "1/16"),
            ParameterEnumerationValue(1.0f, "1/8"),
            ParameterEnumerationValue(2.0f, "1/4"),
            ParameterEnumerationValue(3.0f, "1/2"),
            ParameterEnumerationValue(4.0f, "1/1"),
            ParameterEnumerationValue(5.0f, "2/1"),
            ParameterEnumerationValue(6.0f, "1/16T"),
            ParameterEnumerationValue(7.0f, "1/8T"),
            ParameterEnumerationValue(8.0f, "1/4T"),
            ParameterEnumerationValue(9.0f, "1/2T"),
            ParameterEnumerationValue(10.0f, "1/1T"),
            ParameterEnumerationValue(11.0f, "2/1T"),
            ParameterEnumerationValue(12.0f, "1/16."),
            ParameterEnumerationValue(13.0f, "1/8."),
            ParameterEnumerationValue(14.0f, "1/4."),
            ParameterEnumerationValue(15.0f, "1/2."),
            ParameterEnumerationValue(16.0f, "1/1."),
            ParameterEnumerationValue(17.0f, "2/1."),
        };
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
    {
        val = fSampleIn;
    }
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
    case kAmpLFOSync:
        val = fAmpLFOSync;
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
        makeSFZ();
        break;
    case kSamplePlayDirection:
        fSamplePlayDirection = value;
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
    case kAmpLFOSync:
        fAmpLFOSync = value;
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
    case kPitchEgDepth:
        fPitchEgDepth = value;
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
    case kPitchLFOSync:
        fPitchLFOSync = value;
        makeSFZ();
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
    String retString = String("describe it");
#ifdef DEBUG
    printf("getState(%s)\n", key);
#endif
    if (strcmp(key, "filepath"))
    {
        retString = path.c_str();
    }
    if (strcmp(key, "ui_sample_loaded"))
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
    if (sampleLength == 0)
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
    opcodes["ampeg_attack"] = "0";
    opcodes["ampeg_attack_oncc2010"] = "10";
    opcodes["ampeg_decay"] = "0";
    opcodes["ampeg_decay_oncc202"] = "10";
    opcodes["ampeg_sustain"] = "100";
    opcodes["ampeg_sustain_oncc203"] = "-100";
    opcodes["ampeg_release"] = "1";
    opcodes["ampeg_release_oncc204"] = "10";
    opcodes["lfo01_freq"] = "0";
    opcodes["lfo01_volume"] = "0";
    opcodes["fil_type"] = "lpf_2p";
    opcodes["cutoff"] = "20";
    opcodes["cutoff_oncc310"] = "9600";
    opcodes["resonance"] = "0";
    opcodes["resonance_oncc311"] = "20";
    opcodes["fileg_depth"] = "9600";
    opcodes["fileg_attack"] = "0";
    opcodes["fileg_attack_oncc301"] = "10";
    opcodes["fileg_decay"] = "0";
    opcodes["fileg_decay_oncc302"] = "10";
    opcodes["fileg_sustain"] = "100";
    opcodes["fileg_sustain_oncc303"] = "-100";
    opcodes["fileg_release"] = "0.1";
    opcodes["fileg_release_oncc304"] = "10";
    opcodes["pitcheg_depth"] = "1200";
    opcodes["pitcheg_attack"] = "0";
    opcodes["pitcheg_attack_oncc401"] = "10";
    opcodes["pitcheg_decay"] = "0";
    opcodes["pitcheg_decay_oncc402"] = "10";
    opcodes["pitcheg_sustain"] = "0";
    opcodes["pitcheg_sustain_oncc403"] = "100";
    opcodes["pitcheg_release"] = "0.001";
    opcodes["pitcheg_release_oncc404"] = "10";
    opcodes["lfo2_freq"] = "0";
    opcodes["lfo02_pitch"] = "0";

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
    uint loopEndInFrames = std::min(static_cast<uint>(sampleLength - 1), static_cast<uint>(fSampleLength * fSampleLoopEnd));
    uint sampleInInFrames = fSampleLength * fSampleIn;
    uint sampleOutInFrames = fSampleLength * fSampleOut;
    opcodes["sample"] = path;
    opcodes["loop_start"] = std::to_string(loopstartInFrames);
    opcodes["loop_end"] = std::to_string(loopEndInFrames);
    opcodes["offset"] = std::to_string(sampleInInFrames);
    opcodes["end"] = std::to_string(sampleOutInFrames);
    opcodes["direction"] = direction_[static_cast<uint>(fSamplePlayDirection)];
    opcodes["lfo01_freq"] = std::to_string(fAmpLFOFreq);
    opcodes["lfo01_volume"]=std::to_string(fAmpLFODepth);
    
    

    std::stringstream buffer;
    // amp ADSR
    buffer << "<region>\n";
    // buffer << "<control>\n";
    // buffer << "default_path=\n";
    // buffer << "<global>\n";
    // buffer << "<group>\n";
    buffer << "ampeg_attack=0\n";
    buffer << "ampeg_attack_oncc201=10\n";
    buffer << "ampeg_decay=0\n";
    buffer << "ampeg_decay_oncc202=10\n";
    buffer << "ampeg_sustain=0\n";
    buffer << "ampeg_sustain_oncc203=100\n";
    buffer << "ampeg_release=0.001\n";
    buffer << "ampeg_release_oncc204=10\n";
    buffer << "lfo01_freq=" << opcodes["lfo01_freq"] << "\n";
    buffer << "lfo01_volume=" << opcodes["lfo01_volume"] << "\n";
    // buffer << "fil_type=lpf_2p\n";
    // buffer << "cutoff=" << sampleRate / 2 << "\n";
    // buffer << "cutoff_oncc310=9600\n";
    // buffer << "resonance=0\n";
    // buffer << "resonance_oncc311=20\n";
    // buffer << "fileg_depth=9600\n";
    // buffer << "fileg_attack=0\n";
    // buffer << "fileg_attack_oncc301=10\n";
    // buffer << "fileg_decay=0\n";
    // buffer << "fileg_decay_oncc302=10 \n";
    // buffer << "fileg_sustain=100 \n";
    // buffer << "fileg_sustain_oncc303=-100 \n";
    // buffer << "fileg_release=10 \n";
    // buffer << "fileg_release_oncc304=-10\n";
    // buffer << "pitcheg_depth=1200\n";
    // buffer << "pitcheg_attack=0 \n";
    // buffer << "pitcheg_attack_oncc401=10\n";
    // buffer << "pitcheg_decay=0\n";
    // buffer << "pitcheg_decay_oncc402=10\n";
    // buffer << "pitcheg_sustain=0\n";
    // buffer << "pitcheg_sustain_oncc403=100\n";
    // buffer << "pitcheg_release=0.001\n";
    // buffer << "pitcheg_release_oncc404=10\n";
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

#ifdef DEBUG
    std::cout << "----------------- SFZ FILE ------------------\n";
    std::cout << buffer.str() << std::endl;
#endif

    synth.loadSfzString("", buffer.str());
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
  //  synth.hdcc(0, 210, fAmpLFODepth);
  //  synth.hdcc(0, 211, fAmpLFOFreq);

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
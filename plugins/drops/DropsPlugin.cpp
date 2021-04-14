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

#include "DistrhoPlugin.hpp"
#include "DropsPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------

DropsPlugin::DropsPlugin() : Plugin(kParameterCount, 0, 2)
{

    sampleRate = getSampleRate();
    sig_sampleLoaded = false;
    loadedSample = false;
    bpm = 120.;
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
    fAmpLFOSync = 0.0f;
    fAmpLFOSyncFreq = 0.0f;
    fAmpLFOFade = 0.0f;
    fAmpLFOFreq = 0.0f;
    fAmpLFODepth = 0.0f;
    fAmpLFOFade = 0.0f;
    fAmpLFOSync = 0.0f;

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
    fFilterLFOSync = 0.0f;
    fFilterLFOSyncFreq = 0.0f;
    fFilterLFOFreq = 0.0f;
    fFilterLFODepth = 0.0f;
    fFilterLFOSync = 0.0f;
    fFilterLFOFade = 0.0f;

    // pitch
    fPitchEGDepth = 1.0f;
    fPitchEGAttack = 0.0f;
    fPitchEgDecay = 0.0f;
    fPitchEgSustain = 0.0f;
    fPitchEgRelease = 0.0f;
    fPitchLFOType = 0.0f;
    fPitchLFOSync = 0.0f;
    fPitchLFOFreq = 0.0f;
    fPitchLFOSyncFreq = 0.0f;
    fPitchLFODepth = 0.0f;
    fPitchLFOFade = 0.0f;

    fFilterMaxFreq = sampleRate * .5;
    initSFZ();

    client = synth.createClient(&messageList);
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
    case kSamplePitchKeyCenter:
        parameter.name = "Key Center";
        parameter.symbol = "pitch_center";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 127.0f;
        parameter.ranges.def = 60.0f;
        parameter.hints = kParameterIsInteger;
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
        parameter.ranges.max = 5.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 6;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[6]{
            ParameterEnumerationValue(0.0f, "triangle"),
            ParameterEnumerationValue(1.0f, "sine"),
            ParameterEnumerationValue(2.0f, "square"),
            ParameterEnumerationValue(3.0f, "saw up"),
            ParameterEnumerationValue(4.0f, "saw down"),
            ParameterEnumerationValue(5.0f, "s/h")};
        //parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOSync:
        parameter.name = "Amp LFO Sync";
        parameter.symbol = "amp_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsBoolean;
        break;
    case kAmpLFOFreq:
        parameter.name = "Amp LFO Freq";
        parameter.symbol = "amp_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOSyncFreq:
        parameter.name = "Amp LFO Sync";
        parameter.symbol = "amp_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 17.0f;
        parameter.ranges.def = 2.0f;
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
    case kAmpLFODepth:
        parameter.name = "Amp LFO Depth";
        parameter.symbol = "amp_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kAmpLFOFade:
        parameter.name = "Amp LFO Fade";
        parameter.symbol = "amp_lfo_fade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
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
        parameter.ranges.max = 5.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 6;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[6]{
            ParameterEnumerationValue(0.0f, "triangle"),
            ParameterEnumerationValue(1.0f, "sine"),
            ParameterEnumerationValue(2.0f, "square"),
            ParameterEnumerationValue(3.0f, "saw up"),
            ParameterEnumerationValue(4.0f, "saw down"),
            ParameterEnumerationValue(5.0f, "s/h")};
        parameter.hints = kParameterIsInteger;
        break;
    case kFilterLFOSync:
        parameter.name = "Filter LFO Sync";
        parameter.symbol = "filter_lfo_sync";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsBoolean;
        break;
    case kFilterLFOFreq:
        parameter.name = "Filter LFO Freq";
        parameter.symbol = "filter_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOSyncFreq:
        parameter.name = "Filter LFO Sync Freq";
        parameter.symbol = "filter_lfo_sync_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 17.0f;
        parameter.ranges.def = 2.0f;
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
        parameter.hints = kParameterIsInteger;
        break;
    case kFilterLFODepth:
        parameter.name = "Filter LFO Depth";
        parameter.symbol = "filter_lfo_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kFilterLFOFade:
        parameter.name = "Filter LFO Fade";
        parameter.symbol = "filter_lfo_fade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    // pitch
    case kPitchEgDepth:
        parameter.name = "Pitch EG Depth";
        parameter.symbol = "pitch_eg_depth";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 1.0f;
        parameter.hints = kParameterIsAutomable;
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
        parameter.ranges.max = 5.0f;
        parameter.ranges.def = 0.0f;
        parameter.enumValues.count = 6;
        parameter.enumValues.restrictedMode = true;
        parameter.enumValues.values = new ParameterEnumerationValue[6]{
            ParameterEnumerationValue(0.0f, "triangle"),
            ParameterEnumerationValue(1.0f, "sine"),
            ParameterEnumerationValue(2.0f, "square"),
            ParameterEnumerationValue(3.0f, "saw up"),
            ParameterEnumerationValue(4.0f, "saw down"),
            ParameterEnumerationValue(5.0f, "s/h")};
        parameter.hints = kParameterIsInteger;
        break;
    case kPitchLFOFreq:
        parameter.name = "Pitch LFO Freq";
        parameter.symbol = "pitch_lfo_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.0f;
        parameter.hints = kParameterIsAutomable;
        break;
    case kPitchLFOSyncFreq:
        parameter.name = "Pitch LFO Sync Freq";
        parameter.symbol = "pitch_lfo_sync_freq";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 17.0f;
        parameter.ranges.def = 2.0f;
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
        parameter.hints = kParameterIsInteger;
        break;
    case kPitchLFOFade:
        parameter.name = "Pitch LFO Fade";
        parameter.symbol = "pitch_lfo_fade";
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        parameter.ranges.def = 0.5f;
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
    case kAmpLFOSync:
        val = fAmpLFOSync;
        break;
    case kAmpLFOFreq:
        val = fAmpLFOFreq;
        break;
    case kAmpLFOSyncFreq:
        val = fAmpLFOSyncFreq;
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
    case kFilterLFOSync:
        val = fFilterLFOSync;
        break;
    case kFilterLFOFreq:
        val = fFilterLFOFreq;
        break;
    case kFilterLFOSyncFreq:
        val = fFilterLFOSyncFreq;
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
    case kPitchLFOSync:
        val = fPitchLFOSync;
        break;
    case kPitchLFOFreq:
        val = fPitchLFOFreq;
        break;
    case kPitchLFOSyncFreq:
        val = fPitchLFOSyncFreq;
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
    case kSamplePitchKeyCenter:
        fSamplePitchKeyCenter = value;
        {
            sfizz_arg_t args;
            args.i = static_cast<int>(fSamplePitchKeyCenter);
            synth.sendMessage(*client, 0, "/region0/pitch_keycenter", "i", &args);
            break;
        }
    case kSamplePitch:
        fSamplePitch = std::round(value);
        break;
    case kSamplePlayMode:
        fSamplePlayMode = value;
        {
            sfizz_arg_t args;
            const int i = static_cast<int>(fSamplePlayMode);
            args.s = play_modes_[i];
            synth.sendMessage(*client, 0, "/region0/loop_mode", "s", &args);
        }
        //makeSFZ();
        break;
    case kSamplePlayDirection:
        fSamplePlayDirection = value;
        makeSFZ();
        break;
    case kSampleOversampling:
        // {
        //     std::lock_guard<std::mutex> lock(synthMutex);
        //     const uint index = value;
        //     switch (index)
        //     {
        //     case 0:
        //         synth.setOversamplingFactor(1);
        //         break;
        //     case 1:
        //         synth.setOversamplingFactor(2);
        //         break;
        //     case 2:
        //         synth.setOversamplingFactor(4);
        //         break;
        //     case 3:
        //         synth.setOversamplingFactor(8);
        //         break;
        //     default:
        //         synth.setOversamplingFactor(1);
        //         break;
        //     }
        break;
    //}
    // amp
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
        // lfo
    case kAmpLFOType:
        fAmpLFOType = value;
        {
            sfizz_arg_t args;
            const int i = static_cast<int>(fAmpLFOType);
            args.i = lfo_types_[i];
            synth.sendMessage(*client, 1, "/region0/lfo0/wave", "i", &args);
        }
        break;
    case kAmpLFOSync:
        fAmpLFOSync = value;
        makeSFZ();
        break;
    case kAmpLFOFreq:
        fAmpLFOFreq = value;
        break;
    case kAmpLFOSyncFreq:
        fAmpLFOSyncFreq = value;
        makeSFZ();
        break;
    case kAmpLFODepth:
        fAmpLFODepth = value;
        break;
    case kAmpLFOFade:
        fAmpLFOFade = value;
        break;
        // filter
    case kFilterType:
        fFilterType = value;
        {

            sfizz_arg_t args;
            args.s = filters_[static_cast<uint>(fFilterType)];
            synth.sendMessage(*client, 1, "/region0/filter0/type", "s", &args);
        }
        break;
    case kFilterCutOff:
        fFilterCutOff = value;
        break;
    case kFilterResonance:
        fFilterResonance = value;
        break;
    case kFilterEgDepth:
        fFilterEGDepth = value;
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
        {
            sfizz_arg_t args;
            const int i = static_cast<int>(fFilterLFOType);
            args.i = lfo_types_[i];
            synth.sendMessage(*client, 1, "/region0/lfo1/wave", "i", &args);
        }
        break;
    case kFilterLFOSync:
        fFilterLFOSync = value;
        makeSFZ();
        break;
    case kFilterLFOFreq:
        fFilterLFOFreq = value;
        break;
    case kFilterLFOSyncFreq:
        fFilterLFOSyncFreq = value;
        makeSFZ();
        break;
    case kFilterLFODepth:
        fFilterLFODepth = value;
        break;
    case kFilterLFOFade:
        fFilterLFOFade = value;
        break;
    // pitch
    case kPitchEgDepth:
        fPitchEGDepth = value;
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
        {
            sfizz_arg_t args;
            const int i = static_cast<int>(fPitchLFOType);
            args.i = lfo_types_[i];
            synth.sendMessage(*client, 1, "/region0/lfo2/wave", "i", &args);
        }
        break;
    case kPitchLFOSync:
        fPitchLFOSync = value;
        makeSFZ();
        break;
    case kPitchLFOFreq:
        fPitchLFOFreq = value;
        break;
    case kPitchLFOSyncFreq:
        fPitchLFOSyncFreq = value;
        makeSFZ();
        break;
    case kPitchLFODepth:
        fPitchLFODepth = value;
        break;
    case kPitchLFOFade:
        fPitchLFOFade = value;
        break;

    case kSampleLoaded:
        break;
    default:
        printf("setParameterValue : unknown parameter %i\n", index);
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

    opcodes["offset"] = std::to_string(sampleInInFrames);
    opcodes["end"] = std::to_string(sampleOutInFrames);
    opcodes["direction"] = direction_[static_cast<uint>(fSamplePlayDirection)];

    opcodes["lfo01_wave"] = std::to_string(lfo_types_[static_cast<int>(fAmpLFOType)]);
    opcodes["lfo01_freq"] = std::to_string(fAmpLFOFreq * lfo_max_freq);
    opcodes["lfo01_volume"] = std::to_string(fAmpLFODepth * amp_lfo_depth);
    opcodes["lfo01_fade"] = std::to_string(fAmpLFOFade * lfo_fade);
    opcodes["lfo01_beats"] = lfo_sync_[static_cast<int>(fAmpLFOSyncFreq)];

    opcodes["lfo02_wave"] = std::to_string(lfo_types_[static_cast<int>(fFilterLFOType)]);
    opcodes["lfo02_freq"] = std::to_string(fFilterLFOFreq * lfo_max_freq);
    opcodes["lfo02_cutoff"] = std::to_string(fFilterLFODepth * (fFilterMaxFreq * .5));
    opcodes["lfo02_fade"] = std::to_string(fFilterLFOFade * lfo_fade);
    opcodes["lfo02_beats"] = lfo_sync_[static_cast<int>(fFilterLFOSyncFreq)];

    opcodes["lfo03_wave"] = std::to_string(lfo_types_[static_cast<int>(fPitchLFOType)]);
    opcodes["lfo03_freq"] = std::to_string(fPitchLFOFreq * lfo_max_freq);
    // opcodes["lfo03_pitch"] = std::to_string(fPitchLFODepth * pitch_lfo_depth);
    opcodes["lfo03_fade"] = std::to_string(fPitchLFOFade * lfo_fade);
    opcodes["lfo03_beats"] = lfo_sync_[static_cast<int>(fPitchLFOSyncFreq)];

    opcodes["cutoff"] = std::to_string(fFilterCutOff * fFilterMaxFreq);
    opcodes["fileg_depth"] = std::to_string(fFilterEGDepth * filter_eg_depth);
    opcodes["pitcheg_depth"] = std::to_string(fPitchEGDepth * pitch_eg_depth);
    opcodes["pitch_keycenter"] = std::to_string(static_cast<int>(fSamplePitchKeyCenter));

    std::stringstream buffer;

    buffer << "<region>\n";
    buffer << "sample=" << opcodes["sample"] << "\n";

    // top bar
    buffer << "pitch_keycenter=" << opcodes["pitch_keycenter"] << "\n";
    buffer << "pitch=-100\n"; // tune
    buffer << "pitch_oncc500=200\n";
    buffer << "direction=" << opcodes["direction"] << "\n";
    buffer << "loop_mode=" << play_modes_[static_cast<uint>(fSamplePlayMode)] << "\n";

    // display
    buffer << "offset=0\n";
    buffer << "offset_oncc501=" << std::to_string(sampleLength) << "\n";
    buffer << "end=" << std::to_string(sampleLength) << "\n";
    buffer << "end_oncc502=-" << std::to_string(sampleLength) << "\n";
    buffer << "loop_start=0\n";
    buffer << "loop_start_oncc503=" << std::to_string(sampleLength) << "\n";
    buffer << "loop_end=0\n";
    buffer << "loop_end_oncc504=" << std::to_string(sampleLength) << "\n";

    // amp TAB
    // amp ADSR cc 201 - 299
    buffer << "ampeg_attack=0\n";
    buffer << "ampeg_attack_oncc201=10\n";
    buffer << "ampeg_decay=0\n";
    buffer << "ampeg_decay_oncc202=10\n";
    buffer << "ampeg_sustain=0\n";
    buffer << "ampeg_sustain_oncc203=100\n";
    buffer << "ampeg_release=0.001\n";
    buffer << "ampeg_release_oncc204=10\n";
    // amp LFO
    buffer << "lfo01_wave=" << opcodes["lfo01_wave"] << "\n";
    if (static_cast<bool>(fAmpLFOSync))
    {
        buffer << "lfo01_beats=" << opcodes["lfo01_beats"] << "\n";
        buffer << "lfo01_count=1\n";
    }
    else
    {
        buffer << "lfo01_freq=0\n";
        buffer << "lfo01_freq_oncc205=20\n";
    }
    buffer << "lfo01_volume=0\n";
    buffer << "lfo01_volume_oncc206=12\n";
    buffer << "lfo01_fade=0\n";
    buffer << "lfo01_fade_oncc207=10\n";

    // tab filter
    buffer << "fil_type=" << filters_[static_cast<uint>(fFilterType)] << "\n";
    buffer << "cutoff=20\n"; // << opcodes["cutoff"] << "\n";
    buffer << "cutoff_oncc310=12000\n";
    buffer << "resonance=0\n";
    buffer << "resonance_oncc311=20\n";
    // filter adsr
    buffer << "fileg_depth=0\n";
    buffer << "fileg_depth_oncc312=12000\n";
    buffer << "fileg_attack=0\n";
    buffer << "fileg_attack_oncc301=10\n";
    buffer << "fileg_decay=0\n";
    buffer << "fileg_decay_oncc302=10 \n";
    buffer << "fileg_sustain=0 \n";
    buffer << "fileg_sustain_oncc303=100 \n";
    buffer << "fileg_release=10 \n";
    buffer << "fileg_release_oncc304=-10\n";
    // filter lfo
    buffer << "lfo02_wave=" << opcodes["lfo02_wave"] << "\n";
    if (static_cast<bool>(fFilterLFOSync))
    {
        buffer << "lfo02_beats=" << opcodes["lfo02_beats"] << "\n";
        buffer << "lfo02_count=1\n";
    }
    else
    {
        buffer << "lfo02_freq=0\n";
        buffer << "lfo02_freq_oncc305=20\n";
    }
    buffer << "lfo02_cutoff=0\n";
    buffer << "lfo02_cutoff_oncc306=24000\n";
    buffer << "lfo02_fade=0\n";
    buffer << "lfo02_fade_oncc307=10\n";

    // tab pitch
    // pitch adsr
    buffer << "pitcheg_depth=0\n";
    buffer << "pitcheg_depth_oncc400=1200";
    buffer << "pitcheg_attack=0 \n";
    buffer << "pitcheg_attack_oncc401=10\n";
    buffer << "pitcheg_decay=0\n";
    buffer << "pitcheg_decay_oncc402=10\n";
    buffer << "pitcheg_sustain=0\n";
    buffer << "pitcheg_sustain_oncc403=100\n";
    buffer << "pitcheg_release=0.001\n";
    buffer << "pitcheg_release_oncc404=10\n";
    // pitch lfo
    buffer << "lfo03_wave=" << opcodes["lfo03_wave"] << "\n";
    if (static_cast<bool>(fPitchLFOSync))
    {
        buffer << "lfo03_beats=" << opcodes["lfo03_beats"] << "\n";
        buffer << "lfo03_count=1\n";
    }
    else
    {
        buffer << "lfo03_freq=0\n";
        buffer << "lfo03_freq_oncc405=20\n";
    }
    buffer << "lfo03_pitch=0\n";
    buffer << "lfo03_pitch_oncc406=1200\n";
    buffer << "lfo03_fade=0\n";
    buffer << "lfo03_fade_oncc407=10";
    // buffer << "trigger=attack\n";

    // replace decimal comma wih decimal point
    std::string tmpSFZ = buffer.str();
    std::replace(tmpSFZ.begin(), tmpSFZ.end(), ',', '.'); // TODO don't replace in file path

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
    // get timing info

    std::unique_lock<std::mutex> lock(synthMutex, std::try_to_lock);
    if (!lock.owns_lock())
    { // synth is locked?
        std::fill_n(outputs[0], frames, 0.0f);
        std::fill_n(outputs[1], frames, 0.0f);
        return;
    }
    const TimePosition &timePos(getTimePosition());
    if (timePos.bbt.valid)
    {
        synth.tempo(0, 60.0f / timePos.bbt.beatsPerMinute);
        synth.timeSignature(0, timePos.bbt.beatsPerBar, timePos.bbt.beatType);
        const double beat = timePos.bbt.beat - 1;
        const double fracBeat = timePos.bbt.tick / timePos.bbt.ticksPerBeat;
        const double barBeat = beat + fracBeat;
        synth.timePosition(0, timePos.bbt.bar, barBeat);
        //printf("barBeat %f\n", barBeat);
        synth.playbackState(0, static_cast<int>(timePos.playing));
    }

    uint32_t framesDone = 0;
    uint32_t curEventIndex = 0; // index for midi event to process

    // automation data
    // tab amp
    // amp adsr
    synth.hdcc(0, 201, fAmpEGAttack);
    synth.hdcc(0, 202, fAmpEgDecay);
    synth.hdcc(0, 203, fAmpEgSustain);
    synth.hdcc(0, 204, fAmpEgRelease);
    // amp lfo
    // todo lfo sync
    synth.hdcc(0, 205, fAmpLFOFreq);
    synth.hdcc(0, 206, fAmpLFODepth);
    synth.hdcc(0, 207, fAmpLFOFade);

    // tab filter
    synth.hdcc(0, 310, fFilterCutOff);
    synth.hdcc(0, 311, fFilterResonance);
    // filter adsr
    synth.hdcc(0, 312, fFilterEGDepth);
    synth.hdcc(0, 301, fFilterEGAttack);
    synth.hdcc(0, 302, fFilterEgDecay);
    synth.hdcc(0, 303, fFilterEgSustain);
    synth.hdcc(0, 304, fFilterEgRelease);
    // filter lfo
    // todo lfo sync
    synth.hdcc(0, 305, fFilterLFOFreq);
    synth.hdcc(0, 306, fFilterLFODepth);
    synth.hdcc(0, 307, fFilterLFOFade);

    // tab pitch
    // pitch adsr
    synth.hdcc(0, 400, fPitchEGDepth);
    synth.hdcc(0, 401, fPitchEGAttack);
    synth.hdcc(0, 402, fPitchEgDecay);
    synth.hdcc(0, 403, fPitchEgSustain);
    synth.hdcc(0, 404, fPitchEgRelease);
    // pitch lfo
    // todo lfo sync
    synth.hdcc(0, 405, fPitchLFOFreq);
    synth.hdcc(0, 406, fPitchLFODepth);
    synth.hdcc(0, 407, fPitchLFOFade);

    const float n = fSamplePitch / 200.f;
    synth.hdcc(0, 500, n);
    synth.hdcc(0, 501, fSampleIn);
    synth.hdcc(0, 502, -fSampleOut + 1.f);
    synth.hdcc(0, 503, fSampleLoopStart);
    synth.hdcc(0, 504, fSampleLoopEnd);

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

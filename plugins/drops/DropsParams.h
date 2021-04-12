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

/*    line comments are opcodes        */
/*    https://sfzformat.com/opcodes/   */
enum Parameters
{
   /*    common     */
   // kPitchBendDepth, // bend_down, bend_up
   // kPolyphony,      // polyphony
   // kGain,           // volume

   /*    display     */
   kSampleIn,        // offset
   kSampleOut,       // end
   kSampleLoopStart, // loop_start
   kSampleLoopEnd,   // loop_end

   /*    sample tab    */
   // kSampleXFade,          // loop_crossfade (not implemented)
   // kSampleNormalize,      // amplitude or volume
   kSamplePitchKeyCenter, // pitch_keycenter
   kSamplePitch,          // tune
   kSamplePlayMode,       // loopmode
   kSamplePlayDirection,  // direction
   kSampleOversampling,   // 1,2,4,8

   /*    amp tab */
   kAmpEgAttack,  // ampeg_attack
   kAmpEgDecay,   // ampeg_decay
   kAmpEgSustain, // ampeg_sustain
   kAmpEgRelease, // ampeg_release
   kAmpLFOType,   // lfoN_wave
   kAmpLFOSync,   
   kAmpLFOFreq,   // lfoN_freq
   kAmpLFOSyncFreq,
   kAmpLFODepth,  // lfoN_amplitude
   kAmpLFOFade,   // lfoN_fade
 
   /* filter tab */
   kFilterType,      // fil_type
   kFilterCutOff,    // cutoff , cutoff_oncc200
   kFilterResonance, // resonance
   kFilterEgDepth,   // fileg_depth
   kFilterEgAttack,  // fileg_attack
   kFilterEgDecay,   // fileg_decay
   kFilterEgSustain, // fileg_sustain
   kFilterEgRelease, // fileg_release
   kFilterLFOType,   // lfoN_wave
   kFilterLFOSync,   
   kFilterLFOFreq,  // lofN_freq
   kFilterLFOSyncFreq,
   kFilterLFODepth, // lfoN_filter
   kFilterLFOFade,  // lfoN_fade
   
   
   /*  pitch tab */
   kPitchEgDepth,   // pitcheg_depth
   kPitchEgAttack,  // pitcheg_attack
   kPitchEgDecay,   // pitcheg_decay
   kPitchEgSustain, // pitcheg_sustain
   kPitchEgRelease, // pitcheg_release
   kPitchLFOType,   // lfoN_wave
   kPitchLFOSync,
   kPitchLFOSyncFreq,
   kPitchLFOFreq,   // lfoN_freq
   kPitchLFODepth,  // lfoN_pitch
   kPitchLFOFade,   // lfoN_fade
   // kPitchLFOSync,

   kSampleLoaded, // output port to signal UI

   kParameterCount
};

enum Widgets
{
   kScrollbarHandle = 1000,
   kScrollbarLeft,
   kScrollbarRight,
   kNormalizeMenu,
   kKeyCenterMenu,
   kPlayModeMenu,
   kDirectionMenu,
   kOversamplingMenu,
   kAmpLFOFreqBeat,
   kAmpLFOTypeMenu,
   kVBoxSample,
   kHBoxSampleRow1,
   kHBoxSampleRow2,
   kVBoxAmp,
   kHBoxAmpRow1,
   kHBoxAmpRow2,
   kHBoxAmpSync,
   kVBoxAmpLfo,
   kVBoxPitch,
   kHBoxPitchRow1,
   kHBoxPitchRow2,
   kHBoxPitchSync,
   kVBoxPitchLfo,
   kPitchLFOFreqBeat,
   kPitchLFOTypeMenu,
   
   kVBoxFilter,
   kHBoxFilterRow1,
   kHBoxFilterRow2,
   kHBoxFilterRow3,
   kHBoxFilterSync,
   kVBoxFilterLfo,
   kFilterLFOTypeMenu,
   kFilterBandpass,
   kFilterLowpass,
   kFilterHighpass,

   kHboxZoomIcons,
   kZoomOut,
   kZoomIn,
   kZoomAll,
   kZoomInOut,
};

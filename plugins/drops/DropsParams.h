
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
   kSamplePitch,           // tune
   kSamplePlayMode,      // loopmode
   kSamplePlayDirection, // direction
   kSampleOversampling, // 1,2,4,8
   /*    amp tab */
   kAmpLFOType,   // lfoN_wave
   kAmpLFOFreq,   // lfoN_freq
   kAmpLFODepth,  // lfoN_amplitude
   kAmpLFOFade,   // lfoN_fade
   kAmpEgAttack,  // ampeg_attack
   kAmpEgDecay,   // ampeg_decay
   kAmpEgSustain, // ampeg_sustain
   kAmpEgRelease, // ampeg_release

   // kAmpLFOSync,   //
   /* filter tab */
   kFilterType,      // fil_type
   kFilterLFOType,   // lfoN_wave
   kFilterLFOFreq,   // lofN_freq
   kFilterLFODepth,  // lfoN_filter
   kFilterLFOFade,
   kFilterCutOff,    // cutoff , cutoff_oncc200
   kFilterResonance, // resonance
   kFilterEgAttack,  // fileg_attack
   kFilterEgDecay,   // fileg_decay
   kFilterEgSustain, // fileg_sustain
   kFilterEgRelease, // fileg_release
   //kFilterEgDepth,   // fileg_depth
   //kFilterLFOSync,

   /*  pitch tab */
   kPitchLFOType,   // lfoN_wave
   kPitchLFOFreq,   // lfoN_freq
   kPitchLFODepth,  // lfoN_pitch
   kPitchLFOFade,
   kPitchEgAttack,  // pitcheg_attack
   kPitchEgDecay,   // pitcheg_decay
   kPitchEgSustain, // pitcheg_sustain
   kPitchEgRelease, // pitcheg_release
   // kPitchEgDepth,   // pitcheg_depth
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
   kAmpLFOSyncMenu,
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
   kPitchLFOSyncMenu,

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


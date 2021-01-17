
#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND "Clearly Broken Software"
#define DISTRHO_PLUGIN_NAME "Drops"
#define DISTRHO_PLUGIN_URI "http://github.com/clearly-broken-software/drops"

#define DISTRHO_PLUGIN_IS_SYNTH 1
#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_NUM_INPUTS 0
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_PLUGIN_WANT_TIMEPOS 1
#define DISTRHO_PLUGIN_WANT_STATE 1
#define DISTRHO_PLUGIN_WANT_FULL_STATE 1
#define DISTRHO_UI_USE_NANOVG 1
#define DISTRHO_PLUGIN_WANT_MIDI_INPUT 1
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS 1
#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:InstrumentPlugin"

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
   // kSampleTune,           // tune
   kSamplePlayMode,      // loopmode
   kSamplePlayDirection, // direction
   /*    amp tab */
   kAmpLFOType,   // lfoN_wave
   kAmpLFOFreq,   // lfoN_freq
   kAmpLFODepth,  // lfoN_amplitude
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

static constexpr unsigned int UI_W = 1000;
static constexpr unsigned int UI_H = 515;
static constexpr unsigned int display_left = 35;
static constexpr unsigned int display_top = 107;
static constexpr unsigned int display_width = UI_W - (2 * display_left);
static constexpr unsigned int display_right = display_left + display_width;
static constexpr unsigned int display_height = 183;
static constexpr unsigned int display_bottom = display_top + display_height;
static constexpr unsigned int display_center = (display_bottom - display_top) / 2 + display_top;
static constexpr unsigned int minimap_height = 30;

static constexpr unsigned int sample_inout_font_size = 12;

static constexpr float slider_right_padding = 60.f;
#endif

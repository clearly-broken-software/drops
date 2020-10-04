# Drops

Drops Really Only Plays Samples

Drops is a single audio file sampler.  
Load an audio file, play it, loop it, pitch shift it, and make it into something entirely new.

## Planned feature set

- Sample Playback:
  - Load and play a single audio file
    - Set sample root key
    - Graphically set start, end, and loop points
    - Set Cross-fade length
    - Playback direction
- Play modes:
  - One-shot
    - Play with Loop
    - Loop only
- Modulation:
  - DAHDSR Envelope for each: Amplitude, Pitch & Filter
    - Set MIDI note-on velocity of Envelope
    - Modulation of each Envelope via LFO:
      - Select LFO waveform (including random)
        - Change LFO frequency, or beat sync
        - LFO fade-in
        - LFO Depth control
- Global:
  - Max Polyphony
    - Main Volume
    - Pitchbend range

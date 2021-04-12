# Drops

#### Drops Really Only Plays Samples
![Drops](https://raw.githubusercontent.com/clearly-broken-software/drops/main/Drops1.0beta.png "Screenshot of Drops")

Drops is a single audio file sample player plugin in lv2 and vst format for linux.

Load an audio file, play it, loop it, pitch shift it, and make it into something entirely new.

The sample engine is [sfizz](https://github.com/sfztools/sfizz)

Drops is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

#### Some of the features:
  + audio disk streaming, allowing for large audio files to be played
  + waveform display with zooming and scrolling
    + note: use mousewheel to zoom in and out
  + 'minimap' with scroll area
  + sample in/out and loop points
  + ADSR for amp, filter and pitch
  + LFO, free running or sync to transport and bpm for amp, filter and pitch

---
#### INSTALL

you can use the attached binaries on the release page or build Drops yourself.

#### Build instructions

**Dependencies**

+  libgl1-mesa-dev 
+  libx11-dev
+  libsndfile1-dev

If you want a standalone jack binary install either `libjack-dev` or `libjack-jackd2-dev`

```
git clone --recursive https://github.com/clearly-broken-software/drops.git
cd drops
make
```
There is no ~`make install`~, binaries will appear in `/path/to/drops/bin/`, just copy the binaries to an appropriate location.
Typically, for **lv2** that would be `~/.lv2` or `/usr/lib/lv2` and for **vst** typically `~/.vst` or `/usr/lib/vst` on linux.

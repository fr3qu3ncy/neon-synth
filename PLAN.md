# Neon Synth — VST3 Synthesizer Plugin

## Overview
A polyphonic subtractive synthesizer VST3 plugin with 2 oscillators, dual ADSR envelopes, 3 filter types, and preset management. Modern smart neon aesthetic.

## Tech Stack
- **Framework:** JUCE (C++) — VST3/AU/AWV export, DSP, GUI
- **Build:** CMake
- **Target:** Linux VST3 first (macOS/Windows cross-platform capable)
- **Polyphony:** 16 voices max with voice stealing

## Signal Flow
```
OSC1 ──┐
        ├─→ VCA ──→ VCF ──→ Output
OSC2 ──┘
```

## DSP Architecture

### Oscillators (×2)
- **Waveforms:** sine, saw, square, triangle
- **Range:** 20 Hz – 20 kHz (full keyboard tracking)
- **Detune:** ±50 cents per oscillator
- **Panning:** L/R per oscillator
- **Gain:** per-oscillator level control
- **Implementation:** Wavetable synthesis with zero-crossing interpolation (BLIT/BLIT2 for band-limited square/saw)

### Envelopes (×2, ADSR)
- **Amplitude Envelope (VCA):** Attack, Decay, Sustain, Release — drives VCA gain per note
- **Filter Envelope (VCF):** Attack, Decay, Sustain, Release — drives filter cutoff deviation (0–12 semitones)
- **Keyboard retrigger toggle** per envelope

### Filters (switchable)
- **Low Pass** (12dB/oct) — state-variable filter
- **High Pass** (12dB/oct) — same topology
- **Band Pass** (12dB/oct) — same topology
- **Cutoff:** 20 Hz – 20 kHz (logarithmic scale)
- **Resonance:** 0 – 0.9 (self-oscillation at max)
- **Keyboard tracking toggle** for cutoff

### Voice Management
- 16-voice polyphony
- Per-voice: 2 oscillators + 2 envelopes + 1 filter
- Voice stealing on note overflow (oldest note first)
- Smooth voice release on note-off (respect release phase)

## UI Design — "Smart Neon"

### Palette
- Background: deep charcoal (#0d0d0f)
- Primary accent: cyan (#00f0ff)
- Secondary accent: magenta (#ff00aa)
- Highlights: electric blue (#4060ff)
- Text: white (#f0f0f0)

### Visual Treatment
- Circular knobs with neon ring glow (OpenGL shader blur)
- Active controls pulse subtly (breathing glow animation)
- Vertical neon sliders with gradient fills
- Live oscilloscope visualization (2-channel, side by side)
- Live filter frequency response curve (neon line graph)
- Editable ADSR envelope graphs with draggable nodes
- Preset browser: scrollable list with neon-highlighted selection

### Layout (800×500)
```
┌──────────────────────────────────────────────┐
│  [OSC1 controls]    [OSC2 controls]          │
│  waveform  tune  gain  pan   waveform...     │
├──────────────────────────────────────────────┤
│  [Oscilloscope visualization — live]         │
├──────────────────────┬───────────────────────┤
│  [VCA Envelope]      │  [VCF Envelope]       │
│  A  D  S  R knobs    │  A  D  S  R knobs     │
│  + editable graph    │  + editable graph      │
├──────────────────────┴───────────────────────┤
│  [Filter section]                            │
│  [LP] [HP] [BP]  cutoff  resonance  tracking │
│  + live response curve                       │
├──────────────────────────────────────────────┤
│  [Preset browser]  [Save] [Load] [Delete]    │
└──────────────────────────────────────────────┘
```

## Presets
- **Format:** JSON via `juce::ValueTree` serialization
- **Storage:** `JucePluginUserResourcesPath`
- **Operations:** save / load / delete / browse
- **Factory presets:** 8–12 starters included

## Build Plan

### Phase 1 — DSP Core (Days 1–4)
1. JUCE project scaffolding with CMake
2. `AudioProcessor` with plugin manifest (VST3)
3. Wavetable oscillator class with band-limited waveforms
4. ADSR envelope class (per-voice)
5. State-variable filter class (LP/HP/BP)
6. VCA + VCF signal chain
7. Polyphonic voice manager (16 voices, voice stealing)
8. MIDI note-on/note-off handling
9. Audio processing loop integration
10. Unit tests for DSP components

### Phase 2 — UI (Days 5–8)
1. `AudioProcessorEditor` layout
2. Custom `LookAndFeel` subclass (neon theme)
3. Custom rotary knobs with glow
4. OpenGL oscilloscope component
5. OpenGL filter response curve
6. ADSR envelope editor with draggable nodes
7. Filter type selector buttons
8. Preset browser panel

### Phase 3 — Presets + Polish (Days 9–10)
1. `ValueTree` parameter serialization
2. JSON file I/O for presets
3. Factory presets (8–12)
4. Parameter automation (MIDI CC mapping)
5. Build VST3 binary
6. Test in host (Carla or similar)

## Technical Risks & Mitigations

| Risk | Mitigation |
|------|-----------|
| SVF stability at all sample rates | DC blocking filter + dithering + coefficient clamping |
| Aliasing on square/saw | BLIT/BLIT2 band-limited waveforms |
| Neon glow CPU cost | Offscreen buffer + composite, not per-control blur |
| 16-voice × 2 envs = 32 envelopes | Efficient voice pool + lazy allocation |
| No system apt access | Download cmake binary from GitHub releases |

## Project Structure
```
neon-synth/
├── CMakeLists.txt
├── Source/
│   ├── PluginProcessor.cpp/h    # AudioProcessor (DSP entry)
│   ├── PluginEditor.cpp/h       # Editor (UI entry)
│   ├── DSP/
│   │   ├── Oscillator.cpp/h     # Wavetable oscillator
│   │   ├── Envelope.cpp/h       # ADSR envelope
│   │   ├── Filter.cpp/h         # State-variable filter
│   │   ├── Voice.cpp/h          # Single voice (OSC+ENV+FLT)
│   │   └── VoiceManager.cpp/h   # Polyphonic voice manager
│   └── UI/
│       ├── LookAndFeelNeon.cpp/h # Custom neon theme
│       ├── Oscilloscope.cpp/h    # OpenGL oscilloscope
│       ├── EnvelopeEditor.cpp/h  # ADSR editor
│       ├── FilterEditor.cpp/h    # Filter curve display
│       └── PresetBrowser.cpp/h   # Preset management UI
├── Presets/
│   └── factory/                 # Factory preset JSON files
├── Assets/
│   └── shaders/                 # OpenGL glow shaders
└── Tests/
    └── dsp_tests.cpp            # DSP unit tests
```

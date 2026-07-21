# Neon Synth — VST3 Synthesizer Plugin

## Concept

A dual-oscillator subtractive synthesizer VST3 plugin with a neon-modern aesthetic. Dark charcoal background, cyan/magenta/purple neon accents, glow effects, subtle grid overlay.

---

## Architecture

### DSP Engine

```
┌─────────────────────────────────────────────────────────┐
│                    VoiceManager                          │
│  (polyphonic voice allocation, max 16 voices)            │
├─────────────────────────────────────────────────────────┤
│  Voice 1    Voice 2    ...    Voice N                   │
│  ┌──────────┐ ┌──────────┐       ┌──────────┐          │
│  │ Osc 1    │ │ Osc 1    │       │ Osc 1    │          │
│  │ +        │ │ +        │       │ +        │          │
│  │ Osc 2    │ │ Osc 2    │       │ Osc 2    │          │
│  │ ↓ mix    │ │ ↓ mix    │       │ ↓ mix    │          │
│  │ Amp Env  │ │ Amp Env  │       │ Amp Env  │          │
│  │ ↓        │ │ ↓        │       │ ↓        │          │
│  │ Filter   │ │ Filter   │       │ Filter   │          │
│  │ (F.Env)  │ │ (F.Env)  │       │ (F.Env)  │          │
│  │ ↓ pan    │ │ ↓ pan    │       │ ↓ pan    │          │
│  └──────────┘ └──────────┘       └──────────┘          │
└─────────────────────────────────────────────────────────┘
         ↓
    Stereo Bus → VST3 Output
```

### Oscillators (2 per voice)

| Param    | Range              | Default | Description           |
| -------- | ------------------ | ------- | --------------------- |
| Waveform | Sine/Saw/Sqr/Pulse | Saw     | Oscillator waveform   |
| Detune   | -50 to +50 ct      | 0       | Cents detune vs. base |
| Gain     | 0.0 to 1.0         | 0.5     | Oscillator level      |
| Pan      | -1.0 to +1.0       | 0.0     | Stereo pan position   |

Waveforms generated via band-limited tables (BLIT/BLIT2 for saw/square).

### Amplitude Envelope (per voice)

| Param   | Range      | Default | Description     |
| ------- | ---------- | ------- | --------------- |
| Attack  | 1ms to 5s  | 10ms    | Note-on rise    |
| Decay   | 1ms to 5s  | 300ms   | Fall to sustain |
| Sustain | 0.0 to 1.0 | 0.7     | Held level      |
| Release | 1ms to 10s | 500ms   | Note-off fall   |

### Filter Envelope (per voice, modulates filter cutoff)

| Param   | Range      | Default | Description     |
| ------- | ---------- | ------- | --------------- |
| Attack  | 1ms to 5s  | 10ms    | Env rise time   |
| Decay   | 1ms to 5s  | 300ms   | Fall to sustain |
| Sustain | 0.0 to 1.0 | 0.5     | Held amount     |
| Release | 1ms to 10s | 500ms   | Release time    |

### Filter (per voice)

| Param     | Range           | Default | Description          |
| --------- | --------------- | ------- | -------------------- |
| Type      | LP / HP / BP    | LP      | Low/High/Band-pass   |
| Cutoff    | 20 Hz to 20 kHz | 1 kHz   | Corner frequency     |
| Resonance | 0.0 to 0.9      | 0.0     | Q / self-oscillation |

One-pole resonant filter per voice, switchable between LP/HP/BP topologies. Filter envelope adds multiplicatively to cutoff (multiplicative modulation).

### Presets

- XML-based state serialization via `juce::ValueTree`
- Save/load/delete from user presets directory
- Default presets shipped with plugin

---

## UI Layout (800×500)

```
┌──────────────────────────────────────────────────────────────┐
│  NEON SYNTH                                    [preset list] │
├──────────────────┬──────────────────┬───────────────────────┤
│   OSCILLATOR 1   │   OSCILLATOR 2   │      FILTER           │
│  ┌────────────┐  │  ┌────────────┐  │  ┌────────────┐      │
│  │ Waveform   │  │  │ Waveform   │  │  │ Type: LP   │      │
│  │ [Saw ▼]    │  │  │ [Sine ▼]   │  │  │ [▼ ▼ ▼]    │      │
│  └────────────┘  │  └────────────┘  │  └────────────┘      │
│  ┌────────────┐  │  ┌────────────┐  │  ┌────────────┐      │
│  │ Detune     │  │  │ Detune     │  │  │ Cutoff     │      │
│  │  ────●──── │  │  │  ────●──── │  │  │  ─────●─── │      │
│  └────────────┘  │  └────────────┘  │  └────────────┘      │
│  ┌────────────┐  │  ┌────────────┐  │  ┌────────────┐      │
│  │ Gain       │  │  │ Gain       │  │  │ Resonance  │      │
│  │  ────●──── │  │  │  ────●──── │  │  │  ────●──── │      │
│  └────────────┘  │  └────────────┘  │  └────────────┘      │
│  ┌────────────┐  │  ┌────────────┐  │                       │
│  │ Pan        │  │  │ Pan        │  │                       │
│  │  ────●──── │  │  │  ────●──── │  │                       │
│  └────────────┘  │  └────────────┘  │                       │
├──────────────────┴──────────────────┼───────────────────────┤
│  AMP ENVELOPE     │  FILT ENVELOPE  │                       │
│  ┌────────────┐  │  ┌────────────┐  │                       │
│  │ ADSR visual │  │  │ ADSR visual│  │                       │
│  │  display    │  │  │  display   │  │                       │
│  └────────────┘  │  └────────────┘  │                       │
│  [A][D][S][R]    │  [A][D][S][R]    │                       │
├──────────────────────────────────────┼───────────────────────┤
│  OSCILLOSCOPE                        │  PRESET BROWSER       │
│  ┌────────────────────────────────┐  │  ┌─────────────────┐ │
│  │                                │  │  │ Preset List     │ │
│  │    waveform display            │  │  │                 │ │
│  │    (cyan trace on dark bg)     │  │  │                 │ │
│  │                                │  │  └─────────────────┘ │
│  └────────────────────────────────┘  │  [Save] [Delete]     │
└──────────────────────────────────────┴───────────────────────┘
```

### Neon Theme

- **Background:** `#0d0d0f` (deep charcoal)
- **Grid overlay:** 6% white opacity, 20px spacing
- **Primary neon:** `#00f0ff` (cyan) — knobs, labels, waveforms
- **Secondary neon:** `#ff0066` (magenta) — accents, filter
- **Tertiary neon:** `#9933ff` (purple) — envelopes
- **Glow:** 8px stroke at 19% opacity behind main 2px lines
- **Font:** System sans-serif, bold headers, regular body
- **Knobs:** Custom rotary sliders with neon arc + glow
- **Buttons:** Rounded rectangles with neon border on hover/press

---

## Source Files (scaffolded)

```
neon-synth/
├── CMakeLists.txt                    # Build config, JUCE submodule
├── JUCE/                             # JUCE 8 submodule
├── Source/
│   ├── PluginProcessor.h             # AudioProcessor, parameter tree
│   ├── PluginProcessor.cpp           # DSP init, process loop, MIDI
│   ├── PluginEditor.h                # Main editor window
│   ├── PluginEditor.cpp              # Paint, layout, neon theme
│   ├── DSP/
│   │   ├── Oscillator.h/.cpp         # BLIT waveform generator
│   │   ├── Envelope.h/.cpp           # ADSR envelope engine
│   │   ├── Filter.h/.cpp             # 1-pole LP/HP/BP filter
│   │   ├── Voice.h/.cpp              # Single polyphonic voice
│   │   └── VoiceManager.h/.cpp       # Polyphonic voice allocator
│   └── UI/
│       ├── LookAndFeelNeon.h/.cpp    # Custom LookAndFeel (neon theme)
│       ├── EnvelopeEditor.h/.cpp     # ADSR visual editor
│       ├── FilterEditor.h/.cpp       # Filter type + cutoff + resonance
│       ├── Oscilloscope.h/.cpp       # Real-time waveform display
│       └── PresetBrowser.h/.cpp      # Save/load/delete presets
└── Assets/
    └── presets/                      # Default presets (XML)
```

---

## Build Status

**Scaffolded:** ✅ All source files created with working implementations **JUCE 8 API:** ⚠️ ~20 API mismatches need reconciliation

The JUCE 8 release has more aggressive breaking changes than anticipated:

- `LookAndFeel` override signatures changed (getSliderFont, drawRotarySlider)
- `juce::Button` is now abstract (must use `TextButton`/`ToggleButton`)
- `File::specialFolder` API restructured
- `XMLDocument::parse` replaced with new serialization
- `AudioProcessorValueTreeState` API changed (no setState)
- `Graphics::setStrokeType` removed (use PathStrokeType directly)
- `AudioProcessorPrepareInfo` not in this build
- Various deprecated constructors (Font, drawText)

**Resolution path:** Systematically update each file to match the actual JUCE 8.0.x API in use. Estimated 2-3 hours of API reconciliation work.

---

## What Would Be Next

1. **Fix JUCE 8 API mismatches** — update LookAndFeel overrides, Button types, File API, XML serialization, Graphics calls
2. **Wire up UI ↔ DSP** — connect sliders to parameters, update envelope visuals in real-time
3. **Implement oscilloscope** — render real-time stereo output waveform
4. **Test in DAW** — load in Reaper/Bitwig, verify audio passthrough, MIDI response
5. **Add default presets** — ship 5-10 factory presets (bass, lead, pad, pluck, etc.)
6. **Build distributable** — VST3 binary for all platforms

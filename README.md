# NeonSynth

A dual-oscillator subtractive synthesizer VST3 plugin with a smart neon aesthetic.

![NeonSynth](https://img.shields.io/badge/status-development-blue)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- **2 Oscillators** — sine, saw, square, triangle with detune and panning
- **Dual ADSR Envelopes** — amplitude (VCA) and filter (VCF) with independent attack, decay, sustain, release
- **3 Filter Types** — low pass, high pass, band pass (12 dB/oct state-variable filter)
- **Filter Controls** — cutoff (20 Hz–20 kHz), resonance (0–0.9), keyboard tracking
- **Preset System** — save, load, and browse presets in JSON format
- **Neon UI** — dark theme with cyan/magenta glow effects, live oscilloscope, and filter response visualization

## Signal Flow

```
OSC1 ──┐
        ├─→ VCA (Amp Envelope) ──→ VCF (Filter Envelope) ──→ Output
OSC2 ──┘
```

## Requirements

| Platform | Dependencies |
|----------|-------------|
| **Linux** | g++ 11+, CMake 3.15+, pkg-config, ALSA dev, X11 dev, FreeType, OpenGL |
| **macOS** | Xcode CLT, CMake 3.15+, macOS 10.13+ |
| **Windows** | Visual Studio 2019+, CMake 3.15+ |

### Linux (Debian/Ubuntu)

```bash
sudo apt install -y build-essential cmake pkg-config \
  libasound2-dev libx11-dev libxrandr-dev libxrender-dev \
  libxinerama-dev libxkbcommon-dev libfreetype-dev \
  libfontconfig1-dev libgl1-mesa-dev libjack-jackd2-dev
```

### macOS

```bash
xcode-select --install
brew install cmake
```

### Windows

Install **Visual Studio 2019+** (Desktop development with C++ workload) and **CMake 3.15+**.

## Building

```bash
# Clone and fetch JUCE submodule
git clone --recursive https://github.com/fr3qu3ncy/neon-synth.git
cd neon-synth

# Configure
mkdir Build && cd Build
cmake .. -DJUCE_COPY_PLUGIN_AFTER_BUILD=OFF

# Build (use -j2 on systems with limited RAM)
make -j$(nproc)
```

The VST3 plugin will be output to `Build/bin/VST3/NeonSynth.vst3`.

### Platform-specific output

| Platform | Output Path |
|----------|------------|
| Linux | `Build/bin/VST3/NeonSynth.vst3` |
| macOS | `Build/bin/NeonSynth.vst3` |
| Windows | `Build\bin\VST3\NeonSynth.vst3` |

### Memory-constrained builds

JUCE is large. If compilation is killed by OOM, reduce parallelism:

```bash
make -j2
```

## Installation

Copy the `.vst3` bundle to your DAW's plugin search path:

| DAW | Path |
|-----|------|
| Reaper | `~/.vst3/` (Linux) or `/Library/Audio/Plug-Ins/VST3/` (macOS) |
| Ableton Live | Auto-scans standard VST3 paths |
| Bitwig Studio | Auto-scans standard VST3 paths |
| FL Studio | `Options > Settings > File Paths > VST plugin paths` |

## Architecture

```
Source/
├── DSP/
│   ├── Oscillator.cpp/h      # Wavetable oscillator with 4 waveforms
│   ├── Envelope.cpp/h        # ADSR envelope generator
│   ├── Filter.cpp/h          # State-variable LP/HP/BP filter
│   ├── Voice.cpp/h           # Single polyphonic voice
│   └── VoiceManager.cpp/h    # 16-voice polyphony with voice stealing
├── UI/
│   ├── LookAndFeelNeon.cpp/h # Neon theme (cyan/magenta glow)
│   ├── EnvelopeEditor.cpp/h  # Draggable ADSR graph
│   ├── FilterEditor.cpp/h    # Live filter response curve
│   ├── Oscilloscope.cpp/h    # Real-time waveform display
│   └── PresetBrowser.cpp/h   # Preset save/load/browse
├── PluginProcessor.cpp/h     # AudioProcessor + APVTS parameter state
└── PluginEditor.cpp/h        # Main UI layout
```

## Presets

Presets are stored as JSON files in the plugin's user data directory. Factory presets ship with the plugin. Use the preset browser in the UI to save, load, and manage patches.

## Roadmap

- [ ] MIDI CC mapping for all parameters
- [ ] LFO modulation routing
- [ ] Arpeggiator
- [ ] AU and AAX formats
- [ ] macOS and Windows builds

## License

MIT

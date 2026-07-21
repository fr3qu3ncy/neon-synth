cmake_minimum_required(VERSION 3.15...3.31)

project(NeonSynth
    VERSION 1.0.0
    DESCRIPTION "NeonSynth - Dual oscillator synthesizer"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- JUCE submodule ---
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    message(STATUS "Cloning JUCE submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

add_subdirectory(JUCE EXCLUDE_FROM_ALL)

# --- Plugin format flags ---
set(JUCE_ENABLE_MODULE_SOURCE_LOGOFF ON)
set(JUCE_DISPLAY_SPLASH_SCREEN OFF)
set(JUCE_WEB_BROWSER OFF)

# --- Build plugin (VST3 with GUI) ---
juce_add_plugin(NeonSynth
    PLUGIN_MANUFACTURER_CODE NeSy
    PLUGIN_CODE NnSy
    FORMATS VST3

    PRODUCT_NAME "NeonSynth"
    COMPANY_NAME "NeonAudio"
    IS_SYNTH YES
    NEEDS_MIDI_INPUT YES
    NEEDS_MIDI_OUTPUT NO
    IS_MIDI_PLUGIN NO

    VST3_CATEGORY "instrument"
    VST3_IS_A_SYNTH YES
    VST3_CAN_PROCESS_INPLACE NO
)

# --- Add our source files ---
target_sources(NeonSynth PRIVATE
    # Plugin entry
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp

    # DSP engine
    Source/DSP/Oscillator.cpp
    Source/DSP/Envelope.cpp
    Source/DSP/Filter.cpp
    Source/DSP/Voice.cpp
    Source/DSP/VoiceManager.cpp

    # UI components
    Source/UI/LookAndFeelNeon.cpp
    Source/UI/EnvelopeEditor.cpp
    Source/UI/FilterEditor.cpp
    Source/UI/Oscilloscope.cpp
    Source/UI/PresetBrowser.cpp
)

# --- Generate JuceHeader.h ---
juce_generate_juce_header(NeonSynth)

# --- Include directories ---
target_include_directories(NeonSynth PRIVATE
    Source
    ${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules
)

# --- Link audio + GUI modules ---
target_link_libraries(NeonSynth PRIVATE
    juce_audio_plugin_client
    juce_audio_utils
    juce_dsp
    juce_core
    juce_gui_basics
    juce_gui_extra
    juce_graphics
)

# --- VST3-only defines ---
target_compile_definitions(NeonSynth PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=1
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)

# --- Optimizations ---
target_compile_options(NeonSynth PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-O3 -march=native -ffast-math>
    $<$<CXX_COMPILER_ID:Clang>:-O3 -march=native -ffast-math>
)

# --- Install ---
install(TARGETS NeonSynth
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    BUNDLE  DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

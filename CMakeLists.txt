cmake_minimum_required(VERSION 3.22)
project(VirtualApolloCore VERSION 2.0.0)

# --- INCLUIR JUCE AUTOMÁTICAMENTE DESDE GITHUB ---
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(JUCE)

juce_add_gui_app(VirtualApolloCore
    PRODUCT_NAME "Virtual Apollo Core"
    COMPANY_NAME "Angel Music Records"
    BUNDLE_IDENTIFIER "com.angelmusic.virtualapollo"
)

target_sources(VirtualApolloCore
    PRIVATE
        Source/Main.cpp
)

target_compile_definitions(VirtualApolloCore
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_UI=0
)

target_link_libraries(VirtualApolloCore
    PRIVATE
        juce::juce_gui_basics
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_processors
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11 CACHE STRING "Build for 10.11")

project(surge-fx VERSION 0.9.0 LANGUAGES CXX)

add_subdirectory(deps/JUCE)

execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

juce_add_plugin(surge-fx
    PRODUCT_NAME "SurgeEffectsBank"
    COMPANY_NAME "Surge Synth Team"  
    BUNDLE_ID "org.surge-synth-team.surge-fx"
    PLUGIN_MANUFACTURER_CODE VmbA
    PLUGIN_CODE SFXB

    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    
    FORMATS AU VST3 Standalone
)

juce_generate_juce_header( surge-fx )

set_property(TARGET surge-fx PROPERTY CXX_STANDARD 17)

file(GLOB TWS_RESOURCES_GLOB
  resources/*.svg
  deps/surge/resources/data/configuration.xml
  )

juce_add_binary_data( surge-fx-binary
  SOURCES ${TWS_RESOURCES_GLOB}
)

set_target_properties(surge-fx-binary PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
    
target_include_directories(surge-fx 
  PRIVATE
  src
  deps/surge/src/common
  deps/surge/src/common/dsp
  deps/surge/src/common/effects
  deps/surge/src/headless
  deps/surge/libs/xml
  deps/surge/libs/tuning-library/include
  deps/surge/libs/strnatcmp
  deps/surge/libs/filesystem
)

configure_file(
  ${CMAKE_SOURCE_DIR}/src/version.cpp.in
  ${CMAKE_BINARY_DIR}/generated/version.cpp
)

target_sources(surge-fx PRIVATE
  src/OSSpecificHacks.cpp
  src/SurgeFXEditor.cpp
  src/SurgeFXProcessor.cpp

  deps/surge/libs/strnatcmp/strnatcmp.cpp
  deps/surge/libs/filesystem/filesystem.cpp
  
  deps/surge/libs/xml/tinyxml.cpp
  deps/surge/libs/xml/tinyxmlerror.cpp
  deps/surge/libs/xml/tinyxmlparser.cpp
  
  deps/surge/src/common/dsp/effect/ConditionerEffect.cpp
  deps/surge/src/common/dsp/effect/DistortionEffect.cpp
  deps/surge/src/common/dsp/effect/DualDelayEffect.cpp
  deps/surge/src/common/dsp/effect/Effect.cpp
  deps/surge/src/common/dsp/effect/FlangerEffect.cpp
  deps/surge/src/common/dsp/effect/FreqshiftEffect.cpp
  deps/surge/src/common/dsp/effect/PhaserEffect.cpp
  deps/surge/src/common/dsp/effect/Reverb1Effect.cpp
  deps/surge/src/common/dsp/effect/Reverb2Effect.cpp
  deps/surge/src/common/dsp/effect/RingModulatorEffect.cpp
  deps/surge/src/common/dsp/effect/RotarySpeakerEffect.cpp
  deps/surge/src/common/dsp/effect/VocoderEffect.cpp
  deps/surge/src/common/dsp/AdsrEnvelope.cpp
  deps/surge/src/common/dsp/BiquadFilter.cpp
  deps/surge/src/common/dsp/BiquadFilterSSE2.cpp
  deps/surge/src/common/dsp/DspUtilities.cpp
  deps/surge/src/common/dsp/FilterCoefficientMaker.cpp
  deps/surge/src/common/dsp/FMOscillator.cpp
  deps/surge/src/common/dsp/LfoModulationSource.cpp
  deps/surge/src/common/dsp/Oscillator.cpp
  deps/surge/src/common/dsp/QuadFilterChain.cpp
  deps/surge/src/common/dsp/QuadFilterUnit.cpp
  deps/surge/src/common/dsp/SampleAndHoldOscillator.cpp
  deps/surge/src/common/dsp/SurgeSuperOscillator.cpp
  deps/surge/src/common/dsp/SurgeVoice.cpp
  deps/surge/src/common/dsp/VectorizedSvfFilter.cpp
  deps/surge/src/common/dsp/Wavetable.cpp
  deps/surge/src/common/dsp/WavetableOscillator.cpp
  deps/surge/src/common/dsp/WindowOscillator.cpp
  
  deps/surge/src/common/thread/CriticalSection.cpp
  deps/surge/src/common/util/FpuState.cpp
  
  deps/surge/src/common/vt_dsp/basic_dsp.cpp
  deps/surge/src/common/vt_dsp/halfratefilter.cpp
  deps/surge/src/common/vt_dsp/lipol.cpp
  deps/surge/src/common/vt_dsp/macspecific.cpp
  
  deps/surge/src/common/Parameter.cpp
  deps/surge/src/common/precompiled.cpp
  deps/surge/src/common/SurgeError.cpp
  deps/surge/src/common/SurgePatch.cpp
  deps/surge/src/common/SurgeStorage.cpp
  deps/surge/src/common/SurgeSynthesizer.cpp
  deps/surge/src/common/SurgeSynthesizerIO.cpp
  deps/surge/src/common/UserDefaults.cpp
  deps/surge/src/common/WavSupport.cpp
  
  deps/surge/src/headless/UserInteractionsHeadless.cpp
            
  ${CMAKE_BINARY_DIR}/generated/version.cpp
)

target_compile_definitions(surge-fx PUBLIC
    JUCE_ALLOW_STATIC_NULL_VARIABLES=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1

    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0

    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0

    JUCE_ALSA=1
    JUCE_JACK=1

    TARGET_HEADLESS=1
)

target_link_libraries(surge-fx PRIVATE
    surge-fx-binary
    juce::juce_audio_utils
    juce::juce_audio_processors
)

include(CheckCXXSymbolExists)
CHECK_CXX_SYMBOL_EXISTS(std::filesystem::path::preferred_separator "filesystem" CXX_STD_FS)
if( CXX_STD_FS )
  set( FS_COMPILE_DEFINITIONS USE_STD_FILESYSTEM=1 )
  message( STATUS "Using std::filesystem" )
else()
  if( UNIX AND NOT APPLE )
    set (CMAKE_REQUIRED_FLAGS "-std=c++11")
    set (CMAKE_REQUIRED_LIBRARIES "stdc++fs")
  endif()
  CHECK_CXX_SYMBOL_EXISTS(std::experimental::filesystem::path::preferred_separator "experimental/filesystem" CXX_EXP_STD_FS)
  if( UNIX AND NOT APPLE )
    set (CMAKE_REQUIRED_FLAGS "")
    set (CMAKE_REQUIRED_LIBRARIES "")
  endif()

  if( CXX_EXP_STD_FS )
    set( FS_COMPILE_DEFINITIONS USE_STD_EXPERIMENTAL_FILESYSTEM=1 )
    message( STATUS "Using std::experimental::filesystem" )
  else()
    CHECK_CXX_SYMBOL_EXISTS(std::experimental::filesystem::path::preferred_separator "filesystem" CXX_EXP_STD_FS_FS_HDR)
    if( CXX_EXP_STD_FS_FS_HDR )
      set( FS_COMPILE_DEFINITIONS USE_STD_EXPERIMENTAL_FILESYSTEM_FROM_FILESYSTEM=1 )
      message( STATUS "Using std::experimental::filesystem but from <filesystem> header" )
    else()
      set( FS_COMPILE_DEFINITIONS USE_HOMEGROWN_FILESYSTEM=1 )
      message( STATUS "Using homegrown filesystem" )
    endif()
  endif()
endif()

target_compile_definitions(surge-fx PUBLIC ${FS_COMPILE_DEFINITIONS} )

if( APPLE )
  target_compile_definitions( surge-fx PUBLIC
    MAC=1
    )
  target_compile_options( surge-fx PUBLIC
    "-D_aligned_malloc(x,a)=malloc(x)"
    "-D_aligned_free(x)=free(x)"
    )
elseif( UNIX )
  target_compile_definitions( surge-fx PUBLIC
    LINUX=1
    )
  target_compile_options( surge-fx PUBLIC
    "-D_aligned_malloc(x,a)=malloc(x)"
    "-D_aligned_free(x)=free(x)"
    )
  target_include_directories( surge-fx PRIVATE deps/surge/src/linux )
  target_link_libraries( surge-fx PRIVATE pthread stdc++fs )
else()
  target_compile_definitions( surge-fx PUBLIC
    WINDOWS=1
    NOMINMAX=1
    WIN32
    _WIN32_WINNT=0x0601
    _USRDLL
    VA_SUBTRACTIVE_EXPORTS
    TIXML_USE_STL
    USE_LIBPNG
    _CRT_SECURE_NO_WARNINGS=1
    )
  target_compile_options( surge-fx PUBLIC
    /FIprecompiled.h
    /Zc:alignedNew
    /bigobj
    )
  target_include_directories( surge-fx PRIVATE deps/surge/src/windows )
endif()

# Build installers
get_target_property( OUTPUT_DIR surge-fx RUNTIME_OUTPUT_DIRECTORY )
add_custom_target( well-named-assets )
if( APPLE )
  add_custom_target( well-named-au )
  add_dependencies( well-named-au surge-fx_AU )
  add_dependencies( well-named-assets well-named-au )
endif()
add_custom_target( well-named-vst3 )
add_dependencies( well-named-vst3 surge-fx_VST3 )
add_dependencies( well-named-assets well-named-vst3 )

add_custom_target( installer-pkg )
add_dependencies( installer-pkg well-named-vst3 )
add_dependencies( installer-pkg surge-fx )
add_dependencies( installer-pkg surge-fx_Standalone )
add_dependencies( installer-pkg surge-fx_VST3 )
set( INSTALLER_DIR ${CMAKE_BINARY_DIR}/asset )
set( WELL_NAMED_ASSET_DIR ${CMAKE_BINARY_DIR}/product )
set( INSTALLER_BASENAME "SurgeEffectsBank" )

add_custom_command(
  TARGET well-named-vst3
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} -E make_directory  ${WELL_NAMED_ASSET_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${OUTPUT_DIR}/VST3 ${WELL_NAMED_ASSET_DIR}
  )

if( APPLE )
  add_custom_command(
    TARGET well-named-au
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${WELL_NAMED_ASSET_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${OUTPUT_DIR}/AU ${WELL_NAMED_ASSET_DIR}
  )
endif()

if( APPLE )
  add_custom_command(
    TARGET installer-pkg
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${INSTALLER_DIR}
    COMMAND hdiutil create ${INSTALLER_DIR}/${INSTALLER_BASENAME}-macos-${GIT_COMMIT_HASH}.dmg -ov -volname "${INSTALLER_BASENAME}-${GIT_COMMIT_HASH}" -fs HFS+ -srcfolder ${OUTPUT_DIR}
    )
elseif( UNIX )
  add_custom_command(
    TARGET installer-pkg
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${INSTALLER_DIR}
    COMMAND ls -l ${OUTPUT_DIR}
    COMMAND cd ${OUTPUT_DIR} && tar czf "${INSTALLER_DIR}/${INSTALLER_BASENAME}-linux-${GIT_COMMIT_HASH}.tgz" "Standalone/${INSTALLER_BASENAME}" "VST3/${INSTALLER_BASENAME}.vst3" 
    )
else()
  file( TO_NATIVE_PATH "${INSTALLER_DIR}/${INSTALLER_BASENAME}-win-${GIT_COMMIT_HASH}.zip" OUTPUT_ZIP )
  message( "Creating ${OUTPUT_ZIP}" )
  add_custom_command(
    TARGET installer-pkg
    POST_BUILD
    WORKING_DIRECTORY ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${INSTALLER_DIR}
    COMMAND powershell Compress-Archive 
             -DestinationPath "${OUTPUT_ZIP}"
             -Path "Standalone" "," "VST3" 
    )
endif()

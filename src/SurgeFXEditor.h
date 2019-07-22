/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SurgeFXProcessor.h"
#include "SurgeLookAndFeel.h"

//==============================================================================
/**
*/
class SurgefxAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SurgefxAudioProcessorEditor (SurgefxAudioProcessor&);
    ~SurgefxAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void paramsChangedCallback();
    void setEffectType(int i);

    enum RadioGroupIds {
        FxTypeGroup = 1776
    };
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SurgefxAudioProcessor& processor;

    Slider fxParamSliders[n_fx_params];
    Label  fxNameLabel[n_fx_params];
    Label  fxGroupLabel[n_fx_params];
    Label  fxValueLabel[n_fx_params];
    TextButton selectType[10]; // this had better match the list of fxnames in the constructor
    Slider fxTypeSlider;

    std::unique_ptr<SurgeLookAndFeel> surgeLookFeel;
    ScopedPointer<Drawable> surgeLogo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SurgefxAudioProcessorEditor)
};

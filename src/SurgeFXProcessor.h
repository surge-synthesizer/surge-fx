/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SurgeStorage.h"
#include "dsp/effect/Effect.h"

//==============================================================================
/**
*/
class SurgefxAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SurgefxAudioProcessor();
    ~SurgefxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;    
    
private:
    //==============================================================================
    AudioParameterInt   *fxTypeParam;
    AudioParameterFloat *fxParams[n_fx_params];

    // Members for the FX. If this looks a lot like surge-rack/SurgeFX.hpp that's not a coincidence
    std::unique_ptr<SurgeStorage> storage;
    std::unique_ptr<Effect> surge_effect;
    FxStorage *fxstorage;
    int storage_id_start, storage_id_end;

    int fx_param_remap[n_fx_params];
    std::string group_names[n_fx_params];

    double time = 0;
    
    void reorderSurgeParams();
    void copyGlobaldataSubset(int start, int end);
    void setupStorageRanges(Parameter *start, Parameter *endIncluding);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SurgefxAudioProcessor)
};

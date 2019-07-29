/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "SurgeFXProcessor.h"
#include "SurgeFXEditor.h"

//==============================================================================
SurgefxAudioProcessorEditor::SurgefxAudioProcessorEditor (SurgefxAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    surgeLookFeel.reset(new SurgeLookAndFeel() );
    setLookAndFeel(surgeLookFeel.get());
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 55 * 6 + 150);
    setResizable(false, false); // For now

    for( int i=0; i<n_fx_params; ++i )
    {
        fxParamSliders[i].setRange(0.0, 1.0, 0.001 );
        fxParamSliders[i].setValue(processor.getFXStorageValue01(i), NotificationType::dontSendNotification);
        fxParamSliders[i].setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        fxParamSliders[i].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0 );
        juce::Rectangle<int> position { ( i / 6 ) * getWidth()/2 + 5, ( i % 6 ) * 60 + 100, 55, 55 };
        fxParamSliders[i].setBounds(position);
        fxParamSliders[i].setChangeNotificationOnlyOnRelease(false);
        fxParamSliders[i].setEnabled(processor.getParamEnabled(i));
        fxParamSliders[i].onValueChange = [i, this]() {
            this->processor.setFXParamValue01(i, this->fxParamSliders[i].getValue() );
            fxParamDisplay[i].setDisplay(processor.getParamValueFromFloat(i, this->fxParamSliders[i].getValue()));
        };
        fxParamSliders[i].onDragStart = [i,this]() {
            this->processor.setUserEditingFXParam(i,true);
        };
        fxParamSliders[i].onDragEnd = [i,this]() {
            this->processor.setUserEditingFXParam(i,false);
        };
        addAndMakeVisible(&(fxParamSliders[i]));

        juce::Rectangle<int> tsPos {  ( i / 6 ) * getWidth() / 2 + 2 + 55,
                ( i % 6 ) * 60 + 100 + 12,
                13,
                55 -24 };
        fxTempoSync[i].setBounds(tsPos);
        fxTempoSync[i].setEnabled(processor.canTempoSync(i));
        fxTempoSync[i].setToggleState(processor.getFXStorageTempoSync(i), NotificationType::dontSendNotification);
        fxTempoSync[i].onClick = [i, this]() {
            this->processor.setUserEditingTemposync(i, true);
            this->processor.setFXParamTempoSync(i, this->fxTempoSync[i].getToggleState() );
            this->processor.setFXStorageTempoSync(i, this->fxTempoSync[i].getToggleState() );
            fxParamDisplay[i].setDisplay(processor.getParamValueFromFloat(i, this->fxParamSliders[i].getValue()));
            this->processor.setUserEditingTemposync(i, false);
        };

        addAndMakeVisible(&(fxTempoSync[i]));
        
        
        juce::Rectangle<int> dispPos { ( i / 6 ) * getWidth() / 2 + 4 + 55 + 15,
                ( i % 6 ) * 60 + 100,
                getWidth() / 2 - 68 - 15,
                55 };
        fxParamDisplay[i].setBounds(dispPos);
        fxParamDisplay[i].setGroup(processor.getParamGroup(i).c_str());
        fxParamDisplay[i].setName(processor.getParamName(i).c_str());
        fxParamDisplay[i].setDisplay(processor.getParamValue(i));
        fxParamDisplay[i].setEnabled(processor.getParamEnabled(i));
        
        addAndMakeVisible(fxParamDisplay[i]);
    }

    std::vector<std::string> fxnm = { "delay", "reverb", "phaser", "rotary", "dist", "eq", "freq", "cond", "chorus", "voco" };
    int en = processor.getEffectType() - 1;
    for( int i=0; i<10; ++i )
    {
        selectType[i].setButtonText(fxnm[i]);
        int bxsz = (getWidth()-20)/5;
        int bxmg = 10;
        int bysz = 40;
        int bymg = 10;
        juce::Rectangle<int> bpos { ( i % 5 ) * bxsz + bxmg, (i/5) * bysz + bymg, bxsz, bysz };
        selectType[i].setRadioGroupId(FxTypeGroup);
        selectType[i].setBounds(bpos);
        selectType[i].setClickingTogglesState(true);
        selectType[i].onClick = [this,i] { this->setEffectType(i+1); };
        if( i == en )
        {
            selectType[i].setToggleState(true,  NotificationType::dontSendNotification);
        }
        else
        {
            selectType[i].setToggleState(false,  NotificationType::dontSendNotification);
        }
        addAndMakeVisible(selectType[i]);
    }
    

    this->processor.setParameterChangeListener([this]() { this->paramsChangedCallback(); });
}

SurgefxAudioProcessorEditor::~SurgefxAudioProcessorEditor()
{
}

void SurgefxAudioProcessorEditor::resetLabels()
{
    for( int i=0; i<n_fx_params; ++i )
    {
        fxParamSliders[i].setValue(processor.getFXStorageValue01(i), NotificationType::dontSendNotification);
        fxParamDisplay[i].setDisplay(processor.getParamValue(i).c_str());
        fxParamDisplay[i].setGroup(processor.getParamGroup(i).c_str());
        fxParamDisplay[i].setName(processor.getParamName(i).c_str());
        fxParamDisplay[i].setEnabled(processor.getParamEnabled(i));
        fxParamSliders[i].setEnabled(processor.getParamEnabled(i));
        fxTempoSync[i].setEnabled(processor.canTempoSync(i));
        fxTempoSync[i].setToggleState(processor.getFXStorageTempoSync(i), NotificationType::dontSendNotification);
    }
}

void SurgefxAudioProcessorEditor::setEffectType(int i)
{
    processor.resetFxType(i);
    blastToggleState(i-1);
    resetLabels();
}

void SurgefxAudioProcessorEditor::paramsChangedCallback() {
    bool cv[n_fx_params+1];
    float fv[n_fx_params+1];
    processor.copyChangeValues(cv, fv);
    for( int i=0; i<n_fx_params+1; ++i )
        if( cv[i] )
        {
            if( i < n_fx_params )
            {
                fxParamSliders[i].setValue(fv[i], NotificationType::dontSendNotification);
                fxParamDisplay[i].setDisplay(processor.getParamValue(i));
            }
            else
            {
                // My type has changed - blow out the toggle states by hand
                blastToggleState(processor.getEffectType()-1);
                resetLabels();
            }
        }
}

void SurgefxAudioProcessorEditor::blastToggleState(int w)
{
    for( auto i=0; i<10; ++i )
    {
        selectType[i].setToggleState( i == w, NotificationType::dontSendNotification );
    }
}

//==============================================================================
void SurgefxAudioProcessorEditor::paint (Graphics& g)
{
    surgeLookFeel->paintComponentBackground(g, getWidth(), getHeight() );
}

void SurgefxAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

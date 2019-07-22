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
    setSize (600, 55 * 6 + 150);
    setResizable(false, false); // For now

    for( int i=0; i<n_fx_params; ++i )
    {
        fxParamSliders[i].setRange(0.0, 1.0, 0.005 );
        fxParamSliders[i].setValue(processor.getFXStorageValue01(i), NotificationType::dontSendNotification);
        fxParamSliders[i].setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        fxParamSliders[i].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0 );
        juce::Rectangle<int> position { ( i / 6 ) * getWidth()/2 + 10, ( i % 6 ) * 60 + 100, 55, 55 };
        fxParamSliders[i].setBounds(position);
        fxParamSliders[i].setChangeNotificationOnlyOnRelease(false);
        fxParamSliders[i].onValueChange = [i, this]() {
            this->processor.setFXParamValue01(i, this->fxParamSliders[i].getValue() );
            fxValueLabel[i].setText(processor.getParamValueFromFloat(i, this->fxParamSliders[i].getValue()),
                                    NotificationType::dontSendNotification);
        };
        fxParamSliders[i].onDragStart = [i,this]() {
            this->processor.setUserEditingFXParam(i,true);
        };
        fxParamSliders[i].onDragEnd = [i,this]() {
            this->processor.setUserEditingFXParam(i,false);
        };
        addAndMakeVisible(&(fxParamSliders[i]));

        juce::Rectangle<int> groupPos { ( i / 6 ) * getWidth()/2 + 10 + 60, ( i % 6 ) * 60 + 100, getWidth()/2 - 75, 12 };
        juce::Rectangle<int> namePos { ( i / 6 ) * getWidth()/2 + 10 + 60, ( i % 6 ) * 60 + 100 + 12 , getWidth()/2 - 75, 18 };
        juce::Rectangle<int> valuePos { ( i / 6 ) * getWidth()/2 + 10 + 60, ( i % 6 ) * 60 + 100 + 30, getWidth()/2 - 75, 25 };

        fxGroupLabel[i].setText(processor.getParamGroup(i).c_str(), NotificationType::dontSendNotification);
        fxGroupLabel[i].setFont(Font(10));
        fxGroupLabel[i].setJustificationType(Justification::left | Justification::top );
        fxGroupLabel[i].setBounds(groupPos);
        addAndMakeVisible(fxGroupLabel[i]);
        
        fxNameLabel[i].setText(processor.getParamName(i).c_str(), NotificationType::dontSendNotification);
        fxNameLabel[i].setFont(Font(12));
        fxNameLabel[i].setJustificationType(Justification::left | Justification::top );
        fxNameLabel[i].setBounds(namePos);
        addAndMakeVisible(fxNameLabel[i]);

        fxValueLabel[i].setText(processor.getParamValue(i).c_str(), NotificationType::dontSendNotification);
        fxValueLabel[i].setFont(Font(16));
        fxValueLabel[i].setJustificationType(Justification::left | Justification::top );
        fxValueLabel[i].setBounds(valuePos);
        addAndMakeVisible(fxValueLabel[i]);
    }

    std::vector<std::string> fxnm = { "delay", "reverb", "phaser", "rotary", "dist", "eq", "freq", "cond", "chorus", "voco" };
    for( int i=0; i<10; ++i )
    {
        selectType[i].setButtonText(fxnm[i]);
        int bxsz = (getWidth()-20)/5;
        int bxmg = 10;
        int bysz = 40;
        int bymg = 10;
        juce::Rectangle<int> bpos { ( i % 5 ) * bxsz + bxmg, (i/5) * bysz + bymg, bxsz, bysz };
        //selectType[i].setRadioGroupId(FxTypeGroup);
        //selectType[i].setClickingTogglesState(true);
        selectType[i].setBounds(bpos);
        selectType[i].onClick = [this,i] { this->setEffectType(i+1); };
        addAndMakeVisible(selectType[i]);
    }
    

    this->processor.setParameterChangeListener([this]() { this->paramsChangedCallback(); });
}

SurgefxAudioProcessorEditor::~SurgefxAudioProcessorEditor()
{
}

void SurgefxAudioProcessorEditor::setEffectType(int i)
{
    processor.resetFxType(i);
    for( int i=0; i<n_fx_params; ++i )
    {
        fxGroupLabel[i].setText(processor.getParamGroup(i).c_str(), NotificationType::dontSendNotification);
        fxNameLabel[i].setText(processor.getParamName(i).c_str(), NotificationType::dontSendNotification);
        fxParamSliders[i].setEnabled(processor.getParamEnabled(i));
    }
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
                fxValueLabel[i].setText(processor.getParamValue(i), NotificationType::dontSendNotification);
            }
            else
            {
                // My type has changed
                for( int i=0; i<n_fx_params; ++i )
                {
                    fxParamSliders[i].setValue(processor.getFXStorageValue01(i), NotificationType::dontSendNotification);
                    fxGroupLabel[i].setText(processor.getParamGroup(i).c_str(), NotificationType::dontSendNotification);
                    fxNameLabel[i].setText(processor.getParamName(i).c_str(), NotificationType::dontSendNotification);
                    fxParamSliders[i].setEnabled(processor.getParamEnabled(i));
                }
            }
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

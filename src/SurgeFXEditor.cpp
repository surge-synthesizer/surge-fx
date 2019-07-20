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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    setResizable(true, false);

    for( int i=0; i<n_fx_params; ++i )
    {
        fxParamSliders[i].setRange(0.0, 1.0, 0.005 );
        fxParamSliders[i].setValue(processor.getFXParamValue01(i));
        fxParamSliders[i].setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        fxParamSliders[i].setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0 );
        Rectangle<int> position { ( i % 5 ) * 60, ( i / 5 ) * 60, 55, 55 };
        fxParamSliders[i].setBounds(position);
        fxParamSliders[i].setChangeNotificationOnlyOnRelease(false);
        fxParamSliders[i].onValueChange = [i, this]() {
            this->processor.setFXParamValue01(i, this->fxParamSliders[i].getValue() );
        };
        addAndMakeVisible(&(fxParamSliders[i]));
    }
}

SurgefxAudioProcessorEditor::~SurgefxAudioProcessorEditor()
{
}

//==============================================================================
void SurgefxAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SurgefxAudioProcessorEditor::resized()
{
    std::cout << "Resized" << std::endl;
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

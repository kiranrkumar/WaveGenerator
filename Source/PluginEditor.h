/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define WAVEBUTTONGROUP 1
#define MODBUTTONGROUP 2

//==============================================================================
/**
*/

//KRK - define an enum for wave type
enum WaveType
{
    SINE,
    TRIANGLE,
    SQUARE,
    SAWTOOTH
};

//KRK [1] - add the Slider listener
class WaveGeneratorAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener, private Button::Listener
{
public:
    WaveGeneratorAudioProcessorEditor (WaveGeneratorAudioProcessor&);
    ~WaveGeneratorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void initButton(Button* btn, String btnName, int buttonGroup);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WaveGeneratorAudioProcessor& processor;
    
    //KRK [2] - add some sliders
    Slider fundFreq, waveType, gain;
    //KRK - add four buttons, one for each wave type
    ToggleButton sineButton, triButton, sqButton, sawButton;
    //KRK - also add option to perform ring and amplitude modulation
    ToggleButton RMbutton, AMbutton, noModButton;
    
    //KRK [3] - add the slider callback function. Should be able to use the same one for all and have the function recognize which slider it is
    void sliderValueChanged(Slider* slider) override;
    
    //KRK - add the button listener, too
    void buttonClicked(Button* button) override;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED

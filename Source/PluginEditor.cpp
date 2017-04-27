/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WaveGeneratorAudioProcessorEditor::WaveGeneratorAudioProcessorEditor (WaveGeneratorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (430, 260);
    
    
    //KRK [4] - add our frequency slider (rotary knob)
    
    ////fundFreq
    fundFreq.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fundFreq.setTextBoxStyle(Slider::TextBoxRight, false, gain.getTextBoxWidth(), gain.getTextBoxHeight());
    fundFreq.setColour(Slider::backgroundColourId, Colours::antiquewhite);
    fundFreq.setColour(Slider::trackColourId, Colours::antiquewhite);
    fundFreq.setColour(Slider::rotarySliderFillColourId, Colours::deepskyblue);
    fundFreq.setColour(Slider::thumbColourId, Colours::deepskyblue);
    fundFreq.setColour(Slider::rotarySliderOutlineColourId, Colours::deepskyblue);
    fundFreq.setRange(1.0, 500.0, 1.0);
    fundFreq.setPopupDisplayEnabled(false, this);
    fundFreq.setTextValueSuffix(" Hz");
    fundFreq.setValue(220.0);
    
    //gain slider
    gain.setSliderStyle(Slider::LinearHorizontal);
    gain.setTextBoxStyle(Slider::TextBoxBelow, false, gain.getTextBoxWidth(), gain.getTextBoxHeight());
    gain.setRange(-60.0, 0.0, 0.1);
    gain.setPopupDisplayEnabled(false, this);
    gain.setTextValueSuffix(" dB");
    gain.setValue(0.0);
    
    //KRK [5] - make the sliders visible
    addAndMakeVisible(&fundFreq);
    addAndMakeVisible(&gain);
    
    //KRK [6] - make this component listen to both sliders
    fundFreq.addListener(this);
    gain.addListener(this);
    
    //KRK - initialize the four toggle buttons with relevant data
    initButton(&sineButton, "Sine", WAVEBUTTONGROUP);
    initButton(&triButton, "Triangle", WAVEBUTTONGROUP);
    initButton(&sqButton, "Square", WAVEBUTTONGROUP);
    initButton(&sawButton, "Sawtooth", WAVEBUTTONGROUP);
    
    //KRK - trigger the sine button to start off with SINE as the default value
    sineButton.triggerClick();
    
    
    //KRK - initialize modulation buttons
    initButton(&RMbutton, "Ring", MODBUTTONGROUP);
    initButton(&AMbutton, "Amplitude", MODBUTTONGROUP);
    initButton(&noModButton, "None", MODBUTTONGROUP);
    
    //KRK - start off with no modulation
    noModButton.triggerClick();
}


WaveGeneratorAudioProcessorEditor::~WaveGeneratorAudioProcessorEditor()
{
}

//==============================================================================
void WaveGeneratorAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (15.0f);
    
    g.drawFittedText("Fundamental Frequency (Hz)", 20, 25, getWidth(), 40, Justification::left, 2);
    g.drawFittedText("Waveform Type", 280, 25, getWidth(), 40, Justification::left, 1);
    g.drawFittedText("Volume", 20, 135, getWidth(), 40, Justification::left, 1);
    g.drawFittedText("Input Modulation", 280, 140, getWidth(), 40, Justification::left, 1);
}

void WaveGeneratorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //KRK - set position and size of sliders and buttons relative to the window
    fundFreq.setBounds(35, 20, 125, 125);
    gain.setBounds(20, 160, 200, 50);
    
    sineButton.setBounds(280, 55, 100, 25);
    triButton.setBounds(280, 75, 100, 25);
    sqButton.setBounds(280, 95, 100, 25);
    sawButton.setBounds(280, 115, 100, 25);
    
    RMbutton.setBounds(280, 170, 100, 25);
    AMbutton.setBounds(280, 190, 100, 25);
    noModButton.setBounds(280, 210, 100, 25);
    
}

void WaveGeneratorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &fundFreq)
    {
        processor.curFreq = fundFreq.getValue();
    }
    else if (slider == &waveType)
    {
        processor.curWavetype = waveType.getValue();
    }
    else if (slider == &gain)
    {
        processor.curGain = gain.getValue();
    }
    
}

void WaveGeneratorAudioProcessorEditor::buttonClicked(Button* button)
{
    
    //Waveforms
    if (button->getName() == "Sine")
    {
        processor.curWavetype = SINE;
    }
    else if (button->getName() == "Triangle")
    {
        processor.curWavetype = TRIANGLE;
    }
    else if (button->getName() == "Square")
    {
        processor.curWavetype = SQUARE;
    }
    else if (button->getName() == "Sawtooth")
    {
        processor.curWavetype = SAWTOOTH;
    }
    //Modulation types
    else if (button->getName() == "Ring")
    {
        processor.isRMon = true;
        processor.isAMon = false;
    }
    else if (button->getName() == "Amplitude")
    {
        processor.isAMon = true;
        processor.isRMon = false;
    }
    else if (button->getName() == "None")
    {
        processor.isAMon = false;
        processor.isRMon = false;
    }
}

void WaveGeneratorAudioProcessorEditor::initButton(Button* btn, String btnName, int buttonGroup)
{
    addAndMakeVisible(btn);
    btn->addListener(this);
    btn->setButtonText(btnName);
    btn->setName(btnName);
    btn->setColour(ToggleButton::textColourId, Colours::white);
    btn->setColour(ToggleButton::tickColourId, Colours::deepskyblue);
    btn->setRadioGroupId(buttonGroup);
    
    
}

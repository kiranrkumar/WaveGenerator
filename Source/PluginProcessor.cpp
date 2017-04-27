/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WaveGeneratorAudioProcessor::WaveGeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

WaveGeneratorAudioProcessor::~WaveGeneratorAudioProcessor()
{
}

//==============================================================================
const String WaveGeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveGeneratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveGeneratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double WaveGeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveGeneratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveGeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveGeneratorAudioProcessor::setCurrentProgram (int index)
{
}

const String WaveGeneratorAudioProcessor::getProgramName (int index)
{
    return String();
}

void WaveGeneratorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WaveGeneratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->samplingRate = sampleRate;
}

void WaveGeneratorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveGeneratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaveGeneratorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    static float phase = 0, sampleVal = 0; //KRK - keep track of phase and sample values, even between function calls
    static bool triInc = true; // increase or decrease for triangle wave
    
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    float amp = 0.89;
    static float linGain = 1; //default

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...

        // ..do something to the data...
        
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            //KRK - update variables
            float period = samplingRate / curFreq;
            phase =  std::fmod(phase + 2*PI * curFreq/samplingRate, 2*PI);
            
            //KRK - generate samples of the wave type
            switch (curWavetype)
            {
                case SINE:
                    sampleVal = sinf(phase);
                    break;
                
                case TRIANGLE: //triangle
                    if (triInc) //increasing
                    {
                        sampleVal = sampleVal + 4.0/period;
                        
                        //reverse direction if we go too high
                        if (sampleVal >= 1.0)
                        {
                            sampleVal = (2.0 - sampleVal); //smoothly account for the difference going the other direction
                            triInc = !triInc;
                        }
                    }
                    else //decreasing
                    {
                        sampleVal = sampleVal - 4.0/period;
                        
                        //reverse direction if we go too low
                        if (sampleVal <= -1.0)
                        {
                            sampleVal = (-2.0 - sampleVal);
                            triInc = !triInc;
                        }
                        
                    }
                    
                    break;
            
                case SQUARE:
                    sampleVal = (phase < PI) ? 1.0 : -1.0;
                    break;
                    
                case SAWTOOTH:
                    sampleVal = sampleVal + 2.0/period;
                    if (sampleVal >= 1.0)
                    {
                        sampleVal = -2.0 + sampleVal;
                    }
                    break;
                    
                default:
                    break;
            }
            
            
            //KRK - Set the data for all output channels
            for (int channel = 0; channel < totalNumOutputChannels; channel++)
            {
                linGain = powf(10, curGain/20); //calculate gain
                if (isRMon)
                {
                    buffer.setSample(channel, n, buffer.getSample(channel, n) * linGain * amp * sampleVal);
                }
                else if (isAMon)
                {
                    buffer.setSample(channel, n, buffer.getSample(channel, n) * linGain * amp * (sampleVal + 1.0)/2.0 );
                }
                else
                {
                    buffer.setSample(channel, n, linGain * amp * sampleVal); //store sample
                }
            }
            
        }
}

//==============================================================================
bool WaveGeneratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaveGeneratorAudioProcessor::createEditor()
{
    return new WaveGeneratorAudioProcessorEditor (*this);
}

//==============================================================================
void WaveGeneratorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveGeneratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveGeneratorAudioProcessor();
}

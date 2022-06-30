/*
  ==============================================================================

    fmsynth project
    julianksdj

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FmsynthAudioProcessor::FmsynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FmsynthAudioProcessor::~FmsynthAudioProcessor()
{
}

//==============================================================================
const juce::String FmsynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FmsynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FmsynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FmsynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FmsynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FmsynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FmsynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FmsynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FmsynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FmsynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FmsynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    op1attack = 0.01f;
    op1decay = 0.001f;
    op1sustain = 1.f;
    op1release = 0.09f;
    op2attack = 0.01f;
    op2decay = 0.001f;
    op2sustain = 1.f;
    op2release = 0.09f;
    op1amp = 10.f;
    op2amp = 0.f;
    op1coarse = 1.f;
    op2coarse = 1.f;
    op1fine = 0.f;
    op2fine = 0.f;
    algorithm = 1.f;
}

void FmsynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FmsynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FmsynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // MIDI HANDLING -----------------------------------------------------------------------------------------
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePos;
    
    while (it.getNextEvent(currentMessage, samplePos))
    {
        if (currentMessage.isNoteOn())
        {
            addVoice(juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber()));
        }
        else if (currentMessage.isNoteOff())
        {
            deactivateVoice(juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber()));
        }
    }
    // -------------------------------------------------------------------------------------------------------
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float sumOsc = 0.f;
            // sum of oscillators
            for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
            {
                auto* voice = voices.getUnchecked(voiceIndex);
                if(voice->isActive())
                {
                    auto currentSample = voice->getNextSample(channel);
                    sumOsc += currentSample;
                }
                else
                    voices.remove(voiceIndex);
            }
            channelData[sample] += sumOsc;
        }
    }
}

//==============================================================================
bool FmsynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FmsynthAudioProcessor::createEditor()
{
    return new FmsynthAudioProcessorEditor (*this);
}

//==============================================================================
void FmsynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FmsynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FmsynthAudioProcessor();
}


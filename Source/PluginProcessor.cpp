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
    setFrequency();
    setFrequencyFM();
    
    // midi handling prep
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    // find the first enabled device and use that by default
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled (input.identifier))
        {
            setMidiInput (midiInputs.indexOf (input));
            break;
        }
    }
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
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //auto currentSample = (float) std::sin(currentAngle[channel] + modAmp * std::cos(currentAngleFM[channel]));
            setFrequency();
            setFrequencyFM();
            auto currentSample = getNextSample(channel);
            
            //currentAngle[channel] += angleDelta;
            //currentAngleFM[channel] += angleDeltaFM;
            
            if(noteOn==true) //pressed note on the keyboard
            {
                if (envCount < attackSamples)
                {
                    carrAmp+=(0.125/attackSamples); //the amplitude raises until max amplitude is reached
                }
                else if (envCount < daSamples)
                {
                    carrAmp-=((0.125-sustainLevel)/decaySamples); //the amplitude decreases until sustain level is reached
                }
                releaseLevel = carrAmp; // saves the amplitude state
            }
            else //released note on the keyboard
            {
                if(carrAmp>0.0)
                    carrAmp-=(releaseLevel/releaseSamples); //amplitude decreases until silence
            }
            
            channelData[sample]  = currentSample * carrAmp;
            envCount++;
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

void FmsynthAudioProcessor::setFrequency()
{
    auto cyclesPerSample = carrFreq /currentSampleRate;
    angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
}
void FmsynthAudioProcessor::setFrequencyFM()
{
    auto cyclesPerSample = modFreq / currentSampleRate;
    angleDeltaFM = cyclesPerSample * juce::MathConstants<double>::twoPi;
}

float FmsynthAudioProcessor::getNextSample(int channel)
{
    float sample = std::sin(currentAngle[channel] + modAmp * std::cos(currentAngleFM[channel]));
    currentAngle[channel] += angleDelta;
    currentAngleFM[channel] += angleDeltaFM;
    if (currentAngle[channel]>=juce::MathConstants<double>::twoPi)
        currentAngle[channel] -= juce::MathConstants<double>::twoPi;
    if (currentAngleFM[channel]>=juce::MathConstants<double>::twoPi)
        currentAngleFM[channel] -= juce::MathConstants<double>::twoPi;
    return sample;
}

void FmsynthAudioProcessor::updateAttack()
{
    attackSamples = currentSampleRate * attackTime;
    printf("Attack time: %f\n", attackTime);
    printf("Attack samples: %d\n", attackSamples);
}
void FmsynthAudioProcessor::updateDecay()
{
    decaySamples = currentSampleRate * decayTime;
    printf("decay time %f\n", decayTime);
    daSamples = decaySamples + attackSamples;
    printf("decay Samples %d\n", decaySamples);
    printf("da Samples %d\n", daSamples);
}
void FmsynthAudioProcessor::updateRelease()
{
    releaseSamples = currentSampleRate * releaseTime;
    printf("release time %f\n", releaseTime);
}


void FmsynthAudioProcessor::setMidiInput (int index)
{
    auto list = juce::MidiInput::getAvailableDevices();

    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);

    auto newInput = list[index];

    if (! deviceManager.isMidiInputDeviceEnabled (newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback (newInput.identifier, this);
    midiInputList.setSelectedId (index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}

void FmsynthAudioProcessor::handleIncomingMidiMessage (juce::MidiInput *source, const juce::MidiMessage &message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
    //keyboardState.processNextMidiEvent (message);
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class FmsynthAudioProcessor  : public juce::AudioProcessor ,
                               private juce::MidiInputCallback
{
public:
    //==============================================================================
    FmsynthAudioProcessor();
    ~FmsynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    double carrAmp = 0.0;
    double carrFreq = 50;
    double modFreq = 100;
    double modAmp = 0.0000001;
    double modAmpSmoothed = 0.0;
    double attackTime = 0.0001;
    double decayTime = 0.0001;
    double sustainLevel = 0.125;
    double releaseTime = 0.0001;
    int attackSamples = 0;
    int decaySamples = 0;
    int daSamples = 0;
    int releaseSamples = 0;
    int envCount = 100;
    bool noteOn = false;
    double releaseLevel = 0.0;
    
    void updateAngleDelta();
    void updateAngleFM();
    void updateAttack();
    void updateDecay();
    void updateRelease();
    
    void setMidiInput (int index);
    void handleIncomingMidiMessage (juce::MidiInput *source, const juce::MidiMessage &message) override;
    

private:
    double currentSampleRate = 0.0, angleDelta = 0.0, angleDeltaFM = 0.0;
    double currentAngle[2] = {0.0, 0.0};
    double currentAngleFM[2] = {0.0, 0.0};

    juce::AudioDeviceManager deviceManager;
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmsynthAudioProcessor)
};

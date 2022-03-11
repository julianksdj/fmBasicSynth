/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"

//==============================================================================
/**
*/

class FmsynthAudioProcessor  : public juce::AudioProcessor
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
    
    int getVoicesSize()
    {
        return voices.size();
    };
    void addVoice(float frequency)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            if (voices.getUnchecked(voiceIndex)->getNoteFreq()==frequency)
            {
                voices.remove(voiceIndex);
            }
        }
        Voice *voice = new Voice();
        voice->setNoteOn(true);
        voice->setSampleRate(currentSampleRate);
        voice->setNoteFreq(frequency);
        if (algorithm == 1)
        {
            voice->setFrequency((op1coarse + op1fine) * frequency );
            voice->setFrequencyFM((op2coarse + op2fine) * frequency);
            voice->initFMenv(op1attack, op1decay, op1sustain, op1release, op2attack, op2decay, op2sustain, op2release);
            voice->setCarAmp(op1amp);
            voice->setModAmp(op2amp);
        }
        else if (algorithm == 2)
        {
            voice->setFrequency(op2coarse * frequency + op2fine);
            voice->setFrequencyFM(op1coarse * frequency + op1fine);
            voice->initFMenv(op2attack, op2decay, op2sustain, op2release, op1attack, op1decay, op1sustain, op1release);
            voice->setCarAmp(op2amp);
            voice->setModAmp(op1amp);
        }
        else if (algorithm == 3)
        {
            voice->setFrequency(op1coarse * frequency + op1fine);
            voice->setFrequency2(op2coarse * frequency + op2fine);
            voice->setFrequencyFM(0.f);
            voice->initFMenv(op1attack, op1decay, op1sustain, op1release, 0.f, 0.f, 0.f, 0.f);
            voice->setCarAmp(op1amp);
            voice->setCarAmp2(op2amp);
        }
        voices.add(voice);
    };
    void deactivateVoice(float freq)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            if (voices[voiceIndex]->getNoteFreq() == freq)
            {
                voices[voiceIndex]->setNoteOn(false);
                voices[voiceIndex]->resetEnvCount();
            }
        }
    };
    void removeVoice(float freq)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            if (voices[voiceIndex]->getNoteFreq() == freq)
                voices.remove(voiceIndex);
        }
    };
    void setOp1attack(float a)
    {
        op1attack = a;
    };
    void setOp1decay(float d)
    {
        op1decay = d;
    };
    void setOp1sustain(float s)
    {
        op1sustain = s;
    };
    void setOp1release(float r)
    {
        op1release = r;
    };
    void setOp2attack(float a)
    {
        op2attack = a;
    };
    void setOp2decay(float d)
    {
        op2decay = d;
    };
    void setOp2sustain(float s)
    {
        op2sustain = s;
    };
    void setOp2release(float r)
    {
        op2release = r;
    };
    float getOp1attack()
    {
        return op1attack;
    };
    float getOp1decay()
    {
        return op1decay;
    };
    float getOp1sustain()
    {
        return op1sustain;
    };
    float getOp1release()
    {
        return op1release;
    };
    float getOp2attack()
    {
        return op2attack;
    };
    float getOp2decay()
    {
        return op2decay;
    };
    float getOp2sustain()
    {
        return op2sustain;
    };
    float getOp2release()
    {
        return op2release;
    };
    float getOp1Amp()
    {
        return op1amp;
    };
    float getOp2Amp()
    {
        return op2amp;
    };
    
    void setOp1Amp(float amp)
    {
        op1amp = amp;
    };
    void setOp2Amp(float amp)
    {
        op2amp = amp;
    };
    
    void setOp1Coarse(float c)
    {
        op1coarse = c;
    };
    void setOp2Coarse(float c)
    {
        op2coarse = c;
    };
    float getOp1Coarse()
    {
        return op1coarse;
    };
    float getOp2Coarse()
    {
        return op2coarse;
    };
    void setOp1Fine(float f)
    {
        op1fine = f;
    };
    void setOp2Fine(float f)
    {
        op2fine = f;
    };
    float getOp1Fine()
    {
        return op1fine;
    };
    float getOp2Fine()
    {
        return op2fine;
    };
    int getAlgorithm()
    {
        return algorithm;
    };
    void setAlgorithm(int algo)
    {
        algorithm = algo;
    };
    
private:
    float currentSampleRate = 0.0;
    juce::OwnedArray<Voice> voices; //voices array
    
    // GUI parameters
    float op1attack, op1decay, op1sustain, op1release;
    float op2attack, op2decay, op2sustain, op2release;
    float op1amp, op2amp;
    float op1coarse, op2coarse;
    float op1fine, op2fine;
    int algorithm;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmsynthAudioProcessor)
};

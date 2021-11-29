/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FmsynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener,
                                     private juce::MidiKeyboardStateListener
{
public:
    FmsynthAudioProcessorEditor (FmsynthAudioProcessor&);
    ~FmsynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void sliderValueChanged (juce::Slider *slider) override;
    void handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    
    void setMidiInput (int index);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FmsynthAudioProcessor& audioProcessor;

    //==============================================================================
    juce::Slider modulatorFreqSlider;
    juce::Label  modulatorFreqLabel;
    juce::Slider modulatorAmpSlider;
    juce::Label  modulatorAmpLabel;
    
    //carrier Envelope GUI
    juce::Slider carrierASlider;
    juce::Label  carrierALabel;
    juce::Slider carrierDSlider;
    juce::Label  carrierDLabel;
    juce::Slider carrierSSlider;
    juce::Label  carrierSLabel;
    juce::Slider carrierRSlider;
    juce::Label  carrierRLabel;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    
    juce::AudioDeviceManager deviceManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmsynthAudioProcessorEditor)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

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
    
    void updateToggleState (juce::Button* button, juce::String name);
    
    /*void algoMenuChanged()
    {
        switch (algoMenu.getSelectedId())
        {
            case 1:
                audioProcessor.setAlgorithm(1);
                break;
            case 2:
                audioProcessor.setAlgorithm(2);
                break;
            default: break;
        }
    }*/

private:
    CustomLookAndFeel customLook;
    FmsynthAudioProcessor& audioProcessor;

    juce::Slider op1AmpKnob;
    juce::Slider op1CoarseKnob;
    juce::Slider op1FineKnob;
    juce::Slider op2AmpKnob;
    juce::Slider op2CoarseKnob;
    juce::Slider op2FineKnob;
    
    juce::Slider op1ASlider;
    juce::Slider op1DSlider;
    juce::Slider op1SSlider;
    juce::Slider op1RSlider;
    
    juce::Slider op2ASlider;
    juce::Slider op2DSlider;
    juce::Slider op2SSlider;
    juce::Slider op2RSlider;
    
    juce::Label ampLabel;
    juce::Label coarseLabel;
    juce::Label fineLabel;
    juce::Label aLabel, dLabel, sLabel, rLabel;
    juce::Label op1Label;
    juce::Label op2Label;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    
    juce::AudioDeviceManager deviceManager;
    
    //Algorithm selection
    juce::Label algoLabel;
    juce::ToggleButton algoButton1;
    juce::ToggleButton algoButton2;
    juce::ToggleButton algoButton3;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmsynthAudioProcessorEditor)
};

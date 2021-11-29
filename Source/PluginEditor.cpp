/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SineOscillator.h"

//==============================================================================
FmsynthAudioProcessorEditor::FmsynthAudioProcessorEditor (FmsynthAudioProcessor& p)
    : keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 300);
    
    //modulator frequency
    addAndMakeVisible (modulatorFreqSlider);
    modulatorFreqSlider.setRange (50, 5000.0);
    modulatorFreqSlider.setSkewFactorFromMidPoint (500.0);
    modulatorFreqSlider.setTextValueSuffix (" Hz");
    modulatorFreqSlider.addListener (this);
    modulatorFreqSlider.setValue(audioProcessor.getModFreq());
    addAndMakeVisible (modulatorFreqLabel);
    modulatorFreqLabel.setText ("Modulator freq", juce::dontSendNotification);
    modulatorFreqLabel.attachToComponent (&modulatorFreqSlider, true);
    
    //modulator amplitude
    addAndMakeVisible (modulatorAmpSlider);
    modulatorAmpSlider.setRange (0.0000001, 10);
    modulatorAmpSlider.setSkewFactorFromMidPoint (5);
    modulatorAmpSlider.addListener (this);
    modulatorAmpSlider.setValue(audioProcessor.getModAmp());
    addAndMakeVisible (modulatorAmpLabel);
    modulatorAmpLabel.setText ("Modulator amp", juce::dontSendNotification);
    modulatorAmpLabel.attachToComponent (&modulatorAmpSlider, true);
    
    //keyboard
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);
    
    //attack slider
    carrierASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierASlider);
    // min  attack time must be 0.0001 (100us) to avoid attackSamples from being 0
    // so the division Aa = 0.125 / attackSamples is never inf
    carrierASlider.setRange (0.0001, 10.0);
    carrierASlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getAttack());
    addAndMakeVisible (carrierALabel);
    carrierALabel.setText ("cA", juce::dontSendNotification);
    carrierALabel.attachToComponent (&carrierASlider, true);

    //decay slider
    carrierDSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierDSlider);
    carrierDSlider.setRange (0.0001, 10.0);
    carrierDSlider.addListener (this);
    carrierDSlider.setValue(audioProcessor.getDecay());
    addAndMakeVisible (carrierDLabel);
    carrierDLabel.setText ("cD", juce::dontSendNotification);
    carrierDLabel.attachToComponent (&carrierDSlider, true);
    
    //sustain slider
    carrierSSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierSSlider);
    carrierSSlider.setRange (0, 0.125);
    carrierSSlider.addListener (this);
    carrierSSlider.setValue(audioProcessor.getSustain());
    addAndMakeVisible (carrierSLabel);
    carrierSLabel.setText ("cS", juce::dontSendNotification);
    carrierSLabel.attachToComponent (&carrierSSlider, true);
    
    //release slider
    carrierRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierRSlider);
    carrierRSlider.setRange (0.0001, 10.0);
    carrierRSlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getRelease());
    addAndMakeVisible (carrierRLabel);
    carrierRLabel.setText ("cR", juce::dontSendNotification);
    carrierRLabel.attachToComponent (&carrierRSlider, true);
    
}

FmsynthAudioProcessorEditor::~FmsynthAudioProcessorEditor()
{
}

//==============================================================================
void FmsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void FmsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto sliderLeft = 120;
    modulatorFreqSlider.setBounds (sliderLeft, 40, getWidth() - sliderLeft - 10, 20);
    modulatorAmpSlider.setBounds (sliderLeft, 60, getWidth() - sliderLeft - 10, 20);
    keyboardComponent.setBounds(0, 100, getWidth(), 60);
    
    carrierASlider.setBounds(50, 180, 20, 100);
    carrierDSlider.setBounds(90, 180, 20, 100);
    carrierSSlider.setBounds(130, 180, 20, 100);
    carrierRSlider.setBounds(170, 180, 20, 100);
    modulatorFreqSlider.setValue(50);
    modulatorAmpSlider.setValue(0.0000001);
    carrierASlider.setValue(0.0001);
    carrierDSlider.setValue(0.0001);
    carrierSSlider.setValue(0.125);
    carrierRSlider.setValue(0.0001);
    
}

void FmsynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &modulatorFreqSlider)
    {
        audioProcessor.setModFreq(modulatorFreqSlider.getValue()) ;
    }
    if(slider == &modulatorAmpSlider)
    {
        audioProcessor.setModAmp(modulatorAmpSlider.getValue()) ;
    }
    if(slider == &carrierASlider)
    {
        audioProcessor.setAttack(carrierASlider.getValue());
    }
    if(slider == &carrierDSlider)
    {
        audioProcessor.setDecay(carrierDSlider.getValue());
    }
    if(slider == &carrierSSlider)
    {
        audioProcessor.setSustain(carrierSSlider.getValue());
    }
    if(slider == &carrierRSlider)
    {
        audioProcessor.setRelease(carrierRSlider.getValue());
    }
}

void FmsynthAudioProcessorEditor::handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    printf("\nNOTE PRESSED\n");
    printf("Received note %d\n",midiNoteNumber);
    // frequency calculation from midi note data
    double nota = 0.0;
    nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    printf("Calculated frequency %f\n",nota);
    audioProcessor.addVoice(nota);
}
void FmsynthAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    auto nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    printf("NOTE RELEASED\n");
    audioProcessor.deactivateVoice(nota);
}


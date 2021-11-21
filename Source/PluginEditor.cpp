/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FmsynthAudioProcessorEditor::FmsynthAudioProcessorEditor (FmsynthAudioProcessor& p)
    : keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 300);
    
    //carrier frequency
    addAndMakeVisible (carrierFreqSlider);
    carrierFreqSlider.setRange (50, 5000.0);
    carrierFreqSlider.setSkewFactorFromMidPoint (500.0);
    carrierFreqSlider.setTextValueSuffix (" Hz");
    carrierFreqSlider.addListener (this);
    addAndMakeVisible (carrierFreqLabel);
    carrierFreqLabel.setText ("Carrier freq", juce::dontSendNotification);
    carrierFreqLabel.attachToComponent (&carrierFreqSlider, true);
    
    //modulator frequency
    addAndMakeVisible (modulatorFreqSlider);
    modulatorFreqSlider.setRange (50, 5000.0);
    modulatorFreqSlider.setSkewFactorFromMidPoint (500.0);
    modulatorFreqSlider.setTextValueSuffix (" Hz");
    modulatorFreqSlider.addListener (this);
    addAndMakeVisible (modulatorFreqLabel);
    modulatorFreqLabel.setText ("Modulator freq", juce::dontSendNotification);
    modulatorFreqLabel.attachToComponent (&modulatorFreqSlider, true);
    
    //modulator amplitude
    addAndMakeVisible (modulatorAmpSlider);
    modulatorAmpSlider.setRange (0.0000001, 10);
    modulatorAmpSlider.setSkewFactorFromMidPoint (5);
    //modulatorAmpSlider.setTextValueSuffix (" Amp");
    modulatorAmpSlider.addListener (this);
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
    //carrierASlider.setSkewFactorFromMidPoint (5);
    carrierASlider.addListener (this);
    addAndMakeVisible (carrierALabel);
    carrierALabel.setText ("cA", juce::dontSendNotification);
    carrierALabel.attachToComponent (&carrierASlider, true);
    //carrierASlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 20, 20);

    //decay slider
    carrierDSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierDSlider);
    carrierDSlider.setRange (0.0001, 10.0);
    //carrierDSlider.setSkewFactorFromMidPoint (5);
    carrierDSlider.addListener (this);
    addAndMakeVisible (carrierDLabel);
    carrierDLabel.setText ("cD", juce::dontSendNotification);
    carrierDLabel.attachToComponent (&carrierDSlider, true);
    
    //sustain slider
    carrierSSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierSSlider);
    carrierSSlider.setRange (0, 0.125);
    //carrierSSlider.setSkewFactorFromMidPoint (0.0625);
    carrierSSlider.addListener (this);
    addAndMakeVisible (carrierSLabel);
    carrierSLabel.setText ("cS", juce::dontSendNotification);
    carrierSLabel.attachToComponent (&carrierSSlider, true);
    
    //release slider
    carrierRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierRSlider);
    carrierRSlider.setRange (0.0001, 10.0);
    //carrierRSlider.setSkewFactorFromMidPoint (5);
    carrierRSlider.addListener (this);
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void FmsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto sliderLeft = 120;
    carrierFreqSlider.setBounds (sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    modulatorFreqSlider.setBounds (sliderLeft, 40, getWidth() - sliderLeft - 10, 20);
    modulatorAmpSlider.setBounds (sliderLeft, 60, getWidth() - sliderLeft - 10, 20);
    keyboardComponent.setBounds(0, 100, getWidth(), 60);
    
    carrierASlider.setBounds(50, 180, 20, 100);
    carrierDSlider.setBounds(90, 180, 20, 100);
    carrierSSlider.setBounds(130, 180, 20, 100);
    carrierRSlider.setBounds(170, 180, 20, 100);
    carrierSSlider.setValue(10.0);
}

void FmsynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    //audioProcessor.noteOnVel = midiVolume.getValue();
    if(slider == &carrierFreqSlider)
    {
        audioProcessor.carrFreq = carrierFreqSlider.getValue();
        //audioProcessor.updateAngleDelta();
        audioProcessor.setFrequency();
    }
    if(slider == &modulatorFreqSlider)
    {
        audioProcessor.modFreq = modulatorFreqSlider.getValue();
        //audioProcessor.updateAngleFM();
        audioProcessor.setFrequencyFM();
    }
    if(slider == &modulatorAmpSlider)
    {
        audioProcessor.modAmp = modulatorAmpSlider.getValue();
        //audioProcessor.modAmp[1] = modulatorAmpSlider.getValue();
    }
    if(slider == &carrierASlider)
    {
        audioProcessor.attackTime = carrierASlider.getValue();
        //audioProcessor.updateAttack();
    }
    if(slider == &carrierDSlider)
    {
        audioProcessor.decayTime = carrierDSlider.getValue();
    }
    if(slider == &carrierSSlider)
    {
        audioProcessor.sustainLevel = carrierSSlider.getValue();
    }
    if(slider == &carrierRSlider)
    {
        audioProcessor.releaseTime = carrierRSlider.getValue();
    }
    
}

void FmsynthAudioProcessorEditor::handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    printf("Received note %d\n",midiNoteNumber);
    // frequency calculation from midi note data
    double nota = 0.0;
    nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    printf("Calculated frequency %f\n",nota);
    audioProcessor.carrFreq=nota;
    //audioProcessor.updateAngleDelta();
    audioProcessor.setFrequency();
    audioProcessor.setFrequencyFM();
    audioProcessor.envCount = 0;
    audioProcessor.updateAttack();
    audioProcessor.updateDecay();
    audioProcessor.updateRelease();
    audioProcessor.noteOn=true;
    audioProcessor.carrAmp=0.0;
}
void FmsynthAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    audioProcessor.noteOn=false;
}


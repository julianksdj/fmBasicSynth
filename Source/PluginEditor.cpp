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
    //auto windowWidth = 500;
    //auto windowHeight = 230;
    auto windowWidth = 500;
    auto windowHeight = 230;
    
    setSize (windowWidth, windowHeight);
    
    addAndMakeVisible (algoButton1);
    algoButton1.setLookAndFeel(&customLook);
    algoButton1.onClick = [this] { updateToggleState (&algoButton1, "algorithm1"); };
    algoButton1.setButtonText("1");
    addAndMakeVisible (algoButton2);
    algoButton2.setLookAndFeel(&customLook);
    algoButton2.onClick = [this] { updateToggleState (&algoButton2, "algorithm2"); };
    algoButton2.setButtonText("2");
    addAndMakeVisible (algoButton3);
    algoButton3.setLookAndFeel(&customLook);
    algoButton3.onClick = [this] { updateToggleState (&algoButton2, "algorithm3"); };
    algoButton3.setButtonText("3");
    
    
    // 0,162,26
    customLook.setColour (juce::Slider::thumbColourId, juce::Colour(0,153,138));
    customLook.setColour (juce::Slider::trackColourId, juce::Colour(58,43,43));
    customLook.setColour (juce::Slider::backgroundColourId, juce::Colour(6,4,4));
                          
    // frequency labels
    ampLabel.setText ("Amp", juce::dontSendNotification);
    ampLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (ampLabel);
    coarseLabel.setText ("Coarse", juce::dontSendNotification);
    coarseLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (coarseLabel);
    fineLabel.setText ("Fine", juce::dontSendNotification);
    fineLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (fineLabel);
    op1Label.setText ("OP1", juce::dontSendNotification);
    op1Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (op1Label);
    op2Label.setText ("OP2", juce::dontSendNotification);
    op2Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (op2Label);
    
    //OP1 amplitude
    addAndMakeVisible (op1AmpKnob);
    op1AmpKnob.setNormalisableRange(juce::NormalisableRange<double>(0.f, 10.f, 0.01f, 1.f));
    op1AmpKnob.addListener (this);
    op1AmpKnob.setValue(audioProcessor.getOp1Amp());
    op1AmpKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    op1AmpKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 13);
    op1AmpKnob.setLookAndFeel(&customLook);
    
    //OP1 coarse
    addAndMakeVisible (op1CoarseKnob);
    op1CoarseKnob.setNormalisableRange(juce::NormalisableRange<double>(0.25f, 6.f, 0.25, 1.f));
    op1CoarseKnob.addListener (this);
    op1CoarseKnob.setValue(audioProcessor.getOp1Coarse());
    op1CoarseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    op1CoarseKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 13);
    op1CoarseKnob.setLookAndFeel(&customLook);
    
    //OP1 fine
    addAndMakeVisible (op1FineKnob);
    op1FineKnob.setNormalisableRange(juce::NormalisableRange<double>(-12.f, 12.f, 0.1f, 1.f));
    //op1CoarseKnob.setTextValueSuffix (" Hz");
    op1FineKnob.addListener (this);
    op1FineKnob.setValue(audioProcessor.getOp1Fine());
    op1FineKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    op1FineKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 13);
    op1FineKnob.setLookAndFeel(&customLook);
    
    //OP2 amplitude
    addAndMakeVisible (op2AmpKnob);
    op2AmpKnob.setNormalisableRange(juce::NormalisableRange<double>(0.f, 10.f, 0.01f, 1.f));
    op2AmpKnob.addListener (this);
    op2AmpKnob.setValue(audioProcessor.getOp2Amp());
    op2AmpKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    op2AmpKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 13);
    op2AmpKnob.setLookAndFeel(&customLook);
    
    //OP2 coarse
    addAndMakeVisible (op2CoarseKnob);
    op2CoarseKnob.setNormalisableRange(juce::NormalisableRange<double>(0.25f, 6.f, 0.25, 1.f));
    op2CoarseKnob.addListener (this);
    op2CoarseKnob.setValue(audioProcessor.getOp2Coarse());
    op2CoarseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    op2CoarseKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 13);
    op2CoarseKnob.setLookAndFeel(&customLook);
    
    //OP2 fine
    addAndMakeVisible (op2FineKnob);
    op2FineKnob.setNormalisableRange(juce::NormalisableRange<double>(-12.f, 12.f, 0.1f, 1.f));
    op2FineKnob.addListener (this);
    op2FineKnob.setValue(audioProcessor.getOp2Fine());
    op2FineKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    op2FineKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 13);
    op2FineKnob.setLookAndFeel(&customLook);
    
    //keyboard
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);
    
    //OP1 envelope
    //labels
    aLabel.setText ("A", juce::dontSendNotification);
    aLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (aLabel);
    dLabel.setText ("D", juce::dontSendNotification);
    dLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (dLabel);
    sLabel.setText ("S", juce::dontSendNotification);
    sLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (sLabel);
    rLabel.setText ("R", juce::dontSendNotification);
    rLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (rLabel);
    

    //attack slider
    op1ASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op1ASlider);
    // min  attack time must be 0.0001 (100us) to avoid attackSamples from being 0
    // so the division Aa = 0.125 / attackSamples is never inf
    op1ASlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    op1ASlider.addListener (this);
    op1ASlider.setValue(audioProcessor.getOp1attack());
    op1ASlider.setLookAndFeel(&customLook);
    //decay slider
    op1DSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op1DSlider);
    op1DSlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    op1DSlider.addListener (this);
    op1DSlider.setValue(audioProcessor.getOp1decay());
    op1DSlider.setLookAndFeel(&customLook);
    //sustain slider
    op1SSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op1SSlider);
    op1SSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.0001f, 1.f));
    op1SSlider.addListener (this);
    op1SSlider.setValue(audioProcessor.getOp1sustain());
    op1SSlider.setLookAndFeel(&customLook);
    //release slider
    op1RSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op1RSlider);
    op1RSlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    op1RSlider.addListener (this);
    op1RSlider.setValue(audioProcessor.getOp1release());
    op1RSlider.setLookAndFeel(&customLook);
    //OP2 envelope
    //attack slider
    op2ASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op2ASlider);
    op2ASlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    op2ASlider.addListener (this);
    op2ASlider.setValue(audioProcessor.getOp2attack());
    op2ASlider.setLookAndFeel(&customLook);
    //decay slider
    op2DSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op2DSlider);
    op2DSlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    op2DSlider.addListener (this);
    op2DSlider.setValue(audioProcessor.getOp2decay());
    op2DSlider.setLookAndFeel(&customLook);
    //sustain slider
    op2SSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op2SSlider);
    op2SSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.0001f, 1.f));
    op2SSlider.addListener (this);
    op2SSlider.setValue(audioProcessor.getOp2sustain());
    op2SSlider.setLookAndFeel(&customLook);
    //release slider
    op2RSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (op2RSlider);
    op2RSlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    op2RSlider.addListener (this);
    op2RSlider.setValue(audioProcessor.getOp2release());
    op2RSlider.setLookAndFeel(&customLook);
    
    //combobox algorithm
    addAndMakeVisible (algoLabel);
    algoLabel.setText("Algorithm", juce::dontSendNotification);
    algoLabel.setJustificationType(juce::Justification::centred);
    //addAndMakeVisible (algoMenu);
    //algoMenu.addItem ("OP1 <- OP2",  1);
    //algoMenu.addItem ("OP1 -> OP2",  2);
    //algoMenu.onChange = [this] { algoMenuChanged(); };
    //algoMenu.setSelectedId(audioProcessor.getAlgorithm());
}

FmsynthAudioProcessorEditor::~FmsynthAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void FmsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::backgroundDX_png, BinaryData::backgroundDX_pngSize);
    g.drawImageAt (background, 0, 0);
    juce::Image algorithmChart = juce::ImageCache::getFromMemory (BinaryData::algorithms_png, BinaryData::algorithms_pngSize);
    g.drawImage (algorithmChart,                      //const Image &imageToDraw. the image to overlay
                 355,                  //int destX. the left of the destination rectangle
                 30,                  //int destY. the top of the destination rectangle
                 algorithmChart.getWidth()*0.50f,//width,                     //int destWidth. the width of the destination rectangle
                 algorithmChart.getHeight()*0.50f,//height,                    //int destHeight. the height of the destination rectangle
                 0,                         //int sourceX. the left of the rectangle to copy from the source image
                 0,  //int sourceY. the top of the rectangle to copy from the source image
                 algorithmChart.getWidth(),           //int sourceWidth
                 algorithmChart.getHeight(),           //int sourceHeight
                 false);                    //bool fillAlphaChannelWithCurrentBrush=false
    juce::Image logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_png, BinaryData::logo_pngSize);
    g.drawImage (logoImage,                      //const Image &imageToDraw. the image to overlay
                 355,                  //int destX. the left of the destination rectangle
                 116,                  //int destY. the top of the destination rectangle
                 logoImage.getWidth()*0.15f,//width,                     //int destWidth. the width of the destination rectangle
                 logoImage.getHeight()*0.15f,//height,                    //int destHeight. the height of the destination rectangle
                 0,                         //int sourceX. the left of the rectangle to copy from the source image
                 0,  //int sourceY. the top of the rectangle to copy from the source image
                 logoImage.getWidth(),           //int sourceWidth
                 logoImage.getHeight(),           //int sourceHeight
                 false);                    //bool fillAlphaChannelWithCurrentBrush=false
}

void FmsynthAudioProcessorEditor::resized()
{
    auto border = 20;
    auto sliderWidth = 20;
    auto sliderHeight = 100;
    auto knobWidth = 70;
    auto knobHeight = 70;
    auto buttonWidth = 40;
    
    ampLabel.setBounds(border, 5, knobWidth, 15);
    coarseLabel.setBounds(border+knobWidth, 5, knobWidth, 15);
    fineLabel.setBounds(border+knobWidth*2, 5, knobWidth, 15);
    
    op1AmpKnob.setBounds (border, border, knobWidth, knobHeight);
    op1CoarseKnob.setBounds (border+knobWidth, border, knobWidth, knobHeight);
    op1FineKnob.setBounds (border+knobWidth*2, border, knobWidth, knobHeight);
    op2AmpKnob.setBounds (border, border+knobHeight, knobWidth, knobHeight);
    op2CoarseKnob.setBounds (border+knobWidth, border+knobHeight, knobWidth, knobHeight);
    op2FineKnob.setBounds (border+knobWidth*2, border+knobHeight, knobWidth, knobHeight);
    
    op1Label.setBounds(5, 40, 30, 15);
    op2Label.setBounds(5, 110, 30, 15);
    
    keyboardComponent.setBounds(0, getHeight()-50, getWidth(), 50);
    
    aLabel.setBounds(border+knobWidth*3, 5, sliderWidth, 15);
    dLabel.setBounds(border+knobWidth*3+sliderWidth, 5, sliderWidth, 15);
    sLabel.setBounds(border+knobWidth*3+sliderWidth*2, 5, sliderWidth, 15);
    rLabel.setBounds(border+knobWidth*3+sliderWidth*3, 5, sliderWidth, 15);
    
    op1ASlider.setBounds(border+knobWidth*3, border, 20, 70);
    op1DSlider.setBounds(border+knobWidth*3+sliderWidth, border, 20, 70);
    op1SSlider.setBounds(border+knobWidth*3+sliderWidth*2, border, 20, 70);
    op1RSlider.setBounds(border+knobWidth*3+sliderWidth*3, border, 20, 70);
    
    op2ASlider.setBounds(border+knobWidth*3, border+knobHeight, 20, 70);
    op2DSlider.setBounds(border+knobWidth*3+sliderWidth, border+knobHeight, 20, 70);
    op2SSlider.setBounds(border+knobWidth*3+sliderWidth*2, border+knobHeight, 20, 70);
    op2RSlider.setBounds(border+knobWidth*3+sliderWidth*3, border+knobHeight, 20, 70);

    algoLabel.setBounds(65+knobWidth*3+sliderWidth*4, 5, 100, 20);
    //algoMenu.setBounds(70+knobWidth*3+sliderWidth*4, getHeight()/4+20, 100, 20);
    
    algoButton1.setBounds(35+knobWidth*3+sliderWidth*5, getHeight()/4+20, buttonWidth, buttonWidth);
    algoButton2.setBounds(35+knobWidth*3+sliderWidth*5+buttonWidth, getHeight()/4+20, buttonWidth, buttonWidth);
    algoButton3.setBounds(35+knobWidth*3+sliderWidth*5+buttonWidth*2, getHeight()/4+20, buttonWidth, buttonWidth);
}

void FmsynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &op1CoarseKnob)
    {
        audioProcessor.setOp1Coarse(op1CoarseKnob.getValue()) ;
    }
    else if(slider == &op2CoarseKnob)
    {
        audioProcessor.setOp2Coarse(op2CoarseKnob.getValue()) ;
    }
    else if(slider == &op1FineKnob)
    {
        audioProcessor.setOp1Fine(op1FineKnob.getValue()) ;
    }
    else if(slider == &op2FineKnob)
    {
        audioProcessor.setOp2Fine(op2FineKnob.getValue()) ;
    }
    else if(slider == &op1AmpKnob)
    {
        audioProcessor.setOp1Amp(op1AmpKnob.getValue()) ;
    }
    else if(slider == &op2AmpKnob)
    {
        audioProcessor.setOp2Amp(op2AmpKnob.getValue()) ;
    }
    else if(slider == &op1ASlider)
    {
        audioProcessor.setOp1attack(op1ASlider.getValue());
    }
    else if(slider == &op1DSlider)
    {
        audioProcessor.setOp1decay(op1DSlider.getValue());
    }
    else if(slider == &op1SSlider)
    {
        audioProcessor.setOp1sustain(op1SSlider.getValue());
    }
    else if(slider == &op1RSlider)
    {
        audioProcessor.setOp1release(op1RSlider.getValue());
    }
    else if(slider == &op2ASlider)
    {
        audioProcessor.setOp2attack(op2ASlider.getValue());
    }
    else if(slider == &op2DSlider)
    {
        audioProcessor.setOp2decay(op2DSlider.getValue());
    }
    else if(slider == &op2SSlider)
    {
        audioProcessor.setOp2sustain(op2SSlider.getValue());
    }
    else if(slider == &op2RSlider)
    {
        audioProcessor.setOp2release(op2RSlider.getValue());
    }
}

void FmsynthAudioProcessorEditor::handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    //printf("\nNOTE PRESSED\n");
    //printf("Received note %d\n",midiNoteNumber);
    // frequency calculation from midi note data
    double nota = 0.0;
    nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    //printf("Calculated frequency %f\n",nota);
    audioProcessor.addVoice(nota);
}
void FmsynthAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    auto nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    //printf("NOTE RELEASED\n");
    audioProcessor.deactivateVoice(nota);
}

void FmsynthAudioProcessorEditor::updateToggleState (juce::Button* button, juce::String name)
{
        //auto state = tb.getToggleState();
        //juce::String stateString    = state ? "ON" : "OFF";
        //juce::String selectedString = state ? " (selected)" : "";
 
        //juce::Logger::outputDebugString (name + " Button changed to " + stateString);
        //button->setButtonText (name + selectedString);
    if (name == "algorithm1")
    {
        audioProcessor.setAlgorithm(1);
    }
    else if (name == "algorithm2")
    {
        audioProcessor.setAlgorithm(2);
    }
    else if (name == "algorithm3")
    {
        audioProcessor.setAlgorithm(3);
    }
}

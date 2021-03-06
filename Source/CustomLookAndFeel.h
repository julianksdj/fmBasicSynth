/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 16 Jan 2022 1:08:15am
    Author:  Keepin' Soul

  ==============================================================================
*/

#pragma once

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(){};
    virtual void drawRotarySlider    (juce::Graphics &     g,
                                     int     x,
                                     int     y,
                                     int     width,
                                     int     height,
                                     float     sliderPosProportional,
                                     float     rotaryStartAngle,
                                     float     rotaryEndAngle,
                                     juce::Slider &     slider) override
    {
        juce::Image knob = juce::ImageCache::getFromMemory (BinaryData::knobdxblue_png, BinaryData::knobdxblue_pngSize);
        const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
        const int nFrames = knob.getHeight()/knob.getWidth(); // number of frames for vertical knob
        const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        
        const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius - 1.0f;
        const float ry = centreY - radius;
        // draw image draws part of an image, rescaling it to fit in a given target region. The specified area of the source image is rescaled and drawn to fill the specified destination rectangle.
        g.drawImage (knob,                      //const Image &imageToDraw. the image to overlay
                     (int) rx,                  //int destX. the left of the destination rectangle
                     (int) ry,                  //int destY. the top of the destination rectangle
                     2 * (int)radius,//width,                     //int destWidth. the width of the destination rectangle
                     2 * (int)radius,//height,                    //int destHeight. the height of the destination rectangle
                     0,                         //int sourceX. the left of the rectangle to copy from the source image
                     frameIdx*knob.getWidth(),  //int sourceY. the top of the rectangle to copy from the source image
                     knob.getWidth(),           //int sourceWidth
                     knob.getWidth(),           //int sourceHeight
                     false);                    //bool fillAlphaChannelWithCurrentBrush=false
    };
    
    virtual void drawToggleButton (juce::Graphics& g,
                                   juce::ToggleButton& button,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) override
    {
        juce::Image dxButtonOn = juce::ImageCache::getFromMemory (BinaryData::buttondxblueOn_png, BinaryData::buttondxblueOn_pngSize);
        juce::Image dxButtonOff = juce::ImageCache::getFromMemory (BinaryData::buttondxblueOff_png, BinaryData::buttondxblueOff_pngSize);
        juce::Image dxButton;
        
        if(shouldDrawButtonAsDown)
        {
            dxButton = dxButtonOn;
        }
        else
        {
            dxButton = dxButtonOff;
        }
        
        g.drawImage (dxButton,                      //const Image &imageToDraw. the image to overlay
                    0,                  //int destX. the left of the destination rectangle
                    0,                  //int destY. the top of the destination rectangle
                    button.getWidth(),//width,                     //int destWidth. the width of the destination rectangle
                    button.getHeight(),//height,                    //int destHeight. the height of the destination rectangle
                    0,                         //int sourceX. the left of the rectangle to copy from the source image
                    0,  //int sourceY. the top of the rectangle to copy from the source image
                    dxButton.getWidth(),           //int sourceWidth
                    dxButton.getWidth(),           //int sourceHeight
                    false);                    //bool fillAlphaChannelWithCurrentBrush=false
        // text:
        auto fontSize = juce::jmin (15.0f, (float) button.getHeight() * 0.75f);

        g.setColour (juce::Colours::white);
        g.setFont (fontSize);

        //if (! button.isEnabled())
        //    g.setOpacity (0.5f);

        g.drawFittedText (button.getButtonText(),
                          button.getLocalBounds().withTrimmedLeft (5)
                                                 .withTrimmedRight (2),
                          juce::Justification::centredLeft, 10);
    }
};

//
//  Voice.h
//  fmsynth
//
//  Created by Keepin' Soul on 28/11/2021.
//

#ifndef Voice_h
#define Voice_h

#include "Envelope.h"
#include "SineOscillator.h"

class Voice
{

public:
    
    // voice features functions
    bool isActive ()
    {
        if (env.getCarrAmp()<=0.0000001)
            return false;
        else
            return true;
    };
    
    // Oscillator functions
    void setFrequency (float f)
    {
        osc.setFrequency(f);
    };
    void setFrequencyFM (float f)
    {
        osc.setFrequencyFM(f);
    };
    float getNextSample(int channel) //noexcept
    {
        return osc.getNextSample(channel);
    };
    float getFrequency()
    {
        return osc.getFrequency();
    };
    void setCarrFreq(float m)
    {
        osc.setCarrFreq(m);
    };
    void setModFreq(float m)
    {
        osc.setModFreq(m);
    };
    void setModAmp(float m)
    {
        osc.setModAmp(m);
    };

    // envelope functions
    float getEnvelope()
    {
        return env.getEnvelope();
    };
    void setAttack(float a)
    {
        env.setAttack(a);
    };
    void setDecay(float d)
    {
        env.setDecay(d);
    };
    void setSustain(float s)
    {
        env.setSustain(s);
    };
    void setRelease(float r)
    {
        env.setRelease(r);
    };
    void setSampleRate(int sr)
    {
        env.setSampleRate(sr);
    };
    void setCarrAmp(float a)
    {
        env.setCarrAmp(a);
    };
    void setNoteOn(bool n)
    {
        env.setNoteOn(n);
    };
    void resetEnvCount()
    {
        env.resetEnvCount();
    };
    void setSampleRate(float sr)
    {
        osc.setSampleRate(sr);
        env.setSampleRate(sr);
    };
    
    
private:
    Envelope env;
    SineOscillator osc;
    //double sampleRate;
};

#endif /* Voice_h */

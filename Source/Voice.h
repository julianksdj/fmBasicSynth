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
        if (ampEnv.getEnvelopeValue()<=0.0000001)
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
    float getNoteFreq()
    {
        return osc.getNoteFreq();
    };
    void setModAmp(float m)
    {
        osc.setModAmp(m);
    };

    // envelope functions
    float getAmpEnvelope()
    {
        return ampEnv.getEnvelope();
    };
    void setAmpAttack(float a)
    {
        ampEnv.setAttack(a);
    };
    void setAmpDecay(float d)
    {
        ampEnv.setDecay(d);
    };
    void setAmpSustain(float s)
    {
        ampEnv.setSustain(s);
    };
    void setAmpRelease(float r)
    {
        ampEnv.setRelease(r);
    };
    void setPitchAttack(float a)
    {
        pitchEnv.setAttack(a);
    };
    void setPitchDecay(float d)
    {
        pitchEnv.setDecay(d);
    };
    void setPitchSustain(float s)
    {
        pitchEnv.setSustain(s);
    };
    void setPitchRelease(float r)
    {
        pitchEnv.setRelease(r);
    };
    void setNoteOn(bool n)
    {
        ampEnv.setNoteOn(n);
        pitchEnv.setNoteOn(n);
    };
    void resetEnvCount()
    {
        ampEnv.resetEnvCount();
        pitchEnv.resetEnvCount();
    };
    void setSampleRate(float sr)
    {
        osc.setSampleRate(sr);
        ampEnv.setSampleRate(sr);
        pitchEnv.setSampleRate(sr);
        
    };
    void setNoteFreq(float f)
    {
        osc.setNoteFreq(f);
    };
    
    
private:
    Envelope ampEnv, pitchEnv;
    SineOscillator osc;
    int algorithm;
    //double sampleRate;
};

#endif /* Voice_h */

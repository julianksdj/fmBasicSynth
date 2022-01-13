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
        if (!osc.isActive())
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
    void setCarAmp(float a)
    {
        osc.setCarAmp(a);
    };
    void setModAmp(float a)
    {
        osc.setModAmp(a);
    };

    void setNoteOn(bool n)
    {
        osc.setNoteOn(n);
    };
    
    void resetEnvCount()
    {
        osc.resetEnvCount();
    };
    
    void setSampleRate(float sr)
    {
        osc.setSampleRate(sr);
    };
    void setNoteFreq(float f)
    {
        osc.setNoteFreq(f);
    };
    
    void initFMenv(float ca, float cd, float cs, float cr, float ma, float md, float ms, float mr){
        osc.initFMenv(ca, cd, cs, cr, ma, md, ms, mr);
    };
    
private:
    // this will include filters and other kinds of oscillators (other waveforms + LFO)
    SineOscillator osc;
};

#endif /* Voice_h */

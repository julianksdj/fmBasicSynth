//
//  SineOscillator.h
//  fmsynth
//
//  Created by Keepin' Soul on 21/11/2021.
//

#ifndef SineOscillator_h
#define SineOscillator_h
#include "Envelope.h"

class SineOscillator {
public:
    SineOscillator()
    {
    };
    void setFrequency (float frequency)
    {
        carrFreq=frequency;
        auto cyclesPerSample = frequency / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    void setFrequencyFM (float frequency)
    {
        modFreq=frequency;
        auto cyclesPerSample = frequency / currentSampleRate;
        angleDeltaFM = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        float sample = 0.0125f * carAmp[channel] * std::sin(currentAngle[channel] + modAmp[channel] * std::cos(currentAngleFM[channel]));
        currentAngle[channel] += angleDelta;
        currentAngleFM[channel] += angleDeltaFM;
        if (currentAngle[channel]>=juce::MathConstants<double>::twoPi)
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
        if (currentAngleFM[channel]>=juce::MathConstants<double>::twoPi)
            currentAngleFM[channel] -= juce::MathConstants<double>::twoPi;
        updateFM(channel);
        return sample;
    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    float getNoteFreq()
    {
        return noteFreq;
    };
    void setNoteFreq(float f)
    {
        noteFreq = f;
    };
    void setModAmp(float m)
    {
        modAmp[0] = m;
        modAmp[1] = m;
        modAmp0 = m;
    };
    void setCarAmp(float m)
    {
        carAmp[0] = m;
        carAmp[1] = m;
        carAmp0 = m;
    };
    // Envolvente de pitch (futura implementacion)
    /*
    void updateFM()
    {
        currentFreqFM = noteFreqFM * freqEnv.getEnvelope();
        if (currentFreqFM < 20.f)
            currentFreqFM = 20.f;
        setFrequencyFM(currentFreqFM);
    };*/
    void updateFM(int channel)
    {
        carAmp[channel] = carAmp0 * ampEnv.getEnvelope(channel);
        modAmp[channel] = modAmp0 * fmEnv.getEnvelope(channel);
    };
    void initFMenv(float ca, float cd, float cs, float cr, float ma, float md, float ms, float mr)
    {
        ampEnv.setSampleRate(currentSampleRate);
        ampEnv.setAttack(ca);
        ampEnv.setDecay(cd);
        ampEnv.setSustain(cs);
        ampEnv.setRelease(cr);
        ampEnv.setNoteOn(true);
        ampEnv.resetEnvCount();
        
        fmEnv.setSampleRate(currentSampleRate);
        fmEnv.setAttack(ma);
        fmEnv.setDecay(md);
        fmEnv.setSustain(ms);
        fmEnv.setRelease(mr);
        fmEnv.setNoteOn(true);
        fmEnv.resetEnvCount();
    };
    
    void setNoteOn(bool n)
    {
        ampEnv.setNoteOn(n);
        fmEnv.setNoteOn(n);
    };
    void resetEnvCount()
    {
        ampEnv.resetEnvCount();
        fmEnv.resetEnvCount();
    };
    bool isActive()
    {
        if (ampEnv.getFin())
            return false;
        else
            return true;
    };
    
private:
    float currentAngle[2], currentAngleFM[2], angleDelta, angleDeltaFM;
    float currentSampleRate;
    float carrFreq;
    float modFreq;
    float carAmp[2], modAmp[2];
    float modAmpSmoothed;
    float noteFreq; //, noteFreqFM;
    float carAmp0, modAmp0;
    
    Envelope fmEnv, ampEnv;
    //float currentFreq, currentFreqFM;
};

#endif /* SineOscillator_h */

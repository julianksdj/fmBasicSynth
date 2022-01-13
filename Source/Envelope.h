//
//  Envelope.h
//  fmsynth
//
//  Created by Keepin' Soul on 28/11/2021.
//

#ifndef Envelope_h
#define Envelope_h

class Envelope
{

public:
    Envelope()
    {
        envCount = 0;
    };
    
    float getEnvelope()
    {
        //Calculate envelope
        if(noteOn==true) //pressed note on the keyboard
        {
            if (envCount < attackSamples)
            {
                //carrAmp+=(0.125/attackSamples); //the amplitude raises until max amplitude is reached
                envelope+=(1.f/attackSamples);
            }
            else if (envCount < daSamples)
            {
                envelope-=((1.f-sustainLevel)/decaySamples); //the amplitude decreases until sustain level is reached
            }
            releaseLevel = envelope; // saves the amplitude state
        }
        else //released note on the keyboard
        {
            if(envelope>0.0000001)
                envelope-=(releaseLevel/releaseSamples); //amplitude decreases until silence
        }
        envCount++;
        return envelope;
    };
    float getEnvelopeValue()
    {
        return envelope;
    };
    void setAttack(float a)
    {
        attackTime = a;
        attackSamples = currentSampleRate * attackTime;
        printf("Attack time: %f\n", attackTime);
        printf("Attack samples: %d\n", attackSamples);
    };
    void setDecay(float d)
    {
        decayTime = d;
        decaySamples = currentSampleRate * decayTime;
        printf("decay time %f\n", decayTime);
        daSamples = decaySamples + attackSamples;
        printf("decay Samples %d\n", decaySamples);
        printf("da Samples %d\n", daSamples);
    };
    void setSustain(float s)
    {
        sustainLevel = s;
    };
    void setRelease(float r)
    {
        releaseTime = r;
        releaseSamples = currentSampleRate * releaseTime;
        printf("release level %f\n", releaseLevel);
        printf("release time %f\n", releaseTime);
        printf("release samples %d\n", releaseSamples);
    };

    void setNoteOn(bool n){
        noteOn = n;
    };
    void resetEnvCount(){
        envCount = 0;
    };
    void setSampleRate(float sr){
        currentSampleRate = sr;
    };
    
private:
    float currentSampleRate;
    float envelope;
    float attackTime, decayTime, sustainLevel, releaseTime;
    int attackSamples, decaySamples, releaseSamples, daSamples;
    int envCount;
    bool noteOn;
    float releaseLevel;
};


#endif /* Envelope_h */

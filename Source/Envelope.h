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
        envCount[0] = 0;
        envCount[1] = 0;
        envelope[0] = 0.0000002f;
        envelope[1] = 0.0000002f;
        fin = false;
    };
    
    float getEnvelope(int channel)
    {
        //Calculate envelope
        if(noteOn==true) //pressed note on the keyboard
        {
            if (envCount[channel] < attackSamples)
            {
                //the amplitude raises until max amplitude is reached
                envelope[channel]+=(1.f/attackSamples);
            }
            else if (envCount[channel] < daSamples)
            {
                envelope[channel]-=((1.f-sustainLevel)/decaySamples); //the amplitude decreases until sustain level is reached
            }
            releaseLevel[channel] = envelope[channel]; // saves the amplitude state
        }
        else //released note on the keyboard
        {
            if(envelope[channel]>releaseLevel[channel]/releaseSamples)
                envelope[channel]-=(releaseLevel[channel]/releaseSamples); //amplitude decreases until silence
            else
                fin = true;
        }
        envCount[channel]++;
        return envelope[channel];
    };
    void setAttack(float a)
    {
        attackTime = a;
        attackSamples = currentSampleRate * attackTime;
        //printf("Attack time: %f\n", attackTime);
        //printf("Attack samples: %d\n", attackSamples);
    };
    void setDecay(float d)
    {
        decayTime = d;
        decaySamples = currentSampleRate * decayTime;
        //printf("decay time %f\n", decayTime);
        daSamples = decaySamples + attackSamples;
        //printf("decay Samples %d\n", decaySamples);
        //printf("da Samples %d\n", daSamples);
    };
    void setSustain(float s)
    {
        sustainLevel = s;
    };
    void setRelease(float r)
    {
        releaseTime = r;
        releaseSamples = currentSampleRate * releaseTime;
        //printf("release level %f\n", releaseLevel);
        //printf("release time %f\n", releaseTime);
        //printf("release samples %d\n", releaseSamples);
    };

    void setNoteOn(bool n){
        noteOn = n;
    };
    void resetEnvCount(){
        envCount[0] = 0;
        envCount[1] = 0;
    };
    void setSampleRate(float sr){
        currentSampleRate = sr;
    };
    bool getFin(){
        return fin;
    };
    
private:
    float currentSampleRate;
    float envelope[2];
    float attackTime, decayTime, sustainLevel, releaseTime;
    int attackSamples, decaySamples, releaseSamples, daSamples;
    int envCount[2];
    bool noteOn;
    float releaseLevel[2];
    bool fin;
};


#endif /* Envelope_h */

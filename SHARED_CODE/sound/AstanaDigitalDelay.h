//
//  AstanaSoundDelay.h
//  gestionSonido
//  DigitalDelay.h, created by Marek Bereza on 11/08/2013.
//  Modified by Roy Macdonald on 24-05-17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxSoundObjects.h"

#define MAX_DELAY_TIME 3
class AstanaDigitalDelay: public ofxSoundObject {
public:
    
    vector<float> buffer;
    
    AstanaDigitalDelay() {
        parameters.setName("Digital delay");
        parameters.add(bBypass.set("Bypass", true));
        parameters.add(delay.set("Delay Time (samples)", 4800, 0, 48000*MAX_DELAY_TIME));
        parameters.add(feedback.set("Feedback", 0.5, 0,1));
        parameters.add(mix.set("Mix", 0.5, 0,1));
        buffer.resize(48000*MAX_DELAY_TIME);
        buffer.assign(48000*MAX_DELAY_TIME, 0);
    }
    
    
    float process(float in) {
        pos++;
        pos %= delay;//buffer.size();
        float out = buffer[pos];
        buffer[pos] = feedback*buffer[pos] + in;
        return in + (out - in)*mix;
    };
    
    void process(ofSoundBuffer &input, ofSoundBuffer &output) {
        if (bBypass) {
            input.copyTo(output);
        }else{
            for(int i = 0; i < output.size(); i++) {
                output[i] = process(input[i]);
            }
        }
    }
    
    ofParameterGroup parameters;
    ofParameter<float> feedback;
    ofParameter<float> mix;
    ofParameter<unsigned int>delay;
    ofParameter<bool>bBypass;
    
private:
    int pos;
    
};
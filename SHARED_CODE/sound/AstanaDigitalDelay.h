//
//  AstanaSoundDelay.h
//  gestionSonido
//  DigitalDelay.h, created by Marek Bereza on 11/08/2013.
//  Modified by Roy Macdonald on 24-05-17.
//
//

#pragma once

#include "ofMain.h"
#include "AstanaBaseFX.h"

#define MAX_DELAY_TIME 3
class AstanaDigitalDelay: public AstanaBaseFX {
public:
    
    vector<float> buffer;
    
    AstanaDigitalDelay() {
        parameters.setName("Digital delay");
        parameters.add(bBypass.set("Bypass", true));
        parameters.add(delay.set("Delay Time (samples)", 4800, 1, 48000*MAX_DELAY_TIME));
        parameters.add(feedback.set("Feedback", 0, 0,1));
        parameters.add(mix.set("Mix", 0, 0,1));
		listeners.push_back(bBypass.newListener(this, &AstanaDigitalDelay::bypassChanged));
		listeners.push_back(delay.newListener(this, &AstanaDigitalDelay::delayChanged));
		listeners.push_back(feedback.newListener(this, &AstanaDigitalDelay::feedbackChanged));
		listeners.push_back(mix.newListener(this, &AstanaDigitalDelay::mixChanged));

		_delay = 4800;

		buffer.resize(48000*MAX_DELAY_TIME);
        buffer.assign(48000*MAX_DELAY_TIME, 0);
    }
    
    
    float process(float in) {
        pos++;
        pos %= _delay;//buffer.size();
        float out = buffer[pos];
        buffer[pos] = _feedback*buffer[pos] + in;
        return in + (out - in)*_mix;
    };
    
    virtual void process(ofSoundBuffer &input, ofSoundBuffer &output) {
        if (_bBypass) {
            input.copyTo(output);
        }else{
            for(int i = 0; i < output.size(); i++) {
                output[i] = process(input[i]);
            }
        }
    }
    
    ofParameter<float> feedback;
    ofParameter<float> mix;
    ofParameter<unsigned int>delay;
    ofParameter<bool>bBypass;
    
private:
	void feedbackChanged(float&){
		mutex.lock();
		_feedback = feedback;
		mutex.unlock();
	}
	void mixChanged(float&){
		mutex.lock();
		_mix = mix;
		mutex.unlock();
	}
	void delayChanged(unsigned int&){
		mutex.lock();
		_delay = delay;
		mutex.unlock();
	}
	void bypassChanged(bool&){
		mutex.lock();
		_bBypass = bBypass;
		mutex.unlock();
	}
	vector<ofEventListener>listeners;
	float _feedback;
    float _mix;
    unsigned int _delay;
    bool _bBypass;

	int pos;
    
};
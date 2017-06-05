//
//  AstanaSoundSecuencia.h
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#pragma once
#include "AstanaSoundGroup.h"
class AstanaSoundSecuencia: public AstanaSoundGroup{
public:
    AstanaSoundSecuencia();//const string& _name = "");//:AstanaSoundGroup(_name){}
    virtual void loadFolder(string folderPath);
    virtual bool play();
    ofParameter<float> gapFraseMin;


    virtual bool isNextAllowed();
    virtual void setupParameters();
protected:
    void onCurrentEnd(size_t&);
    float lastEndTime= 0.0;
    bool bFirstPlay =true;
};
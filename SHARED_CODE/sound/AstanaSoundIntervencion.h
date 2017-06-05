//
//  AstanaSoundIntervencion.h
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#pragma once
#include "AstanaSoundGroup.h"
class AstanaSoundIntervencion: public AstanaSoundGroup{
public:
    AstanaSoundIntervencion();

    bool isLoopPlaying();
    bool isIntervencionPlaying();
    virtual bool play();
    virtual bool playNext();
    virtual bool isNextAllowed();
    size_t getLoopIndex(){return loopIndex;}
    vector<size_t>intervencionesIndices;
    //virtual void setListeners(bool e = true);
    void onLoopEnd(size_t&);
    void onIntervencionEnd(size_t&);
    virtual void loadFolder(string folderPath);
    
    ofParameter<float>probabilidadLoopIntervencion;
    virtual void setupParameters();
protected:
    size_t loopIndex;
    bool bFirstLoop = true;

};
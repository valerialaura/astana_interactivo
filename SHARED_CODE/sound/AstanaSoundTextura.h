//
//  AstanaSoundTextura.h
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#pragma once
#include "AstanaSoundGroup.h"
class AstanaSoundTextura:public AstanaSoundGroup{
public:
    AstanaSoundTextura():AstanaSoundGroup(){
        bIsTextura = true;
    }
	virtual void loadFolder(string folderPath);
    void playAll();

    void printVols(){
        for(auto& p: players){
            cout << p.getName() << "  " << p.getVolume() << endl;
        }
    }
};
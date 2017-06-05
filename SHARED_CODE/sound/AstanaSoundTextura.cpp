//
//  AstanaSoundTextura.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundTextura.h"
//void AstanaSoundTextura::loadFolder(string folderPath){
//	AstanaSoundGroup::loadFolder(folderPath);
//	for (auto& p : players) {
//		p.setMultiPlay(true);
//	}
//}
//-------------------------------------------------------------------------
void AstanaSoundTextura::playAll(){
    for(auto& p: players){
        p.play();
    }
}
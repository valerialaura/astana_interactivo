//
//  AstanaSoundTextura.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundTextura.h"

void AstanaSoundTextura::playAll(){
    for(auto& p: players){
        p.play();
    }
}
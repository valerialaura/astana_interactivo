//
//  avatarLogo.cpp
//  example-simplePoints copy
//
//  Created by 3er on 5/25/17.
//
//

#include "avatarLogo.h"

void avatarLogo::setup(int _id){
    forma = int(ofRandom(4));
	id = _id;
	pos = ofPoint(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    shader.setGeometryInputType(GL_LINES);
    shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    shader.setGeometryOutputCount(4);
    shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    
    ofLog() << "Maximum number of output vertices support is: " << shader.getGeometryMaxOutputCount();
    
    ofImage pp;
    for (int i =0; i<4; i++){
        pp.load("icon"+ofToString(i+1)+".png");
        logos.push_back(pp);
        
    }
    ofColor c;
    c.set(63, 188, 167);
    colores.push_back(c);
    c.set(235, 229, 27);
    colores.push_back(c);
    c.set(120, 120, 117);
    colores.push_back(c);
    c.set(237, 36, 91);
    colores.push_back(c);
    
    if (forma == 0) {
        formaCuadrado();
    }
     if (forma == 1) {
    formaTriangulo();
     }
    if (forma == 2) {
        formaHexagono();
    }
    if (forma == 3) {
        formaCirculo();
    }
    
    create();
    

}

void avatarLogo::update(ofPoint _pos, ofPoint _size){
	//pos = _pos;
//#define DEPTH_WIDTH 512
//#define DEPTH_HEIGHT 424
	pos = ofPoint(ofMap(_pos.y, 0, 424, 0, ofGetWidth()), ofGetHeight()/2, ofMap(_pos.x, 0, 512, 100, -200));
	size = _size;
    float time = ofGetElapsedTimef();
    float noiseTime = time * 0.2;
    
    int numTri = triangulation.getNumTriangles();
    ofMesh triTempo = meshAvatar;
    meshAvatar.clear();
    meshAvatar.setMode(OF_PRIMITIVE_TRIANGLES);
// if (forma == 1) {
    for(int i =0; i<triangulos.size(); i++){
        if(puntosEstado[i] >= 1){
            //meshAvatar.addVertex( triTempo.getVertex(ii) );
            meshAvatar.addVertex(  triangulos[i]  );
            meshAvatar.addColor(ofFloatColor(0,0,0,0));
            if(i > 1){
            ofVec3f diff = triangulos[i] - triangulos[i-1];
                if (diff.length() > 5 || puntosEstado[i] == 2){
                    meshAvatar.addColor(colores[forma]);
                }
            
            }
           
        }
        //  ofVec3f noisePos = triTempo.getVertex(i) * 0.8;
        //  meshAvatar.addVertex( triTempo.getVertex(i) +  triTempo.getVertex(i)* ofRandom(-0.01, 0.01) );
    }
    meshAvatar.addIndices(triangulation.triangleMesh.getIndices());
// }
}

void avatarLogo::draw(){
    
   
    ofNoFill();
    
      ofPushMatrix();
	  
    ofTranslate(pos);
	ofScale(size.x*0.01);
    if (forma == 0) {
        
        float time = ofGetElapsedTimef();
        float noiseTime = time*4;
        // noise de puntos
        for(int i =4; i<triangulos.size(); i++){
            if(puntosEstado[i] == 2){
                triangulos[i] = triangulos[i] + ofSignedNoise(noiseTime * triangulos[i] );
                
            }
        }
        
        
        ofFill();
        ofSetColor(colores[forma],30);
        ofBeginShape();
        for(int i =4; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
                ofVec3f temp =triangulos[i]*0.64;
                ofVertex(temp);
            }
        }
        ofNextContour(true);
        for(int i =4; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
                ofVec3f temp =triangulos[i]*0.9;
                ofVertex(temp);
            }
        }
        ofEndShape(true);
        
        ofSetColor(colores[forma]);
        ofNoFill();
        ofSetLineWidth(4);
        ofBeginShape();
        for(int i =0; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
                
                ofVec3f temp =triangulos[i];
                ofVertex(temp);
            }
        }
        ofEndShape(true);
        
        ofSetLineWidth(1);
        meshAvatar.drawWireframe();

        
        ofFill();
        for(int i =0; i<triangulos.size(); i++){
            if(puntosEstado[i] == 2){
                ofDrawEllipse(triangulos[i], 15, 15);
            }
        }

        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofPushMatrix();
        ofSetColor(colores[forma]);
        ofTranslate(0, 0,40);
        logos[forma].draw(0, 0);
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CORNER);
        
    }
    if (forma == 1) {
        
        
        
        
        ofPushStyle();
      //  ofScale(1.4, 1.4);
        ofFill();
        ofColor(255);
        float time = ofGetElapsedTimef();
        float noiseTime = ofGetMouseX();

        // noise de puntos
        for(int i =0; i<12; i+=2){
                        triangulos[i+4] = triangulos[i+4] + ofSignedNoise(noiseTime * triangulos[i+4] );
          //  triangulos[i+6] = triangulos[i+4] + ofSignedNoise(t * triangulos[i+4] );
        }
        for(int i =1; i<4; i++){
            triangulos[triangulos.size()-i] = triangulos[triangulos.size()-i] + ofSignedNoise(noiseTime * triangulos[triangulos.size()-i] );
            //  triangulos[i+6] = triangulos[i+4] + ofSignedNoise(t * triangulos[i+4] );
        }
               // triangulo shape
        ofSetColor(colores[forma],30);
        ofBeginShape();
             for(int i =0; i<6; i++){
                 ofVec3f temp =triangulos[i+3]*0.8;
                 ofVertex(temp);
             }
        ofNextContour(true);
            for(int i =0; i<6; i++){
                ofVec3f temp =triangulos[i+3]*1.1;
                ofVertex(temp);
            }
        ofEndShape(true);
        
        meshAvatar.drawWireframe();
        // triangulo borde
        ofSetColor(colores[forma]);
        ofNoFill();
        ofSetLineWidth(4);
        ofBeginShape();
        for(int i =0; i<6; i++){
            
            ofVec3f temp =triangulos[i+9];
            ofVertex(temp);
           // ofVec3f temp =triangulos[i+3]*1.2;
           // ofVertex(temp);
        }
        ofEndShape(true);
        ofFill();
        for(int i =0; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
                ofDrawEllipse(triangulos[i], 15, 15);
            }
        }
        ofPopStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofPushMatrix();
        ofSetColor(colores[forma]);
        ofTranslate(0, 0,40);
        logos[forma].draw(0, 0);
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CORNER);

        
        
        
        }

    if (forma == 2) {
        
        float time = ofGetElapsedTimef();
        float noiseTime = time*4;
        // noise de puntos
        for(int i =0; i<triangulos.size(); i+=2){
            if(puntosEstado[i] == 1){
                triangulos[i] = triangulos[i] + ofSignedNoise(noiseTime * triangulos[i] );

            }
        }
        
        
        ofPushStyle();
        //ofScale(1.4, 1.4);
        
        ofSetColor(colores[forma]);
        
        ofFill();
        ofSetColor(colores[forma],30);
        ofBeginShape();
        for(int i =3; i<triangulos.size()-3; i++){
            ofVec3f temp =triangulos[i]*0.64;
            ofVertex(temp);
        }
        ofNextContour(true);
        for(int i =3; i<triangulos.size()-3; i++){
            ofVec3f temp =triangulos[i]*0.9;
            ofVertex(temp);
        }
        ofEndShape(true);
        
        ofSetColor(colores[forma]);
        ofNoFill();
        ofSetLineWidth(4);
        ofBeginShape();
        for(int i =3; i<triangulos.size()-3; i++){
            //if(puntosEstado[i] == 0){

            ofVec3f temp =triangulos[i];
            ofVertex(temp);
           // }
        }
        ofEndShape(true);
         ofSetLineWidth(1);
        meshAvatar.drawWireframe();
        
        ofFill();
        
        for(int i =triangulos.size()-3; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
                ofDrawEllipse(triangulos[i], 15, 15);
            }
        }
        ofPopStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofPushMatrix();
        ofSetColor(colores[forma]);
        ofTranslate(0, 0,40);
        logos[forma].draw(0, 0);
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CORNER);
        
        
        
        
    }
    
    if (forma == 3) {
        
        float time = ofGetElapsedTimef();
        float noiseTime = time*4;
        // noise de puntos
        for(int i =0; i<triangulos.size(); i+=2){
            if(puntosEstado[i] == 1){
                triangulos[i] = triangulos[i] + ofSignedNoise(noiseTime * triangulos[i] );
                
            }
        }

        
        ofFill();
        ofSetColor(colores[forma],30);
        ofBeginShape();
        for(int i =4; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
            ofVec3f temp =triangulos[i]*0.64;
            ofVertex(temp);
            }
        }
        ofNextContour(true);
        for(int i =4; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
            ofVec3f temp =triangulos[i]*0.9;
            ofVertex(temp);
            }
        }
        ofEndShape(true);
        
        ofSetColor(colores[forma]);
        ofNoFill();
        ofSetLineWidth(4);
        ofBeginShape();
        for(int i =0; i<triangulos.size(); i++){
            if(puntosEstado[i] == 1){
            
            ofVec3f temp =triangulos[i];
            ofVertex(temp);
             }
        }
        ofEndShape(true);
        ofSetLineWidth(1);
        meshAvatar.drawWireframe();
        
        ofFill();
        for(int i =0; i<triangulos.size(); i++){
            if(puntosEstado[i] == 2){
                ofDrawEllipse(triangulos[i], 15, 15);
            }
        }

        
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofPushMatrix();
        ofSetColor(colores[forma]);
        ofTranslate(0, 0,40);
        logos[forma].draw(0, 0);
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CORNER);

    }

    
    ofPopMatrix();
    

}

void avatarLogo::create(){
    triangulation.reset();
    for(int i =0; i<triangulos.size(); i++){
        if(puntosEstado[i] >= 1){
        triangulation.addPoint(triangulos[i].x,triangulos[i].y, triangulos[i].z);
        }
    }
    triangulation.triangulate();
    
    ofMesh triTempo = triangulation.triangleMesh;
    meshAvatar.clear();
    //triangleMesh.clear();
    meshAvatar.setMode(OF_PRIMITIVE_TRIANGLES);
    
    for(int i =0; i<triTempo.getNumVertices(); i++){
        meshAvatar.addVertex( triTempo.getVertex(i));
    }
    meshAvatar.addIndices(triTempo.getIndices());
}

void avatarLogo::reset(){
     triangulation.reset();
}

void avatarLogo::formaTriangulo(){
    triangulos.clear();
    puntosEstado.clear();
    // triangulo invertido
    ofVec3f pA = ofVec3f(-50, 30, pointRandom(-20,20).x);
    ofVec3f pB = ofVec3f(0, -50,pointRandom(-20,20).x);
    ofVec3f pC = ofVec3f(50, 30,pointRandom(-20,20).x);
    ofVec3f pAinvert = ofVec3f(-50, -30,pointRandom(-20,20).x);
    ofVec3f pBinvert = ofVec3f(0, 50,pointRandom(-20,20).x);
    ofVec3f pCinvert = ofVec3f(50, -30,pointRandom(-20,20).x);
    
    ofVec3f pAB = ofRandom(0.3,0.8) * (pB - pA) + pA;
    ofVec3f pBC = ofRandom(0.3,0.8) * (pC - pB) + pB;
    ofVec3f pCA = ofRandom(0.3,0.8) * (pA - pC) + pC;
    triangulos.push_back(pAinvert);
    puntosEstado.push_back(1);
    triangulos.push_back(pBinvert);
    puntosEstado.push_back(1);
    triangulos.push_back(pCinvert);
    puntosEstado.push_back(1);
    
    // triangulo base
    float ss = 4;
    triangulos.push_back(pA*ss);
    puntosEstado.push_back(0);
    triangulos.push_back(pAB*ss+pointRandom(-10,10));
    puntosEstado.push_back(0);
    triangulos.push_back(pB*ss);
    puntosEstado.push_back(0);
    triangulos.push_back(pBC*ss+pointRandom(-10,10));
    puntosEstado.push_back(0);
    triangulos.push_back(pC*ss);
    puntosEstado.push_back(0);
    triangulos.push_back(pCA*ss+pointRandom(-10,10));
    puntosEstado.push_back(0);
    ss = 5;
    triangulos.push_back(pA*ss);
    puntosEstado.push_back(2);
    triangulos.push_back(pAB*ss+pointRandom(-10,10));
    puntosEstado.push_back(1);
    triangulos.push_back(pB*ss);
    puntosEstado.push_back(2);
    triangulos.push_back(pBC*ss+pointRandom(-10,10));
    puntosEstado.push_back(1);
    triangulos.push_back(pC*ss);
    puntosEstado.push_back(2);
    triangulos.push_back(pCA*ss+pointRandom(-10,10));
    puntosEstado.push_back(1);
    ss = 4;
    triangulos.push_back(pAinvert*ss);
    puntosEstado.push_back(1);
    triangulos.push_back(pBinvert*ss);
    puntosEstado.push_back(1);
    triangulos.push_back(pCinvert*ss);
    puntosEstado.push_back(1);

}

void avatarLogo::formaHexagono(){
    triangulos.clear();
    puntosEstado.clear();
    
    ofVec3f pA = ofVec3f(-50, 30, pointRandom(-20,20).x);
    ofVec3f pB = ofVec3f(0, -50,pointRandom(-20,20).x);
    ofVec3f pC = ofVec3f(50, 30,pointRandom(-20,20).x);
    ofVec3f pAinvert = ofVec3f(-50, -30,pointRandom(-20,20).x);
    ofVec3f pBinvert = ofVec3f(0, 50,pointRandom(-20,20).x);
    ofVec3f pCinvert = ofVec3f(50, -30,pointRandom(-20,20).x);
    
    ofVec3f pAB = ofRandom(0.3,0.8) * (pB - pA) + pA;
    ofVec3f pBC = ofRandom(0.3,0.8) * (pC - pB) + pB;
    ofVec3f pCA = ofRandom(0.3,0.8) * (pA - pC) + pC;
    
    triangulos.push_back(pAinvert);
    puntosEstado.push_back(1);
    triangulos.push_back(pBinvert);
    puntosEstado.push_back(1);
    triangulos.push_back(pCinvert);
    puntosEstado.push_back(1);
    
   // glBegin(GL_POLYGON);
    ofPushMatrix();
    ofRotateZ(30);
    
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    ofMatrix4x4 mat(m);
    ofPopMatrix();
    
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    ofMatrix4x4 matDefault(m);
    float ss = 180;
    for(int i = 0; i < 6; ++i) {
        
         pA = ofVec3f(sin(i/6.0*2*PI)*ss, cos(i/6.0*2*PI)*ss, pointRandom(-50,50).x);
        triangulos.push_back(pA  * mat.getRotate());
        puntosEstado.push_back(1);
    }
    ss = 230;
    
   // GLfloat m[16];
    ofPushMatrix();
    ofRotateZ(ofRandom(120));
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
   //  mat(m);
    ofPopMatrix();
    
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    // matDefault(m);
    
    
    for(int i = 0; i < 3; ++i) {
        
        pA = ofVec3f(sin(i/3.0*2*PI)*ss, cos(i/3.0*2*PI)*ss, pointRandom(-50,50).x);
        triangulos.push_back(pA  * mat.getRotate() + pointRandom(-50,50));
        puntosEstado.push_back(1);
    }
  //  glEnd();


    
}

void avatarLogo::formaCirculo(){
    
    triangulos.clear();
    puntosEstado.clear();
    
    ofVec3f pA = ofVec3f(0, 80, pointRandom(-20,0).x);
    ofVec3f pB = ofVec3f(80, 0,pointRandom(20,20).x);
    ofVec3f pC = ofVec3f(0, -80,pointRandom(-20,20).x);
    ofVec3f pD = ofVec3f(-80, 0,pointRandom(-20,20).x);
   
    triangulos.push_back(pA);
    puntosEstado.push_back(2);
    triangulos.push_back(pB);
    puntosEstado.push_back(2);
    triangulos.push_back(pC);
    puntosEstado.push_back(2);
    triangulos.push_back(pD);
    puntosEstado.push_back(2);
    
    float ss = 180;
    for(int i = 0; i < 10; ++i) {
        
        pA = ofVec3f(sin(i/10.0*2*PI)*ss, cos(i/10.0*2*PI)*ss, pointRandom(-50,50).x);
        triangulos.push_back(pA+pointRandom(-20,20));
        puntosEstado.push_back(1);
        
       
    }
    
    for(int i = 1; i < 3; ++i) {
        ss = 240;
        float ran =ofRandom(5)+i*5;
        pA = ofVec3f(sin(ran/10.0*2*PI)*ss, cos(ran/10.0*2*PI)*ss, pointRandom(-50,50).x);
        triangulos.push_back(pA+pointRandom(-20,20));
        puntosEstado.push_back(2);
        
        
    }
    
    
    
    
}
void avatarLogo::formaCuadrado(){
    triangulos.clear();
    puntosEstado.clear();
    
    ofVec3f pA = ofVec3f(0, 80, pointRandom(-20,0).x);
    ofVec3f pB = ofVec3f(80, 0,pointRandom(20,20).x);
    ofVec3f pC = ofVec3f(0, -80,pointRandom(-20,20).x);
    ofVec3f pD = ofVec3f(-80, 0,pointRandom(-20,20).x);
    
    triangulos.push_back(pA);
    puntosEstado.push_back(2);
    triangulos.push_back(pB);
    puntosEstado.push_back(2);
    triangulos.push_back(pC);
    puntosEstado.push_back(2);
    triangulos.push_back(pD);
    puntosEstado.push_back(2);
    
    ofPushMatrix();
    float rr =ofRandom(180);
    ofRotateZ(rr);
    
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    ofMatrix4x4 mat(m);
    ofPopMatrix();
    
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    ofMatrix4x4 matDefault(m);
    
    float ss = 220;
    for(int i = 0; i < 4; ++i) {
        
        pA = ofVec3f(sin(i/4.0*2*PI)*ss, cos(i/4.0*2*PI)*ss, pointRandom(-50,50).x);
        triangulos.push_back(pA  * mat.getRotate());
        puntosEstado.push_back(1);
    }
    
    ofPushMatrix();
    ofRotateZ(rr+45);
    
    GLfloat m2[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m2);
    ofMatrix4x4 mat2(m2);
    ofPopMatrix();
    
    glGetFloatv(GL_MODELVIEW_MATRIX, m2);
    ofMatrix4x4 matDefault2(m2);

    
     ss = 220;
    for(int i = 0; i < 4; ++i) {
        
        pA = ofVec3f(sin(i/4.0*2*PI)*ss, cos(i/4.0*2*PI)*ss, pointRandom(-50,50).x);
        triangulos.push_back(pA  * mat2.getRotate());
        puntosEstado.push_back(2);
    }

    
}


ofVec3f avatarLogo::pointRandom(float min, float max){
    return ofVec3f(ofRandom(min, max), ofRandom(min, max),ofRandom(min, max));
}





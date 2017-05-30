//
//  avatarLogo.hpp
//  example-simplePoints copy
//
//  Created by 3er on 5/25/17.
//
//
#include "ofMain.h"
#include "ofxDelaunay.h"


class avatarLogo{
public:
    void setup(int id);
    void update(ofPoint _pos, ofPoint _scale);
    void draw();
    
    void create();
    void reset();
    void formaTriangulo();
    void formaHexagono();
    void formaCirculo();
    void formaCuadrado();
    ofVec3f pointRandom(float min, float max);
    
    ofxDelaunay triangulation;
    ofMesh  meshAvatar;
    int forma;
    vector<ofPoint> triangulos;
    vector<int> puntosEstado;
    vector<ofImage> logos;
    ofShader shader;
    
    vector<ofColor> colores;
	int id;
	ofPoint pos;
	ofPoint size;
	float vida;
 };

//
//  character.cpp
//  popping_bubbles
//
//  Created by William Jattin on 2/22/17.
//
//

#include "character.hpp"
Character::Character(){}
void Character::setup(){
    x = ofGetWidth()/2;
    y = ofGetHeight()-100;
    //speedX = ofRandom(-10, 10);           // and random speed and direction
    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
}

//------------------------------------------------------------
void Character::draw(){
    ofSetColor(color);
    ofDrawBox(x, y, 100, 100);
}
void Character::update(float dt){
}

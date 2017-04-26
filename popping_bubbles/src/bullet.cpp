//
//  bullet.cpp
//  popping_bubbles
//
//  Created by William Jattin on 3/5/17.
//
//

#include "bullet.hpp"
Bullet::Bullet(){}

void Bullet::setup(){
    x = ofGetWidth()/2;
    y = ofGetHeight()-100;
    //speedX = ofRandom(-10, 10);           // and random speed and direction
    color.set(0,0,0);
}

//------------------------------------------------------------
void Bullet::draw(){
    ofSetColor(color);
    ofDrawBox(x, y, 30, 30);
}
void Bullet::update(bool shoot){
    if(shoot){
        if(this->y > 200 ){
        this->y = this->y * 0.9;
        } else { shoot = false;  }
    } if((shoot == false) && (this->y < (ofGetHeight()-100))) {
        this->y = this->y * 1.1;
    } else if((shoot == false) && (this->y > (ofGetHeight()-100))) {
        this->y = ofGetHeight()-100;
    }
}

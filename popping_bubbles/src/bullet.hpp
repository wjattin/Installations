//
//  bullet.hpp
//  popping_bubbles
//
//  Created by William Jattin on 3/5/17.
//
//

#ifndef bullet_hpp
#define bullet_hpp

#include "ofMain.h"

class Bullet{
    
public:
    
    Bullet();
    virtual ~Bullet(){};
    // methods, equivalent to specific functions of your class objects
    void setup();	// setup method, use this to setup your object's initial state
    void update(bool shoot);  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing
    void keyPressed();    // draw method, this where you'll do the object's drawing
    // variables
    float x;        // position
    float y;
    float speedX;
    ofColor color;  // color using ofColor type
    
    
protected:
private:
};

#endif /* bullet_hpp */

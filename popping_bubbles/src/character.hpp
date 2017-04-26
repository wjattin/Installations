//
//  character.hpp
//  popping_bubbles
//
//  Created by William Jattin on 2/22/17.
//
//

#ifndef character_hpp
#define character_hpp


#include "ofMain.h"
#include "character.hpp"

class Character{
    
public:

    Character();
    virtual ~Character(){};
    // methods, equivalent to specific functions of your class objects
    void setup();	// setup method, use this to setup your object's initial state
    void update(float dt);  // update method, used to refresh your objects properties
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


#endif /* character_hpp */

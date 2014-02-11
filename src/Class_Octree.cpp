/*
 * Class_Octree.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: Edoardo Lombardi
 */

// =================================================================================== //
// INCLUDES                                                                            //
// =================================================================================== //
#include "Class_Octree.hpp"

// =================================================================================== //
// CONSTRUCTORS                                                                        //
// =================================================================================== //
Class_Octant::Class_Octant(){
	x = y = z = 0;
	level = 0;
};

Class_Octant::Class_Octant(const int8_t lev){
	x = y = z = 0;
	level = lev;
};

// =================================================================================== //
// METHODS                                                                        //
// =================================================================================== //
int Class_Octant::mylevel(){

	return int(level);

}

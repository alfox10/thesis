// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
// Include GLEW
#include <glew.h>

// Include GLFW
#include <glfw3.h>


// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm; 

//bullet
#include <btBulletDynamicsCommon.h>

//defines
/*
#define CASTLE_HEIGHT 5
#define CASTLE_WIDTH 2
#define CASTLE_DEPTH 2
#define CASTLE_WALL_HEIGHT 4
#define CASTLE_WALL_WIDTH 5
#define CASTLE_WALL_DEPTH 1
#define CASTLE_WALL_WIDTH_T 1
#define CASTLE_WALL_DEPTH_T 5
#define CASTLE_PILLAR_COUNT 4
#define CASTLE_WALL_COUNT 4
*/
// castle strings
#define CASTLE_OBJ_NAME "castle.obj"
#define CASTLE_OBJ_MTL "castle.mtl"
#define CASTLE_OBJ_POS "mCastlePos.alfox"
#define CASTLE_OBJ_NUM 56
//cone Radius and Height
#define CONE_RADIUS 1.282
#define CONE_HEIGHT 2.563
//boxs Width Height Depth
#define BOX_W 3.0
#define BOX_H 0.8
#define BOX_D 0.8
// cylinder Width Height Depth
#define CYL_W 2.0
#define CYL_H 1.0
#define CYL_D 2.0
//balls strings
#define BALL_OBJ_NAME "ball.obj"
#define BALL_OBJ_MTL "ball.mtl"
#define BALL_OBJ_POS "mPosBall.alfox"
//screen define
#define WINDOW_WIDTH 1024 //if change, also change constant in controls.cpp
#define WINDOW_HEIGHT 768 //if change, also change constant in controls.cpp

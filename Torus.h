#pragma once

#ifndef TORUS_H
#define TORUS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

using namespace std;

class Torus : public Mesh {
public:
    Torus(Shader& litShdr, Shader& ltCUBEshdr, unsigned int difMap, unsigned int specMap) {
        this->lightingShader = litShdr;
        this->lightCUBEshader = ltCUBEshdr;
        this->diffuseMap = difMap;
        this->specularMap = specMap;
        buildMesh(this->shapeMesh, this->vertsVector);
    };

protected:

private:

    vector<GLfloat> vertsVector = {


        // front 
        0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 
       -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
       -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
       -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
        0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
        0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 

        // back 
       -0.5f,  0.5f, -0.5f,      0.2f, 0.7f,  // V4 
        0.5f,  0.5f, -0.5f,      0.8f, 0.7f,  // V5 
        0.5f, -0.5f, -0.5f,      0.8f, 1.0f,  // V6 
        0.5f, -0.5f, -0.5f,      0.8f, 1.0f,  // V6 
       -0.5f, -0.5f, -0.5f,      0.2f, 1.0f,  // V7 
       -0.5f,  0.5f, -0.5f,      0.2f, 0.7f,  // V4 

        // left 
       -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
       -0.5f,  0.5f, -0.5f,      0.0f, 0.5f,  // V4 
       -0.5f, -0.5f, -0.5f,      0.0f, 0.2f,  // V7 
       -0.5f, -0.5f, -0.5f,      0.0f, 0.2f,  // V7 
       -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
       -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 

        // right 
        0.5f,  0.5f, -0.5f,      1.0f, 0.5f,  // V5 
        0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 
        0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
        0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
        0.5f, -0.5f, -0.5f,      1.0f, 0.2f,  // V6 
        0.5f,  0.5f, -0.5f,      1.0f, 0.5f,  // V5 

        // top 
        0.5f,  0.5f, -0.5f,      0.8f, 0.7f,  // V5 
       -0.5f,  0.5f, -0.5f,      0.2f, 0.7f,  // V4 
       -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
       -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
        0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 
        0.5f,  0.5f, -0.5f,      0.8f, 0.7f,  // V5 

        // bottom 
        0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
       -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
       -0.5f, -0.5f, -0.5f,      0.2f, 0.0f,  // V7 
       -0.5f, -0.5f, -0.5f,      0.2f, 0.0f,  // V7 
        0.5f, -0.5f, -0.5f,      0.8f, 0.0f,  // V6 
        0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
    };

    //vector<GLshort> verticesVector = {
    //    // CUBE //////////////////////////////
    //    0, 1, 2,       // FT1
    //    3, 4, 5,       // FT2
    //    6, 7, 8,       // BaT1
    //    9, 10, 11,     // BaT2
    //    12, 13, 14,    // LT1
    //    15, 16, 17,    // LT2
    //    18, 19, 20,    // RT1
    //    21, 22, 23,    // RT2
    //    24, 25, 26,    // TT1
    //    27, 28, 29,    // TT2
    //    30, 31, 32,    // BoT1
    //    33, 34, 35     // BoT2
    //};
};

#endif
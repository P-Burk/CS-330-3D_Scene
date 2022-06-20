#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

using namespace std;

class Cube : public Mesh {
    public:
        Cube(Shader& litShdr, Shader& ltCUBEshdr, unsigned int difMap, unsigned int specMap) {
            this->lightingShader = litShdr;
            this->lightCUBEshader = ltCUBEshdr;
            this->diffuseMap = difMap;
            this->specularMap = specMap;
            buildMesh(this->shapeMesh, this->vertsVector);
        };

    protected:

    private:

        vector<GLfloat> vertsVector = {
            // CUBE ////////////////////////////////////////////////////////////////////////////////////
            //    v4----- v5
            //   /|      /|
            //  v0------v1|
            //  | |     | |
            //  | |v7---|-|v6
            //  |/      |/
            //  v3------v2

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
};
#endif
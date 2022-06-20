#ifndef SPEAKER_H
#define SPEAKER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

using namespace std;

class Speaker : public Mesh {
    public:
        Speaker(Shader& litShdr, Shader& ltCUBEshdr, unsigned int difMap, unsigned int specMap) {
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
            0.5f,  0.5f,  0.5f,      0.691f, 0.497f,  // V1 
           -0.5f,  0.5f,  0.5f,      0.312f, 0.497f,  // V0 
           -0.5f, -0.5f,  0.5f,      0.312f, 0.281f,  // V3 
           -0.5f, -0.5f,  0.5f,      0.312f, 0.281f,  // V3 
            0.5f, -0.5f,  0.5f,      0.691f, 0.281f,  // V2 
            0.5f,  0.5f,  0.5f,      0.691f, 0.497f,  // V1 

            // back 
           -0.5f,  0.5f, -0.5f,      0.312f, 0.778f,  // V4 
            0.5f,  0.5f, -0.5f,      0.691f, 0.778f,  // V5 
            0.5f, -0.5f, -0.5f,      0.691f, 1.000f,  // V6 
            0.5f, -0.5f, -0.5f,      0.691f, 1.000f,  // V6 
           -0.5f, -0.5f, -0.5f,      0.312f, 1.000f,  // V7 
           -0.5f,  0.5f, -0.5f,      0.312f, 0.778f,  // V4 

            // left 
           -0.5f,  0.5f,  0.5f,      0.312f, 0.497f,  // V0 
           -0.5f,  0.5f, -0.5f,      0.000f, 0.497f,  // V4 
           -0.5f, -0.5f, -0.5f,      0.000f, 0.281f,  // V7 
           -0.5f, -0.5f, -0.5f,      0.000f, 0.281f,  // V7 
           -0.5f, -0.5f,  0.5f,      0.312f, 0.281f,  // V3 
           -0.5f,  0.5f,  0.5f,      0.312f, 0.497f,  // V0 

            // right 
            0.5f,  0.5f, -0.5f,      1.000f, 0.497f,  // V5 
            0.5f,  0.5f,  0.5f,      0.691f, 0.497f,  // V1 
            0.5f, -0.5f,  0.5f,      0.691f, 0.281f,  // V2 
            0.5f, -0.5f,  0.5f,      0.691f, 0.281f,  // V2 
            0.5f, -0.5f, -0.5f,      1.000f, 0.281f,  // V6 
            0.5f,  0.5f, -0.5f,      1.000f, 0.497f,  // V5 

            // top 
            0.5f,  0.5f, -0.5f,      0.691f, 0.778f,  // V5 
           -0.5f,  0.5f, -0.5f,      0.312f, 0.778f,  // V4 
           -0.5f,  0.5f,  0.5f,      0.312f, 0.497f,  // V0 
           -0.5f,  0.5f,  0.5f,      0.312f, 0.497f,  // V0 
            0.5f,  0.5f,  0.5f,      0.691f, 0.497f,  // V1 
            0.5f,  0.5f, -0.5f,      0.691f, 0.778f,  // V5 

            // bottom 
            0.5f, -0.5f,  0.5f,      0.691f, 0.281f,  // V2 
           -0.5f, -0.5f,  0.5f,      0.312f, 0.281f,  // V3 
           -0.5f, -0.5f, -0.5f,      0.312f, 0.000f,  // V7 
           -0.5f, -0.5f, -0.5f,      0.312f, 0.000f,  // V7 
            0.5f, -0.5f, -0.5f,      0.691f, 0.000f,  // V6 
            0.5f, -0.5f,  0.5f,      0.691f, 0.281f,  // V2 
        };
};
#endif
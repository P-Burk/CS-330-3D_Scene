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
            //FRONT
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0
            -0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V2
            -0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V3
            -0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V3
             0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V1
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0

            -0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V2
            -0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V4
            -0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V5
            -0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V5
            -0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V3
            -0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V2

            -0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V4
            -0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V6
            -0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V7
            -0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V7
            -0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V5
            -0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V4

            -0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V6
            -0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V8
            -0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V9
            -0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V9
            -0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V7
            -0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V6

            -0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V8
             0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V10
             0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V11
             0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V11
            -0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V9
            -0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V8

             0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V10
             0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V12
             0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V13
             0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V13
             0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V11
             0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V10

             0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V12
             0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V14
             0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V15
             0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V15
             0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V13
             0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V12

             0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V14
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0
             0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V1
             0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V1
             0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V15
             0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V14

             //BACK
            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18
             0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V16
             0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V17
             0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V17
            -0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V19
            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18

            -0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V20
            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18
            -0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V19
            -0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V19
            -0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V21
            -0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V20

            -0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V22
            -0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V20
            -0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V21
            -0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V21
            -0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V23
            -0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V22

            -0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V24
            -0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V22
            -0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V23
            -0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V23
            -0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V25
            -0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V24

             0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V26
            -0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V24
            -0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V25
            -0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V25
             0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V27
             0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V26

             0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V28
             0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V26
             0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V27
             0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V27
             0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V29
             0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V28

             0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V30
             0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V28
             0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V29
             0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V29
             0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V31
             0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V30

             0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V16
             0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V30
             0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V31
             0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V31
             0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V17
             0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V16

            //OUTSIDE SIDES
            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18
            -0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V2
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0
             0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V16
            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18

            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18
            -0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V20
            -0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V4
            -0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V4
            -0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V2
            -0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V18

            -0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V20
            -0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V22
            -0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V6
            -0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V6
            -0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V4
            -0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V20

            -0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V22
            -0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V24
            -0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V8
            -0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V8
            -0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V6
            -0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V22

            -0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V24
             0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V26
             0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V10
             0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V10
            -0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V8
            -0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V24

             0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V26
             0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V28
             0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V12
             0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V12
             0.3f, -0.8f,  0.5f,    0.0f, 0.0f,  //V10
             0.3f, -0.8f, -0.5f,    0.0f, 0.0f,  //V26

             0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V28
             0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V30
             0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V14
             0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V14
             0.8f, -0.3f,  0.5f,    0.0f, 0.0f,  //V12
             0.8f, -0.3f, -0.5f,    0.0f, 0.0f,  //V28

             0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V30
             0.3f,  0.7f, -0.5f,    0.0f, 0.0f,  //V16
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0
             0.3f,  0.7f,  0.5f,    0.0f, 0.0f,  //V0
             0.8f,  0.2f,  0.5f,    0.0f, 0.0f,  //V14
             0.8f,  0.2f, -0.5f,    0.0f, 0.0f,  //V30

            //INSIDED SIDES
             0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V17
             0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V1
            -0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V3
            -0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V3
            -0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V19
             0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V17

            -0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V19
            -0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V3
            -0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V5
            -0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V5
            -0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V21
            -0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V19

            -0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V21
            -0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V5
            -0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V7
            -0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V7
            -0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V23
            -0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V21

            -0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V23
            -0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V7
            -0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V9
            -0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V9
            -0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V25
            -0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V23

            -0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V25
            -0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V9
             0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V11
             0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V11
             0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V27
            -0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V25

             0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V27
             0.3f, -0.6f,  0.5f,    0.0f, 0.0f,  //V11
             0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V13
             0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V13
             0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V29
             0.3f, -0.6f, -0.5f,    0.0f, 0.0f,  //V27

             0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V29
             0.6f, -0.3f,  0.5f,    0.0f, 0.0f,  //V13
             0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V15
             0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V15
             0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V31
             0.6f, -0.3f, -0.5f,    0.0f, 0.0f,  //V29

             0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V31
             0.6f,  0.2f,  0.5f,    0.0f, 0.0f,  //V15
             0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V1
             0.3f,  0.5f,  0.5f,    0.0f, 0.0f,  //V1
             0.3f,  0.5f, -0.5f,    0.0f, 0.0f,  //V17
             0.6f,  0.2f, -0.5f,    0.0f, 0.0f,  //V31

        };
};

#endif
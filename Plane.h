#ifndef PLANE_H
#define PLANE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

using namespace std;

class Plane : public Mesh {
	public:
        Plane(Shader& litShdr, Shader& ltCUBEshdr, unsigned int difMap, unsigned int specMap) {
            this->lightingShader = litShdr;
            this->lightCUBEshader = ltCUBEshdr;
            this->diffuseMap = difMap;
            this->specularMap = specMap;
            buildMesh(this->shapeMesh, this->vertsVector);
        };

	protected:


	private:

        // PLANE ///////////////////////////////////////////////////////////////////////
        //  v0-----v1
        //  |       | 
        //  |       |
        //  v3-----v2

        vector<GLfloat> vertsVector = {
             5.0f, -1.3f, -5.0f,      1.0f, 1.0f, // V1
            -5.0f, -1.3f, -5.0f,      0.0f, 1.0f, // V0
            -5.0f, -1.3f,  5.0f,      0.0f, 0.0f, // V3

            -5.0f, -1.3f,  5.0f,      0.0f, 0.0f, // V3
             5.0f, -1.3f,  5.0f,      1.0f, 0.0f, // V2
             5.0f, -1.3f, -5.0f,      1.0f, 1.0f, // V1
        };
};

#endif
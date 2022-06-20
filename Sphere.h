/****** CODE CITATION **************************************************************
* Title: OpenGL Sphere: Sphere.h
* Author: Song Ho Ahn
* Date Accessed: 18 June 2022
* Code Version: N/A
* Availability: https://songho.ca/opengl/gl_sphere.html
************************************************************************************/

#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

using namespace std;

class Sphere : public Mesh 
{
	public:
        Sphere(Shader& litShdr, Shader& ltCUBEshdr, unsigned int difMap, unsigned int specMap, float radius = 0.8f, int sectorCount = 36, int stackCount = 36);

	protected:

	private:
        float radius;
        int sectorCount;        //vertical slices                    
        int stackCount;         //horizontal slices

        //helper/builder vectors
        vector<GLfloat> vertices;
        vector<GLfloat> normals;
        vector<GLfloat> texCoords;

        // final vector of V, N, T info
        vector<GLfloat> interleavedVertices;     
        int interleavedStride = 32;

        void set(float radius, int sectorCount, int stackCount);
        void buildVerticesFlat();
        void buildInterleavedVertices();
        void clearArrays();
        vector<float> computeFaceNormal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3);

        //helper functions for building helper vectors
        void addVertex(float x, float y, float z);
        void addNormal(float x, float y, float z);
        void addTexCoord(float s, float t);

        void buildMesh(GLMesh& mesh, vector<GLfloat>& vertsVector);
        
};
#endif
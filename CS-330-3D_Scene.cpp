/*
 * Class: CS-330-T5621 Computer Graphics and Visualization
 * Instructor: Malcolm Wabara, M.S
 * Assignment: 6-5 Milestone: Lighting Complex Objects
 * Student: Preston Burkhardt
 * Date: 12 June 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <vector>

// GLM imports
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Lights.h"
#include "Sphere.h"


using namespace std;

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif


// initialize mesh and shader program variables
GLuint shapeProgramID;
GLuint lampProgramID;
GLuint shapeProgramID2;
GLuint lampProgramID2;
GLuint textureID1;
GLuint textureID2;
GLuint textureID3;
glm::vec2 gUVScale(5.0f, 5.0f);
GLint gTexWrapMode = GL_REPEAT;

// constants for windown attributes
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char* const WINDOW_TITLE = "6-5 Milestone: Lighting Complex Objects";
//TODO: will need to bind specular and diffuse texture for each texture used in the scene
const char* woodTextureFile = "resources/textures/dark_wood.jpg";
const char* camBodyTextureFile = "resources/textures/Full_camera.png";
const char* camLensTextureFile = "resources/textures/Full_lens.png";
const char* holderTexture = "resources/textures/brick_wall.jpg";
const bool WIREFRAME_MODE = false;
float ROTATE_DEG = 0.0f;
float ROTATE_X = 1.0f;
float ROTATE_Y = 1.0f;
float ROTATE_Z = 1.0f;

// CAMERA VARIABLES
Camera camera(glm::vec3(0.0f, 3.0f, 10.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// perspective switch
bool perspectiveSwitch = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderCamMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch);
void renderSpkrMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch);
void renderPlaneMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch);
void renderCylinderMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch);
void renderSphereMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch);
void destroyShaderProgram(GLuint programID);
void mouseCameraMovement(GLFWwindow* window, double xPos, double yPos);
void scrollCameraMovement(GLFWwindow* window, double xPosOffset, double yPosOffset);
void scrollCameraSpeed(GLFWwindow* window, double xPosOffset, double yPosOffset);
void perspectiveToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
void flipImageVertically(unsigned char* image, int width, int height, int channels);
bool createTexture(const char* filename, GLuint& textureId);
void destroyTexture(GLuint textureId);
unsigned int loadTexture(char const* path);




int main() {
    cout << WINDOW_TITLE << endl;

    // initialize GLFW and specify versions + profile to be used
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creates window object
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window!!!!" << endl;
        glfwTerminate();
        return -1;
    }

    // sets window object as main context on current thread
    glfwMakeContextCurrent(window);
    // generate window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // camera control calls
    glfwSetCursorPosCallback(window, mouseCameraMovement);
    glfwSetScrollCallback(window, scrollCameraSpeed);


    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // settings so triangles are rendered on one side only
    // CCW winding order
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // initialize AFTER glewExperimental to avoid error "Access violation.... 0X00000000"
    //Cube cubeMesh;
    //Cylinder cylinderMesh;
    //Plane planeMesh;
    
    // Load texture
    if (!createTexture(woodTextureFile, textureID1)) {
        cout << "Failed to load texture " << woodTextureFile << endl;
        return EXIT_FAILURE;
    }
    // Load texture
    if (!createTexture(camBodyTextureFile, textureID2)) {
        cout << "Failed to load texture " << camBodyTextureFile << endl;
        return EXIT_FAILURE;
    }
    // Load texture
    if (!createTexture(camLensTextureFile, textureID3)) {
        cout << "Failed to load texture " << camLensTextureFile << endl;
        return EXIT_FAILURE;
    }

    /****** CODE CITATION **************************************************************
    * Title: Learn OpenGL: multiple_lights_exercise1.cpp
    * Author: Joey de Vries
    * Date Accessed: 14 June 2022
    * Code Version: N/A
    * Availability: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/multiple_lights.cpp
    ************************************************************************************/

    //positions of the point lights
    vector<glm::vec3> pointLightPositions = {
        glm::vec3(  0.0f,  3.5f,  -2.0f),
        glm::vec3(  4.0f,  1.5f,  0.0f),
        glm::vec3( -1.0f,  0.8f, -3.0f),
        glm::vec3(  2.0f,  0.5f,  0.0f)
    };
    glm::vec3 spotLightPos(0.0f, 6.0f, 13.0f);

    Shader lightingShader("include/multiple_lights.vs", "include/multiple_lights.fs");
    Shader lightCubeShader("include/light_cube.vs", "include/light_cube.fs");

    unsigned int cameraBodyDiffuseMap = loadTexture(camBodyTextureFile);
    unsigned int cameraBodySpecularMap = loadTexture(camBodyTextureFile);
    unsigned int cameraLensDiffuseMap = loadTexture(camLensTextureFile);
    unsigned int cameraLensSpecularMap = loadTexture(camLensTextureFile);
    unsigned int planeDiffuseMap = loadTexture(woodTextureFile);
    unsigned int planeSpecularMap = loadTexture(woodTextureFile);
    unsigned int holderDiffuseMap = loadTexture(holderTexture);
    unsigned int holderSpecularMap = loadTexture(holderTexture);

    //NOTE: for debugging
    //unsigned int cameraBodyDiffuseMap = textureID2;
    //unsigned int cameraBodySpecularMap = textureID2;
    //unsigned int cameraLensDiffuseMap = textureID3;
    //unsigned int cameraLensSpecularMap = textureID3;
    //unsigned int planeDiffuseMap = textureID1;
    //unsigned int planeSpecularMap = textureID1;
    //unsigned int holderDiffuseMap = textureID1;
    //unsigned int holderSpecularMap =textureID1;

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    Lights lights(lightingShader, lightCubeShader, camera, planeDiffuseMap, planeSpecularMap, pointLightPositions, spotLightPos);

    Cube cubeMesh(lightingShader, lightCubeShader, cameraBodyDiffuseMap, cameraBodySpecularMap);
    Cylinder cylinderMesh(lightingShader, lightCubeShader, cameraLensDiffuseMap, cameraLensSpecularMap);
    Plane planeMesh(lightingShader, lightCubeShader, planeDiffuseMap, planeSpecularMap);
    Cube speakerMesh(lightingShader, lightCubeShader, holderDiffuseMap, holderSpecularMap);
    Sphere aSphere(lightingShader, lightCubeShader, holderDiffuseMap, holderSpecularMap);


    /******* END OF CITED CODE **********************************************************/


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // We set the texture as texture unit 0
    //glUniform1i(glGetUniformLocation(shapeProgramID, "TEXTURE"), 0);

    // while loop to continually render until user closes window
    while (!glfwWindowShouldClose(window)) {

        // enable z-depth buffer
        glEnable(GL_DEPTH_TEST);

        // clear the background frame and z-depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // frame timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input
        processInput(window);  

        // create projection, view, and model matrices, then render the lights
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        lights.renderLights(pointLightPositions, projection, view, model, perspectiveSwitch);

        //render shapes
        renderPlaneMesh(planeMesh.getShapeMesh(), lightingShader, planeDiffuseMap, planeSpecularMap, window, WIREFRAME_MODE, perspectiveSwitch);
        renderCamMesh(cubeMesh.getShapeMesh(), lightingShader, cameraBodyDiffuseMap, cameraBodySpecularMap, window, WIREFRAME_MODE, perspectiveSwitch);
        renderCylinderMesh(cylinderMesh.getShapeMesh(), lightingShader, cameraLensDiffuseMap, cameraLensSpecularMap, window, WIREFRAME_MODE, perspectiveSwitch);
        renderSpkrMesh(speakerMesh.getShapeMesh(), lightingShader, holderDiffuseMap, holderSpecularMap, window, WIREFRAME_MODE, perspectiveSwitch);
        renderSphereMesh(aSphere.getShapeMesh(), lightingShader, holderDiffuseMap, holderSpecularMap, window, WIREFRAME_MODE, perspectiveSwitch);
        

        glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
        glfwPollEvents();
    }


    // EXIT CODE /////////////////////////////////////////////
    cylinderMesh.destoryMesh();
    cubeMesh.destoryMesh();
    planeMesh.destoryMesh();
    glfwTerminate();
    return 0;
}

// function used for window resizing
void framebuffer_size_callback(GLFWwindow* window, int passedWidth, int passedHeight) {
    glViewport(0, 0, passedWidth, passedHeight);

}

// processes user input
// closes the OpenGL window when ESC is pressed
// use WASD for camera movement, Q E for up and down
void processInput(GLFWwindow* window) {

    // close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    
    // perspective switch
    glfwSetKeyCallback(window, perspectiveToggle);
}

// render the cube
void renderCamMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch) 
{
    
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    //////////// SHAPE DRAW FUNCTIONS ////////////
    glUseProgram(lightShader.getID());

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(3.3f, 2.5f, 1.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);
    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 4.0f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = rotation * scale * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    glm::mat4 projection;
    if (perspectiveSwitch) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(lightShader.getID(), "model");
    GLint viewLoc = glGetUniformLocation(lightShader.getID(), "view");
    GLint projLoc = glGetUniformLocation(lightShader.getID(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint UVScaleLoc = glGetUniformLocation(lightShader.getID(), "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));


    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw the triangle.
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
    glUseProgram(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

void renderSpkrMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch) {
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    //////////// SHAPE DRAW FUNCTIONS ////////////
    glUseProgram(lightShader.getID());

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(3.9f, 2.7f, 3.3f));
    //glm::mat4 scale = glm::scale(glm::vec3(5.5f, 2.7f, 3.5f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);
    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(3.5f, 0.0f, 0.5f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = translation  * rotation * scale;
    //glm::mat4 model = rotation * scale * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    glm::mat4 projection;
    if (perspectiveSwitch) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    } else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(lightShader.getID(), "model");
    GLint viewLoc = glGetUniformLocation(lightShader.getID(), "view");
    GLint projLoc = glGetUniformLocation(lightShader.getID(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint UVScaleLoc = glGetUniformLocation(lightShader.getID(), "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));


    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw the triangle.
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
    glUseProgram(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

//TODO: fix cylinder renderer
// render the cylinder
void renderCylinderMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch) 
{
 
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    //////////// SHAPE DRAW FUNCTIONS ////////////
    glUseProgram(lightShader.getID());

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(1.8f, 2.0f, 2.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);
    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.23f, -0.1f, 2.7f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = scale * rotation * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection;
    if (perspectiveSwitch) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(lightShader.getID(), "model");
    GLint viewLoc = glGetUniformLocation(lightShader.getID(), "view");
    GLint projLoc = glGetUniformLocation(lightShader.getID(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint UVScaleLoc = glGetUniformLocation(lightShader.getID(), "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw the triangle.
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
    glUseProgram(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

void renderSphereMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch) {
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    //////////// SHAPE DRAW FUNCTIONS ////////////
    glUseProgram(lightShader.getID());

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);
    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(-2.0f, -0.1f, 0.0f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = rotation * scale * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    glm::mat4 projection;
    if (perspectiveSwitch) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    } else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(lightShader.getID(), "model");
    GLint viewLoc = glGetUniformLocation(lightShader.getID(), "view");
    GLint projLoc = glGetUniformLocation(lightShader.getID(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint UVScaleLoc = glGetUniformLocation(lightShader.getID(), "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));


    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw the triangle.
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
    glUseProgram(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

void mouseCameraMovement(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse == true) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xPosOffest = xPos - lastX;
    float yPosOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xPosOffest, yPosOffset);
}

// Use the scroll wheel for camera ZOOM //////////////////////////////////////////
void scrollCameraMovement(GLFWwindow* window, double xPosOffset, double yPosOffset) {
    camera.ProcessMouseScroll(yPosOffset);
}

// Use the scroll wheel for adjusting camera SPEED //////////////////////////////
void scrollCameraSpeed(GLFWwindow* window, double xPosOffset, double yPosOffset) {
    camera.ProcessMouseScroll_Speed(yPosOffset);
}

void perspectiveToggle(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        perspectiveSwitch = !perspectiveSwitch;
}

void renderPlaneMesh(const GLMesh& mesh, Shader lightShader, unsigned int diffuseMap, unsigned int specularMap, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspectiveSwitch) {
    
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    //////////// SHAPE DRAW FUNCTIONS ////////////
    glUseProgram(lightShader.getID());

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);
    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.6f, 0.0f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = scale * rotation * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    glm::mat4 projection;
    if (perspectiveSwitch) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    } else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(lightShader.getID(), "model");
    GLint viewLoc = glGetUniformLocation(lightShader.getID(), "view");
    GLint projLoc = glGetUniformLocation(lightShader.getID(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint UVScaleLoc = glGetUniformLocation(lightShader.getID(), "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));


    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw the triangle.
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
    glUseProgram(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}


// flips image vertically because images are loaded opposite of how OpenGL interprets Y-axis
void flipImageVertically(unsigned char* image, int width, int height, int channels) {
    for (int j = 0; j < height / 2; ++j) {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i) {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}

// creates the texture
bool createTexture(const char* filename, GLuint& textureId) {

    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image) {

        // flip the image on y-axis
        flipImageVertically(image, width, height, channels);

        // generate and bind texture
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // logic for handling 3 or 4 channel images
        if (channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        } else if (channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        } else {
            cout << "No support for images with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);

        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    return false;
}

// destroys texture
void destroyTexture(GLuint textureId) {
    glGenTextures(1, &textureId);
}

/****** CODE CITATION **************************************************************
* Title: Learn OpenGL: multiple_lights_exercise1.cpp
* Author: Joey de Vries
* Date Accessed: 14 June 2022
* Code Version: N/A
* Availability: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/multiple_lights.cpp
************************************************************************************/

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        flipImageVertically(data, width, height, nrComponents);
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
/*
 * Class: CS-330-T5621 Computer Graphics and Visualization
 * Instructor: Malcolm Wabara, M.S
 * Assignment: 4-5 Milestone: Interactivity in a 3D Scene
 * Student: Preston Burkhardt
 * Date: 29 May 2022
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

#include "Camera.h"

using namespace std;

// used for storage of the IDs for the VAO and VBO
struct GLMesh {
    GLuint vao;         // Handle for the vertex array object
    GLuint vbo[2];      // Handle for the vertex buffer object
    GLuint nVertices;   // Number of vertices of the mesh
};

// initialize mesh and shader program variables
GLMesh gMesh;
GLMesh cubeMesh;
GLMesh cylinderMesh;
GLMesh planeMesh;
GLuint gProgramID;
GLuint textureID;

// constants for windown attributes
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char* const WINDOW_TITLE = "4-5 Milestone: Interactivity in a 3D Scene";
const char* textureFile = "resources/textures/brick_wall.jpg";
const bool WIREFRAME_MODE = false;
float ROTATE_DEG = 0.0f;
float ROTATE_X = 1.0f;
float ROTATE_Y = 1.0f;
float ROTATE_Z = 1.0f;

// camera and timing
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// perspective switch
bool perspectiveSwitch = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void createCubeMesh(GLMesh& mesh, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfixed edgeLen);
void createCylinderMesh(GLMesh& mesh, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfixed edgeLen);
void createPlaneMesh(GLMesh& mesh);
void renderCubeMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective);
void renderCylinderMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective);
void renderPlaneMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective);
void destoryMesh(GLMesh& mesh);
bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void destroyShaderProgram(GLuint programID);
void mouseCameraMovement(GLFWwindow* window, double xPos, double yPos);
void scrollCameraMovement(GLFWwindow* window, double xPosOffset, double yPosOffset);
void scrollCameraSpeed(GLFWwindow* window, double xPosOffset, double yPosOffset);
void perspectiveToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
void flipImageVertically(unsigned char* image, int width, int height, int channels);
bool createTexture(const char* filename, GLuint& textureId);
void destroyTexture(GLuint textureId);


// vertex shader source code
const char* vertexShaderSource = "#version 440 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 2) in vec2 textureCoordinate;\n"
"out vec2 vertexTextureCoordinate;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(position, 1.0f);\n"    // transforms vertex data 
"   vertexTextureCoordinate = textureCoordinate;\n"
"}\n\0";

// fragment shader source code
const char* fragmentShaderSource = "#version 440 core\n"
"in vec2 vertexTextureCoordinate;\n"
"out vec4 fragmentColor;\n"
"uniform sampler2D TEXTURE;\n"

"void main()\n"
"{\n"
"   fragmentColor = texture(TEXTURE, vertexTextureCoordinate);\n"
"}\n\0";


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


    // create mesh and shader program
    //createMesh(gMesh);
    createCubeMesh(cubeMesh, 0, 0, 0, 1);
    createCylinderMesh(cylinderMesh, 0, 0, 0, 1);
    createPlaneMesh(planeMesh);
    createShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramID);

    // Load texture
    if (!createTexture(textureFile, textureID)) {
        cout << "Failed to load texture " << textureFile << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramID);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramID, "TEXTURE"), 0);

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

        processInput(window);                       // process input

        //renderMesh(gMesh, gProgramID, window, WIREFRAME_MODE);      // render the frame
        renderCubeMesh(cubeMesh, gProgramID, window, WIREFRAME_MODE, perspectiveSwitch);
        renderCylinderMesh(cylinderMesh, gProgramID, window, WIREFRAME_MODE, perspectiveSwitch);
        renderPlaneMesh(planeMesh, gProgramID, window, WIREFRAME_MODE, perspectiveSwitch);

        glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
        glfwPollEvents();
    }


    // EXIT CODE /////////////////////////////////////////////
    destoryMesh(gMesh);
    destroyShaderProgram(gProgramID);
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

void createCubeMesh(GLMesh& mesh, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfixed edgeLen) {
    GLfloat halfEdgeLen = edgeLen * 0.5;

    // cube ///////////////////////////////////////////////////////////////////////
    //    v4----- v5
    //   /|      /|
    //  v0------v1|
    //  | |     | |
    //  | |v7---|-|v6
    //  |/      |/
    //  v3------v2

    // Position and Color data
    GLfloat verts[] = {
        // CUBE ////////////////////////////////////////////////////////////////////////////////////
        // front
        xPos - halfEdgeLen, yPos + halfEdgeLen, zPos + halfEdgeLen,     1.0f, 0.0f, 0.0f, 1.0f, // 0
        xPos + halfEdgeLen, yPos + halfEdgeLen, zPos + halfEdgeLen,     0.0f, 1.0f, 0.0f, 1.0f, // 1
        xPos + halfEdgeLen, yPos - halfEdgeLen, zPos + halfEdgeLen,     0.0f, 0.0f, 1.0f, 1.0f, // 2
        xPos - halfEdgeLen, yPos - halfEdgeLen, zPos + halfEdgeLen,     1.0f, 1.0f, 1.0f, 1.0f, // 3

        // back
        xPos - halfEdgeLen, yPos + halfEdgeLen, zPos - halfEdgeLen,     1.0f, 1.0f, 0.0f, 1.0f, // 4
        xPos + halfEdgeLen, yPos + halfEdgeLen, zPos - halfEdgeLen,     0.0f, 1.0f, 1.0f, 1.0f, // 5
        xPos + halfEdgeLen, yPos - halfEdgeLen, zPos - halfEdgeLen,     1.0f, 0.0f, 1.0f, 1.0f, // 6
        xPos - halfEdgeLen, yPos - halfEdgeLen, zPos - halfEdgeLen,     0.5f, 1.0f, 1.0f, 1.0f, // 7
    };

    // Creates a buffer object for the indices
    GLshort vertices[] = {
        // CUBE //////////////////////////////
        0, 1, 3,  // FT1
        2, 1, 3,  // FT2
        4, 5, 7,  // BaT1
        6, 5, 7,  // BaT2
        0, 3, 4,  // LT1
        7, 3, 4,  // LT2
        1, 2, 5,  // RT1
        6, 2, 5,  // RT2
        4, 0, 5,  // TT1
        1, 0, 5,  // TT2
        2, 6, 3,  // BoT1
        7, 6, 3,   // BoT2
    };

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint strideLen = sizeof(float) * (vertexFloats + colorFloats);

    // vertex attribute pointer for position
    glVertexAttribPointer(0, vertexFloats, GL_FLOAT, GL_FALSE, strideLen, 0);
    glEnableVertexAttribArray(0);

    // vertex attribute pointer for color
    glVertexAttribPointer(1, colorFloats, GL_FLOAT, GL_FALSE, strideLen, (char*)(sizeof(float) * vertexFloats));
    glEnableVertexAttribArray(1);

}

// creates cylinder
void createCylinderMesh(GLMesh& mesh, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfixed edgeLen) {
    GLfloat halfEdgeLen = edgeLen * 0.5;

    // Position and Color data
    GLfloat verts[] = {
        // front
         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 0
         0.1f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 1
         0.3f,  0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 2
         0.4f,  0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 3
         0.5f,  0.1f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,   // 4
         0.5f, -0.1f, 0.5f,     0.0f, 1.0f, 1.0f, 1.0f,   // 5
         0.4f, -0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 6
         0.3f, -0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 7
         0.1f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 8
        -0.1f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 9
        -0.3f, -0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 10
        -0.4f, -0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 11
        -0.5f, -0.1f, 0.5f,     0.0f, 1.0f, 1.0f, 1.0f,   // 12
        -0.5f,  0.1f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,   // 13
        -0.4f,  0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 14
        -0.3f,  0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 15
        -0.1f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 16

        // back
         0.0f,  0.0f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 17
         0.1f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 18
         0.3f,  0.4f, -0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 19
         0.4f,  0.3f, -0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 20
         0.5f,  0.1f, -0.5f,     1.0f, 1.0f, 0.0f, 1.0f,   // 21
         0.5f, -0.1f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,   // 22
         0.4f, -0.3f, -0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 23
         0.3f, -0.4f, -0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 24
         0.1f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 25
        -0.1f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 26
        -0.3f, -0.4f, -0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 27
        -0.4f, -0.3f, -0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 28
        -0.5f, -0.1f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,   // 29
        -0.5f,  0.1f, -0.5f,     1.0f, 1.0f, 0.0f, 1.0f,   // 30
        -0.4f,  0.3f, -0.5f,     1.0f, 1.0f, 1.0f, 1.0f,   // 31
        -0.3f,  0.4f, -0.5f,     0.0f, 0.0f, 1.0f, 1.0f,   // 32
        -0.1f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,   // 33


    };

    // Creates a buffer object for the indices
    GLshort vertices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 9,
        0, 9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12, 13,
        0, 13, 14,
        0, 14, 15,
        0, 15, 16,
        0, 16, 1,

        17, 18, 19,
        17, 19, 20,
        17, 20, 21,
        17, 21, 22,
        17, 22, 23,
        17, 23, 24,
        17, 24, 25,
        17, 25, 26,
        17, 26, 27,
        17, 27, 28,
        17, 28, 29,
        17, 29, 30,
        17, 30, 31,
        17, 31, 32,
        17, 32, 33,
        17, 33, 18,

        2, 1, 19,
        18, 1, 19,
        3, 2, 20,
        19, 2, 20,
        4, 3, 21,
        20, 3, 21,
        5, 4, 22,
        21, 4, 22,
        6, 5, 23,
        22, 5, 23,
        7, 6, 24,
        23, 6, 24,
        8, 7, 25,
        24, 7, 25,
        9, 8, 26,
        25, 8, 26,
        10, 9, 27,
        26, 9, 27,
        11, 10, 28,
        27, 10, 28,
        12, 11, 29,
        28, 11, 29,
        13, 12, 30,
        29, 12, 30,
        14, 13, 31,
        30, 13, 31,
        15, 14, 32,
        31, 14, 32,
        16, 15, 33,
        32, 15, 33,
        1, 16, 18,
        33, 16, 18
    };


    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint strideLen = sizeof(float) * (vertexFloats + colorFloats);

    // vertex attribute pointer for position
    glVertexAttribPointer(0, vertexFloats, GL_FLOAT, GL_FALSE, strideLen, 0);
    glEnableVertexAttribArray(0);

    // vertex attribute pointer for color
    glVertexAttribPointer(1, colorFloats, GL_FLOAT, GL_FALSE, strideLen, (char*)(sizeof(float) * vertexFloats));
    glEnableVertexAttribArray(1);
}

// render the cube
void renderCubeMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective) {
    /*
    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // clear the background frame and z-depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    */

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(3.0f, 2.5f, 1.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);

    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));
    //rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = rotation * scale * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection;
    if (perspective) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used.
    glUseProgram(programID);

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(programID, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Draw the triangle.
    glDrawElements(GL_TRIANGLES, mesh.nVertices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

// render the cylinder
void renderCylinderMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective) {
    /*
    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // clear the background frame and z-depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    */
    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(1.5f, 1.5f, 2.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);

    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.5f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = scale * rotation * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection;
    if (perspective) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used.
    glUseProgram(programID);

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(programID, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Draw the triangle.
    glDrawElements(GL_TRIANGLES, mesh.nVertices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

// used for clearing the mesh
void destoryMesh(GLMesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbo);
}

// creates the shader program
bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programID) {

    // compilation and linkage error reporting
    int successNum = 0;
    char infoLog[512];

    // create shader program object
    programID = glCreateProgram();


    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);       // create vertex shader object
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);   // create fragment shader object

    glShaderSource(vertexShaderID, 1, &vtxShaderSource, NULL);      // get the shader source for vertex fragment
    glShaderSource(fragmentShaderID, 1, &fragShaderSource, NULL);   // get the shader source for fragment shader

    glCompileShader(vertexShaderID);                                // compile vertex shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &successNum);  // check for and print errors
    if (!successNum) {
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    glCompileShader(fragmentShaderID);                              // compile fragment shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &successNum);// check for and print errors
    if (!successNum) {
        glGetShaderInfoLog(fragmentShaderID, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    // attach compiled shaders to their respective shader programs
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);


    glLinkProgram(programID);                                       // links the shader program
    glGetProgramiv(programID, GL_LINK_STATUS, &successNum);         // check for and print errors
    if (!successNum) {
        glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return false;
    }

    glUseProgram(programID);

    return true;

}

// clears the shader program
void destroyShaderProgram(GLuint programID) {
    glDeleteProgram(programID);
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

void createPlaneMesh(GLMesh& mesh) {
    // PLANE ///////////////////////////////////////////////////////////////////////
    //     v0----v1
    //    /      / 
    //   /      /
    //  v3-----v2

    // Position and Color data
    GLfloat verts[] = {
        -5.0f, -1.3f,  5.0f,     1.0f, 1.0f, 1.0f, 1.0f, // 0
         5.0f, -1.3f,  5.0f,     1.0f, 1.0f, 1.0f, 1.0f, // 1
         5.0f, -1.3f, -5.0f,     1.0f, 1.0f, 1.0f, 1.0f, // 2
        -5.0f, -1.3f, -5.0f,     1.0f, 1.0f, 1.0f, 1.0f, // 3
    };

    // Creates a buffer object for the indices
    GLshort vertices[] = {
        0, 1, 3,  // T1
        2, 1, 3   // T2
    };

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint strideLen = sizeof(float) * (vertexFloats + colorFloats);

    // vertex attribute pointer for position
    glVertexAttribPointer(0, vertexFloats, GL_FLOAT, GL_FALSE, strideLen, 0);
    glEnableVertexAttribArray(0);

    // vertex attribute pointer for color
    glVertexAttribPointer(1, colorFloats, GL_FLOAT, GL_FALSE, strideLen, (char*)(sizeof(float) * vertexFloats));
    glEnableVertexAttribArray(1);
}

void renderPlaneMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective) {

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);

    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.5f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = scale * rotation * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    glm::mat4 projection;
    if (perspective) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used.
    glUseProgram(programID);

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(programID, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Draw the triangle.
    glDrawElements(GL_TRIANGLES, mesh.nVertices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
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
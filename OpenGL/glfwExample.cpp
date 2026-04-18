#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "png++/png.hpp"

#include "GLSL.h"
#include "PerspectiveCamera.h"
#include "Sphere.h"

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        exit (-1);
    }
    // throw std::runtime_error("Error! initialization of glfw failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    int winWidth = 800;
    int winHeight = 600;

    float aspectRatio = float(winWidth) / float(winHeight);

    
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(err != GLEW_OK) {
        std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
        exit(EXIT_FAILURE);
    }

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f); 

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    // Need to set a projection matrix that fits the aspect ratio set
    // by the window frame.
    //
    // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    float halfWidth = 15.0 / 2.0;
    float halfHeight = halfWidth / aspectRatio;
    glm::mat4 M_ortho = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 5.0f, -5.0f);

    // perspective matrix
    glm::mat4 M_perspective = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;

    //Initialize all my data and get it on the GPU

    // Texture Mapping Data
    std::string texFilename = "../OpenGL/textureAtlas.png";
    std::cout << "Reading texture map data from file: " << texFilename << std::endl;
    png::image<png::rgb_pixel> texPNGImage;
    texPNGImage.read(texFilename);

    int pngWidth = texPNGImage.get_width();
    int pngHeight = texPNGImage.get_height();

    std::vector<float> texData(pngHeight * pngWidth * 3);

    size_t idx = 0;
    for (size_t row = 0; row < pngHeight; ++row) {
        for (size_t col = 0; col < pngWidth; ++col) {
            png::rgb_pixel pixel = texPNGImage[pngHeight - row - 1][col]; // <-- notice the flip of height!!!
            texData[idx++] = pixel.red / 255.0f;
            texData[idx++] = pixel.green / 255.0f;
            texData[idx++] = pixel.blue / 255.0f;
        }
    }

    // Generate an OpenGL texture object and upload the texture data to the GPU
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                pngWidth, pngHeight, 
                0, GL_RGB, GL_FLOAT, texData.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    sivelab::GLSLObject shader;

    /*
    // Sphere data  
    std::vector<float> sphereVerts;
    std::vector<unsigned int> sphereIndices;
    Sphere sphere( 1.0f, vec3(0, 0, 0));
    sphere.getMeshData(sphereVerts, sphereIndices, 30);

    
    GLuint m_VBO, m_VAO, m_EBO;
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVerts.size() * sizeof(float), sphereVerts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    */

    // Triangle data
    std::vector<float> triVerts = {
        // Position (x,y,z)  // Normal (x,y,z)  // Texture Coords (u,v)
        -2.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.0f,
        2.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        2.0f,  2.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.5f,
        2.0f,  2.0f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.5f,
        -2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.5f,
        -2.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.0f,
        2.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.5f,
        6.0f,  -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.5f,
        6.0f,   2.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f,
        6.0f,   2.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f,
        2.0f,  2.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        2.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.5f
    };
    std::vector<unsigned int> triIndices = { 0, 1, 2,
                                            3, 4, 5,
                                            6, 7, 8,
                                            9, 10, 11 };

    // Create a new VAO/VBO for the Triangle
    GLuint triVAO, triVBO, triEBO;
    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &triVBO);
    glGenBuffers(1, &triEBO);

    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, triVerts.size() * sizeof(float), triVerts.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndices.size() * sizeof(unsigned int), triIndices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers for position, normal, and texture coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid *)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid *)24);
    glBindVertexArray(0);

    // Create a shader using my GLSLObject class                                                            
    shader.addShader( "OpenGL/Debug/vertexShader_perFrag.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "OpenGL/Debug/fragmentShader_perFrag.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();

    // get the uniform variable locations for the projection matrix, view matrix, and model matrix in our shader code
    GLuint projMatrixID = shader.createUniform( "projMatrix" );
    GLuint viewMatrixID = shader.createUniform( "viewMatrix" );
    GLuint modelMatrixID = shader.createUniform( "modelMatrix" );
    GLuint normalMatrixID = shader.createUniform( "normalMatrix" );
    GLuint lightPosID = shader.createUniform( "light.position" );
    GLuint lightIntID = shader.createUniform( "light.intensity" );
    GLuint Ia_ID = shader.createUniform("Ia");
    GLuint ka_ID = shader.createUniform("ka");
    GLuint kd_ID = shader.createUniform("kd");
    GLuint ks_ID = shader.createUniform("ks");
    GLuint phongExpID = shader.createUniform("phongExp");
    GLuint texUnitID = shader.createUniform("textureUnit");
    
    // Set the texture uniform to use texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(texUnitID, 0);

    glm::mat4 M_model = glm::mat4(1.0f);
    float rot = 0.0f;
    M_model = glm::rotate(M_model, rot, glm::vec3(0, 1, 0));

    glm::mat4 M_normal = glm::transpose(glm::inverse(M_model));

    float imgHeight = 10.0f;
    float imgWidth = imgHeight * aspectRatio;
    PerspectiveCamera camera( vec3(0,0,10), vec3(0,0,-1), vec3(0,1,0), 
                            5.0f, // focalLength (zoom level)
                            imgWidth, imgHeight, winWidth, winHeight );

    glm::mat4 M_proj = camera.getProjectionMatrix(0.1f, 100.0f);

    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        endFrameTime = glfwGetTime();
        timeDiff = endFrameTime - startFrameTime;
        startFrameTime = glfwGetTime();

        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render your objects here */
        shader.activate();

        // Global Camera/Light Uniforms
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(M_proj));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
        glUniform3f(lightPosID, 0.0f, 0.0f, 10.0f);
        glUniform3f(lightIntID, 1.0f, 1.0f, 1.0f);
        /*
        // DRAW SPHERE 1
        M_model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)); // Move left
        M_model = glm::rotate(M_model, rot, glm::vec3(0, 1, 0));
        M_normal = glm::transpose(glm::inverse(camera.getViewMatrix() * M_model));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(M_normal));
        glUniform3f(kd_ID, 0.5f, 1.0f, 1.0f);
        glUniform3f(ks_ID, 0.0f, 0.0f, 0.0f);
        glUniform1f(phongExpID, 0.0f); 
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

        // DRAW SPHERE 2 
        M_model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)); // Move right
        M_model = glm::scale(M_model, glm::vec3(0.5f)); // Make it smaller
        M_normal = glm::transpose(glm::inverse(camera.getViewMatrix() * M_model));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(M_normal));
        glUniform3f(kd_ID, 1.0f, 0.5f, 0.5f); 
        glUniform3f(ks_ID, 1.0f, 1.0f, 1.0f);
        glUniform1f(phongExpID, 64.0f);
        glBindVertexArray(m_VAO); 
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        */
        // DRAW TRIANGLE 
        M_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f)); 
        M_normal = glm::transpose(glm::inverse(camera.getViewMatrix() * M_model));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(M_model));
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(M_normal));
        glUniform3f(kd_ID, 1.0f, 1.0f, 1.0f); 
        glUniform3f(ks_ID, 0.0f, 0.0f, 0.0f);
        glUniform1f(phongExpID, 0.0f);
        glBindVertexArray(triVAO); 
        glDrawElements(GL_TRIANGLES, triIndices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        shader.deactivate();

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        float moveRatePerFrame = 0.01;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveLeft(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveBackward(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera.moveUp(moveRatePerFrame);
        }
        else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            camera.moveDown(moveRatePerFrame);
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        std::cout << "fps: " << 1.0 / timeDiff << std::endl;
        }

        if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS) {
            std::cout << "fps: " << 1.0/timeDiff << std::endl;
        }
        if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
  
    glfwTerminate();
    return 0;
}

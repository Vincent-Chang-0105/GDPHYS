#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "P6/MyVector.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


//Modifier for the model's x Position
float theta = 0.0f; // Initialize rotation angle to 0

// Set rotation axis to x-axis initially
float axis_x = 0.0f;
float axis_y = 1.0f;
float axis_z = 0.0f;

// Adjust initial position and scale of the model
float x_mod = 0.0f;
float y_mod = 0.0f;
float z_mod = -3.0f;

float scale = 0.5;

int main(void)
{
    //load the vert shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;

    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //load the frag shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;

    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLFWwindow* window;

    float window_width = 600;
    float window_height = 600;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Vincent Ralph N. Chang", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);

    //Compile the vertex shader
    glCompileShader(vertexShader);

    //Create a Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //Assign the source to the fragment shader
    glShaderSource(fragmentShader, 1, &f, NULL);

    //Compile the fragment shader
    glCompileShader(fragmentShader);


    //Create shader program
    GLuint shaderProg = glCreateProgram();


    //Attach the compiled Vertex shader
    glAttachShader(shaderProg, vertexShader);
    //Attach the compiled Vertex shader
    glAttachShader(shaderProg, fragmentShader);

    //Finalize the compilation process
    glLinkProgram(shaderProg);

    glViewport(0, // Min x
        0,//Min y
        600,//Width
        600); // Height


    std::string path = "3D/sphere.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]
    {
        //x  //y   //z
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f //2

    };

    GLuint indices[]
    {
        0,1,2
    };

    //Create VAO,VBO,EBO Variables
    GLuint VAO, VBO, EBO;

    //Initialize VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Currently Editing VAO = null
    glBindVertexArray(VAO);
    //Currently Editing VAO = VAO
    // 
    //Currently Editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Currently Editing VBO = VBO
    //VAO <-VBO

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size() /*Size of buffer in bytes*/,
        attributes.vertices.data() /*Array*/, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3 /*x y z*/, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Currently Editing VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Currently Editing VBO = EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_martix = glm::mat4(1.0f);

    glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 100.0f);

    P6::MyVector position(0, 3, 0);
    P6::MyVector scale(3, 3, 0);

    std::cout << position.Magnitude() << std::endl;
    std::cout << position.Direction().x << std::endl;
    std::cout << position.Direction().y << std::endl;
    std::cout << position.Direction().z << std::endl;

    std::cout << position.scalarMultiplication(2.f).x << std::endl;
    std::cout << position.scalarMultiplication(2.f).y << std::endl;
    std::cout << position.scalarMultiplication(2.f).z << std::endl;

    std::cout << position.dotProduct(scale) << std::endl;

    std::cout << position.crossProduct(scale).x << std::endl;
    std::cout << position.crossProduct(scale).y << std::endl;
    std::cout << position.crossProduct(scale).z << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Start with the translation matrix
        glm::mat4 transformation_matrix = glm::translate(identity_martix, (glm::vec3)position);

        //Multiply the resulting matrix with the scale matrix
        transformation_matrix = glm::scale(transformation_matrix, (glm::vec3)scale);

        //Finally, multiply it with the rotation matrix
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        //Get location of projection matrix
        //unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        ////Assign the matrix
        //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        //Get location of transformation matrix
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        //Assign the matrix
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix)); 


            //Tell openGL to use this shader
            //for VAO/s below
            glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        //Bind The VAO to prep it for drawing
        glBindVertexArray(VAO);
        //Draw the triangle
        //glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    //Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include <iomanip>
#include <cmath>
#include "GameObject.h"

#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "P6/ParticleContact.h"
/*#include "P6/AnchoredSpring.h" */ 
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "RenderLine.h"
#include "Randomization/RandomGen.h"

#include "P6/Links/Rod.h"
#include "P6/bungee/bungee.h"
#include "P6/Chain/Chain.h"


using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);

enum class CameraMode {
    Orthographic,
    Perspective
};

int main(void)
{

    GLFWwindow* window;
    float window_width = 800;
    float window_height = 800;

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(window_width, window_height, "Quiz_Vincent Ralph N. Chang", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    //OrthoCamera
    auto ortho_camera = new OrthoCamera();
    ortho_camera->setCameraPosition(glm::vec3(0.0f, 0.0f, 400.0f));

    //PerspectiveCamera
    auto pers_camera = new PerspectiveCamera();
    pers_camera->setCameraPosition(glm::vec3(0, 0.f, 550.f));

    //Initiliaze PhysicsWorld
    auto pWorld = physics::PhysicsWorld();

    //sphere
    auto sphere = GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag");

    glViewport(0, 0, 800, 800);

    //Initialize RenderParticles
    std::list<RenderParticle*> RenderParticles;

    //Spawning Variables
    float fThreshHold = 0.2f;
    float fTicks = 0.0f;
    
    //Default CameraMode
    CameraMode currentCameraMode = CameraMode::Orthographic;

    //Matrices
    glm::mat4 identity_matrix = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::mat4(1.f);
    glm::mat4 view_matrix = glm::mat4(1.0f);

    // Pause/play flag
    bool paused = false;
    bool pressed = false;

    //Instantiating PhysicParticle
    physics::PhysicsParticle p1 = physics::PhysicsParticle();
    p1.Position = physics::MyVector(250, 100, 0);
    p1.mass = 50;
    p1.radius = 50.f;
    pWorld.AddParticle(&p1);

    physics::PhysicsParticle p2 = physics::PhysicsParticle();
    p2.Position = physics::MyVector(-200, 100, 0);
    p2.mass = 50;
    p2.radius = 50.f;
    pWorld.AddParticle(&p2);

    physics::MyVector springPosBungee = physics::MyVector(-200, 150, 0);
    physics::bungee bungeeSpring = bungee(springPosBungee, 0.7, 200);
    pWorld.forceRegistry.Add(&p2, &bungeeSpring);

    physics::MyVector springPosChain = physics::MyVector(200, 150, 0);
    physics::Chain chainSpring = Chain(springPosChain, 0, 300);
    pWorld.forceRegistry.Add(&p1, &chainSpring);

    RenderParticle rp1 = RenderParticle(&p1, &sphere, glm::vec4(0.4f, 0, 0, 0.f), p1.radius);
    RenderParticles.push_back(&rp1);

    RenderParticle rp2 = RenderParticle(&p2, &sphere, glm::vec4(0, 0, 0.4f, 1.f), p2.radius);
    RenderParticles.push_back(&rp2);

    //Renderlines
    RenderLine BungeeLine = RenderLine(springPosBungee, p2.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine = RenderLine(springPosChain, p1.Position, glm::vec4(1, 1, 1, 1));

    while (!glfwWindowShouldClose(window))
    {
        glClear(0x00004000);

        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;


        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;

            //Key inputs
            if (glfwGetKey(window, GLFW_KEY_1))
            {
                std::cout << "Switching to Orthographic Camera" << std::endl;
                currentCameraMode = CameraMode::Orthographic;
            }
            if (glfwGetKey(window, GLFW_KEY_2))
            {
                std::cout << "Switching to Perspective Camera" << std::endl;
                currentCameraMode = CameraMode::Perspective;
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !pressed)
            {
                paused = !paused; // Toggle pause/play
                std::cout << (paused ? "Paused" : "Resumed") << std::endl;
                pressed = true;
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
            {
                pressed = false;
            }

            // Update cameras based on mode
            if (currentCameraMode == CameraMode::Orthographic) {
                projection_matrix = ortho_camera->getViewProjection();
                ortho_camera->Update(window, (float)ms.count() / 1000);
                view_matrix = ortho_camera->GetViewMatrix();
            }
            else if (currentCameraMode == CameraMode::Perspective) {
                projection_matrix = pers_camera->getViewProjection();
                pers_camera->Update(window, (float)ms.count() / 1000);
                view_matrix = pers_camera->GetViewMatrix();
            }

            if (!paused) {
                pWorld.Update((float)ms.count() / 1000);
            }
        }

        //Drawing Objects
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin(); i != RenderParticles.end(); i++) {
            (*i)->Draw(identity_matrix, projection_matrix, view_matrix);
        }

        //Drawing and Updating Renderlines
        BungeeLine.Update(springPosBungee, p2.Position, projection_matrix);
        BungeeLine.Draw();

        ChainLine.Update(springPosChain, p1.Position, projection_matrix);
        ChainLine.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
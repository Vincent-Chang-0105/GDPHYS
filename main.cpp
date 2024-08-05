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

enum class PlayMode {
    Uninitiated,
    Initiated
};

float rotationSpeed = 0.0f;

//Default Playmode
PlayMode currentPlayMode = PlayMode::Uninitiated;

bool isStopping = false;  // Global flag to indicate if stopping process is active


physics::MyVector RotateAroundOrigin(const physics::MyVector& position, float angle, const glm::vec3& axis) {

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    glm::vec4 rotatedPosition = rotationMatrix * glm::vec4((glm::vec3)position, 1.0f);

    return MyVector(rotatedPosition.x, rotatedPosition.y, rotatedPosition.z);
}


void StopSwingRide(float deltaTime)
{
    currentPlayMode = PlayMode::Uninitiated;
    float decelerationRate = 10.0f; // Adjust this value for gradual stopping
    if (rotationSpeed > 0)
    {
        rotationSpeed -= 50.f * deltaTime;
        std::cout << rotationSpeed << std::endl;
        if (rotationSpeed < 0)
        {
            rotationSpeed = 0; // Ensure it does not go negative
        }
    }
    else
    {
        isStopping = false; // Stop deceleration when speed reaches zero
    }
}

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

    window = glfwCreateWindow(window_width, window_height, "PC02<Chang>-<Magaling>", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    //OrthoCamera
    auto ortho_camera = new OrthoCamera();
    ortho_camera->setCameraPosition(glm::vec3(0.0f, 0.0f, 100.0f));

    //PerspectiveCamera
    auto pers_camera = new PerspectiveCamera();
    pers_camera->setCameraPosition(glm::vec3(0, 0.f, 400.f));

    //Initiliaze PhysicsWorld
    auto pWorld = physics::PhysicsWorld(MyVector(0, -50.f, 0));

    //sphere
    auto sphere = GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/Image_0.png");

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
    physics::PhysicsParticle anchorPoint = physics::PhysicsParticle();
    anchorPoint.Position = physics::MyVector(0, 0, 0);
    anchorPoint.mass = 50;
    anchorPoint.radius = 5;
    anchorPoint.affectedByGravity = false;
    pWorld.AddParticle(&anchorPoint);

    physics::PhysicsParticle p1 = physics::PhysicsParticle();
    p1.Position = physics::MyVector(40, 0, 0);
    p1.mass = 50;
    p1.radius = 5;
    p1.affectedByGravity = false;
    p1.affectedByContacts = false;
    pWorld.AddParticle(&p1);

    physics::PhysicsParticle p2 = physics::PhysicsParticle();
    p2.Position = physics::MyVector(28, 0, 28);
    p2.mass = 50;
    p2.radius = 5;
    p2.affectedByGravity = false;
    p2.affectedByContacts = false;
    pWorld.AddParticle(&p2);

    physics::PhysicsParticle p3 = physics::PhysicsParticle();
    p3.Position = physics::MyVector(0, 0, 40);
    p3.mass = 50;
    p3.radius = 5;
    p3.affectedByGravity = false;
    p3.affectedByContacts = false;
    pWorld.AddParticle(&p3);

    physics::PhysicsParticle p4 = physics::PhysicsParticle();
    p4.Position = physics::MyVector(-28.f, 0, 28);
    p4.mass = 50;
    p4.radius = 5;
    p4.affectedByGravity = false;
    p4.affectedByContacts = false;
    pWorld.AddParticle(&p4);

    physics::PhysicsParticle p5 = physics::PhysicsParticle();
    p5.Position = physics::MyVector(-40, 0, 0);
    p5.mass = 50;
    p5.radius = 5;
    p5.affectedByGravity = false;
    p5.affectedByContacts = false;
    pWorld.AddParticle(&p5);

    physics::PhysicsParticle p6 = physics::PhysicsParticle();
    p6.Position = physics::MyVector(-28, 0, -28);
    p6.mass = 50;
    p6.radius = 5;
    p6.affectedByGravity = false;
    p6.affectedByContacts = false;
    pWorld.AddParticle(&p6);

    physics::PhysicsParticle p7 = physics::PhysicsParticle();
    p7.Position = physics::MyVector(0, 0, -40);
    p7.mass = 50;
    p7.radius = 5;
    p7.affectedByGravity = false;
    p7.affectedByContacts = false;
    pWorld.AddParticle(&p7);

    physics::PhysicsParticle p8 = physics::PhysicsParticle();
    p8.Position = physics::MyVector(28, 0, -28);
    p8.mass = 50;
    p8.radius = 5;
    p8.affectedByGravity = false;
    p8.affectedByContacts = false;
    pWorld.AddParticle(&p8);

    //seats
    physics::PhysicsParticle p1Load = physics::PhysicsParticle();
    p1Load.Position = physics::MyVector(40, -15, 0);
    p1Load.mass = 60;
    p1Load.radius = 5;
    pWorld.AddParticle(&p1Load);

    physics::PhysicsParticle p2Load = physics::PhysicsParticle();
    p2Load.Position = physics::MyVector(28, -15, 28);
    p2Load.mass = 60;
    p2Load.radius = 5;
    pWorld.AddParticle(&p2Load);

    physics::PhysicsParticle p3Load = physics::PhysicsParticle();
    p3Load.Position = physics::MyVector(0, -15, 40);
    p3Load.mass = 60;
    p3Load.radius = 5;
    pWorld.AddParticle(&p3Load);

    physics::PhysicsParticle p4Load = physics::PhysicsParticle();
    p4Load.Position = physics::MyVector(-28, -15, 28);
    p4Load.mass = 60;
    p4Load.radius = 5;
    pWorld.AddParticle(&p4Load);

    physics::PhysicsParticle p5Load = physics::PhysicsParticle();
    p5Load.Position = physics::MyVector(-40, -15, 0);
    p5Load.mass = 60;
    p5Load.radius = 5;
    pWorld.AddParticle(&p5Load);

    physics::PhysicsParticle p6Load = physics::PhysicsParticle();
    p6Load.Position = physics::MyVector(-28, -15, -28);
    p6Load.mass = 60;
    p6Load.radius = 5;
    pWorld.AddParticle(&p6Load);

    physics::PhysicsParticle p7Load = physics::PhysicsParticle();
    p7Load.Position = physics::MyVector(0, -15, -40);
    p7Load.mass = 60;
    p7Load.radius = 5;
    pWorld.AddParticle(&p7Load);

    physics::PhysicsParticle p8Load = physics::PhysicsParticle();
    p8Load.Position = physics::MyVector(28, -15, -28);
    p8Load.mass = 60;
    p8Load.radius = 5;
    pWorld.AddParticle(&p8Load);

    float cableLength = 15;

    physics::Chain chainSpring1 = Chain(p1.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p1Load, &chainSpring1);

    physics::Chain chainSpring2 = Chain(p2.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p2Load, &chainSpring2);

    physics::Chain chainSpring3 = Chain(p3.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p3Load, &chainSpring3);

    physics::Chain chainSpring4 = Chain(p4.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p4Load, &chainSpring4);

    physics::Chain chainSpring5 = Chain(p5.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p5Load, &chainSpring5);

    physics::Chain chainSpring6 = Chain(p6.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p6Load, &chainSpring6);

    physics::Chain chainSpring7 = Chain(p7.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p7Load, &chainSpring7);

    physics::Chain chainSpring8 = Chain(p8.Position, 0, cableLength);
    pWorld.forceRegistry.Add(&p8Load, &chainSpring8);

    RenderParticle rAnchorPoint = RenderParticle(&anchorPoint, &sphere, glm::vec4(0, 1.f, 0, 0.f), anchorPoint.radius);
    //RenderParticles.push_back(&rAnchorPoint);

    RenderParticle rp1 = RenderParticle(&p1, &sphere, glm::vec4(1.f, 0, 0, 0.f), p1.radius);
    RenderParticles.push_back(&rp1);
    
    RenderParticle rp2 = RenderParticle(&p2, &sphere, glm::vec4(1.f, 0, 0, 0.f), p1.radius);
    RenderParticles.push_back(&rp2);

    RenderParticle rp3 = RenderParticle(&p3, &sphere, glm::vec4(1.f, 0, 0, 0.f), p3.radius);
    RenderParticles.push_back(&rp3);

    RenderParticle rp4 = RenderParticle(&p4, &sphere, glm::vec4(1.f, 0, 0, 0.f), p4.radius);
    RenderParticles.push_back(&rp4);

    RenderParticle rp5 = RenderParticle(&p5, &sphere, glm::vec4(1.f, 0, 0, 0.f), p5.radius);
    RenderParticles.push_back(&rp5);
    
    RenderParticle rp6 = RenderParticle(&p6, &sphere, glm::vec4(1.f, 0, 0, 0.f), p5.radius);
    RenderParticles.push_back(&rp6);

    RenderParticle rp7 = RenderParticle(&p7, &sphere, glm::vec4(1.f, 0, 0, 0.f), p5.radius);
    RenderParticles.push_back(&rp7);

    RenderParticle rp8 = RenderParticle(&p8, &sphere, glm::vec4(1.f, 0, 0, 0.f), p5.radius);
    RenderParticles.push_back(&rp8);

    RenderParticle rpl1 = RenderParticle(&p1Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p1Load.radius);
    RenderParticles.push_back(&rpl1);

    RenderParticle rpl2 = RenderParticle(&p2Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p2Load.radius);
    RenderParticles.push_back(&rpl2);

    RenderParticle rpl3 = RenderParticle(&p3Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p3Load.radius);
    RenderParticles.push_back(&rpl3);

    RenderParticle rpl4 = RenderParticle(&p4Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p4Load.radius);
    RenderParticles.push_back(&rpl4);

    RenderParticle rpl5 = RenderParticle(&p5Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p5Load.radius);
    RenderParticles.push_back(&rpl5);

    RenderParticle rpl6 = RenderParticle(&p6Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p6Load.radius);
    RenderParticles.push_back(&rpl6);

    RenderParticle rpl7 = RenderParticle(&p7Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p7Load.radius);
    RenderParticles.push_back(&rpl7);

    RenderParticle rpl8 = RenderParticle(&p8Load, &sphere, glm::vec4(0, 0, 1.0f, 0.f), p8Load.radius);
    RenderParticles.push_back(&rpl8);

    //Renderlines
    RenderLine RodLine1 = RenderLine(anchorPoint.Position, p1.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine2 = RenderLine(anchorPoint.Position, p2.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine3 = RenderLine(anchorPoint.Position, p3.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine4 = RenderLine(anchorPoint.Position, p4.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine5 = RenderLine(anchorPoint.Position, p5.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine6 = RenderLine(anchorPoint.Position, p6.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine7 = RenderLine(anchorPoint.Position, p7.Position, glm::vec4(1, 1, 1, 1));
    RenderLine RodLine8 = RenderLine(anchorPoint.Position, p8.Position, glm::vec4(1, 1, 1, 1));

    //
    RenderLine ChainLine1 = RenderLine(p1Load.Position, p1.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine2 = RenderLine(p2Load.Position, p2.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine3 = RenderLine(p3Load.Position, p3.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine4 = RenderLine(p4Load.Position, p4.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine5 = RenderLine(p5Load.Position, p5.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine6 = RenderLine(p6Load.Position, p6.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine7 = RenderLine(p7Load.Position, p7.Position, glm::vec4(1, 1, 1, 1));
    RenderLine ChainLine8 = RenderLine(p8Load.Position, p8.Position, glm::vec4(1, 1, 1, 1));

    float testing = 0;

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

            float deltaTime = ((float)ms.count() / 1000);
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
                //p1.AddForce(physics::MyVector(xForce, yForce, zForce));
                pressed = true;
                currentPlayMode = PlayMode::Initiated;
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
            {
                pressed = false;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                rotationSpeed -= 10 * deltaTime;
                std::cout << rotationSpeed << std::endl;
                if (rotationSpeed <= 0.5)
                {
                    rotationSpeed = 0;
                    currentPlayMode = PlayMode::Uninitiated;
                }
                
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                rotationSpeed += 10 * deltaTime;
                std::cout << rotationSpeed << std::endl;
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                chainSpring1.AddrestLength(10 * deltaTime);
                chainSpring2.AddrestLength(10 * deltaTime);
                chainSpring3.AddrestLength(10 * deltaTime);
                chainSpring4.AddrestLength(10 * deltaTime);
                chainSpring5.AddrestLength(10 * deltaTime);
                chainSpring6.AddrestLength(10 * deltaTime);
                chainSpring7.AddrestLength(10 * deltaTime);
                chainSpring8.AddrestLength(10 * deltaTime);

            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                chainSpring1.AddrestLength(-10 * deltaTime);
                chainSpring2.AddrestLength(-10 * deltaTime);
                chainSpring3.AddrestLength(-10 * deltaTime);
                chainSpring4.AddrestLength(-10 * deltaTime);
                chainSpring5.AddrestLength(-10 * deltaTime);
                chainSpring6.AddrestLength(-10 * deltaTime);
                chainSpring7.AddrestLength(-10 * deltaTime);
                chainSpring8.AddrestLength(-10 * deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                if (!isStopping) {
                    isStopping = true; // Start the stopping process
                }
            }

            if (isStopping)
            {
                StopSwingRide(deltaTime);
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
            if (currentPlayMode == PlayMode::Initiated)
            {
                if (p1.Position.y <= 40)
                {
                    // Move the anchors up by 40 meters
                    //anchorPoint.Position.y += 10.f * deltaTime;
                    p1.Position.y += 40.f * deltaTime;
                    p2.Position.y += 40.f * deltaTime;
                    p3.Position.y += 40.f * deltaTime;
                    p4.Position.y += 40.f * deltaTime;
                    p5.Position.y += 40.f * deltaTime;
                    p6.Position.y += 40.f * deltaTime;
                    p7.Position.y += 40.f * deltaTime;
                    p8.Position.y += 40.f * deltaTime;
                }
                else if (p1.Position.y >= 40) 
                {
                    p1.Position.y = 40.f;
                    p2.Position.y = 40.f;
                    p3.Position.y = 40.f;
                    p4.Position.y = 40.f;
                    p5.Position.y = 40.f;
                    p6.Position.y = 40.f;
                    p7.Position.y = 40.f;
                    p8.Position.y = 40.f;
                    if (rotationSpeed == 0)
                    {
                        rotationSpeed = 45.f;
                    }

                }
            }
            float rotationAngle = rotationSpeed * ((float)ms.count() / 1000); // Rotation angle in degrees
            glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // Rotation axis

            physics::MyVector newParticlePosition1 = RotateAroundOrigin(p1.Position, rotationAngle, rotationAxis);
            p1.Position = newParticlePosition1;

            physics::MyVector newParticlePosition2 = RotateAroundOrigin(p2.Position, rotationAngle, rotationAxis);
            p2.Position = newParticlePosition2;

            physics::MyVector newParticlePosition3 = RotateAroundOrigin(p3.Position, rotationAngle, rotationAxis);
            p3.Position = newParticlePosition3;

            physics::MyVector newParticlePosition4 = RotateAroundOrigin(p4.Position, rotationAngle, rotationAxis);
            p4.Position = newParticlePosition4;

            physics::MyVector newParticlePosition5 = RotateAroundOrigin(p5.Position, rotationAngle, rotationAxis);
            p5.Position = newParticlePosition5;

            physics::MyVector newParticlePosition6 = RotateAroundOrigin(p6.Position, rotationAngle, rotationAxis);
            p6.Position = newParticlePosition6;

            physics::MyVector newParticlePosition7 = RotateAroundOrigin(p7.Position, rotationAngle, rotationAxis);
            p7.Position = newParticlePosition7;

            physics::MyVector newParticlePosition8 = RotateAroundOrigin(p8.Position, rotationAngle, rotationAxis);
            p8.Position = newParticlePosition8;

            chainSpring1.Update(p1.Position);
            chainSpring2.Update(p2.Position);
            chainSpring3.Update(p3.Position);
            chainSpring4.Update(p4.Position);
            chainSpring5.Update(p5.Position);
            chainSpring6.Update(p6.Position);
            chainSpring7.Update(p7.Position);
            chainSpring8.Update(p8.Position);

            if (!paused) {
                pWorld.Update((float)ms.count() / 1000);
            }
        }

        //Drawing Objects
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin(); i != RenderParticles.end(); i++) {
            (*i)->Draw(identity_matrix, projection_matrix, view_matrix);
        }

        RodLine1.Update(anchorPoint.Position, p1.Position, projection_matrix, view_matrix);
        //RodLine1.Draw();

        RodLine2.Update(anchorPoint.Position, p2.Position, projection_matrix, view_matrix);
        //RodLine2.Draw();

        RodLine3.Update(anchorPoint.Position, p3.Position, projection_matrix, view_matrix);
        //RodLine3.Draw();

        RodLine4.Update(anchorPoint.Position, p4.Position, projection_matrix, view_matrix);
        //RodLine4.Draw();

        RodLine5.Update(anchorPoint.Position, p5.Position, projection_matrix, view_matrix);
        //RodLine5.Draw();

        RodLine6.Update(anchorPoint.Position, p6.Position, projection_matrix, view_matrix);
        //RodLine6.Draw();

        RodLine7.Update(anchorPoint.Position, p7.Position, projection_matrix, view_matrix);
        //RodLine7.Draw();

        RodLine8.Update(anchorPoint.Position, p8.Position, projection_matrix, view_matrix);
        //RodLine8.Draw();

        ChainLine1.Update(p1Load.Position, p1.Position, projection_matrix, view_matrix);
        ChainLine1.Draw();

        ChainLine2.Update(p2Load.Position, p2.Position, projection_matrix, view_matrix);
        ChainLine2.Draw();
        
        ChainLine3.Update(p3Load.Position, p3.Position, projection_matrix, view_matrix);
        ChainLine3.Draw();

        ChainLine4.Update(p4Load.Position, p4.Position, projection_matrix, view_matrix);
        ChainLine4.Draw();

        ChainLine5.Update(p5Load.Position, p5.Position, projection_matrix, view_matrix);
        ChainLine5.Draw();

        ChainLine6.Update(p6Load.Position, p6.Position, projection_matrix, view_matrix);
        ChainLine6.Draw();

        ChainLine7.Update(p7Load.Position, p7.Position, projection_matrix, view_matrix);
        ChainLine7.Draw();

        ChainLine8.Update(p8Load.Position, p8.Position, projection_matrix, view_matrix);
        ChainLine8.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
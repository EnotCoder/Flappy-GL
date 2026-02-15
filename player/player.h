#ifndef PLAYER_H
#define PLAYER_H

#include "rect/rect.h"
#include "animSprite/anim.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class player
{
private:
    //sprite
    bool flip = false;
    const unsigned int Playertexture{CreateTexture("tex/Player/AllBird1.png")};
    glm::vec2 PlayerPos = glm::vec2(-0.7,0.5);
    anim birdSprite;

    //rotate
    float rotateBird = 0.0;
    const float jumpForce = 0.025f;
    float targetRotation = 0.0f;
    const float rotationSpeed = 90.0f;
    float lastVelocityY = 0.0f;

    //fall
    int prevSpaceState = GLFW_RELEASE;
    const float gravity = -0.001f;
    float velocityY = 0.0f;

public:
    int store = 0;
    bool active = true;
    void move(GLFWwindow* window,float deltaTime);
    void RestartPlayer();
};

#endif

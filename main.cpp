#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glm/fwd.hpp"
#include "stb_image.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rect/rect.h"
#include "shader/shader.h"
#include "player/player.h"
#include "animSprite/anim.h"
#include <random>
#include "spriteClass/sprite.h"
#include "text/text.h"
#include "control/control.h"


float vert[] = {
    -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f,  0.0f,  0.0f, 0.5f,
     0.5f, -0.5f,  0.0f,  0.5f, 0.5f
};


unsigned int frag[] = {
    0,1,2,
    1,2,3
};

//Random number
double getRandomDouble(double min, double max) {
    static std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

std::vector <std::string> getFileSource()
{   
    std::string line;
    std::ifstream file("game.txt");
    std::vector <std::string> ret;
    if (file.is_open())
    {
        while (std::getline(file,line))
        {
            ret.push_back(line);
        }
        file.close();
    }else{
        std::cerr << "не удалось открыть файл";
    }

    return ret;
}

//MAIN
int main()
{   
    //Glfw INIT
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);    


    //Window
    GLFWwindow* window = glfwCreateWindow(1200, 800, "C++ game", NULL, NULL);
    if (window == NULL){std::cout << "ERROR WINDOW CREATE";glfwTerminate();}

    glfwSetWindowPos(window,500,500);
    glfwMakeContextCurrent(window);    

    //GLAD INIT
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {   
        std::cout << "ERROR GLAD INIT";
        return -1;
    }

    //VAO VBO EBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frag), frag, GL_STATIC_DRAW);


    //glEnable
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //shaders
    shaderProgram = CreateShader("./shaders/Vert.glsl", "./shaders/Frag.glsl");

    SetupTextBuffers();
    unsigned int textShader = CreateShader("./shaders/textVert.glsl", "./shaders/textFrag.glsl");
    InitText(20);

    initUniforms();

    //fon sprite
    Sprite FonTexture;
    FonTexture.Texture = CreateTexture("tex/Background/Background2.png");
    FonTexture.rectSize = glm::vec2(2.0,1.0);
    FonTexture.scale = glm::vec2(4.0,2.0);
    unsigned int starTexture = CreateTexture("tex/star.png");

    //player
    player bird;

    //tube
    Sprite tube;
    tube.Texture = CreateTexture("tex/Tiles/PipeStyle1.png");
    tube.cutX = 4;tube.cutY = 1;tube.isCollide = true;
    tube.scale = glm::vec2(0.3,1.5);tube.uvFrame = glm::vec2(1.0,1.0);

    //down tubes
    Sprite tube1 = tube;tube1.position = glm::vec2(2.0,-(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube2 = tube;tube2.position = glm::vec2(3.5,-(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube3 = tube;tube3.position = glm::vec2(5.0,-(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tubesDown[] = {tube1, tube2, tube3};

    //up tubes
    tube.angle = 180;
    Sprite tube4 = tube;tube4.position = glm::vec2(2.0,(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube5 = tube;tube5.position = glm::vec2(3.5,(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tube6 = tube;tube6.position = glm::vec2(5.0,(static_cast <float>(getRandomDouble(1.0,1.35))));
    Sprite tubesUp[] = {tube4, tube5, tube6};


    std::vector <std::string> f = getFileSource();
    std::cout << f[0] << std::endl;

    //while
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        //delta time
        float now = glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;

        //exit
        static int EscKeyState = GLFW_RELEASE;
        
        if (IsKeyJustPressed(window, GLFW_KEY_ESCAPE, EscKeyState))
        {
            glfwSetWindowShouldClose(window, true);
        }

        //restart
        static int RKeyState = GLFW_RELEASE;

        if (IsKeyJustPressed(window, GLFW_KEY_R, RKeyState))
        {
            tubesDown[0].position = glm::vec2(2.0,-(static_cast <float>(getRandomDouble(1.0,1.35))));
            tubesDown[1].position = glm::vec2(3.5,-(static_cast <float>(getRandomDouble(1.0,1.35))));
            tubesDown[2].position = glm::vec2(5.0,-(static_cast <float>(getRandomDouble(1.0,1.35))));

            tubesUp[0].position = glm::vec2(2.0,(static_cast <float>(getRandomDouble(1.0,1.35))));
            tubesUp[1].position = glm::vec2(3.5,(static_cast <float>(getRandomDouble(1.0,1.35))));
            tubesUp[2].position = glm::vec2(5.0,(static_cast <float>(getRandomDouble(1.0,1.35))));

            bird.RestartPlayer();
        }
        

        //Draw Object
        glClearColor(30.0f/255, 28.0f/255, 52.0f/255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        //fon
        FonTexture.draw();


        //tubes
        areaCollide.clear();
        
        //tube
        for (int i =0;i<3;i++)
        {   
            float &tubePosX = tubesDown[i].position.x;
            float &tubePosY = tubesDown[i].position.y;
            if (tubePosX > -2.0f) {if (bird.active) tubePosX -= 0.02f;} 
            else {tubePosX = 2.5f;tubePosY = -getRandomDouble(1.0,1.35);}
            tubesDown[i].draw();
        }
        for (int i =0;i<3;i++)
        {   
            float &tubePosX = tubesUp[i].position.x;
            float &tubePosY = tubesUp[i].position.y;
            if (tubePosX > -2.0f) {if (bird.active) tubePosX -= 0.02f;} 
            else {tubePosX = 2.5f;tubePosY = getRandomDouble(1.0,1.35);bird.store+=1;}
            tubesUp[i].draw();
        }

        //player
        bird.move(window,deltaTime);

        //glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 800.0f, 0.0f);
        //glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //text
        glUseProgram(textShader);
        RenderText(textShader, "Store: " + std::to_string(bird.store), 50.0f, 700.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        if (!bird.active)RenderText(textShader,"Presed R-restart", 750.0f, 700.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));


        //window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}

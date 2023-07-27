#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <ctime>
#include <cstdlib>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

///Képernyö méret
const unsigned int XX = 1280;
const unsigned int YY = 720;
///Pálya méret
const float StadWidth  = 50.0f;
const float StadLength = 100.0f;

Camera camera(glm::vec3(0.0f, 0.3f, 0.0f));

bool flyUp = false;
bool up = false;
bool down = false;
bool left = false;
bool right = false;
bool invert = true;

const unsigned int gurkoNumber = 15;
float gurkoX[gurkoNumber];
float gurkoY[gurkoNumber];
float gurkoZ[gurkoNumber];

float cikeszX =  0.0f;
float cikeszY = 20.0f;
float cikeszZ =  0.0f;
bool  cikeszU = false;
bool  cikeszD = false;
bool  cikeszF = false;
bool  cikeszB = false;
bool  cikeszL = false;
bool  cikeszR = false;
float cikeszSpd  = 0.0f;
float cikeszTime = 0.0f;

bool speedUp  = false;
bool speedDwn = false;
float turn = 0.0f;
float speed = 0.0f;
float animationTimeX = 0.0f;
float animationTimeY = 0.0f;

bool gameover = false;
bool grassGO = false;
bool wallGO = false;
bool flyGO = false;
bool gurkoGO = false;
bool win = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void groundCreator(Shader& shader, Texture& texture, float width, float length);
void backgroundCreator(Shader& shader, Texture texV[4], float width, float length);
void boxCreator(Shader& shader, Texture texV[6], glm::vec3 boxPos, float boxYaw, float boxPitch, std::string what);
void gameoverCreator(Shader& shader, Texture& texture);
void startOver();
bool SameLocation(float aX,float aY,float aZ,float bX, float bY, float bZ, float bonus);

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(XX, YY, "Harry Potter and the Quidditch cup", nullptr, nullptr);
    if(window == nullptr){
        std::cout<< "Failed to create GLFW window" <<std::endl;
        glfwTerminate();
        return -1;
    }glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<< "Failed to initialize GLAD" <<std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("shader.vs", "shader.fs");
    ourShader.use();

    float vertices[] = {
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, /// A (0)
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f, /// B (1)
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f, /// C (2)
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f  /// D (3)
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

///Talaj és pálya textúrák
    Texture GrassTex("textures/background/Grass.png");
    Texture BgFrontBackTex("textures/background/RedGreen.png");
    Texture BgLeftRightTex("textures/background/LeftRightLong.png");
///Labda textúrák
    Texture GurkoTex("textures/balls/Gurko.png");
    Texture CikeszBodyTex("textures/balls/Body.png");
    Texture CikeszLeftTex("textures/balls/Left.png");
    Texture CikeszRightTex("textures/balls/Right.png");
///Oszlop textúrák
    Texture RedColumnBackTex("textures/columns/red/Back.png");
    Texture RedColumnLeftTex("textures/columns/red/Left.png");
    Texture RedColumnRightTex("textures/columns/red/Right.png");
    Texture GreenColumnBackTex("textures/columns/green/Back.png");
    Texture GreenColumnLeftTex("textures/columns/green/Left.png");
    Texture GreenColumnRightTex("textures/columns/green/Right.png");
    Texture YellowColumnFrontTex("textures/columns/yellow/Front.png");
    Texture YellowColumnBackTex("textures/columns/yellow/Back.png");
    Texture BlueColumnFrontTex("textures/columns/blue/Front.png");
    Texture BlueColumnBackTex("textures/columns/blue/Back.png");
///Fej textúrák
    Texture HeadTopTex("textures/head/Top.png");
    Texture HeadBottomTex("textures/head/Bottom.png");
    Texture HeadFrontTex("textures/head/Front.png");
    Texture HeadBackTex("textures/head/Back.png");
    Texture HeadLeftTex("textures/head/Left.png");
    Texture HeadRightTex("textures/head/Right.png");
///Test textúrák
    Texture BodyTopBottomTex("textures/body/TopBottom.png");
    Texture BodyFrontTex("textures/body/Front.png");
    Texture BodyBackTex("textures/body/Back.png");
    Texture BodyLeftTex("textures/body/Left.png");
    Texture BodyRightTex("textures/body/Right.png");
///Seprü textúrák
    Texture BroomBottomTex("textures/broom/Bottom.png");
    Texture BroomTopTex("textures/broom/Top.png");
    Texture BroomLeftTex("textures/broom/Left.png");
    Texture BroomRightTex("textures/broom/Right.png");
    Texture BroomFrontBackTex("textures/broom/FrontBack.png");
    Texture BroomStickTex("textures/broom/Stick.png");
///GameOver és Win textúrák
    Texture OverGrassTex("textures/gameover/GOgrass.png");
    Texture OverWallTex("textures/gameover/GOwall.png");
    Texture OverFlyTex("textures/gameover/GOfly.png");
    Texture OverGurkoTex("textures/gameover/GOgurko.png");
    Texture WinTex("textures/gameover/Win.png");

///Textúrák tömbe pakolása, hogy könyeben kezelhetök legyenek
    Texture background[4] = { BgFrontBackTex, BgLeftRightTex, BgFrontBackTex, BgLeftRightTex };
    Texture gurko[6] = { GurkoTex, GurkoTex, GurkoTex, GurkoTex, GurkoTex, GurkoTex };
    Texture cikesz[6] = { CikeszBodyTex, CikeszLeftTex, CikeszRightTex, CikeszBodyTex, CikeszBodyTex, CikeszBodyTex };
    Texture columnR[6] = { BroomStickTex, RedColumnBackTex, RedColumnLeftTex, RedColumnBackTex, RedColumnRightTex, BroomStickTex };
    Texture columnG[6] = { BroomStickTex, GreenColumnBackTex, GreenColumnLeftTex, GreenColumnBackTex, GreenColumnRightTex, BroomStickTex };
    Texture columnY[6] = { BroomStickTex, YellowColumnBackTex, YellowColumnBackTex, YellowColumnBackTex, YellowColumnFrontTex, BroomStickTex };
    Texture columnB[6] = { BroomStickTex, BlueColumnBackTex, BlueColumnBackTex, BlueColumnBackTex, BlueColumnFrontTex, BroomStickTex };
    Texture head[6] = { HeadBottomTex, HeadLeftTex, HeadRightTex, HeadFrontTex, HeadBackTex, HeadTopTex };
    Texture body[6] = { BodyTopBottomTex, BodyLeftTex, BodyRightTex, BodyFrontTex, BodyBackTex, BodyTopBottomTex };
    Texture broom[6] = { BroomBottomTex, BroomLeftTex, BroomRightTex, BroomFrontBackTex, BroomFrontBackTex, BroomTopTex };
    Texture stick[6] = { BroomStickTex, BroomStickTex, BroomStickTex, BroomStickTex, BroomStickTex, BroomStickTex };

    glfwSetKeyCallback(window, key_callback);

    for(unsigned int i=0; i<gurkoNumber; i++) gurkoX[i] = -30.0f; ///Gurkók kezdeti helyzete
    srand(time(NULL));

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.7f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(55.0f+(speed*10.0f)), (float)XX/(float)YY, 0.1f, 150.0f);
        ourShader.setMat4("u_projection", projection);

        if(!gameover){
///Elöre-hátra mozgás kezelése
            if(speedUp &&  speed<3.0f) speed += 0.01f;
            else if(speed>=0.0f) speed -= 0.002f;           /// EZT...
            if(speedDwn && speed>-1.0f) speed -= 0.01f;     /// ...és...
            else if(speed<=0.0f) speed += 0.002f;           /// ... EZT (a 2 else if-et) kikommentelve nem lassúl a seprü és akkor nem kell nyomva tartani az W-t a folyamatos repüléshez
///Fej megforgatása tolatás esetén
            if(speed<0.0f){
                if(turn<160.0f) turn+=2.0f;
            }else if(turn>0.0f) turn-=1.5;
///A megfelelö sebességgel folyamatos mozgás (speed=0 eseten természetesen nem mozog)
            camera.ProcessKeyboard(FORWARD, deltaTime*speed);
///Mozgás kezelése animációkkal együtt
            if(up){
                camera.ProcessMouseMovement(0.0f, -10.0f);
                if(animationTimeY >= -0.5f) animationTimeY -= 0.02f;
            }else if(animationTimeY <= 0.0f) animationTimeY += 0.02f;
            if(down){
                camera.ProcessMouseMovement(0.0f, 10.0f);
                if(animationTimeY <= 0.5f) animationTimeY += 0.02f;
            }else if(animationTimeY >= 0.0f) animationTimeY -= 0.02f;
            if(left){
                camera.ProcessMouseMovement(-10.0f, 0.0f);
                if(animationTimeX <= 0.5f) animationTimeX += 0.02f;
            } else if(animationTimeX >= 0.0f) animationTimeX -= 0.02f;
            if(right){
                camera.ProcessMouseMovement(10.0f, 0.0f);
                if (animationTimeX >= -0.5f)animationTimeX -= 0.02f;
            }else if(animationTimeX <= 0.0f) animationTimeX += 0.02f;
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("u_view", view);
///Gurkók véletlen áthaladása a pályán bizonyos idö közönként
            for(unsigned int i=0; i<gurkoNumber; i++){
                if(gurkoX[i]>-30) gurkoX[i]-=0.1f;
                else{
                    gurkoX[i] = 30.0f+rand()%30;
                    gurkoY[i] = rand()%(int)StadLength/5+1;
                    gurkoZ[i] = rand()%(int)StadWidth*2-(int)StadWidth;
                }
///Megnézi, hogy Harry ütközött-e egy gurkóval
                if(SameLocation(camera.Position.x, camera.Position.y, camera.Position.z, gurkoX[i], gurkoY[i], gurkoZ[i], 0.5f))
                gurkoGO = true;
            }

///Cikesz kezelése
            if(cikeszTime>=5.0f){
                ///Cikesz irány számító----
                cikeszU=rand()%2;
                if(cikeszU) cikeszD=false;
                else cikeszD=true;
                cikeszF=rand()%2;
                if(cikeszF) cikeszB=false;
                else cikeszB=true;
                cikeszL=rand()%2;
                if(cikeszL) cikeszR=false;
                else cikeszR=true;
                ///------------------------
                cikeszTime=0.0f;
                cikeszSpd=(rand()%5+1)*0.01; ///Cikesz sebessége
            }else cikeszTime+=0.01f;

///Cikesz mozgatása
            if(cikeszU && cikeszY < StadLength/5)    cikeszY+=cikeszSpd;
            if(cikeszD && cikeszY > 2.0f)            cikeszY-=cikeszSpd;
            if(cikeszF && cikeszZ >-StadLength/2.5f) cikeszZ-=cikeszSpd;
            if(cikeszB && cikeszZ < StadLength/2.5f) cikeszZ+=cikeszSpd;
            if(cikeszL && cikeszX >- StadWidth/2.5f) cikeszX-=cikeszSpd;
            if(cikeszR && cikeszX <  StadWidth/2.5f) cikeszX+=cikeszSpd;
///Megnézi, hogy a cikesz el lett-e kapva
            if(SameLocation(camera.Position.x, camera.Position.y, camera.Position.z, cikeszX, cikeszY, cikeszZ, 0.2f)) win = true;

///Ütközések (vagy szökés) figyelése----------------------------------------------------------------------------------------------------------
            if(camera.Position.y<0.1f) grassGO = true;
            if(camera.Position.y>140.0f) flyGO = true;
            if(camera.Position.x< -StadWidth/2.0f || camera.Position.x>StadWidth/2.0f ||
            camera.Position.z<-StadLength/2.0f || camera.Position.z>StadLength/2.0f){
                if(camera.Position.y>(StadWidth-5.0f)/2.0f) flyGO = true;
                else wallGO = true;
            }
///-------------------------------------------------------------------------------------------------------------------------------------------

///Kirajzolások-------------------------------------------------------------------------------------------------------------------------------
            groundCreator(ourShader, GrassTex, StadWidth, StadLength); ///Talaj
            backgroundCreator(ourShader, background, StadWidth, StadLength); ///Pálya határok

            for(unsigned int i=0; i<gurkoNumber; i++){
                boxCreator(ourShader, gurko, glm::vec3(gurkoX[i], gurkoY[i], gurkoZ[i]), 0.0f, 0.0f, "gurko"); ///Gurkók
            }
            boxCreator(ourShader, cikesz, glm::vec3(cikeszX, cikeszY, cikeszZ), 90.0f, 0.0f, "cikesz"); ///Cikesz

            boxCreator(ourShader, columnR, glm::vec3(-StadWidth/2.0f, 20.0f, -StadLength/2.0f), -90.0f, 0.0f, "column"); /// O
            boxCreator(ourShader, columnG, glm::vec3( StadWidth/2.0f, 20.0f, -StadLength/2.0f),   0.0f, 0.0f, "column"); /// S
            boxCreator(ourShader, columnR, glm::vec3( StadWidth/2.0f, 20.0f,  StadLength/2.0f),  90.0f, 0.0f, "column"); /// Z
            boxCreator(ourShader, columnG, glm::vec3(-StadWidth/2.0f, 20.0f,  StadLength/2.0f), 180.0f, 0.0f, "column"); /// L
            boxCreator(ourShader, columnY, glm::vec3(-StadWidth/1.9f, 20.0f,  0.0f), 180.0f, 0.0f, "column");            /// O
            boxCreator(ourShader, columnB, glm::vec3( StadWidth/1.9f, 20.0f,  0.0f),   0.0f, 0.0f, "column");            /// P

            boxCreator(ourShader, head, camera.Position, camera.Yaw+animationTimeX*15.0f, camera.Pitch/1.15f, "harryHead"); ///Fej
            boxCreator(ourShader, body, camera.Position, camera.Yaw+animationTimeX*15.0f, camera.Pitch/1.15f, "harryBody"); ///Test
            boxCreator(ourShader, stick, camera.Position, camera.Yaw+animationTimeX*15.0f, camera.Pitch/1.15f, "broomStick"); ///Seprünyél
            boxCreator(ourShader, broom, camera.Position, camera.Yaw+animationTimeX*15.0f, camera.Pitch/1.15f, "broom"); ///Seprü
///---------------------------------------------------------------------------------------------------------------------------------------------

///Ellenörzi, hogy vége lett-e a játéknak
            if(grassGO || wallGO || flyGO || gurkoGO || win) gameover = true;
        }
        else if(grassGO) gameoverCreator(ourShader, OverGrassTex);  ///Talajjal ütközéses halál
        else if(wallGO) gameoverCreator(ourShader, OverWallTex);    ///Falal ütközéses halál
        else if(flyGO) gameoverCreator(ourShader, OverFlyTex);      ///Pálya elhagyása
        else if(gurkoGO) gameoverCreator(ourShader, OverGurkoTex);  ///Gurkós halál
        else if(win) gameoverCreator(ourShader, WinTex);            ///Cikesz megszerzése

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);  ///Kilépés
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && gameover) startOver();                  ///Újrakezdés
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) invert = true;                                ///Invert irányítás BE
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) invert = false;                               ///Invert irányítás KI

///Mozgás------------------------------------------------------------------------------------------------------------------
    if(invert){
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) up = true;
        else up = false;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) down = true;
        else down = false;
    }else{
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) down = true;
        else down = false;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) up = true;
        else up = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) left = true;
    else left = false;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) right = true;
    else right = false;
///-------------------------------------------------------------------------------------------------------------------------

///Sebesség állítás---------------------------------------------------------------------------------------------------------
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) speedUp=true;
    else speedUp=false;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) speedDwn=true;
    else speedDwn=false;
///-------------------------------------------------------------------------------------------------------------------------
}


///Kirajzolja a talajt------------------------------------------------------------------------------------------------------
void groundCreator(Shader& shader, Texture& texture, float width, float length){
    texture.Bind();
    for(float i = -length/2.0f; i < length/2.0f; i++) for(float j = -width/2.0f; j < width/2.0f; j++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f+j, 0.5f+i, 0.0f));
        shader.setMat4("u_model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}
///-------------------------------------------------------------------------------------------------------------------------


///Kirajzolja a pálya határokat---------------------------------------------------------------------------------------------
void backgroundCreator(Shader& shader, Texture texV[4], float width, float length){
    for(unsigned int i=0; i<4; i++){
        texV[i].Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(i*90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        if(i%2==0){
            model = glm::translate(model, glm::vec3(0.0f, (width-5.0f)/4.0f, -length/2.0f));
            model = glm::scale(model, glm::vec3(width-5.0f, (width-5.0f)/2.0f, 1.0f));
        }else{
            model = glm::translate(model, glm::vec3(0.0f, (width-5.0f)/4.0f, -width/2.0f));
            model = glm::scale(model, glm::vec3(length-5.0f, (width-5.0f)/2.0f, 1.0f));
        }
        shader.setMat4("u_model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}
///-------------------------------------------------------------------------------------------------------------------------


///Kirajzol egy kockát------------------------------------------------------------------------------------------------------
void boxCreator(Shader& shader, Texture texV[6], glm::vec3 boxPos, float boxYaw, float boxPitch, std::string what){
    for(unsigned int i=0; i<6; i++){
        texV[i].Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, boxPos);
        model = glm::rotate(model, glm::radians(boxYaw)+glm::half_pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(boxPitch), glm::vec3(1.0f, 0.0f, 0.0f));

        if(what=="gurko"){
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        if(what=="cikesz"){
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            model = glm::rotate(model, (float)glfwGetTime()*2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        if(what=="column"){
            model = glm::scale(model, glm::vec3(5.0f, 40.0f, 5.0f));
        }

        if(what=="harryHead"){
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));
            model = glm::rotate(model, animationTimeX, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, animationTimeY, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(turn), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
        }

        if(what=="harryBody"){
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));
            model = glm::rotate(model, animationTimeX, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, animationTimeY, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, -0.03f, 0.0f));
            model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        }

        if(what=="broomStick"){
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));
            model = glm::rotate(model, animationTimeX, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, animationTimeY, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0, -0.037f, -0.015f));
            model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.1f));
        }

        if(what=="broom"){
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));
            model = glm::rotate(model, animationTimeX, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, animationTimeY, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, -0.037f, 0.05f));
            model = glm::scale(model, glm::vec3(0.03f, 0.01f, 0.04f));
        }

        if(i==0) model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        else if(i==1) model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        else if(i==2) model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        else if(i==3) model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        else if(i==4) model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        else model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
        shader.setMat4("u_model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}
///-------------------------------------------------------------------------------------------------------------------------


///GameOver és Win képek kirajzolása----------------------------------------------------------------------------------------
void gameoverCreator(Shader& shader, Texture& texture){
    speed = 0.9f;
    texture.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, camera.Position);
    model = glm::rotate(model, glm::radians(camera.Yaw)+glm::half_pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.8f));
    model = glm::scale(model, glm::vec3(1.8f, 1.0f, 1.0f));
    shader.setMat4("u_model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
///-------------------------------------------------------------------------------------------------------------------------


///Ujrakezdés kezelése------------------------------------------------------------------------------------------------------
void startOver(){
    speed=0.0f;
    camera.Yaw=-90.0f;
    camera.Pitch=0.0f;
    camera.Position=glm::vec3(0.0f, 0.3f, 0.0f);
    camera.ProcessMouseMovement(0.0f, 0.0f);
    for(unsigned int i=0; i<gurkoNumber; i++) gurkoX[i] = -30.0f;
    cikeszX=0.0f;
    cikeszY=20.0f;
    cikeszZ=0.0f;
    win=false;
    grassGO=false;
    wallGO=false;
    flyGO=false;
    gurkoGO=false;
    gameover=false;
}
///-------------------------------------------------------------------------------------------------------------------------


///Ütközés nézése-----------------------------------------------------------------------------------------------------------
bool SameLocation(float aX,float aY,float aZ,float bX, float bY, float bZ, float bonus){
    return (aX<=bX+bonus&&aX>=bX-bonus && aY<=bY+bonus&&aY>=bY-bonus && aZ<=bZ+bonus&&aZ>=bZ-bonus);
}
///-------------------------------------------------------------------------------------------------------------------------

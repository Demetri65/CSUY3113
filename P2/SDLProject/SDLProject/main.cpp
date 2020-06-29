#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program, ballP;
glm::mat4 viewMatrix, projectionMatrix, ballMatrix, paddle1Matrix, paddle2Matrix;

GLuint ball, paddle1, paddle2;

// Start at 0, 0, 0
glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 paddle1_position = glm::vec3(-4.5f, 0, 0);
glm::vec3 paddle2_position = glm::vec3(4.5f, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 ball_movement = glm::vec3(0, 0, 0);
glm::vec3 paddle1_movement = glm::vec3(0, 0, 0);
glm::vec3 paddle2_movement = glm::vec3(0, 0, 0);

float ball_speed = 0.50f;
float paddle1_speed = 1.0f;
float paddle2_speed = 1.0f;

bool ball_move = false;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n"; assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
    }

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    ballP.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    ballMatrix = glm::mat4(1.0f);
    paddle1Matrix = glm::mat4(1.0f);
    paddle2Matrix = glm::mat4(1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    ball = LoadTexture("ball.png");
    paddle1 = LoadTexture("paddle.png");
    paddle2 = LoadTexture("paddle.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    ball_movement = glm::vec3(0);
    paddle1_movement = glm::vec3(0);
    paddle2_movement = glm::vec3(0);
    
    if (keys[SDL_SCANCODE_W]) {
        paddle1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        paddle1_movement.y = -1.0f;
    }
    
    if (keys[SDL_SCANCODE_UP]) {
        paddle2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        paddle2_movement.y = -1.0f;
    }
    
    if (glm::length(ball_movement) > 1.0f) {
        ball_movement = glm::normalize(ball_movement);
    }
}

bool posY = true;
bool posX = true;
bool changeX = false;
bool end = false;

float lastTicks = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    
    // Add (direction * units per second * elapsed time)

    
    
    ball_movement.x = 4.0f;
    ball_movement.y = 4.0f;
    
    if (ball_position.x > 4.75 || ball_position.x < -4.75) {
        end = true;
    }
    if (end) {
        ball_movement.x = 0;
        ball_movement.y = 0;
        paddle1_movement.y = 0;
        paddle2_movement.y = 0;
    }
    else {
        paddle1_position += paddle1_movement * paddle1_speed * deltaTime;
        paddle2_position += paddle2_movement * paddle2_speed * deltaTime;
    }

    if (ball_position.y > 3.0f) {
        posY = false;
    }
    else if (ball_position.y < -3.0f) {
        posY = true;
    }
    
    if (posY) {
        ball_position.y += ball_movement.y * ball_speed * deltaTime;
    }
    else {
        ball_position.y -= ball_movement.y * ball_speed * deltaTime;
    }
    
    float xdist2 = fabs(paddle2_position.x - ball_position.x) - ((.25 + .25) / 2.0f); //p1 = ball p2 == paddle 2
    float ydist2 = fabs(paddle2_position.y - ball_position.y) - ((.25 + 1.5) / 2.0f);
    
    float xdist1 = fabs(paddle1_position.x - ball_position.x) - ((.25 + .25) / 2.0f); //p1 = ball p2 == paddle 2
    float ydist1 = fabs(paddle1_position.y - ball_position.y) - ((.25 + 1.5) / 2.0f);
    
    if (xdist2 < 0 && ydist2 < 0) {
        changeX = !changeX;
    }
    if (xdist1 < 0 && ydist1 < 0) {
        changeX = !changeX;
    }
    if (changeX) {
        ball_position.x -= ball_movement.x * ball_speed * deltaTime;
    }
    else {
        ball_position.x += ball_movement.x * ball_speed * deltaTime;
    }
    
    paddle1_position += paddle1_movement * paddle1_speed * deltaTime;
    paddle2_position += paddle2_movement * paddle2_speed * deltaTime;
    
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);
    
    paddle1Matrix = glm::mat4(1.0f);
    paddle1Matrix = glm::translate(paddle1Matrix, paddle1_position);

    paddle2Matrix = glm::mat4(1.0f);
    paddle2Matrix = glm::translate(paddle2Matrix, paddle2_position);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = { -0.25, -1.5, 0.25, -1.5, 0.25, 1.5, -0.25, -1.5, 0.25, 1.5, -0.25, 1.5 };
    float texCoords[] = { 0.0, 1.5, 0.25, 1.5, .25, 0.0, 0.0, 1.5, .25, 0.0, 0.0, 0.0 };
    
    float ball_vertices[] = {-0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25};
    float ball_texCoords[] = {0.0, .5, .5, .5, .5, 0.0, 0.0, .5, .5, 0.0, 0.0, 0.0};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(paddle1Matrix);
    glBindTexture(GL_TEXTURE_2D, paddle1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(paddle2Matrix);
    glBindTexture(GL_TEXTURE_2D, paddle2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, ball_texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ball);
    glDrawArrays(GL_TRIANGLES, 0, 6);
     
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}

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

#include "Entity.h"

#define PLATFORM_COUNT 11
#define ENEMY_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
    Entity *text;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
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
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
   
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 1.0f;
    state.player->jumpPower = 6.0f;
    state.player->textureID = LoadTexture("george_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.0f;
    
    state.text = new Entity[2];
    GLuint letterTextureID = LoadTexture("font2.png");
    state.text[0].textureID = letterTextureID;
    state.text[0].position = glm::vec3(-2.15f, 2.5f,0);
    state.text[0].entityType = WIN_LETTERS;
    state.text[0].isActive = false;
    state.text[1].textureID = letterTextureID;
    state.text[1].position = glm::vec3(-2.15f, 2.5f,0);
    state.text[1].entityType = LOSE_LETTERS;
    state.text[1].isActive = false;
    state.text[0].Update(0,nullptr, nullptr, nullptr, 0, 0);
    state.text[1].Update(0,nullptr, nullptr, nullptr, 0, 0); 
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    GLuint platformTextureID = LoadTexture("platformPack_tile007.png");
    state.platforms = new Entity[PLATFORM_COUNT];
    state.platforms[0].textureID = platformTextureID;
    state.platforms[0].position = glm::vec3(3, -3.25f, 0);
    state.platforms[0].entityType = WIN_PLATFORM;
    state.platforms[1].textureID = platformTextureID;
    state.platforms[1].position = glm::vec3(-3, -3.25f, 0);
    state.platforms[1].entityType = WIN_PLATFORM;
    
    GLuint platformTextureIDLose = LoadTexture("platformPack_tile020.png");
    
    
    state.platforms[2].textureID = platformTextureIDLose;
    state.platforms[2].position = glm::vec3(-4, -3.25f, 0);
    state.platforms[3].textureID = platformTextureIDLose;
    state.platforms[3].position = glm::vec3(-5, -3.25f, 0);
    state.platforms[4].textureID = platformTextureIDLose;
    state.platforms[4].position = glm::vec3(-2, -3.25f, 0);
    state.platforms[5].textureID = platformTextureIDLose;
    state.platforms[5].position = glm::vec3(-1, -3.25f, 0);
    state.platforms[6].textureID = platformTextureIDLose;
    state.platforms[6].position = glm::vec3(0, -3.25f, 0);
    state.platforms[7].textureID = platformTextureIDLose;
    state.platforms[7].position = glm::vec3(1, -3.25f, 0);
    state.platforms[8].textureID = platformTextureIDLose;
    state.platforms[8].position = glm::vec3(2, -3.25f, 0);
    state.platforms[9].textureID = platformTextureIDLose;
    state.platforms[9].position = glm::vec3(4, -3.25f, 0);
    state.platforms[10].textureID = platformTextureIDLose;
    state.platforms[10].position = glm::vec3(5, -3.25f, 0);
    state.platforms[11].textureID = platformTextureIDLose;
    state.platforms[11].position = glm::vec3(6, -3.25f, 0);
    
    
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, nullptr, nullptr, nullptr, 0, 0);
    }
    
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("duck.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].speed = 0.5f;
        state.enemies[i].height = 0.85;
        state.enemies[i].isActive = true;
        state.enemies[i].position = glm::vec3(2.5f +i, -2.25f, 0);
        state.enemies[i].Update(0, nullptr, nullptr, nullptr, 0, 0);
    }
    
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = WALKING;
    
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].aiState = IDLE;
    
    state.enemies[2].aiType = WAITANDGO;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0,-9.81f,0);
    state.enemies[2].jumpPower = 5.0f;
    
    
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
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
                        
                        break;
                        
                    case SDLK_SPACE:
                        state.player->jump = true;
                        break;
                }

                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.50f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.50f;
        state.player->animIndices = state.player->animRight;
    }
    

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, state.enemies, PLATFORM_COUNT, ENEMY_COUNT);
        deltaTime -= FIXED_TIMESTEP;
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, state.enemies, PLATFORM_COUNT, ENEMY_COUNT);
        }
    }
    if (state.player->lastCollision == ENEMY) {
        if (state.player->collidedLeft || state.player->collidedRight) {
            if (state.player->lastColl->isActive == true) {
                state.text[1].isActive = true;
            }
        }
    }
    
    bool allDead = true;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive) {allDead = false;}
    }
    if (allDead) {
        state.text[0].isActive = true;
    }
//    if (state.player->lastCollision == LOSE_PLATFORM || state.player->lastCollision == SAW) {
//        state.text[1].isActive = true;
//        state.player->velocity = glm::vec3(0);
//        state.player->acceleration = glm::vec3(0);
//
//    }
//    else if (state.player->lastCollision == WIN_PLATFORM) {
//        state.text[0].isActive = true;
//        state.player->velocity = glm::vec3(0);
//        state.player->acceleration = glm::vec3(0);
//    }
    
    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    state.text[0].Render(&program);
    state.text[1].Render(&program);
    state.player->Render(&program);
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
    
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

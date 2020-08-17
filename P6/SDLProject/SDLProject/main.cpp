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
#include "SDL_mixer.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"

#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"



SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music *music;
Mix_Chunk *bounce;

Scene *currentScene;
Scene *sceneList[5];

void SwitchToScene(Scene *scene, int lives) {
    currentScene = scene;
    currentScene->Initialize();
    currentScene->state.lives = lives;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("back.mp3");
    Mix_PlayMusic(music, -1);
    
    bounce = Mix_LoadWAV("jump.wav");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -4.5f, 4.0f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    
    sceneList[0] = nullptr; //IDK why but index zero wont render
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level0();
    SwitchToScene(sceneList[4], 3);
    
    
    
}

void ProcessInput() {
    
    if (currentScene != sceneList[0]) {
        currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        // Move the player left
                        if (currentScene == sceneList[4]) {
                            SwitchToScene(sceneList[1], 3);
                        }
                        break;
                        
                    case SDLK_RIGHT:
                        
                        break;
                        
                    case SDLK_SPACE:
                        Mix_PlayChannel(-1, bounce, 0);
                        currentScene->state.player->jump = true;
                        break;
                }

                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (currentScene != sceneList[4]) {
        currentScene->state.player->movement.x = 1.50f;
    }
    
    
    if (keys[SDL_SCANCODE_LEFT]) {
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    
    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
    
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
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
        glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    } else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    

}
    
void Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
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
        
        if (currentScene->state.nextScene >= 0) {
            SwitchToScene(sceneList[currentScene->state.nextScene], currentScene->state.lives);
        }
            
        Render();
    }
    
    Shutdown();
    return 0;
}

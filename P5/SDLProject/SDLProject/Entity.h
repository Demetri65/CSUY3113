#define GL_SILENCE_DEPRECATION
#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"

enum EntityType { PLAYER, WIN_PLATFORM, WIN_LETTERS, LOSE_PLATFORM, LOSE_LETTERS, ENEMY};

enum AIType {WALKER, JUMPER, WAITANDGO};
enum AIState {IDLE, WALKING, JUMPING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    bool isActive;
    
    bool jump = false;
    float jumpPower;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    EntityType lastCollision;
    Entity* lastColl;
    
    float speed;
    
    float height = .9f;
    float width = .6;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();
    
    bool CheckCollision(Entity* other);
    void CheckEnemyCollX(Entity* enemies, int enemyNum);
    void CheckEnemyCollY(Entity* enemies, int enemyNum);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    
    
    void Update(float deltaTime, Entity* player, Map* map, Entity* object, int objectCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
    void AI(Entity *player);
    void AIWalker();
    void AIJumper(Entity *player);
    void AIWaitNGo(Entity *player);
};

#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 1

unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
     
    state.nextScene = -1; 
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 1.0f;
    state.player->jumpPower = 6.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    
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
    
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
        
        
    state.enemies = new Entity();
    GLuint enemyTextureID = Util::LoadTexture("duck.png");
    
    state.enemies->entityType = ENEMY;
    state.enemies->textureID = enemyTextureID;
    state.enemies->speed = 0.5f;
    state.enemies->height = 1.0f;
    state.enemies->width = 1.0f;

    state.enemies->aiType = JUMPER;
    state.enemies->aiState = IDLE;
    state.enemies->animIndices = nullptr;
    state.enemies->position = glm::vec3(5, -5, 0);
    state.enemies->movement = glm::vec3(0);
    state.enemies->acceleration = glm::vec3(0,-9.81f,0);

}
void Level1::Update(float deltaTime) {
    state.enemies->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    
    if ((state.player->collidedLeft || state.player->collidedRight) && state.player->lastCollision == ENEMY) {
        state.player->lastColl->isActive = false;
        state.lives--;
    }
    
    bool allDead = true;
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive) {allDead = false;}
    }
    if (allDead) {state.nextScene = 2;}
}
void Level1::Render(ShaderProgram *program) {
    std::string lives = "Lives: " + std::to_string(state.lives);
    Util::DrawText(program, Util::LoadTexture("font2.png"), lives, .25f, .25f, glm::vec3(state.player->position.x -4.3f, -.5f,0));
    state.map->Render(program);
    state.enemies[0].Render(program);
    state.player->Render(program);
}

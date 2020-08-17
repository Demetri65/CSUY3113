#include "Level2.h"
#define LEVEL2_WIDTH 60
#define LEVEL2_HEIGHT 9

#define LEVEL2_ENEMY_COUNT 1

unsigned int level2_data[] =
{
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6,
    17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0, 0, 6, 6, 0, 6, 0, 0, 6, 6, 0, 0, 0, 0,
    17, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
};

void Level2::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("platformPack_tilesheet.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 14, 7);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 1.0f;
    state.player->jumpPower = 6.0f;
    state.player->textureID = Util::LoadTexture("bird.png");

    state.player->movement = glm::vec3(0);
    state.player->speed = 1.0f;
        
        
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
    state.enemies->position = glm::vec3(7, -5, 0);
    state.enemies->movement = glm::vec3(0);
    state.enemies->acceleration = glm::vec3(0,-9.81f,0);
    
}
void Level2::Update(float deltaTime) {
    state.enemies->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT);
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT);
    
    if (state.player->collidedLeft || state.player->collidedRight || state.player->collidedBottom || state.player->collidedTop) {
        state.nextScene = 3;
        state.lives--;
    }
    if (state.player->position.x > 60) {
       state.nextScene = 3;
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.enemies[0].Render(program);
    state.player->Render(program);
    std::string lives = "Lives: " + std::to_string(state.lives);
    Util::DrawText(program, Util::LoadTexture("font2.png"), lives, .25f, .25f, glm::vec3(state.player->position.x -4.3f, -.5f,0));
}

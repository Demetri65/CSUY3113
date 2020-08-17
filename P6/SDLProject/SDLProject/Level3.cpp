#include "Level3.h"
#define LEVEL3_WIDTH 60
#define LEVEL3_HEIGHT 9

#define LEVEL3_ENEMY_COUNT 1

unsigned int level3_data[] =
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

void Level3::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("platformPack_tilesheet.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 14, 7);
    
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
    state.enemies->position = glm::vec3(23, -2, 0);
    state.enemies->movement = glm::vec3(0);
    state.enemies->acceleration = glm::vec3(0,-9.81f,0);
    
}
void Level3::Update(float deltaTime) {
    state.enemies->Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY_COUNT);
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY_COUNT);
    
    if (state.player->collidedLeft || state.player->collidedRight || state.player->collidedBottom || state.player->collidedTop) {
        if (!state.gameOver) {
            state.lives--;
        }
        state.gameOver = true;
    }
    if (state.player->position.x > 60) {
       state.gameOver = true;
    }
}
void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.enemies[0].Render(program);
    state.player->Render(program);
    std::string lives = "Lives: " + std::to_string(state.lives);
    Util::DrawText(program, Util::LoadTexture("font2.png"), lives, .25f, .25f, glm::vec3(state.player->position.x -4.3f, -.5f,0));
    if (state.gameOver) {
        if (state.lives > 0) {
            Util::DrawText(program, Util::LoadTexture("font2.png"), "You Win!", .25f, .25f, glm::vec3(state.player->position.x-2, -2.0f,0));
        } else {
            Util::DrawText(program, Util::LoadTexture("font2.png"), "You Lose!", .25f, .25f, glm::vec3(state.player->position.x-2, -2.0f,0));
        }
        state.player->speed = 0;
    }
}

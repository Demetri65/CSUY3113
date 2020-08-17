#include "Level0.h"
#define LEVEL0_WIDTH 14
#define LEVEL0_HEIGHT 9

#define LEVEL0_ENEMY_COUNT 1

unsigned int level0_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


void Level0::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("platformPack_tilesheet.png");
    state.map = new Map(LEVEL0_WIDTH, LEVEL0_HEIGHT, level0_data, mapTextureID, 1.0f, 14, 7);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 1.0f;
    state.player->jumpPower = 6.0f;
    state.player->textureID = Util::LoadTexture("bird.png");

    state.player->movement = glm::vec3(0);
    state.player->speed = 1.0f;

    
    state.enemies = new Entity[LEVEL0_ENEMY_COUNT];
}
void Level0::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL0_ENEMY_COUNT);
    
    
}
void Level0::Render(ShaderProgram *program) {
    state.map->Render(program);
    Util::DrawText(program, Util::LoadTexture("font2.png"), "Duck Takeover", .25f, .25f, glm::vec3(2.0f, -3,0));
    Util::DrawText(program, Util::LoadTexture("font2.png"), "Press Enter to Start", .25f, .20f, glm::vec3(0.7f, -4,0));
    state.player->Render(program);
}

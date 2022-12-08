#pragma once

#include "GameState.hpp"
#include "MainGame.hpp"

#include "object/enemy/EnemySpawner.hpp"
#include "object/enemy/FloatingMine.hpp"
#include "object/enemy/FireballMine.hpp"
#include "object/enemy/MagneticMine.hpp"
#include "object/enemy/MagneticFIreballMine.hpp"
#include "object/enemy/MineLayer.hpp"

class AnimGame : public GameState
{
private:
    MainGame& game;
    C2D_Sprite shipSprite;
    C2D_Sprite bulletSprite;
    C2D_TextBuf buffer;
    C2D_Text textL;
    float animTimer = 0.0f;
    u32 level = 0;
    FloatingMine floating;
    MagneticMine magnetic;
    FireballMine fireball;
    MagneticFireballMine magneticFireball;
    MineLayer mineLayer;

    void InitGame();
    void InitLevel();
    void AddEnemy(Enemy* type, u32 count);
public:
    AnimGame(C2D_SpriteSheet& spriteSheet, MainGame& gameIn);
    ~AnimGame();

    u32 GetLevel();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};

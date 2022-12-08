#pragma once

#include <vector>

#include "GameState.hpp"

#include "object/Scores.hpp"
#include "object/Bullet.hpp"
#include "object/enemy/Enemy.hpp"
#include "object/enemy/EnemySpawner.hpp"
#include "object/Player.hpp"
#include "object/particle/Particle.hpp"
#include "helper/InputHelper.hpp"

class AnimGame;
class PauseMenu;

class MainGame : public GameState
{
private:
    Player player;
    std::vector<Enemy*> enemies;
    std::vector<EnemySpawner> enemySpawners;
    std::vector<Bullet> bullets;
    std::vector<Particle*> particles;
    C2D_Image spriteImages[15];
    Scores& scores;

    void UpdateCollisions();
public:
    MainGame(C2D_SpriteSheet& spriteSheet, Scores& scoreIn);
    ~MainGame();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
    void TopScreenExtras() override;
    void SpawnBullet(bool isPlayerSpawned, float rotation, float x, float y);
    void CreateThrustParticle(float posX, float posY, float angle, float speed);
    void CreateExplosion(float posX, float posY);
    void AddToScore(u32 value);
    void AddEnemy(Enemy* element);
    Enemy* GetRandomEnemy();
    EnemySpawner& GetFirstSpawner();
    Vec2D GetPlayerPosition(Vec2D refPos);
    Vec2D GetNearestClampedPos(Vec2D pos, Vec2D refPos);
    std::vector<EnemySpawner>& GetSpawners();

    friend class AnimGame;
    friend class PauseMenu;
};

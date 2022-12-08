#include "gamestate/MainGame.hpp"

#include "object/particle/ThrustParticle.hpp"
#include "object/particle/ExplosionParticle.hpp"
#include "helper/RenderHelper.hpp"
#include "object/enemy/FloatingMine.hpp"
#include "object/enemy/MagneticFireballMine.hpp"
#include "object/enemy/MagneticMine.hpp"
#include "object/enemy/FireballMine.hpp"
#include "object/enemy/MineLayer.hpp"
#include "helper/SoundHelper.hpp"

MainGame::MainGame(C2D_SpriteSheet& spriteSheet, Scores& scoreIn) : player(spriteSheet, inputs), scores(scoreIn)
{
    for (u8 i = 0; i < 15; i++)
    {
        spriteImages[i] = C2D_SpriteSheetGetImage(spriteSheet, i+2);
    }
    Bullet::SetSourceImage(&spriteImages[0]);
    EnemySpawner::SetSourceImage(&spriteImages[1]);
    MineLayer::SetSourceImage(&spriteImages[2]);
    FloatingMine::SetSourceImage(&spriteImages[3]);
    MagneticMine::SetSourceImage(&spriteImages[6]);
    MagneticFireballMine::SetSourceImage(&spriteImages[9]);
    FireballMine::SetSourceImage(&spriteImages[12]);
}

MainGame::~MainGame()
{
}

void MainGame::SpawnBullet(bool isPlayerSpawned, float rotation, float x, float y)
{
    for (u32 i = 0; i < static_cast<u32>(bullets.size()); i++)
    {
        if (bullets[i].IsValid) continue;
        bullets[i] = Bullet(rotation, x, y, isPlayerSpawned);
        return;
    }
    bullets.push_back(Bullet(rotation, x, y, isPlayerSpawned));
}

void MainGame::CreateThrustParticle(float posX, float posY, float angle, float speed)
{
    for (u32 i = 0; i < static_cast<u32>(particles.size()); i++)
    {
        if (particles[i] && particles[i]->IsValid) continue;
        if (particles[i]) delete particles[i];
        particles[i] = new ThrustParticle(Vec2D(posX, posY), angle, speed);
        return;
    }
    particles.push_back(new ThrustParticle(Vec2D(posX, posY), angle, speed));
}

void MainGame::CreateExplosion(float posX, float posY)
{
    u8 count = 10;
    for (u32 i = 0; i < particles.size(); i++) {
        if (particles[i] && particles[i]->IsValid) continue;
        if (particles[i]) delete particles[i];
        particles[i] = new ExplosionParticle(Vec2D(posX, posY), RandomRange(0.0f, M_PI*2), RandomRange(0.7f, 1.1f), RandomRange(-.2f,0));
        count--;
        if (count <= 0) return;
    }
    while (count > 0)
    {
        particles.push_back(new ExplosionParticle(Vec2D(posX, posY), RandomRange(0.0f, M_PI*2), RandomRange(0.7f, 1.1f), RandomRange(-.2f,0)));
        count--;
    }
}

void MainGame::AddToScore(u32 value)
{
    scores.actualScore += value;
}

EnemySpawner& MainGame::GetFirstSpawner()
{
    for (u32 i = 0; i < enemySpawners.size(); i++)
    {
        if (enemySpawners[i].IsValid && !enemySpawners[i].assigned)
        {
            return enemySpawners[i];
        }
    }
    printf("Warning! Not enough spawners");
    enemySpawners.push_back(EnemySpawner());
    return enemySpawners.back();
}

GameType MainGame::UpdateGameState(GameType lastSate, float deltaTime)
{
    player.Update(deltaTime, this);
    bool isFinished = true;
    for (u32 i = 0; i < enemies.size(); i++)
    {
        if (enemies[i])
        {
            if (enemies[i]->IsValid)
            {
                enemies[i]->Update(deltaTime, this);
                isFinished = false;
            }
            if (!enemies[i]->IsValid)
            {
                delete enemies[i];
                enemies[i] = nullptr;
            }
        }
    }
    for (u32 i = 0; i < enemySpawners.size(); i++)
    {
        if (enemySpawners[i].IsValid)
        {
            enemySpawners[i].Update(deltaTime, this);
            isFinished = false;
        }
    }
    for (u32 i = 0; i < bullets.size(); i++)
    {
        if (bullets[i].IsValid)
        {
            bullets[i].Update(deltaTime, this);
        }
    }
    for (u32 i = 0; i < particles.size(); i++)
    {
        if (particles[i])
        {
            if (particles[i]->IsValid)
            {
                particles[i]->Update(deltaTime);
            }
            if (!particles[i]->IsValid)
            {
                delete particles[i];
                particles[i] = nullptr;
            }
        }
    }
    UpdateCollisions();
    if (inputs->GetButton(KEY_START | KEY_SELECT, false))
    {
        return GameType::MENU_PAUSE;
    }
    if (isFinished)
    {
        return GameType::MENU_WIN;
    }
    if (player.life == 0)
    {
        return GameType::MENU_GAMEOVER;
    }
    return GameType::GAME_MAIN;
}

void MainGame::Render(bool isTopScreen)
{
    bool debug = inputs->IsDebug();
    for (u32 i = 0; i < particles.size(); i++)
    {
        if (particles[i] && particles[i]->IsValid)
        {
            particles[i]->Render(isTopScreen);
        }
    }
    for (u32 i = 0; i < enemySpawners.size(); i++)
    {
        if (enemySpawners[i].IsValid)
        {
            enemySpawners[i].Render(isTopScreen);
        }
    }
    for (u32 i = 0; i < enemies.size(); i++)
    {
        if (enemies[i] && enemies[i]->IsValid)
        {
            enemies[i]->Render(isTopScreen);
            if (debug && enemies[i]->isActive)
            {
                Poly* pl = enemies[i]->GetPolys(Vec2D(enemies[i]->spr.params.pos.x, enemies[i]->spr.params.pos.y));
                for (u8 n = 0; n < enemies[i]->GetPolyCount(); n++)
                {
                    pl[n].RenderPoly(isTopScreen);
                }
            }
        }
    }
    for (u32 i = 0; i < bullets.size(); i++)
    {
        if (bullets[i].IsValid)
        {
            bullets[i].Render(isTopScreen);
            if (debug)
            {
                Poly* pl = bullets[i].GetPoly();
                pl->RenderPoly(isTopScreen);
            }
        }
    }
    player.Render(isTopScreen);
    if (debug)
    {
        Poly* pl = player.GetPoly();
        pl->RenderPoly(isTopScreen);
    }
}

Vec2D MainGame::GetPlayerPosition(Vec2D refPos)
{
    return GetNearestClampedPos(Vec2D(player.spr.params.pos.x, player.spr.params.pos.y), refPos);
}

Vec2D MainGame::GetNearestClampedPos(Vec2D pos, Vec2D refPos)
{
    Vec2D minPos;
    float min = infinityf();
    for (s8 i = -1; i < 2; i++)
    {
        for (s8 j = -1; j < 2; j++)
        {
            Vec2D elemPos = pos + Vec2D(SCREEN_WIDTH * i, SCREEN_DHEIGHT * j);
            float dist = (elemPos - refPos).lengthSquared();
            if (dist < min)
            {
                min = dist;
                minPos = elemPos;
            }
        }
    }
    return minPos;
}

void MainGame::UpdateCollisions()
{
    for (u32 i = 0; i < bullets.size(); i++)
    {
        if (!bullets[i].IsValid) continue;
        Poly* box = bullets[i].GetPoly();
        Vec2D a = Vec2D(bullets[i].spr.params.pos.x, bullets[i].spr.params.pos.y);
        if (bullets[i].isPlayerSpawned)
        {
            for (u32 j = 0; j < enemies.size(); j++)
            {
                if (!enemies[j] || !enemies[j]->IsValid || !enemies[j]->isActive) continue;
                Vec2D b = GetNearestClampedPos(Vec2D(enemies[j]->spr.params.pos.x, enemies[j]->spr.params.pos.y), a);
                if ((a-b).lengthSquared() < 1600)
                {
                    Poly* other = enemies[j]->GetPolys(b);
                    u8 count = enemies[j]->GetPolyCount();
                    if (box->collideMultiple(other, count))
                    {
                        enemies[j]->KillEnemy(this);
                        bullets[i].IsValid = false;
                        break;
                    }
                }
            }
        }
        else
        {
            for (u32 j = 0; j < bullets.size(); j++)
            {
                if (!bullets[j].IsValid || !bullets[j].isPlayerSpawned) continue;
                Vec2D b = GetNearestClampedPos(Vec2D(bullets[j].spr.params.pos.x, bullets[j].spr.params.pos.y), a);
                if ((a-b).lengthSquared() < 50)
                {
                    bullets[i].IsValid = false;
                    bullets[j].IsValid = false;
                    SoundHelper::PlayHitSound();
                    scores.actualScore += 100;
                    break;
                }
            }
            if (player.CanBeDamaged())
            {
                Vec2D b = GetNearestClampedPos(Vec2D(player.spr.params.pos.x, player.spr.params.pos.y), a);
                if ((a-b).lengthSquared() < 1024)
                {
                    Poly* other = player.GetPoly();
                    if (box->collide(*other))
                    {
                        player.Damage();
                        bullets[i].IsValid = false;
                    }
                }
            }
        }
    }
    if (!player.CanBeDamaged()) return;
    Poly* box = player.GetPoly();
    Vec2D a = Vec2D(player.spr.params.pos.x, player.spr.params.pos.y);
    for (u32 i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i] || !enemies[i]->IsValid || !enemies[i]->isActive) continue;
        Vec2D b = GetNearestClampedPos(Vec2D(enemies[i]->spr.params.pos.x, enemies[i]->spr.params.pos.y), a);
        if ((a-b).lengthSquared() < 2916)
        {
            Poly* other = enemies[i]->GetPolys(b);
            u8 count = enemies[i]->GetPolyCount();
            if (box->collideMultiple(other, count))
            {
                player.Damage();
                break;
            }
        }
    }
}

void MainGame::AddEnemy(Enemy* element)
{
    for (u32 i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i] || !enemies[i]->IsValid)
        {
            if (enemies[i]) delete enemies[i];
            enemies[i] = element;
            return;
        }
    }
    enemies.push_back(element);
}

Enemy* MainGame::GetRandomEnemy()
{
    std::vector<Enemy*> newVec;
    for (u32 i = 0; i < enemies.size(); i++)
    {
        if (enemies[i] && enemies[i]->IsValid && enemies[i]->canDivide)
        {
            newVec.push_back(enemies[i]);
        }
    }
    if (newVec.size() == 0)
    {
        return nullptr;
    }
    u32 index = RandomRange(0, newVec.size());
    if (index >= newVec.size()) index = newVec.size()-1;
    return newVec[index];
}

std::vector<EnemySpawner>& MainGame::GetSpawners()
{
    return enemySpawners;
}

void MainGame::TopScreenExtras()
{
    GameState::TopScreenExtras();
    float& posX = player.spr.params.pos.x;
    float& posY = player.spr.params.pos.y;
    Vec2D tmpPos = Vec2D(posX, posY);
    float tmpRot = player.spr.params.angle;
    posX = inputs->IsWide() ? 20 : 60;
    player.spr.params.angle = 0.0f;
    for (u8 i = 0; i < player.life; i++)
    {
        posY = SCREEN_HEIGHT/2 - i*50 + 50;
        C2D_DrawSprite(&player.spr);
    }
    posX = tmpPos.x;
    posY = tmpPos.y;
    player.spr.params.angle = tmpRot;
}
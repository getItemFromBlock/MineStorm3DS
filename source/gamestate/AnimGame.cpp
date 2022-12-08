#include "gamestate/AnimGame.hpp"

#include "helper/RenderHelper.hpp"

AnimGame::AnimGame(C2D_SpriteSheet& spriteSheet, MainGame& gameIn) : game(gameIn)
{
    buffer = C2D_TextBufNew(16);
    C2D_SpriteFromSheet(&bulletSprite, spriteSheet, 2);
    C2D_SpriteFromSheet(&shipSprite, spriteSheet, 4);
    C2D_SpriteSetCenter(&bulletSprite, 0.5f, 0.5f);
    C2D_SpriteSetCenter(&shipSprite, 0.5f, 0.5f);
}

AnimGame::~AnimGame()
{
    C2D_TextBufDelete(buffer);
}

u32 AnimGame::GetLevel()
{
    return level;
}

GameType AnimGame::UpdateGameState(GameType lastSate, float deltaTime)
{
    if (lastSate != GameType::GAME_ANIM)
    {
        if (lastSate == GameType::MENU_MAIN)
        {
            level = 1;
        }
        else
        {
            level++;
        }
        InitGame();
    }
    shipSprite.params.pos.x = SCREEN_WIDTH/2;
    shipSprite.params.pos.y = animTimer * 200 - 150;
    animTimer -= deltaTime;
    if (animTimer <= 0.0f) return GameType::GAME_MAIN;
    return GameType::GAME_ANIM;
}

void AnimGame::Render(bool isTopScreen)
{
    s32 deltaPosY = isTopScreen ? 0 : SCREEN_HEIGHT;
    s32 deltaPosX = isTopScreen ? 40 : 0;
    static const u32 color = C2D_Color32f(0.4f, 0.9f, 1.0f, 1.0f);
    for (u32 i = 0; i < game.enemySpawners.size(); i++)
    {
        if (shipSprite.params.pos.y > game.enemySpawners[i].spr.params.pos.y) continue;
        float decX = game.enemySpawners[i].spr.params.pos.x - shipSprite.params.pos.x;
        float decY = game.enemySpawners[i].spr.params.pos.y - shipSprite.params.pos.y;
        float delta = (decY - fabsf(decX));
        if (delta >= 0.0f)
        {
            game.enemySpawners[i].Render(isTopScreen);
        }
        else
        {
            bulletSprite.params.pos.x = shipSprite.params.pos.x + copysignf(decY, decX) + deltaPosX;
            bulletSprite.params.pos.y = game.enemySpawners[i].spr.params.pos.y - deltaPosY;
            C2D_DrawSprite(&bulletSprite);
        }
    }
    shipSprite.params.pos.y -= deltaPosY;
    shipSprite.params.pos.x += deltaPosX;
    C2D_DrawSprite(&shipSprite);
    shipSprite.params.pos.y += deltaPosY;
    shipSprite.params.pos.x -= deltaPosX;
    if (isTopScreen)
    {
        C2D_DrawText(&textL, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2 + 40, 80, 0.0f, 1.0f, 1.0f, color);
    }
}

void AnimGame::InitGame()
{
    animTimer = 3.5f;
    char tmp[16];
    snprintf(tmp, 16, "LEVEL %lu", level);
    C2D_TextBufClear(buffer);
    C2D_TextFontParse(&textL, NULL, buffer, tmp);
    C2D_TextOptimize(&textL);
    game.enemySpawners.clear();
    game.bullets.clear();
    for (u32 i = 0; i < game.particles.size(); i++)
    {
        if (game.particles[i])
        {
            delete game.particles[i];
        }
    }
    game.particles.clear();
    for (u32 i = 0; i < game.enemies.size(); i++)
    {
        if (game.enemies[i])
        {
            delete game.enemies[i];
        }
    }
    game.enemies.clear();
    game.player.Reset();
    InitLevel();
}

void AnimGame::AddEnemy(Enemy* type, u32 count)
{
    Enemy* tmp;
    for (u32 i = 0; i < count; i++)
    {
        tmp = type->CreateNew(Vec2D(), RandomRange(0.0f, 2*M_PI), type->GetDefaultVelocity(), EnemySize::SIZE_LARGE);
        game.enemies.push_back(tmp);
        if (type == &mineLayer)
        {
            game.enemySpawners.push_back(EnemySpawner());
            game.enemySpawners.back().Assign(tmp);
        }
        else
        {
            for (u8 n = 0; n < 7; n++) game.enemySpawners.push_back(EnemySpawner());
            game.enemySpawners.back().Assign(tmp);
        }
    }
}

void AnimGame::InitLevel()
{
    switch (level)
    {
    case 1:
        AddEnemy(&floating, 3);
        AddEnemy(&mineLayer, 1);
        return;
    case 2:
        AddEnemy(&floating, 5);
        return;
    case 3:
        AddEnemy(&floating, 5);
        AddEnemy(&magnetic, 1);
        return;
    case 4:
        AddEnemy(&floating, 3);
        AddEnemy(&magnetic, 2);
        AddEnemy(&fireball, 1);
        AddEnemy(&mineLayer, 1);
        return;
    case 5:
        AddEnemy(&floating, 2);
        AddEnemy(&magnetic, 3);
        AddEnemy(&fireball, 3);
        return;
    case 6:
        AddEnemy(&magnetic, 3);
        AddEnemy(&fireball, 3);
        AddEnemy(&magneticFireball, 1);
        return;
    case 7:
        AddEnemy(&magnetic, 3);
        AddEnemy(&fireball, 4);
        AddEnemy(&magneticFireball, 2);
        return;
    case 8:
        AddEnemy(&floating, 2);
        AddEnemy(&magnetic, 2);
        AddEnemy(&fireball, 2);
        AddEnemy(&magneticFireball, 2);
        AddEnemy(&mineLayer, 2);
        return;
    case 9:
        AddEnemy(&magnetic, 2);
        AddEnemy(&fireball, 4);
        AddEnemy(&magneticFireball, 4);
        return;
    case 10:
        AddEnemy(&magneticFireball, 8);
        AddEnemy(&mineLayer, 3);
        return;
    default:
        AddEnemy(&floating, 20);
        return;
    }
}
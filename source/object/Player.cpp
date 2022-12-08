#include "object/Player.hpp"

#include "helper/RenderHelper.hpp"
#include "gamestate/MainGame.hpp"
#include "helper/SoundHelper.hpp"

#define BULLET_COOLDOWN 0.3f
#define PLAYER_VELOCITY 50.0f
#define PLAYER_FRICTION 0.96f
#define PLAYER_SHOOT_COOLDOWN 10
#define PLAYER_TELEPORT_COOLDOWN 100
#define PLAYER_DAMAGE_COOLDOWN 120
#define PLAYER_ANGULAR_VELOCITY M_PI

static const Vec2D PlayerHitBox[7] = {
    {-2.5f, -74.0f}, {29.0f, 4.0f}, {40.0f, 51.0f}, {30.0f, 72.0f}, {-34.0f, 72.0f}, {-45.0f, 51.0f}, {-34.0f, 4.0f}
};

static Poly hitbox[] = {
    Poly(PlayerHitBox, 7),
};

Poly* Player::GetPoly()
{
    hitbox->SetParameters(Vec2D(spr.params.pos.x, spr.params.pos.y), 0.265f, spr.params.angle);
    return hitbox;
}

Player::Player(C2D_SpriteSheet& spriteSheet, InputHelper* inputsIn) : inputs(inputsIn)
{
    C2D_SpriteFromSheet(&spr, spriteSheet, 1);
    C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
	Reset();
}

Player::~Player()
{
}

void Player::Update(float deltaTime, GameState* holder)
{
    bulletTimer -= deltaTime;
    invTimer -= deltaTime;
    float ang = spr.params.angle - M_PI_2;
    if (inputs->GetButton(KEY_A, true))
    {
        ((MainGame*)holder)->CreateThrustParticle(spr.params.pos.x, spr.params.pos.y, spr.params.angle + M_PI_2 + RandomRange(-0.7, 0.7), 100);
        vel = vel + Vec2D(cosf(ang), sinf(ang)) * PLAYER_VELOCITY;
        float speed = vel.getLength();
        if (speed > PLAYER_VELOCITY * 2.5f)
        {
            vel = vel.unitVector() * PLAYER_VELOCITY * 2.5f;
        }
    }
    vel = vel * (1 - ((1 - PLAYER_FRICTION) * 60 * deltaTime));
    spr.params.angle = spr.params.angle + inputs->GetSteer() * PLAYER_ANGULAR_VELOCITY * deltaTime;
    C2D_SpriteMove(&spr, vel.x * deltaTime, vel.y * deltaTime);
    if (inputs->GetButton(KEY_B, true))
    {
        if (bulletTimer <= 0.0f)
        {
            bulletTimer = BULLET_COOLDOWN;
            Vec2D pos = Vec2D(spr.params.pos.x, spr.params.pos.y) + Vec2D(cosf(ang), sinf(ang)) * 5.0f;
            ((MainGame*)holder)->SpawnBullet(true, ang, pos.x, pos.y);
            SoundHelper::PlayShootSound();
        }
    }
    Entity::Update(deltaTime, holder);
}

void Player::Reset()
{
    C2D_SpriteSetPos(&spr, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT);
	C2D_SpriteSetRotation(&spr, 0.0f);
    vel = Vec2D();
    speed = 0.0f;
    bulletTimer = 0.0f;
    life = 3;
    invTimer = 0.0f;
}

void Player::Damage()
{
    if (invTimer <= 0.0f && life > 0)
    {
        life--;
        invTimer = 3.0f;
        SoundHelper::PlayDamageSound();
    }
}

bool Player::CanBeDamaged()
{
    return invTimer <= 0.0f && life > 0;
}

u8 Player::GetLife()
{
    return life;
}

void Player::Render(bool isTopScreen)
{
    if (invTimer <= 0.0f || (static_cast<s32>(invTimer*6)%2 == 1))
    {
        Entity::Render(isTopScreen);
    }
}
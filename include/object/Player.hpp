#pragma once

#include <citro2d.h>
#include <3ds.h>

#include "Entity.hpp"
#include "helper/MathHelper.hpp"
#include "helper/InputHelper.hpp"
#include "helper/ColliderHelper.hpp"

class MainGame;
class PauseMenu;

class Player : public Entity
{
	private:
		float speed = 0.0f;
		float bulletTimer = 0.0f;
		float invTimer = 0.0f;
		u8 life = 3;
		Vec2D vel;
		InputHelper* inputs;

    public:
		Player(C2D_SpriteSheet& spriteSheet, InputHelper* inputs);
		~Player();

		void Update(float deltaTime, GameState* holder) override;
		void Render(bool isTopScreen) override;
		void Reset();
		void Damage();
		bool CanBeDamaged();
		u8 GetLife();
		Poly* GetPoly();

	friend class MainGame;
	friend class PauseMenu;
};
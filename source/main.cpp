#include <citro2d.h>
#include <3ds.h>

#include <stdio.h>

#include "helper/RenderHelper.hpp"
#include "helper/SoundHelper.hpp"
#include "helper/InputHelper.hpp"

#include "object/Scores.hpp"

#include "gamestate/MainMenu.hpp"
#include "gamestate/ScoreMenu.hpp"
#include "gamestate/OptionMenu.hpp"
#include "gamestate/PauseMenu.hpp"
#include "gamestate/WinMenu.hpp"
#include "gamestate/MainGame.hpp"
#include "gamestate/AnimGame.hpp"
#include "gamestate/GameOverMenu.hpp"

int main()
{
	C2D_TextBuf textBuf = C2D_TextBufNew(16);
	C2D_Text debugText;
	C2D_SpriteSheet spriteSheet;
	InputHelper inputs;
	GameState* states[static_cast<u8>(GameType::GAME_EXIT)];
	GameType actualType = GameType::MENU_MAIN;
	GameType lastType = GameType::MENU_MAIN;

	romfsInit();
	ndspInit();
	gfxInitDefault();
	fsInit();
	//osSetSpeedupEnable(true);

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet)
		svcBreak(USERBREAK_PANIC);

	SoundHelper sounds = SoundHelper();
	RenderHelper renders = RenderHelper();
	Scores scores = Scores();
	scores.LoadScores();
	GameState::SetInputs(&inputs);

	if (!sounds.audioInit())
	{
		printf("Failed to initialise audio\n");
		gfxExit();
		ndspExit();
		romfsExit();
		return EXIT_FAILURE;
	}
	srand(time(NULL));
	MainMenu mainMenu(spriteSheet);
	ScoreMenu scoreMenu(scores);
	OptionMenu optionMenu(scores);
	MainGame mainGame(spriteSheet, scores);
	AnimGame animGame(spriteSheet, mainGame);
	PauseMenu pauseMenu(mainGame);
	WinMenu winMenu(animGame);
	GameOverMenu gameoverMenu(scores, animGame);
	states[static_cast<u8>(GameType::MENU_MAIN)] = &mainMenu;
	states[static_cast<u8>(GameType::MENU_SCORES)] = &scoreMenu;
	states[static_cast<u8>(GameType::MENU_OPTIONS)] = &optionMenu;
	states[static_cast<u8>(GameType::MENU_PAUSE)] = &pauseMenu;
	states[static_cast<u8>(GameType::MENU_GAMEOVER)] = &gameoverMenu;
	states[static_cast<u8>(GameType::MENU_WIN)] = &winMenu;
	states[static_cast<u8>(GameType::GAME_MAIN)] = &mainGame;
	states[static_cast<u8>(GameType::GAME_ANIM)] = &animGame;

	sounds.spawnAudioThread();

	u64 lastTime = svcGetSystemTick();
	u64 actualTime;
	float deltaTime = 0.0166666666666f;
	u32 averageFPS = 60;
	char tmpChr[16];
	u32 frames = 0;
	u64 unix = time(NULL);
	// Main loop
	while (aptMainLoop())
	{
		frames++;
		actualTime = svcGetSystemTick();
		deltaTime = (actualTime - lastTime) / CPU_TICKS_PER_MSEC / 1000.0f;
		lastTime = actualTime;
		u64 unixN = time(NULL);
		if (unix != unixN)
		{
			unix = unixN;
			averageFPS = frames;
			frames = 0;
		}
		inputs.Update();
		GameType newType = states[static_cast<u8>(actualType)]->UpdateGameState(lastType, deltaTime);
		renders.BeginTop();
		states[static_cast<u8>(actualType)]->Render(true);
		if (inputs.IsDebug())
		{
			C2D_TextBufClear(textBuf);
			snprintf(tmpChr, 16, "FPS: %lu", averageFPS);
			C2D_TextFontParse(&debugText, NULL, textBuf, tmpChr);
			C2D_TextOptimize(&debugText);
			C2D_DrawText(&debugText, C2D_WithColor, 50, 10, 0.0f, 1.0f, 1.0f, 0xff22ff22);
		}
		states[static_cast<u8>(actualType)]->TopScreenExtras();
		renders.BeginBottom();
		states[static_cast<u8>(actualType)]->Render(false);
		renders.EndRender();
		lastType = actualType;
		actualType = newType;
		if (actualType == GameType::GAME_EXIT) break;
	}
	C2D_TextBufDelete(textBuf);
	C2D_SpriteSheetFree(spriteSheet);
	sounds.exitAudioThread();
	scores.SaveScores();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
#include "gamestate/GameOverMenu.hpp"

#include "helper/RenderHelper.hpp"
#include "helper/SoundHelper.hpp"

GameOverMenu::GameOverMenu(Scores& scoreIn, AnimGame& gameIn) : scores(scoreIn), game(gameIn)
{
	bufferS = C2D_TextBufNew(64);
	bufferA = C2D_TextBufNew(16);
	C2D_TextFontParse(&textT, NULL, bufferS, "GAME OVER");
    C2D_TextOptimize(&textT);
	C2D_TextFontParse(&textY, NULL, bufferS, "Your Score:");
    C2D_TextOptimize(&textY);
	C2D_TextFontParse(&textN, NULL, bufferS, "New High Score!");
    C2D_TextOptimize(&textN);
	C2D_TextFontParse(&textA, NULL, bufferS, "Press A to continue");
    C2D_TextOptimize(&textA);
}

GameOverMenu::~GameOverMenu()
{
	C2D_TextBufDelete(bufferS);
	C2D_TextBufDelete(bufferA);
}

GameType GameOverMenu::UpdateGameState(GameType lastSate, float deltaTime)
{
    if (lastSate != GameType::MENU_GAMEOVER)
    {
		if (lastSate == GameType::MENU_WIN)
		{
			isWin = true;
		}
		else
		{
			isWin = false;
			SoundHelper::PlayGameOverSound();
		}
		if (scores.actualScore > scores.GetHighestScore()) isNew = true;
		else isNew = false;
        C2D_TextBufClear(bufferA);
		char tmp[16];
    	snprintf(tmp, 16, "%lu", scores.actualScore);
    	C2D_TextFontParse(&textS, NULL, bufferA, tmp);
    	C2D_TextOptimize(&textS);
    }
	if (inputs->GetButton(KEY_A, false))
	{
		swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 2, -1);
		swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);
		swkbdSetFeatures(&swkbd, SWKBD_ALLOW_HOME | SWKBD_ALLOW_RESET | SWKBD_ALLOW_POWER);
        bool shouldQuit = false;
        scores.actualName[0] = '\0';
	    do
	    {
		    swkbdSetInitialText(&swkbd, scores.actualName);
			swkbdSetHintText(&swkbd, "Enter a name");
		    button = swkbdInputText(&swkbd, scores.actualName, 24);
		    if (button != SWKBD_BUTTON_NONE)
			    break;
		    SwkbdResult res = swkbdGetResult(&swkbd);
		    if (res == SWKBD_RESETPRESSED)
		    {
			    shouldQuit = true;
			    aptSetChainloaderToSelf();
			    return GameType::GAME_EXIT;
		    }
		    else if (res != SWKBD_HOMEPRESSED && res != SWKBD_POWERPRESSED)
			    return GameType::GAME_EXIT;
	    } while (!shouldQuit);
        scores.AddHighScore();
		scores.actualScore = 0;
		return GameType::MENU_MAIN;
	}
    return GameType::MENU_GAMEOVER;
}

void GameOverMenu::Render(bool isTopScreen)
{
	static const u32 colorL = C2D_Color32f(1.0f, 0.1f, 0.1f, 1.0f);
	static const u32 colorW = C2D_Color32f(0.1f, 1.0f, 0.1f, 1.0f);
	static const u32 colorY = C2D_Color32f(1.0f, 1.0f, 0.1f, 1.0f);
	static const u32 colorP = C2D_Color32f(1.0f, 0.1f, 1.0f, 1.0f);
	if (isTopScreen)
	{
		C2D_DrawText(&textT, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2 + 40, 80, 0.0f, 1.0f, 1.0f, isWin ? colorW : colorL);
		C2D_DrawText(&textY, C2D_AlignCenter | C2D_WithColor, 150, 120, 0.0f, 0.5f, 0.5f, colorY);
		C2D_DrawText(&textS, C2D_AlignCenter | C2D_WithColor, 250, 120, 0.0f, 0.5f, 0.5f, colorP);
		if (isNew) C2D_DrawText(&textN, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2 + 40, 170, 0.0f, 1.0f, 1.0f, colorW);
	}
	else
	{
		C2D_DrawText(&textA, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 80, 0.0f, 1.0f, 1.0f, colorY);
	}
}
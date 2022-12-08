#pragma once

#include <3ds.h>
#include <citro2d.h>

#define SCORE_MAX_SIZE 16
#define SCORE_MAX_NAME_SIZE 24

struct ScoreData
{
    u32 scoresSolo[SCORE_MAX_SIZE];
    char namesSolo[SCORE_MAX_SIZE][SCORE_MAX_NAME_SIZE];
};


class Scores
{
public:
    Scores();
    bool LoadScores();
    bool SaveScores();
    bool IsHighScore();
    u32 GetHighestScore();
    bool AddHighScore();
    void ResetScores();

    C2D_Text names[SCORE_MAX_SIZE];
    C2D_Text values[SCORE_MAX_SIZE];
    u32 actualScore = 0;
    char actualName[SCORE_MAX_NAME_SIZE] = "";
private:
    FS_Archive sdmcArchive;
    ScoreData data;
    const char* filepath = "/scores.bin";
    void MoveScoresDown(u8 index);
    void RecalculateScores();

    C2D_TextBuf nameBuffer;
    C2D_TextBuf scoreBuffer;
};
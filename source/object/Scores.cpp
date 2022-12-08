#include "object/Scores.hpp"

#define SAVEFILE_PATH "/3ds/saves/scores.bin"
#define SAVEFILE_FOLDER "/3ds/saves/"

Scores::Scores()
{
    FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY,""));
	nameBuffer = C2D_TextBufNew(SCORE_MAX_SIZE*SCORE_MAX_NAME_SIZE);
	scoreBuffer = C2D_TextBufNew(SCORE_MAX_SIZE*10);
}

inline void SetDefaultName(char ptr[SCORE_MAX_NAME_SIZE])
{
	ptr[0] = 'E';
	ptr[1] = 'm';
	ptr[2] = 'p';
	ptr[3] = 't';
	ptr[4] = 'y';
	ptr[5] = '\0';
}

inline void CopyName(char dest[SCORE_MAX_NAME_SIZE], const char source[SCORE_MAX_NAME_SIZE])
{
	for (u8 i = 0; i < SCORE_MAX_NAME_SIZE; i++) dest[i] = source[i];
}

bool Scores::LoadScores()
{
	Handle file;
	Result res = FSUSER_OpenFile(&file, sdmcArchive, fsMakePath(PATH_ASCII, SAVEFILE_PATH), FS_OPEN_READ, 0);
	if (res)
	{
		ResetScores();
		return false;
	}
	u64 size = 0;
	res = FSFILE_GetSize(file, &size);
	if (res || size != sizeof(ScoreData)) return false; // Corrupted or invalid file
	u32 bytesread;
	FSFILE_Read(file,&bytesread,0,(u32*)&data, sizeof(ScoreData));
	FSFILE_Close(file);
	RecalculateScores();
	return true;
}

bool Scores::SaveScores()
{
    Handle file;
	FS_Path fsPath = fsMakePath(PATH_ASCII, SAVEFILE_PATH);
	Result res = FSUSER_OpenFile(&file, sdmcArchive, fsPath, FS_OPEN_WRITE | FS_OPEN_CREATE, 0);
	if (res)
	{
		res = FSUSER_CreateFile(sdmcArchive, fsPath, FS_ATTRIBUTE_ARCHIVE, sizeof(ScoreData));
		if (res)
		{
			res = FSUSER_CreateDirectory(sdmcArchive, fsPath, FS_ATTRIBUTE_DIRECTORY);
			if (res) return false;
			res = FSUSER_CreateFile(sdmcArchive, fsMakePath(PATH_ASCII, SAVEFILE_FOLDER), FS_ATTRIBUTE_ARCHIVE, sizeof(ScoreData));
			if (res) return false;
		}
	}
	u64 size = 0;
	res = FSFILE_GetSize(file, &size);
	if (res) return false;
	if (size != sizeof(ScoreData)) // Corrupted or invalid file
	{
		res = FSFILE_SetSize(file, sizeof(ScoreData));
		if (res) return false;
	}
	u32 byteswritten;
	FSFILE_Write(file, &byteswritten, 0, (u32*)&data, sizeof(ScoreData), FS_WRITE_FLUSH | FS_WRITE_UPDATE_TIME);
	FSFILE_Close(file);
	return true;
}

bool Scores::AddHighScore()
{
	for (u8 i = 0; i < SCORE_MAX_SIZE; i++)
	{
		if (actualScore > data.scoresSolo[i])
		{
			MoveScoresDown(i);
			data.scoresSolo[i] = actualScore;
			CopyName(data.namesSolo[i], actualName);
			RecalculateScores();
			return true;
		}
	}
    return false;
}

bool Scores::IsHighScore()
{
	for (u8 i = 0; i < SCORE_MAX_SIZE; i++)
	{
		if (actualScore > data.scoresSolo[i])
		{
			return true;
		}
	}
    return false;
}

void Scores::MoveScoresDown(u8 index)
{
	for (u8 i = SCORE_MAX_SIZE-1; i > index; i--)
	{
		data.scoresSolo[i] = data.scoresSolo[i-1];
		CopyName(data.namesSolo[i], data.namesSolo[i-1]);
	}
}

void Scores::RecalculateScores()
{
	C2D_TextBufClear(nameBuffer);
	C2D_TextBufClear(scoreBuffer);
	char tmp[32];
	for (u8 i = 0; i < SCORE_MAX_SIZE; i++)
	{
		C2D_TextFontParse(&names[i], NULL, nameBuffer, data.namesSolo[i]);
		C2D_TextOptimize(&names[i]);
		snprintf(tmp, 32, "%lu", data.scoresSolo[i]);
		C2D_TextFontParse(&values[i], NULL, nameBuffer, tmp);
		C2D_TextOptimize(&values[i]);
	}
}

void Scores::ResetScores()
{
	for (u8 i = 0; i < SCORE_MAX_SIZE; i++)
	{
		data.scoresSolo[i] = 0;
		SetDefaultName(data.namesSolo[i]);
	}
	RecalculateScores();
}

u32 Scores::GetHighestScore()
{
	return data.scoresSolo[0];
}
#include "solver.h"
#include <stdio.h>

#ifndef FILE_HANDLER
#define FILE_HANDLER

void AddNumberToText(char* text, int number);

void ClearAllChunks(int max);

void UpdateValue(MazeData* maze, int chunkIndex, int dataIndex, int newValue);

void LoadChunk(MazeData* maze, Tile** chunk, int chunkIndex);

//void EditNumberInChunk(MazeData* maze, int chunkIndex, int lineIndex, int firstCharIndex, int newValue, FILE* optionalFilePointer);

//void ReadDataInChunk(MazeData* maze, Tile* tile, int chunkIndex, int lineIndex, FILE* optionalFilePointer);

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue);

#endif
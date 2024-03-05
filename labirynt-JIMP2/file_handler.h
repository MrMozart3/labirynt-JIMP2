#include "solver.h"
#include <stdio.h>

#ifndef FILE_HANDLER
#define FILE_HANDLER

void PrintMaze(MazeData* maze);

void AddNumberToText(char* text, int number);

void ClearAllChunks(int max);

void UpdateChunk(MazeData* maze, Tile** chunk, int chunkIndex);

void UpdateValue(MazeData* maze, int y, int x, int newValue);

void LoadChunk(MazeData* maze, Tile** chunk, int chunkIndex);

void LoadTile(MazeData* maze, Tile* tile, int y, int x);

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue);

#endif
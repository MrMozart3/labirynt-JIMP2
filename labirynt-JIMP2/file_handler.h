#include "solver.h"
#include <stdio.h>

#ifndef FILE_HANDLER
#define FILE_HANDLER

void OpenFiles(MazeData* maze, FILE** files);

void AddNumberToString(char* output, int number);

void ClearAllChunks(int max);

void EditNumberInChunk(MazeData* maze, int chunkIndex, int lineIndex, int firstCharIndex, int newValue, FILE* optionalFilePointer);

void ReadDataInChunk(MazeData* maze, Tile* tile, int chunkIndex, int lineIndex, FILE* optionalFilePointer);

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue);

#endif
#include "solver.h"

#ifndef FILE_HANDLER
#define FILE_HANDLER

void ClearAllChunks(int max); //removes files from chunk_0 to chunk_max

void GetLineFromFile(char* fileName, int lineIndex, char* output);

void EditLineInFile(char* fileName, int lineIndex, char* text, int addNewLine);

void AppendNumberToFile(char* fileName, int num, int addNewLine); //addNewLine = 1 -> add '\n'

void AppendToFile(char* fileName, char* text, int addNewLine); //addNewLine = 1 -> add '\n'

int SaveMazeToChunks(char* fileName, int chunkSize, MazeData* maze, int* numberOfChunksOut);

#endif
#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int distDiff(MazeData* maze, Tile** chunk, int destY, int destX)
{

}

int GetChunkIndex(MazeData* maze, int y, int x)
{
	return maze->sizeX % maze->chunkSize == 0 ? (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize) + (x / maze->chunkSize) : (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize + 1) + (x / maze->chunkSize);
}

void FillWithDistances(MazeData *maze)
{
	int y = 0, x = 0, nextY, nextX;
	int modY[4] = { -1, 1, 0, 0 };
	int moxX[4] = { 0, 0, -1, 1 };
	int distance = 1;

	Tile** chunk = malloc(sizeof(Tile*) * maze->chunkSize);
	Tile** tempChunk = malloc(sizeof(Tile*) * maze->chunkSize);
	for (int i = 0; i < maze->chunkSize; i++) {
		chunk[i] = malloc(sizeof(Tile) * maze->chunkSize);
		tempChunk[i] = malloc(sizeof(Tile) * maze->chunkSize);
	}

	int chunkIndex = GetChunkIndex(maze, y, x);
	LoadChunk(maze, chunk, chunkIndex);

	while (1)
	{
	}



	//freeing
	for (int i = 0; i < maze->chunkSize; i++) free(chunk[i]);
	free(chunk);
}
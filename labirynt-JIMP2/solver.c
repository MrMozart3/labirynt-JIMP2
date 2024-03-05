#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int distDiff(MazeData* maze, int dist, int destY, int destX)
{
	Tile* tempTile = malloc(sizeof(Tile));
	LoadTile(maze, tempTile, destY, destX);

	int diff = dist - tempTile->dist;
	free(tempTile);
	return diff;
}

int GetChunkIndex(MazeData* maze, int y, int x)
{
	return maze->sizeX % maze->chunkSize == 0 ? (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize) + (x / maze->chunkSize) : (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize + 1) + (x / maze->chunkSize);
}

void FillWithDistances(MazeData *maze)
{
	int y = maze->start[0], x = maze->start[1], nextY, nextX;
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = 1, currentChunk = -1;

	Tile** chunk = malloc(sizeof(Tile*) * maze->chunkSize);
	Tile** tempChunk = malloc(sizeof(Tile*) * maze->chunkSize);
	for (int i = 0; i < maze->chunkSize; i++) {
		chunk[i] = malloc(sizeof(Tile) * maze->chunkSize);
		tempChunk[i] = malloc(sizeof(Tile) * maze->chunkSize);
	}
	int validTempChunk = 0;

	int repeatLoop = 0;

	int chunkIndex = GetChunkIndex(maze, y, x);
	LoadChunk(maze, chunk, chunkIndex);

	while (1)
	{
		repeatLoop = 0;
		//printf("%d %d %d %d\n", y, x, distance, currentChunk);
		//PrintMaze(maze);
		if (GetChunkIndex(maze, y, x) != currentChunk)
		{
			currentChunk = GetChunkIndex(maze, y, x);
			LoadChunk(maze, chunk, currentChunk);
			printf("%d\n", currentChunk);
		}
		if (chunk[y % maze->chunkSize][x % maze->chunkSize].dist > distance) {
			chunk[y % maze->chunkSize][x % maze->chunkSize].dist = distance;
		}
		//checking for higher values
		for (int i = 0; i < 4; i++)
		{
			if (chunk[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[y % maze->chunkSize + modY[i]][x % maze->chunkSize + modX[i]].dist > distance + 1) {
						y += modY[i];
						x += modX[i];
						distance++;
						repeatLoop = 1;
						break;
					}
				}
				else if(distDiff(maze, chunk[y % maze->chunkSize][x % maze->chunkSize].dist, y + modY[i], x + modX[i]) < -1) {
					UpdateChunk(maze, chunk, currentChunk);
					y += modY[i];
					x += modX[i];
					distance++;
					repeatLoop = 1;
					break;
				}
			}
		}
		if (repeatLoop == 1) {
			repeatLoop = 0;
			continue;
		}
		//checking for lower values
		for (int i = 0; i < 4; i++)
		{
			if (chunk[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[y % maze->chunkSize + modY[i]][x % maze->chunkSize + modX[i]].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						repeatLoop = 1;
						break;
					}
				}
				else if (distDiff(maze, chunk[y % maze->chunkSize][x % maze->chunkSize].dist, y + modY[i], x + modX[i]) == 1) {
					UpdateChunk(maze, chunk, currentChunk);
					y += modY[i];
					x += modX[i];
					distance--;
					repeatLoop = 1;
					break;
				}
			}
		}
		if (repeatLoop == 1) {
			repeatLoop = 0;
			continue;
		}
		UpdateChunk(maze, chunk, currentChunk);
		break;
	}



	//freeing
	for (int i = 0; i < maze->chunkSize; i++) free(chunk[i]);
	free(chunk);
}
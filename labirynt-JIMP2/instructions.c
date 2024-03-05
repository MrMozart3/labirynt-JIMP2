#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateInstructions(MazeData *maze)
{
	int y = maze->end[0], x = maze->end[1];
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = -1;

	Tile **chunk = malloc(sizeof(Tile*) * maze->chunkSize);
	for (int i = 0; i < maze->chunkSize; i++) {
		chunk[i] = malloc(sizeof(Tile) * maze->chunkSize);
	}
	int currentChunk = -1;

	int dir = 2, currentDir;
	int currentForward = 0;

	int repeatLoop = 0;
	while (1)
	{
		repeatLoop = 0;
		//printf("%d %d\n", y, x);
		if (GetChunkIndex(maze, y, x) != currentChunk)
		{
			currentChunk = GetChunkIndex(maze, y, x);
			LoadChunk(maze, chunk, currentChunk);
			if (distance == -1) {
				distance = chunk[y % maze->chunkSize][x % maze->chunkSize].dist;
			}
			//printf("%d\n", currentChunk);
		}
		for (int i = 0; i < 4; i++)
		{
			if (chunk[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[y % maze->chunkSize + modY[i]][x % maze->chunkSize + modX[i]].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
				else if (distDiff(maze, chunk[y % maze->chunkSize][x % maze->chunkSize].dist, y + modY[i], x + modX[i]) == -1) {
					UpdateChunk(maze, chunk, currentChunk);
					y += modY[i];
					x += modX[i];
					distance--;
					currentDir = i;
					repeatLoop = 1;
					break;
				}
			}
		}
		if (repeatLoop == 1) {
			if (dir == currentDir) {
				currentForward++;
			}
			else {
				if (currentForward > 0) {
					printf("FORWARD %d\n", currentForward);
				}
				//up
				if (dir == 0 && currentDir == 3) {
					printf("RIGHT\n");
				}
				else if (dir == 0 && currentDir == 2) {
					printf("LEFT\n");
				}
				//down
				else if (dir == 1 && currentDir == 2) {
					printf("RIGHT\n");
				}
				else if (dir == 1 && currentDir == 3) {
					printf("LEFT\n");
				}
				//left
				else if (dir == 2 && currentDir == 0) {
					printf("RIGHT\n");
				}
				else if (dir == 2 && currentDir == 1) {
					printf("LEFT\n");
				}
				//right
				else if (dir == 3 && currentDir == 1) {
					printf("RIGHT\n");
				}
				else if (dir == 3 && currentDir == 0) {
					printf("LEFT\n");
				}
				dir = currentDir;
				currentForward = 1;
			}
			continue;
		}
		if (currentForward > 0) {
			printf("FORWARD %d\n", currentForward);
		}
		break;
	}
}
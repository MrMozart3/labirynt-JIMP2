#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateBinaryInstructons(MazeData* maze, FILE* out) {
	int y = maze->start[0], x = maze->start[1];
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = -1;

	Chunk* chunk = malloc(sizeof(Chunk) * 2);
	for (int k = 0; k < 2; k++)
	{
		chunk[k].tiles = malloc(sizeof(Tile*) * maze->chunkSize);
		for (int i = 0; i < maze->chunkSize; i++) {
			chunk[k].tiles[i] = malloc(sizeof(Tile) * maze->chunkSize);
		}
	}
	int currentChunk = -1;

	int currentDir = -1;

	int repeatLoop = 0;
	while (1) {
		repeatLoop = 0;
		if (GetChunkIndex(maze, y, x) != currentChunk)
		{
			currentChunk = GetChunkIndex(maze, y, x);
			LoadChunk(maze, &chunk[0], currentChunk);
			if (distance == -1) {
				distance = chunk[0].tiles[y % maze->chunkSize][x % maze->chunkSize].dist;
			}
			//printf("%d\n", currentChunk);
		}
		for (int i = 0; i < 4; i++)
		{
			if (chunk->tiles[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[0].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
				else {
					LoadChunk(maze, &chunk[1], GetChunkIndex(maze, y + modY[i], x + modX[i]));
					if (chunk[1].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
			}
		}
		if (repeatLoop == 1){
			if (currentDir == 0) {
				printf("%c", 'N');
			}
			else if (currentDir == 1) {
				printf("%c", 'S');
			}
			else if (currentDir == 2) {
				printf("%c", 'W');
			}
			else if (currentDir == 3) {
				printf("%c", 'E');
			}

			continue;
		}
		break;
	}
}

void GenerateInstructions(MazeData* maze, FILE* out)
{
	int y = maze->start[0], x = maze->start[1];
	int modY[4] = { -1, 1, 0, 0 };
	int modX[4] = { 0, 0, -1, 1 };
	int distance = -1;

	Chunk* chunk = malloc(sizeof(Chunk) * 2);
	for (int k = 0; k < 2; k++)
	{
		chunk[k].tiles = malloc(sizeof(Tile*) * maze->chunkSize);
		for (int i = 0; i < maze->chunkSize; i++) {
			chunk[k].tiles[i] = malloc(sizeof(Tile) * maze->chunkSize);
		}
	}
	int currentChunk = -1;

	int dir = 3, currentDir;
	int currentForward = 0;

	int repeatLoop = 0;
	while (1)
	{
		repeatLoop = 0;
		//printf("%d %d\n", y, x);
		if (GetChunkIndex(maze, y, x) != currentChunk)
		{
			currentChunk = GetChunkIndex(maze, y, x);
			LoadChunk(maze, &chunk[0], currentChunk);
			if (distance == -1) {
				distance = chunk[0].tiles[y % maze->chunkSize][x % maze->chunkSize].dist;
			}
			//printf("%d\n", currentChunk);
		}
		for (int i = 0; i < 4; i++)
		{
			if (chunk->tiles[y % maze->chunkSize][x % maze->chunkSize].walls[i] == ' ')
			{
				if (GetChunkIndex(maze, y + modY[i], x + modX[i]) == currentChunk) {
					if (chunk[0].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
				else {
					LoadChunk(maze, &chunk[1], GetChunkIndex(maze, y + modY[i], x + modX[i]));
					if (chunk[1].tiles[(y + modY[i]) % maze->chunkSize][(x + modX[i]) % maze->chunkSize].dist == distance - 1) {
						y += modY[i];
						x += modX[i];
						distance--;
						currentDir = i;
						repeatLoop = 1;
						break;
					}
				}
			}
		}
		if (repeatLoop == 1) {
			if (dir == currentDir) {
				currentForward++;
			}
			else {
				if (currentForward > 0) {
					fprintf(out, "FORWARD %d\n", currentForward);
				}
				//up
				if (dir == 0 && currentDir == 3) {
					fprintf(out, "RIGHT\n");
				}
				else if (dir == 0 && currentDir == 2) {
					fprintf(out, "LEFT\n");
				}
				//down
				else if (dir == 1 && currentDir == 2) {
					fprintf(out, "RIGHT\n");
				}
				else if (dir == 1 && currentDir == 3) {
					fprintf(out, "LEFT\n");
				}
				//left
				else if (dir == 2 && currentDir == 0) {
					fprintf(out, "RIGHT\n");
				}
				else if (dir == 2 && currentDir == 1) {
					fprintf(out, "LEFT\n");
				}
				//right
				else if (dir == 3 && currentDir == 1) {
					fprintf(out, "RIGHT\n");
				}
				else if (dir == 3 && currentDir == 0) {
					fprintf(out, "LEFT\n");
				}
				dir = currentDir;
				currentForward = 1;
			}
			continue;
		}
		if (currentForward > 0) {
			fprintf(out, "FORWARD %d\n", currentForward);
		}
		break;
	}
	

	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < maze->chunkSize; i++) {
			free(chunk[k].tiles[i]);
		}
		free(chunk[k].tiles);
	}
	free(chunk);
	
}

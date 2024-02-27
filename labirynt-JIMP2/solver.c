#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getLineNumber(MazeData* maze, int y, int x, int chunkIndex)
{
	char fileName[30] = "chunk_";
	char tempNum[20];
	sprintf(tempNum, "%d", chunkIndex);
	strcat(fileName, tempNum);

	char output[50];
	GetLineFromFile(fileName, 0, output);
	int chunkXSIZE = atoi(output);

	int line = (y % maze->chunkSize) * chunkXSIZE + x % maze->chunkSize;
	return line + 1;
}

int GetChunkIndex(MazeData* maze, int y, int x)
{
	int index = maze->sizeX % maze->chunkSize == 0 ? (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize) + x / maze->chunkSize
		: (y / maze->chunkSize) * (maze->sizeX / maze->chunkSize + 1) + x / maze->chunkSize;
	return index;
}

void LoadTileData(Tile* tile, MazeData *maze, int y, int x)
{
	int chunkIndex = GetChunkIndex(maze, y, x);
	int lineIndex = getLineNumber(maze, y, x, chunkIndex);

	char fileName[30] = "chunk_";
	char tempNum[20];
	sprintf(tempNum, "%d", chunkIndex);
	strcat(fileName, tempNum);

	char output[30];
	GetLineFromFile(fileName, lineIndex, output);

	tile->walls[0] = output[0];
	tile->walls[1] = output[1];
	tile->walls[2] = output[2];
	tile->walls[3] = output[3];
	tile->walls[4] = '\0';

	char *temp = &(output[5]);
	tile->dist = atoi(temp);
	return;
}

int CompareDistances(MazeData *maze, int dist, int secondY, int secondX)
{
	Tile tile;
	LoadTileData(&tile, maze, secondY, secondX);
	return dist - tile.dist;
}

void ChangeDistance(MazeData* maze, int y, int x, int new_dist)
{
	Tile tile;
	LoadTileData(&tile, maze, y, x);

	char text[30];
	strcpy(text, tile.walls);
	strcat(text, " ");
	char num[20];
	sprintf(num, "%d", new_dist);
	strcat(text, num);
	
	int chunkIndex = GetChunkIndex(maze, y, x);
	int lineIndex = getLineNumber(maze, y, x, chunkIndex);

	char fileName[30] = "chunk_";
	char tempNum[20];
	sprintf(tempNum, "%d", chunkIndex);
	strcat(fileName, tempNum);

	EditLineInFile(fileName, lineIndex, text, 1);
	return;
}

void FillWithDistancesIteration(MazeData* maze, Tile* currentTile, int currentY, int currentX, int currentDis)
{
	//printf("%d\n", currentDis);
	if (currentDis > currentTile->EndDist) {
		return;
	}
	if (maze->end[0] == currentY && maze->end[1] == currentX && currentDis < currentTile->EndDist){
		currentTile->EndDist = currentDis;
		ChangeDistance(maze, currentY, currentX, currentDis);
		LoadTileData(currentTile, maze, currentY, currentX);
		return;
	}
	ChangeDistance(maze, currentY, currentX, currentDis);
	LoadTileData(currentTile, maze, currentY, currentX);
	//up
	if (currentTile->walls[0] == ' ' && CompareDistances(maze, currentTile->dist, currentY - 1, currentX) < 0) {
		FillWithDistancesIteration(maze, currentTile, currentY - 1, currentX, currentDis + 1);
		LoadTileData(currentTile, maze, currentY, currentX);
	}
	//down
	if (currentTile->walls[1] == ' ' && CompareDistances(maze, currentTile->dist, currentY + 1, currentX) < 0) {
		FillWithDistancesIteration(maze, currentTile, currentY + 1, currentX, currentDis + 1);
		LoadTileData(currentTile, maze, currentY, currentX);
	}
	//left
	if (currentTile->walls[2] == ' ' && CompareDistances(maze, currentTile->dist, currentY, currentX - 1) < 0) {
		FillWithDistancesIteration(maze, currentTile, currentY, currentX - 1, currentDis + 1);
		LoadTileData(currentTile, maze, currentY, currentX);
	}
	//right
	if (currentTile->walls[3] == ' ' && CompareDistances(maze, currentTile->dist, currentY, currentX + 1) < 0) {
		FillWithDistancesIteration(maze, currentTile, currentY, currentX + 1, currentDis + 1);
		LoadTileData(currentTile, maze, currentY, currentX);
	}
	return;
}

void FillWithDistances(MazeData* maze, int startY, int startX)
{
	Tile* currentTile = malloc(sizeof(Tile));
	currentTile->EndDist = 10000000;
	FillWithDistancesIteration(maze, currentTile, startY, startX, 1);
}
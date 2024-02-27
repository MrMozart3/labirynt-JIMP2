#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int DistanceDiff(MazeData* maze, FILE** files, int currentDist, int nextY, int nextX)
{
	Tile temp;
	ReadDataInChunk(maze, &temp, nextY, nextX, files[nextY]);
	return currentDist - temp.dist;
}

void FillWithDistancesIteration(MazeData* maze, Tile* currentTile, FILE** files, int currentY, int currentX, int currentDis)
{
	printf("%d %d %d\n", currentY, currentX, currentDis);
	EditNumberInChunk(maze, currentY, currentX, 5, currentDis, files[currentY]);
	ReadDataInChunk(maze, currentTile, currentY, currentX, files[currentY]);

	//up
	if (currentTile->walls[0] == ' ' && DistanceDiff(maze, files, currentDis, currentY - 1, currentX) < -1) {
		FillWithDistancesIteration(maze, currentTile, files, currentY - 1, currentX, currentDis + 1);
		ReadDataInChunk(maze, currentTile, currentY, currentX, files[currentY]);
	}
	//down
	if (currentTile->walls[1] == ' ' && DistanceDiff(maze, files, currentDis, currentY + 1, currentX) < -1) {
		FillWithDistancesIteration(maze, currentTile, files, currentY + 1, currentX, currentDis + 1);
		ReadDataInChunk(maze, currentTile, currentY, currentX, files[currentY]);
	}
	//left
	if (currentTile->walls[2] == ' ' && DistanceDiff(maze, files, currentDis, currentY, currentX - 1) < -1) {
		FillWithDistancesIteration(maze, currentTile, files, currentY, currentX - 1, currentDis + 1);
		ReadDataInChunk(maze, currentTile, currentY, currentX, files[currentY]);
	}
	//right
	if (currentTile->walls[3] == ' ' && DistanceDiff(maze, files, currentDis, currentY, currentX + 1) < -1) {
		FillWithDistancesIteration(maze, currentTile, files, currentY, currentX + 1, currentDis + 1);
		ReadDataInChunk(maze, currentTile, currentY, currentX, files[currentY]);
	}
	return;
}

void FillWithDistances(MazeData* maze, FILE** files, int startY, int startX)
{
	Tile* currentTile = malloc(sizeof(Tile));
	FillWithDistancesIteration(maze, currentTile, files, startY, startX, 1);
}
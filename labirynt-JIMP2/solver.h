#include <stdio.h>

#ifndef SOLVER
#define SOLVER

typedef struct MazeDataStruct {
	int sizeY;
	int sizeX;
	int start[2]; // {y, x}
	int end[2]; // {y, x}
	int recordSize;
	int recordOffset;
}MazeData;

typedef struct TileStruct {
	char walls[5];
	int dist;
}Tile;

int DistanceDiff(MazeData* maze, FILE** files, int currentDist, int nextY, int nextX);

void FillWithDistancesIteration(MazeData* maze, Tile* currentTile, FILE** files, int currentY, int currentX, int currentDis);

void FillWithDistances(MazeData* maze, FILE** files, int startY, int startX);

#endif
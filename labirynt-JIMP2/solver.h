#include <stdio.h>

#ifndef SOLVER
#define SOLVER

typedef struct MazeDataStruct {
	int sizeY;
	int sizeX;
	int start[2]; // {y, x}
	int end[2]; // {y, x}
	int recordSize;
	int chunkSize;
}MazeData;

typedef struct TileStruct {
	char walls[5];
	int dist;
}Tile;

int GetChunkIndex(MazeData* maze, int y, int x);

void FillWithDistances(MazeData* maze);

#endif
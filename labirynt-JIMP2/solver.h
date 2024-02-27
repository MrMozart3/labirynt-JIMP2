#ifndef SOLVER
#define SOLVER

typedef struct MazeDataStruct {
	int sizeY;
	int sizeX;
	int chunkSize;
	int start[2];
	int end[2];
}MazeData;

typedef struct TileStruct {
	char walls[5];
	int dist;
	int EndDist;
}Tile;

int getLineNumber(MazeData* maze, int y, int x, int chunkIndex); //Get line from file based on chunkIndex, y, x

int GetChunkIndex(MazeData* maze, int y, int x); //Get chunk Index based on y and x

void LoadTileData(Tile* tile, MazeData* maze, int y, int x); //Loads data to tile struct

int CompareDistances(MazeData* maze, int dist, int secondY, int secondX); //dist - neighbour.dist

void ChangeDistance(MazeData* maze, int y, int x, int new_dist);

void FillWithDistancesIteration(MazeData* maze, Tile* currentTile, int currentY, int currentX, int currentDis);

void FillWithDistances(MazeData* maze, int startY, int startX);

#endif

#define _CRT_SECURE_NO_WARNINGS
#include "file_handler.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	MazeData* maze = malloc(sizeof(MazeData));
	maze->recordSize = 12;
	maze->chunkSize = 4;
	maze->chunksY = 0; maze->chunksX = 0;
	char mazeFileName[20] = "maze_10.txt";
    //if(argc == 1) strcpy(mazeFileName, "maze_10.txt");
    //else strcpy(mazeFileName,  argv[1]);

	//CLEARING LEFTOVERS
	ClearAllChunks(100000);
	//TXT TO CHUNKS
	clock_t start1 = clock();
	SaveMazeToChunks(mazeFileName, maze, 100000);
	printf("Zaladowano plik %s do %dx%d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n", 
		mazeFileName, maze->chunksY, maze->chunksX, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);
	clock_t end1 = clock();
	printf("Time Taken To Load Maze:%f\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
	//TEST
	Tile** test = malloc(sizeof(Tile*) * maze->chunkSize);
	for (int i = 0; i < maze->chunkSize; i++) test[i] = malloc(sizeof(Tile) * maze->chunkSize);

	for (int i = 0; i < maze->chunksY; i++) for (int j = 0; j < maze->chunksX; j++)
	{
		printf("chunk %d - ", i * maze->chunksX + j);
		LoadChunk(maze, test, i * maze->chunksX + j);
	}

	for (int i = 0; i < maze->chunkSize; i++)
	{
		for (int j = 0; j < maze->chunkSize; j++)
		{
			//printf(" %c \n%c %c\n %c \n%d\n-------\n", test[i][j].walls[0], test[i][j].walls[2], test[i][j].walls[3], test[i][j].walls[1], test[i][j].dist);
		}
	}
	//UpdateValue(maze, 0, 0, 10);
	//ClearAllChunks(100000);
	printf("\n\n");
	free(maze);
}

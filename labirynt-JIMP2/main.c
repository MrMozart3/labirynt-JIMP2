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
	maze->chunkSize = 5;
	char mazeFileName[20] = "maze_10.txt";
    //if(argc == 1) strcpy(mazeFileName, "maze_10.txt");
    //else strcpy(mazeFileName,  argv[1]);

	//CLEARING LEFTOVERS
	ClearAllChunks(100000);
	//TXT TO CHUNKS
	clock_t start1 = clock();
	SaveMazeToChunks(mazeFileName, maze, 100000);
	printf("Zaladowano plik %s do chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n", mazeFileName, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);
	clock_t end1 = clock();
	printf("Time Taken To Load Maze:%f\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
	//TEST
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)printf("%d", GetChunkIndex(maze, y, x));
		printf("\n");
	}
	//Tile** test = malloc(sizeof(Tile*) * maze->chunkSize);
	//for (int i = 0; i < maze->chunkSize; i++) test[i] = malloc(sizeof(Tile) * maze->chunkSize);
	//LoadChunk(maze, test, 0);
	//printf("%s %d", test[0][0].walls, test[0][0].dist);
	//UpdateValue(maze, 0, 0, 10);
	//ClearAllChunks(100000);
	printf("\n\n");
	free(maze);
}

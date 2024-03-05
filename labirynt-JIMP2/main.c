#define _CRT_SECURE_NO_WARNINGS
#include "file_handler.h"
#include "solver.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



int main()
{
	//
	//			CONFIG
	//

	MazeData* maze = malloc(sizeof(MazeData));
	maze->recordSize = 14;
	maze->chunkSize = 50;
	maze->chunksY = 0; maze->chunksX = 0;
	maze->chunksCache = 5;
	char mazeFileName[20] = "maze_1000.txt";
	ClearAllChunks(10000);

	//
	//			TXT TO CHUNKS
	//

	clock_t start1 = clock();
	SaveMazeToChunks(mazeFileName, maze, 1000000);
	printf("Zaladowano plik %s do %dx%d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n",
		mazeFileName, maze->chunksY, maze->chunksX, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);
	clock_t end1 = clock();
	printf("Time Taken To Load Maze:%f\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);

	//
	//			TEST
	//
	
	//PrintMaze(maze);
	clock_t start2 = clock();
	FillWithDistances(maze);
	clock_t end2 = clock();
	printf("Time Taken To Fill Maze:%f\n", ((double)(end2 - start2)) / CLOCKS_PER_SEC);
	//PrintMaze(maze);
	//GenerateInstructions(maze);
	printf("\n\n");
	free(maze);

	//ClearAllChunks(1000000);
}

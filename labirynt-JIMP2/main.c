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
	maze->recordSize = 15;
	maze->chunkSize = 50;
	maze->chunksY = 0; maze->chunksX = 0;
	maze->chunksCache = 20;
	maze->terminatorSize = 0;
	char mazeFileName[20] = "maze_1000.txt";
	ClearAllChunks(10000000, 1);

	//
	//			VERIFYING FILE
	//

	clock_t start0 = clock();
	VerifyFile(mazeFileName, maze);
	clock_t end0 = clock();

	printf("Zaladowano plik %s do %dx%d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\nMinimum y:%d x:%d\nTerminator size:%d\n\n",
		mazeFileName, maze->chunksY, maze->chunksX, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1], maze->minInChunkY, maze->minInChunkX, maze->terminatorSize);

	printf("Time Taken Verify Maze:%f\n\n", ((double)(end0 - start0)) / CLOCKS_PER_SEC);


	//
	//			TXT TO CHUNKS
	//

	clock_t start1 = clock();
	SaveMazeToChunks(mazeFileName, maze, 1000000);
	clock_t end1 = clock();
	printf("Time Taken To Load Maze:%f\n\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
	//Test(maze->chunksY * maze->chunksX);
	
	//
	//			FILLING WITH DISTANCE
	//
	
	clock_t start2 = clock();
	FillWithDistances(maze);
	clock_t end2 = clock();
	printf("Time Taken To Fill Maze:%f\n\n", ((double)(end2 - start2)) / CLOCKS_PER_SEC);

	//
	//			PRINTING
	//

	//PrintMaze(maze);

	FILE* out = fopen("output.txt", "w");
	clock_t start3 = clock();
	GenerateInstructions(maze, out);
	clock_t end3 = clock();
	printf("Time Taken To Print Maze:%f\n\n", ((double)(end3 - start3)) / CLOCKS_PER_SEC);
	fclose(out);

	//
	//			TESTING
	//

	printf("\n\n");
	free(maze);

	ClearAllChunks(1000000, 1);

	return 0;
}

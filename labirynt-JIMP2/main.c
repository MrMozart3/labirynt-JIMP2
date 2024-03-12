#define _CRT_SECURE_NO_WARNINGS
#include "file_handler.h"
#include "solver.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Test(int numberOfFiles)
{
	char file1[30] = "chunk_";
	char file2[30] = "chunk_test_";
	for (int i = 0; i < numberOfFiles; i++)
	{
		char file1_temp[30];
		strcpy(file1_temp, file1);
		AddNumberToText(file1_temp, i);

		char file2_temp[30];
		strcpy(file2_temp, file2);
		AddNumberToText(file2_temp, i);

		FILE* f1 = fopen(file1_temp, "r");
		FILE* f2 = fopen(file2_temp, "r");

		char c1, c2;
		int j = 0;
		while ((c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF)
		{
			if (c1 != c2) {
				printf("plik chunk_%d  znak %d", i, j);
				return;
			}
			j++;
		}
		fclose(f1);
		fclose(f2);
	}
}


int main()
{
	//
	//			CONFIG
	//

	MazeData* maze = malloc(sizeof(MazeData));
	maze->recordSize = 16;
	maze->chunkSize = 50;
	maze->chunksY = 0; maze->chunksX = 0;
	maze->chunksCache = 5;
	maze->terminatorSize = 2;
	char mazeFileName[20] = "maze_200.txt";
	ClearAllChunks(10000000, 1);

	//
	//			TXT TO CHUNKS
	//

	clock_t start = clock();
	SaveMazeToChunks(mazeFileName, maze, 1000000);
	printf("Zaladowano plik %s do %dx%d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n",
		mazeFileName, maze->chunksY, maze->chunksX, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);
	clock_t end = clock();
	printf("Time Taken To Load Maze:%f\n\n", ((double)(end - start)) / CLOCKS_PER_SEC);


	//
	//			TXT TO CHUNKS
	//

	clock_t start1 = clock();
	SaveMazeToChunksTest(mazeFileName, maze, 1000000);
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


	//
	//			TESTING
	//

	printf("\n\n");
	free(maze);

	//ClearAllChunks(1000000, 1);
}

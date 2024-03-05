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
	MazeData* maze = malloc(sizeof(MazeData));
	maze->recordSize = 12;
	maze->chunkSize = 100;
	maze->chunksY = 0; maze->chunksX = 0;
	char mazeFileName[20] = "maze_1000.txt";
	//if(argc == 1) strcpy(mazeFileName, "maze_10.txt");
	//else strcpy(mazeFileName,  argv[1]);

	//CLEARING LEFTOVERS
	ClearAllChunks(1000);
	//TXT TO CHUNKS
	clock_t start1 = clock();
	SaveMazeToChunks(mazeFileName, maze, 100000);
	printf("Zaladowano plik %s do %dx%d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n",
		mazeFileName, maze->chunksY, maze->chunksX, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);
	clock_t end1 = clock();
	printf("Time Taken To Load Maze:%f\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
	//TEST
	//UpdateValue(maze, 1, 0, 10);
	//Tile** test = malloc(sizeof(Tile*) * 4);
	//for (int i = 0; i < 4; i++) test[i] = malloc(sizeof(Tile) * 4);
	//LoadChunk(maze, test, 5);
	//for (int i = 0; i < 2; i++)for (int j = 0; j < 4; j++)test[i][j].dist = 3;
	//UpdateChunk(maze, test, 5);
	

	//PrintMaze(maze);
	clock_t start2 = clock();
	FillWithDistances(maze);
	clock_t end2 = clock();
	printf("Time Taken To Fill Maze:%f\n", ((double)(end2 - start2)) / CLOCKS_PER_SEC);
	//PrintMaze(maze);
	//ClearAllChunks(100000);
	//GenerateInstructions(maze);
	printf("\n\n");
	free(maze);
}

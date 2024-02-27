#define _CRT_SECURE_NO_WARNINGS
#include "file_handler.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void PrintToConsole(MazeData* maze)
{
	Tile tile[1];
	printf("XX");
	for (int x = 0; x < maze->sizeX; x++)
	{
		LoadTileData(tile, maze, 0, x);
		printf("%c%cXX", tile->walls[0], tile->walls[0]);
	}
	printf("\n");
	for (int y = 0; y < maze->sizeY; y++)
	{
		LoadTileData(tile, maze, y, 0);
		printf("%c%c", tile->walls[2], tile->walls[2]);
		for (int x = 0; x < maze->sizeX; x++)
		{
			LoadTileData(tile, maze, y, x);
			int dist = tile->dist;
			if (dist == 100000) {
				printf("??");
			}
			else if (dist >= 10) {
				printf("%d", dist);
			}
			else printf("%d ", dist);

			printf("%c%c", tile->walls[3], tile->walls[3]);
		}
		printf("\n");
		printf("XX");
		for (int x = 0; x < maze->sizeX; x++)
		{
			LoadTileData(tile, maze, y, x);
			printf("%c%cXX", tile->walls[1], tile->walls[1]);
		}
		printf("\n");
	}
}

int main()
{
	//CONFIG
	MazeData* maze = malloc(sizeof(MazeData));
	char mazeFileName[] = "maze_200.txt";
	int numberOfChunks = 0;

	maze->chunkSize = 100;
	//CZYSZCZENIE CHUNKOW
	ClearAllChunks(10000);
	//ROZ£ADOWANIE PLIKU TXT DO CHUNKOW
	clock_t start = clock();

	SaveMazeToChunks(mazeFileName, maze->chunkSize, maze, &numberOfChunks);
	printf("Zaladowano plik %s do %d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n", mazeFileName, numberOfChunks, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);

	clock_t end = clock();

	double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	printf("Time taken: %f seconds\n", cpu_time_used);

	//CALCULATE DISTANCES
	clock_t start2 = clock();
	FillWithDistances(maze, maze->start[0], maze->start[1]);

	clock_t end2 = clock();
	double time_used2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
	printf("Time 2: %f\n", time_used2);
	//SOLVE
	
	//TEST
	
	PrintToConsole(maze);
}
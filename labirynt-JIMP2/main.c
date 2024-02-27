#include "file_handler.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	MazeData* maze = malloc(sizeof(MazeData));
	if (maze == NULL) return 1;
	maze->recordSize = 20;
	maze->recordOffset = 2;
	char mazeFileName[] = "maze.txt";
	//CLEARING LEFTOVERS
	ClearAllChunks(10000);
	//TXT TO CHUNKS
	clock_t start1 = clock();
	SaveMazeToChunks(mazeFileName, maze, 100000);
	printf("Zaladowano plik %s do chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\n\n", mazeFileName, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1]);
	clock_t end1 = clock();
	printf("Time Taken To Load Maze:%f\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
	//OPEN FILES
	FILE** files = malloc(sizeof(FILE*) * maze->sizeY);
	OpenFiles(maze, files);
	//FILL WITH DISTANCES
	clock_t start2 = clock();
	FillWithDistances(maze, files, maze->start[0], maze->start[1]);
	clock_t end2 = clock();
	printf("Time Taken To Fill Distances:%f\n", ((double)(end2 - start2)) / CLOCKS_PER_SEC);

	//TEST
	ClearAllChunks(10000);

	printf("\n\n");
}
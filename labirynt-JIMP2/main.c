#define _CRT_SECURE_NO_WARNINGS
#include "file_handler.h"
#include "solver.h"
#include "instructions.h"
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void PrintHelp()
{
	printf("\nProgram znajduje najszybsza sciezke w labiryncie i zapisuje ja do pliku\n\n");
	printf("Argumenty:\n\n");
	printf("-n <nazwa pliku z labiryntem> (wymagany)\n\n");
	
	printf("-o <nazwa pliku wynikowego> (niewymagany)\n");
	printf("W przypadku braku domyslnym plikiem jest output.txt\n\n");
	
	printf("-s <rozmiar chunku> (niewymagany)\n");
	printf("Rozmiar chunku\nIm wiecej, tym wieksze zuzycie pamieci RAM\n");
	printf("Warunki: s >= 2\n\n");
	
	printf("-c <rozmiar cache> (niewymagany)\n");
	printf("Liczba chunkow przechowywana w pamieci programu\nIm wiecej, tym wieksze zuzycie pamieci RAM\n");
	printf("Warunki: c <= liczba chunkow w labiryncie\n\n");
	
	
	printf("-r <rozmiar rekordu> (niewymagany)\n");
	printf("Definiuje rozmiar jednego rekordu w pliku\nIm wiecej, tym wieksze zapelnienie dysku podczas dzialania programu\n");
	printf("Warunki: r >= 15\n\n");
	
	printf("-d (niewymagany)\n");
	printf("Uruchamia program w trybie debug mode\n\n");
}

int main(int argc, char *argv[])
{
	//PrintHelp();
	//
	//			CONFIG
	//
	
	MazeData* maze = malloc(sizeof(MazeData));
	maze->recordSize = 15;
	maze->chunkSize = 3;
	maze->chunksY = 0; maze->chunksX = 0;
	maze->chunksCache = 1;
	maze->terminatorSize = 0;
	maze->debugMode = 0;
	char mazeFileName[100] =  "maze_3.txt";
	char outputFileName[100] = "output.txt";

	
	//
	//			ARGUMENTS
	//

	
	int opt = 0;
	while (( opt = getopt(argc, argv, "n:o:r:c:s:hd")) != -1)
	{
		switch(opt) {
			case 'n':
				strcpy(mazeFileName, optarg);
				break;
			case 'o':
				strcpy(outputFileName, optarg);
				break;
			case 'r':
				maze->recordSize = atoi(optarg);
				if(maze->recordSize < 15){
					printf("Zbyt mala wartosc rozmiaru rekordu ( -c )\n");
					printf("Minimalna wartosc: 15\n");
					printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
					return 1;
				}
				break;
			case 'c':
				maze->chunksCache = atoi(optarg);
				break;
			case 's':
				maze->chunkSize = atoi(optarg);
				if(maze->chunkSize < 2){
					printf("Zbyt maly rozmiar chunku ( -s )\n");
					printf("Minimalna wartosc: 2\n");
					printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
					return 1;
				}
				break;
			case 'h':
				PrintHelp();
				return 0;
				
				break;
			case 'd':
				maze->debugMode = 1;
				break;
			case '?':
				printf("Blad w parametrach wywolania\n");
				printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
				return 1;
				
				break;
			default:
				PrintHelp();
				return 1;
				break;
		}
	}	
	
	if(mazeFileName[0] == '\0'){
		printf("Brak nazwy pliku w argumentach\n");
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;
	}
	
	//
	//			VERIFYING
	//
	clock_t start0 = clock();
	
	int ver = VerifyFile(mazeFileName, maze);
	if(ver == 1){
		printf("Problem z otwarciem pliku %s\n", mazeFileName);
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;
	}
	
	clock_t end0 = clock();

	printf("Zaladowano plik %s do %dx%d chunkow\nRozmiar  y:%d  x:%d\nPoczatek  y:%d  x:%d\nKoniec  y:%d  x:%d\nSmallest Chunk y:%d x:%d\nCache Size:%d\nRecord Size:%d\nTerminator size:%d\nDebugMode:%d\n\n",
		mazeFileName, maze->chunksY, maze->chunksX, maze->sizeY, maze->sizeX, maze->start[0], maze->start[1], maze->end[0], maze->end[1], maze->minInChunkY, maze->minInChunkX, maze->chunksCache, maze->recordSize, maze->terminatorSize, maze->debugMode);

	printf("Time Taken Verify Maze:%f\n\n", ((double)(end0 - start0)) / CLOCKS_PER_SEC);
	
	if(maze->chunksCache > maze->chunksX * maze->chunksY){
		printf("Zbyt duzy cache -> wiekszy od ilosci chunkow ( -c )\n");
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;			
	}

	//
	//			CALCULATING
	//



	//
	//			TXT TO CHUNKS
	//
	ClearAllChunks(10000000, 1);
	
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

	FILE* out = fopen(outputFileName, "w");
	clock_t start3 = clock();
	GenerateInstructions(maze, out);
	clock_t end3 = clock();
	printf("Time Taken To Print Maze:%f\n\n", ((double)(end3 - start3)) / CLOCKS_PER_SEC);
	fclose(out);

	//
	//			TESTING
	//

	printf("\n\n");
	//PrintMaze(maze);
	free(maze);

	ClearAllChunks(1000000, 1);

	return 0;
}

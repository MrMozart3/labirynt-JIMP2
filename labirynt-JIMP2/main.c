#define _CRT_SECURE_NO_WARNINGS
#include "file_handler.h"
#include "solver.h"
#include "instructions.h"
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void PrintMazeData(MazeData *maze, char *inputFileName, char *outputFileName) {
	printf("Tryb: %d\n", maze->t);
	printf("Plik wejsciowy/wyjsciowy: %s/%s\n", inputFileName, outputFileName);
	printf("Rozmiar y:%d  x:%d\n", maze->sizeY, maze->sizeX);
	printf("Poczatek y:%d  x:%d\n", maze->start[0], maze->start[1]);
	printf("Koniec y:%d  x:%d\n", maze->end[0], maze->end[1]);
	printf("Smallest Chunk y:%d  x:%d\n", maze->minInChunkY, maze->minInChunkX);
	printf("Cache: %d\n", maze->chunksCache);
	printf("Record Size: %d\n", maze->recordSize);
	printf("Terminator size: %d\n", maze->terminatorSize);
	printf("Debug Mode: %d\n", maze->debugMode);
	printf("\n");
	
}

void PrintHelp()
{
	printf("\nProgram znajduje najszybsza sciezke w labiryncie i zapisuje ja do pliku\n\n");
	printf("Argumenty:\n\n");
	printf("-t <tryb pracy>\n");
	printf("0 - Wejscie z pliku tekstowego, wyjscie w pliku tekstowym (flaga -o wymagana)\n");
	printf("1 - Wejscie z pliku binarnego, wyjscie w pliku binarnym (flaga -o zabroniona)\n");
	printf("2 - Wejscie z pliku binarnego, wyjscie w pliku tekstowym (flaga -o wymagana)\n\n");

	printf("-n <nazwa pliku z labiryntem> (wymagany)\n\n");
	
	printf("-o <nazwa pliku wynikowego> (czy wymagane zalezy od flagi t)\n");
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
	//
	//			CONFIG
	//
	
	MazeData* maze = malloc(sizeof(MazeData));
	maze->recordSize = 15;
	maze->chunkSize = 50;
	maze->chunksY = 0; maze->chunksX = 0;
	maze->chunksCache = 5;
	maze->terminatorSize = 0;
	maze->debugMode = 1;
	maze->t = 1;
	char mazeFileName[100] =  "maze.bin"; //"\0"
	char outputFileName[100] = "\0";

	
	//
	//			ARGUMENTS
	//

	
	int opt = 0;
	while (( opt = getopt(argc, argv, "t:n:o:r:c:s:hd")) != -1)
	{
		switch(opt) {
			case 't':
				if (atoi(optarg) == 0) {
					//text to text
					maze->t = 0;
				}
				else if (atoi(optarg) == 1) {
					//bin to bin
					maze->t = 1;
				}
				else if (atoi(optarg) == 2) {
					//bin to text
					maze->t = 2;
				}
				else {
					printf("Blad w argumencie -t\n");
					printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
					return 1;
				}
				break;
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
	if (maze->t != 0 && maze->t != 1 && maze->t != 2) {
		printf("Brak flagi -t");
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;
	}
	if (mazeFileName[0] == '\0') {
		printf("Brak nazwy pliku w argumentach\n");
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;
	}
	if (maze->t == 1) {
		if (outputFileName[0] != '\0') {
			printf("W przypadku zapisu do pliku binarnego nie nalezy uzywac flagi -o\n");
			printf("Rozwiazanie automatycznie jest zapisywane w pliku wejsciowym %s\n", mazeFileName);
			return 1;
		}
	}
	else if (outputFileName[0] == '\0') {
		printf("Podaj nazwe pliku wyjsciowego");
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;
	}
	if (maze->chunksCache > maze->chunksX * maze->chunksY) {
		printf("Zbyt duzy cache -> wiekszy od ilosci chunkow ( -c )\n");
		printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
		return 1;
	}
	

	if (maze->t == 0) {
		ClearAllChunks(10000000, 1);
		//verify text file
		clock_t start0 = clock();
		int ver = VerifyFile(mazeFileName, maze);
		if (ver == 1) {
			printf("Problem z otwarciem pliku %s\n", mazeFileName);
			printf("Po instrukcje uzyj:\n%s -h\n", argv[0]);
			return 1;
		}
		clock_t end0 = clock();
		printf("Time Taken Verify Maze:%f\n\n", ((double)(end0 - start0)) / CLOCKS_PER_SEC);
		//save to chunks from text file
		clock_t start1 = clock();
		SaveMazeToChunks(mazeFileName, maze, 1000000);
		clock_t end1 = clock();
		printf("Time Taken To Load Maze:%f\n\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
		//fill with distances
		clock_t start2 = clock();
		FillWithDistances(maze);
		clock_t end2 = clock();
		printf("Time Taken To Fill Maze:%f\n\n", ((double)(end2 - start2)) / CLOCKS_PER_SEC);
		//generate instructions to text file
		FILE* out = fopen(outputFileName, "w");
		clock_t start3 = clock();
		GenerateInstructions(maze, out);
		clock_t end3 = clock();
		printf("Time Taken To Print Maze:%f\n\n", ((double)(end3 - start3)) / CLOCKS_PER_SEC);
		fclose(out);
	}
	else if (maze->t == 1) {
		ClearAllChunks(10000000, 1);
		//Read From BInary FIle
		char tempFileName[100] = "maze_temp_file.txt";
		clock_t start0 = clock();
		BinaryRead(mazeFileName, tempFileName, maze);
		clock_t end0 = clock();
		//Verify File
		clock_t start00 = clock();
		VerifyFile(tempFileName, maze);
		if (maze->debugMode == 1) {
			PrintMazeData(maze, mazeFileName, mazeFileName);
		}
		clock_t end00 = clock();
		if (maze->debugMode == 1) printf("Time Taken To Load BInary: %f\n", ((double)(end0 - start0)) / CLOCKS_PER_SEC);
		if (maze->debugMode == 1) printf("Time Taken To Verify Maze: %f\n", ((double)(end00 - start00)) / CLOCKS_PER_SEC);
		//saving to chunks from TEMP text file
		clock_t start1 = clock();
		SaveMazeToChunks(tempFileName, maze, 1000000);
		clock_t end1 = clock();
		if(maze->debugMode == 1) printf("Time Taken To Load Maze:%f\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
		//fill with distances
		clock_t start2 = clock();
		FillWithDistances(maze);
		clock_t end2 = clock();
		if(maze->debugMode == 1) printf("Time Taken To Fill Maze:%f\n\n", ((double)(end2 - start2)) / CLOCKS_PER_SEC);
		//generate instructions to BINARY FILE
		GenerateBinaryInstructons(maze, NULL);
	}

	//
	//			CALCULATING
	//



	//
	//			TXT TO CHUNKS
	//
	
	
	//
	//			FILLING WITH DISTANCE
	//

	//
	//			PRINTING
	//

	//PrintMaze(maze);

	

	//
	//			TESTING
	//

	printf("\n\n");
	//PrintMaze(maze);
	free(maze);

	ClearAllChunks(1000000, 0);

	return 0;
}

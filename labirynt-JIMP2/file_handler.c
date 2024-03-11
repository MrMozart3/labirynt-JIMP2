#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void PrintMaze(MazeData* maze) {
	Tile* tile = malloc(sizeof(Tile));

	printf("\n");
	for (int y = 0; y < maze->sizeY; y++)
	{
		if (y == 0)
		{
			printf("XX");
			for (int x = 0; x < maze->sizeX; x++)
			{
				LoadTile(maze, tile, y, x);
				printf("%c%cXX", tile->walls[0], tile->walls[0]);
			}
			printf("\n");
		}
		for (int x = 0; x < maze->sizeX; x++)
		{
			LoadTile(maze, tile, y, x);
			if (x == 0) {
				if (tile->dist < 10)
				{
					printf("%c%c%d %c%c", tile->walls[2], tile->walls[2], tile->dist, tile->walls[3], tile->walls[3]);
				}
				else printf("%c%c%d%c%c", tile->walls[2], tile->walls[2], tile->dist, tile->walls[3], tile->walls[3]);
			}
			else
			{
				if (tile->dist < 10)
				{
					printf("%d %c%c", tile->dist, tile->walls[3], tile->walls[3]);
				}
				else printf("%d%c%c", tile->dist, tile->walls[3], tile->walls[3]);
			}
		}
		printf("\n");
		for (int x = 0; x < maze->sizeX; x++)
		{
			LoadTile(maze, tile, y, x);
			if (x == 0) {
				printf("XX%c%cXX", tile->walls[1], tile->walls[1]);
			}
			else printf("%c%cXX", tile->walls[1], tile->walls[1]);
		}
		printf("\n");
	}
	free(tile);
}

void AddNumberToText(char* text, int number)
{
	char tempNum[20];
	sprintf(tempNum, "%d", number);
	strcat(text, tempNum);
	return;
}

void ClearAllChunks(int max, int StopAfterError)
{
	printf("Removing Data, successfully removed:\n");
	int count = 0;
	for (int i = 0; i <= max; i++) {
		char fileName[30] = "chunk_";
		AddNumberToText(fileName, i);

		if (remove(fileName) == 0)
		count++;
		else if (StopAfterError == 1) {
			break;
		}
	}
	printf("Removed %d files\n\n", count);
	return;
}

void UpdateChunk(MazeData *maze, Chunk *chunk)
{
	int horizontalNumber = chunk->chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
	int verticalNumber = chunk->chunkIndex / maze->chunksY == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunk->chunkIndex);
	FILE* out = fopen(fileName, "wb");

	char* data = malloc(sizeof(char) * (horizontalNumber * verticalNumber * maze->recordSize + 1));
	char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	for (int y = 0; y < verticalNumber; y++)
	{
		for (int x = 0; x < horizontalNumber; x++)
		{
			for (int i = 0; i < 4; i++)
			{
				data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + i] = chunk->tiles[y][x].walls[i];
			}
			data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + 4] = ' ';
			sprintf(tempNum, "%d", chunk->tiles[y][x].dist);
			int temp = strlen(tempNum);
			for (int i = 5; i < maze->recordSize; i++)
			{
				if (i < temp + 5)
					data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + i] = tempNum[i - 5];
				else
					data[y * horizontalNumber * maze->recordSize + x * maze->recordSize + i] = ' ';
			}
		}
	}
	data[horizontalNumber * verticalNumber * maze->recordSize] = '\0';
	fwrite(data, sizeof(char), horizontalNumber * verticalNumber * maze->recordSize, out);

	fclose(out);
	free(tempNum);
	free(data);
}

//najlepiej to usunac gowno jebane
void LoadTile(MazeData* maze, Tile* tile, int y, int x)
{
	int chunkIndex = GetChunkIndex(maze, y, x);
	int horizontalNumber = chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
	int verticalNumber = chunkIndex / maze->chunksY == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunkIndex);
	FILE* in = fopen(fileName, "r+b");

	char* data = malloc(sizeof(char) * (maze->recordSize + 1));
	char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));

	fseek(in, (horizontalNumber * (y % maze->chunkSize) + x % maze->chunkSize) * maze->recordSize, SEEK_SET);
	fread(data, sizeof(char), maze->recordSize, in);
	for (int i = 0; i < 4; i++)
		tile->walls[i] = data[i];
	tile->walls[4] = '\0';
	for (int i = 5; i < maze->recordSize; i++)
	{
		tempNum[i - 5] = data[i];
	}
	tempNum[maze->recordSize - 5] = '\0';
	tile->dist = atoi(tempNum);

	fclose(in);
	free(data);
	free(tempNum);
}

void LoadChunk(MazeData* maze, Chunk* chunk, int chunkIndex)
{
	chunk->chunkIndex = chunkIndex;

	char fileName[30] = "chunk_";
	AddNumberToText(fileName, chunkIndex);
	FILE* in = fopen(fileName, "r+b");

	int horizontalNumber = chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
	int verticalNumber = chunkIndex / maze->chunksY == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

	int length = horizontalNumber * verticalNumber * maze->recordSize;
	char* data = malloc(sizeof(char) * (length + 1));
	char* tempNum = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	fread(data, sizeof(char), length, in);

	for (int y = 0; y < maze->chunkSize; y++)
	{
		for (int x = 0; x < maze->chunkSize; x++)
		{
			if (y < verticalNumber && x < horizontalNumber)
			{
				for (int i = 0; i < 4; i++)
				{
					chunk->tiles[y][x].walls[i] = data[y * maze->recordSize * horizontalNumber + x * maze->recordSize + i];
					//printf("%d ", y * maze->recordSize * maze->chunkSize + x * maze->recordSize + i);
				}
				chunk->tiles[y][x].walls[4] = '\0';
				for (int i = 5; i < maze->recordSize; i++)
				{
					tempNum[i - 5] = data[y * maze->recordSize * horizontalNumber + x * maze->recordSize + i];
				}
				tempNum[maze->recordSize - 5] = '\0';
				chunk->tiles[y][x].dist = atoi(tempNum);
			}
			else {
				strcpy(chunk->tiles[y][x].walls, "NNNN");
				chunk->tiles[y][x].dist = -1;
			}
		}
	}
	fclose(in);
	free(tempNum);
	free(data);
}

void SaveMazeToChunks(char* fileName, MazeData* maze, int fillValue)
{
	FILE** out = NULL;
	FILE* in = fopen(fileName, "r");
	int opened = 0;

	int inputX = 0, inputY = 0;

	char buff[2100];
	char lines[3][2100];

	char* additionalFill = malloc(sizeof(char) * (maze->recordSize - 5 + 1));
	sprintf(additionalFill, "%d", fillValue);
	int tempLen = strlen(additionalFill);
	for (int i = tempLen; i < maze->recordSize - 5; i++) {
		additionalFill[i] = ' ';
	}
	additionalFill[maze->recordSize - 5] = '\0';

	char* data = malloc(sizeof(char) * (maze->recordSize + 1));

	int y = 0;
	
	char tempC;
	int countC = 0;
	while((tempC = fgetc(in)) != -1)
	{
		//char to char*
		if(tempC != '\n'){
			buff[countC] = tempC;
			countC++;
			continue;
		}
		else{
			buff[countC] = '\0';
			countC = 0;
		}
		//handling lines
		if (inputY == 0) {
			strcpy(lines[2], buff);
			maze->sizeX = (strlen(buff) - 1) / 2;
			maze->chunksX = maze->sizeX % maze->chunkSize == 0 ? maze->sizeX / maze->chunkSize : maze->sizeX / maze->chunkSize + 1;
			out = malloc(sizeof(FILE*) * maze->chunksX);
		}
		else if (inputY % 2 == 1) {
			strcpy(lines[1], buff);
		}
		else if (inputY % 2 == 0)
		{
			strcpy(lines[0], lines[2]);
			strcpy(lines[2], buff);
			//open file
			if (y % maze->chunkSize == 0)
			{
				for (int i = 0; i < maze->chunksX; i++)
				{
					char outFileName[30] = "chunk_";
					int currentChunk = (y / maze->chunkSize) * maze->chunksX + i;
					AddNumberToText(outFileName, currentChunk);
					out[i] = fopen(outFileName, "ab");
				}
				maze->chunksY++;
				opened = 1;
			}
			
			//saving to file
			for (int x = 0; x < maze->sizeX; x++)
			{
				
				data[0] = lines[0][2 * x + 1];
				data[1] = lines[2][2 * x + 1];
				data[2] = lines[1][2 * x];
				data[3] = lines[1][2 * x + 2];
				data[4] = ' ';
				data[5] = '\0';
				strcat(data, additionalFill);
				data[maze->recordSize] = '\0';
			
				fwrite(data, sizeof(char), maze->recordSize, out[x / maze->chunkSize]);
				
				
				//START OR END
				for (int i = 0; i < 4; i++)
				{
					if (data[i] == 'P') {
						maze->start[0] = y;
						maze->start[1] = x;
					}
					if (data[i] == 'K') {
						maze->end[0] = y;
						maze->end[1] = x;
					}
				}
			}
			if (y % maze->chunkSize == maze->chunkSize - 1)
				{
					for (int i = 0; i < maze->chunksX; i++)
					{
						fclose(out[i]);
					}
					opened = 0;
				}
			y++;
			
		}
		inputY++;
	}
	if(opened == 1)
	{
		for (int i = 0; i < maze->chunksX; i++)
		{
			fclose(out[i]);
		}
	}
	maze->sizeY = y;
	maze->minInChunkY = maze->sizeX % maze->chunkSize == 0 ? maze->chunkSize : maze->sizeX - (maze->sizeY / maze->chunkSize) * maze->chunkSize;
	maze->minInChunkX = maze->sizeY % maze->chunkSize == 0 ? maze->chunkSize : maze->sizeY - (maze->sizeY / maze->chunkSize) * maze->chunkSize;

	fclose(in);
	free(out);
	free(additionalFill);
	free(data);
	return;
}

void SaveMazeToChunksTest(char* fileName, MazeData* maze, int fillValue)
{
	FILE* in = fopen(fileName, "rb");
	FILE* out = NULL;
	int opened = 0;
	for (int y = 0; y < maze->chunksY; y++)
	{
		for (int x = 0; x < maze->chunksX; x++)
		{
			int chunkIndex = y * maze->chunksX + x;
			int horizontalNumber = chunkIndex % maze->chunksX == maze->chunksX - 1 ? maze->minInChunkX : maze->chunkSize;
			int verticalNumber = chunkIndex / maze->chunksY == maze->chunksY - 1 ? maze->minInChunkY : maze->chunkSize;

			char tempFileName[30] = "chunk_";
			AddNumberToText(tempFileName, chunkIndex);
			out = fopen(tempFileName, "w");

			char* line = malloc(sizeof(char) * (horizontalNumber * 2 + 1) + 1);
			//readline
			for (int i = 0; i < verticalNumber; i++)
			{
				//fseek(out, y *)
				//fread(line, sizeof(char), (horizontalNumber * 2 + 1, out);
			}
			free(line);
			fclose(out);
		}
	}

	fclose(in);
}
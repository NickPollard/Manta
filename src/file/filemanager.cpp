#include "src/common.h"
#include "src/file/filemanager.h"
//--------------------------------------------------------
#include <stdio.h>

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

bool FileManager::ReadWord(byte** buffer, byte* wordBuffer)
{
	unsigned int	wordPosition = 0;
	while (((*buffer)[wordPosition] != '\0') && ((*buffer)[wordPosition] != ' ') 
			&& (wordPosition < MAX_FILE_WORD_LENGTH-1))
	{
		wordBuffer[wordPosition] = (*buffer)[wordPosition];
		wordPosition++;
	}
	wordBuffer[wordPosition] = '\0';
	if ((*buffer)[wordPosition] == '\0')
	{
		return true;
	}
	else
	{
		(*buffer)+=wordPosition+1;
		return false;
	}
}

bool FileManager::ReadLine(byte** buffer, byte* lineBuffer)
{
	unsigned int	linePosition = 0;
	while (((*buffer)[linePosition] != '\n') && ((*buffer)[linePosition] != '\0') 
			&& (linePosition < MAX_FILE_LINE_LENGTH-1))
	{
		lineBuffer[linePosition] = (*buffer)[linePosition];
		linePosition++;
	}
	lineBuffer[linePosition] = '\0';
	if ((*buffer)[linePosition] == '\0')
	{
		return true;
	}
	else
	{
		(*buffer)+=linePosition+1;
		return false;
	}
}

// Load a file into Manta
bool FileManager::LoadFile(byte** buffer, string filename)
{
	FILE*	modelFile;
	unsigned long	filesize;
	size_t	result;

	// Open the model file as a text file for reading
	modelFile = fopen(filename, "r");
	if (modelFile==NULL)
	{
		printf("Error opening file \"%s\".\n", filename);
		return false;
	}

	// Get the filesize
	fseek(modelFile, 0, SEEK_END);
	filesize = ftell(modelFile);
	rewind(modelFile);

	// Allocate memory to read in the whole file
	(*buffer) = (byte*) malloc (sizeof(byte)*filesize);
	if ((*buffer) == NULL)
	{
		printf("Error allocating memory for file \"%s\" of length %ld.\n", filename, filesize);
		fclose(modelFile);
		return false;
	}

	// Read in the file
	result = fread((*buffer), 1, filesize, modelFile);
	if (result != filesize)
	{
		printf("Error reading file \"%s\".\n", filename);
		fclose(modelFile);
		return false;
	}
	// Close the file
	fclose(modelFile);
	return true;
}

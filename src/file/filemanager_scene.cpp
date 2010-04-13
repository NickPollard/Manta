#include "src/common.h"
#include "src/file/filemanager_scene.h"
//--------------------------------------------------------
#include "src/mantaMgr.h"
#include "src/prop.h"
#include "src/scene.h"
#include <string.h>

FileManager_Scene::FileManager_Scene()
{
}

FileManager_Scene::~FileManager_Scene()
{
}

Scene* FileManager_Scene::LoadScene(string filename)
{
	byte* buffer;
	byte  lineBuffer[MAX_FILE_LINE_LENGTH];
	bool  reachedEnd = false;
	byte* position;

	if (LoadFile(&buffer, filename))
	{
		currentScene = new Scene();
		modelManager->setScene(currentScene);
		// Loop through each line
		position = buffer;
		while (!reachedEnd)
		{
			reachedEnd = ReadLine(&position, lineBuffer);
			switch (lineBuffer[0])
			{
				case 'p':
					LoadScene_AddProp(lineBuffer);
					break;
				case 'l':
					LoadScene_AddLight(lineBuffer);
					break;
				default:
//					printf("-- Skipping line --\n");
					break;
			}
		}
		free(buffer);
		return currentScene;
	}
	return NULL;
}

void FileManager_Scene::LoadScene_AddProp(string line)
{
	vec3	propPosition;
	char	propFile[MAX_FILE_WORD_LENGTH];

	string position = line+2;
	byte wordBuffer[MAX_FILE_WORD_LENGTH];

	ReadWord(&position, wordBuffer);
	strcpy(propFile, wordBuffer);

	ReadWord(&position, wordBuffer);
	propPosition.x = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	propPosition.y = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	propPosition.z = atof(wordBuffer);

	Prop* myProp;
	myProp = propManager->createProp(propFile);
	myProp->setPosition(&propPosition);
}

void FileManager_Scene::LoadScene_AddLight(string line)
{
	vec3	lightPosition;
	color3f	lightColor;

	string position = line+2;
	byte wordBuffer[MAX_FILE_WORD_LENGTH];

	ReadWord(&position, wordBuffer);
	lightPosition.x = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	lightPosition.y = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	lightPosition.z = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	lightColor.x = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	lightColor.y = atof(wordBuffer);

	ReadWord(&position, wordBuffer);
	lightColor.z = atof(wordBuffer);

	currentScene->setLight(&lightPosition, &lightColor);	
}

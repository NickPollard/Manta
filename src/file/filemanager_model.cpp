#include "src/common.h"
#include "src/file/filemanager_model.h"
//--------------------------------------------------------
#include "src/scene.h"

FileManager_Model::FileManager_Model()
{
}

FileManager_Model::~FileManager_Model()
{
}


// Load an .obj file into Manta
model* FileManager_Model::LoadObj(string filename, vec3* objPosition)
{
	byte* buffer;
	byte  lineBuffer[MAX_FILE_LINE_LENGTH];
	bool  reachedEnd = false;
	byte* position;

	if (LoadFile(&buffer, filename))
	{
		int vertexCount = 0;
		int polyCount = 0;
		// Set vertex offset (-1 since obj is indexed from 1)
		vertexOffset = scene->getVertexCount() - 1;
		polyOffset = scene->getPolyCount();
		// Loop through each line
		position = buffer;
		while (!reachedEnd)
		{
			reachedEnd = ReadLine(&position, lineBuffer);
			switch (lineBuffer[0])
			{
				case 'v':
					vertexCount++;
					LoadObj_AddVertex(lineBuffer);
					break;
				case 'f':
					polyCount++;
					LoadObj_AddPolygon(lineBuffer);
					break;
				default:
//					printf("-- Skipping line --\n");
					break;
			}
		}
		free(buffer);
		return scene->addModel(objPosition, vertexOffset+1, vertexCount, polyOffset, polyCount);
	}
	return NULL;
}

void FileManager_Model::LoadObj_AddVertex(string line)
{
	vec3 vertex;
	string position = line+2;
	byte wordBuffer[MAX_FILE_WORD_LENGTH];
	ReadWord(&position, wordBuffer);
	vertex.x = atof(wordBuffer);
	ReadWord(&position, wordBuffer);
	vertex.y = atof(wordBuffer);
	ReadWord(&position, wordBuffer);
	vertex.z = atof(wordBuffer);
	scene->addVertex(&vertex);
}
void FileManager_Model::LoadObj_AddPolygon(string line)
{
	int	val[3];
	string position = line+2;
	byte wordBuffer[MAX_FILE_WORD_LENGTH];
	ReadWord(&position, wordBuffer);
	val[0] = atoi(wordBuffer) + vertexOffset;
	ReadWord(&position, wordBuffer);
	val[1] = atoi(wordBuffer) + vertexOffset;
	ReadWord(&position, wordBuffer);
	val[2] = atoi(wordBuffer) + vertexOffset;
	scene->addPolygon(val);
}

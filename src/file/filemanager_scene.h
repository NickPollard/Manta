#ifndef __FILEMANAGER_SCENE__
#define __FILEMANAGER_SCENE__

#include "src/file/filemanager.h"

class Scene;

class FileManager_Scene : public FileManager
{
	protected:
		Scene*	currentScene;
	public:
		FileManager_Scene();
		virtual ~FileManager_Scene();

		Scene* LoadScene(string filename);
		void LoadScene_AddProp(string line);
		void LoadScene_AddLight(string line);

};

#endif //__FILEMANAGER_SCENE__

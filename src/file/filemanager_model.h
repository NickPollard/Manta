#ifndef __FILEMANAGER_MODEL__
#define __FILEMANAGER_MODEL__

#include "src/file/filemanager.h"

class Scene;

class FileManager_Model : public FileManager
{
	protected:
		Scene*	scene;
		int		vertexOffset;
		int		polyOffset;

	public:
		FileManager_Model();
		virtual ~FileManager_Model();

		void setScene(Scene* activeScene) { scene = activeScene; }
	
		// --- Obj Loading	
		model* LoadObj(string filename, vec3* position);
		void LoadObj_AddVertex(string line);
		void LoadObj_AddPolygon(string line);
};

#endif // __FILEMANAGER_MODEL__

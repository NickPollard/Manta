#ifndef __MANTAMGR_H__
#define __MANTAMGR_H__

#include "src/file/filemanager_model.h"
#include "src/file/filemanager_scene.h"
#include "src/propMgr.h"

class RenderArea;
class Scene;
class View;

// Global Manager Pointers
extern FileManager_Model*	modelManager;
extern FileManager_Scene*	sceneManager;
extern PropMgr*				propManager;

class MantaMgr
{
	public:
		MantaMgr();
		virtual ~MantaMgr();

		// Initial entry point into the App
		void main(int argc, char** argv);

		// Parse command line arguments
		void parseArguments(int argc, char** argv);

		// Initialise data values
		void init();

		// The business
		bool tick();

	protected:
		float		thisFrame;
		float		lastFrame;

		RenderArea* mantaArea;
		Scene*		mantaScene;
		View*		mantaView;

		int			width;
		int			height;

};

#endif // __MANTAMGR_H__

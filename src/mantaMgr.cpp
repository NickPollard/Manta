#include "src/common.h"
#include "src/mantaMgr.h"
//--------------------------------------------------------
#include "src/matrix.h"
#include "src/prop.h"
#include "src/propMgr.h"
#include "src/renderArea.h"
#include "src/scene.h"
#include "src/unittest.h"
#include "src/vector.h"
#include "src/view.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>

// Global Manager Pointers
FileManager_Model*	modelManager;
FileManager_Scene*	sceneManager;
PropMgr*			propManager;

//--------------------------------------------------------
MantaMgr::MantaMgr()
{
	init();
}

//--------------------------------------------------------
MantaMgr::~MantaMgr()
{
}

//--------------------------------------------------------
void MantaMgr::init()
{
	Glib::signal_idle().connect( sigc::mem_fun(*this, &MantaMgr::tick) );
	modelManager = new FileManager_Model();
	sceneManager = new FileManager_Scene();
	propManager	 = new PropMgr();
	lastFrame = (float)rdtsc(); 	
	width = 640;
	height = 480;
}

//--------------------------------------------------------
bool MantaMgr::tick()
{
#if PROFILE_ENABLE
	newFrame();
#endif // PROFILE_ENABLE
	thisFrame = (float)rdtsc(); 	
	float dt = (thisFrame - lastFrame)/1000000000.f;
	lastFrame = thisFrame;

	// Tick Managers
	propManager->tick(dt);
	mantaView->tick(dt);

	// Render the scene
	mantaArea->render();
	return true;
}

//--------------------------------------------------------
void MantaMgr::parseArguments(int argc, char** argv)	{
	printf("Parsing %d arguments.\n", argc);
	for (int i = 1; i < argc; i++)	{
		// Width
		if (!strcmp(argv[i], "-w"))	{
			width = atoi(argv[i + 1]);
			printf("Width: %d\n", width);
			i++;
			continue;
		}
		// Height
		if (!strcmp(argv[i], "-h"))	{
			height = atoi(argv[i + 1]);
			printf("Height: %d\n", height);
			i++;
			continue;
		}
	}
}

//--------------------------------------------------------
void MantaMgr::main(int argc, char** argv)
{
	// parse arguments
	parseArguments(argc, argv);

	// Perform start up unit test
	UnitTest tester;
	tester.TestVectors();
	tester.TestSpeed();

	// Initialise the toolkit
	Gtk::Main kit(argc, argv);

	// Build a window
	Gtk::Window win;
	win.set_default_size(width, height);
	win.set_title("Manta");

	// Add some models
	mantaScene = sceneManager->LoadScene((string)"assets/scenes/cityscene.scn");

	// Setup a view and a place to render it
	mantaView = new View(mantaScene);
	mantaArea = new RenderArea(mantaView, width, height);
	win.add(*mantaArea);
	mantaArea->show();

	// Run the main GTK loop
	Gtk::Main::run(win);
	
#if PROFILE_ENABLE
	dumpProfileTimes();
	mantaView->PrintStats();
#endif // PROFILE_ENABLE
}

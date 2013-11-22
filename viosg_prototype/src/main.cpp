#include <osgViewer/Viewer>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

#include "CityGmlReader.h"
#include "PickObjectHandler.h"
#include "UiThread.h"
#include "UserCommands.h"

using namespace std;
using namespace citygml;




int main(int argc, char** argv )
{

	//VERIFICATION ARGC/ARGV
	//TODO Faire la gestion du nom de fichier et arguments et aide
	if (argc!=2){
		cout<<"Mettre le chemin absolu ou relatif du fichier en argument!"<<endl;
		return 0;
	}
	string filePath=argv[1];

	//CHARGEMENT FICHIER
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);
	cityGroup->setDataVariance(osg::Object::DYNAMIC);

	//CREATION DU VIEWER
	osgViewer::Viewer viewer ;
	viewer.setSceneData(cityGroup);
	viewer.addEventHandler(new PickObjectHandler(&viewer));
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.setUpViewInWindow( 50, 50, 640, 480 );

	//RENDER THREAD ET UI THREAD
	UiThread::instance()->startThread();
	UserCommands userCommands(cityGroup);
	string command;
	viewer.realize();
	while(!viewer.done()){
		//Update Scene if command input
		if( UiThread::instance()->copyNewCommandTo(command)) {
		userCommands.executeCommand(command);
		}
		viewer.frame();
	}
	UiThread::instance()->cancel();
	cout<<"Exiting program"<<endl;
	return 0;
}



// -----FONCTIONS POUR RECUPERER LES DONNES DE LA SCENE ICI










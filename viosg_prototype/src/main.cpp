#include <osgViewer/Viewer>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

#include "CityGmlReader.h"
#include "PickObjectHandler.h"
#include "UiThread.h"
#include "UserCommands.h"
#include "ExternalDataLoader.h"

using namespace std;
using namespace citygml;

int main(int argc, char** argv )
{

	//VERIFICATION ARGC/ARGV
	//TODO Faire la gestion du nom de fichier et arguments et aide
	if (argc!=2){
		cout<<"Echec. Mettre le chemin absolu ou relatif d'un fichier gml ou citygml en argument"<<endl;
		return 0;
	}
	string filePath=argv[1];


	osg::ref_ptr<osg::Group> root(new osg::Group);
	root->setDataVariance(osg::Object::DYNAMIC);

	//CHARGEMENT FICHIER
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);
	root->addChild(cityGroup);

	//CHARGEMENT DES DONNEES EXTERNES
	osg::ref_ptr<osg::Group> externalDataGroup(new osg::Group);
	ExternalDataLoader externalDataLoader(externalDataGroup);
	externalDataLoader.loadData(filePath);
	root->addChild(externalDataGroup);


	//CREATION DU VIEWER
	osgViewer::Viewer viewer ;
	viewer.setSceneData(root);
	viewer.addEventHandler(new PickObjectHandler(&viewer));
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.setUpViewInWindow( 50, 50, 640, 480 );

	//RENDER THREAD ET UI THREAD
	UiThread::instance()->startThread();
	UserCommands userCommands(root);
	string command;
	viewer.realize();
	while(!viewer.done()){
		if( UiThread::instance()->copyNewCommandTo(command)) {
		userCommands.executeCommand(command);
		}
		viewer.frame();
	}

	//FIXME Hotfix pour terminer l'appel blocant cin.getLine() dans UiThread: à améliorer
	cout<<"Fenêtre graphique fermée, appuyer sur Entrée pour terminer le programme"<<endl;

	UiThread::instance()->cancel();
	cout<<"Exiting program"<<endl;
	return 0;
}









#include <osgViewer/Viewer>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

#include "CityGmlReader.h"
#include "PickObjectHandler.h"
#include "UiThread.h"
#include "UserCommands.h"
#include "MetadataMap.h"
using namespace std;
using namespace citygml;




int main(int argc, char** argv )
{

	//VERIFICATION ARGC/ARGV
	//TODO Faire la gestion du nom de fichier et arguments et aide
	if (argc!=2){
		cout<<"Failure. Enter the absolute path of a citygml/gml file as argument "<<endl;
		return 0;
	}
	string filePath=argv[1];

	//CHARGEMENT FICHIER
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);
	osg::ref_ptr<osg::Group> root(new osg::Group);
	root->addChild(cityGroup);
	root->setDataVariance(osg::Object::DYNAMIC);

	//CREATION DU VIEWER
	osgViewer::Viewer viewer ;
	viewer.setSceneData(root);
	viewer.addEventHandler(new PickObjectHandler(&viewer));
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.setUpViewInWindow( 50, 50, 640, 480 );
	viewer.getCamera()->setClearColor(osg::Vec4(0.0,0.0,0.0,1.0));

	//ANALYSE PRELIMINAIRE DE LA SCENE ET PREPARATION DES COMMANDES
	MetadataMap* metadataMap=new MetadataMap(root);
	UserCommands userCommands(root,metadataMap);


	//RENDER THREAD ET UI THREAD
	string command;
	UiThread::instance()->startThread();
	viewer.realize();
	while(!viewer.done()){
		if( UiThread::instance()->copyNewCommandTo(command)) {
		userCommands.executeCommand(command);
		}
		viewer.frame();
	}

	//FIXME Hotfix pour empecher une appel blocant thread ui: à améliorer
	cout<<"!!IMPORTANT!!:Presser Entrée dans le terminal pour fermer le programme"<<endl;
	UiThread::instance()->cancel();

	cout<<"Exiting program"<<endl;

	delete metadataMap;
	return 0;
}










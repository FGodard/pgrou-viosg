#include <osgViewer/Viewer>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

#include "CityGmlReader.h"
#include "PickObjectHandler.h"
#include "UiThread.h"
#include "UserCommands.h"


#include <fstream>
using std::ifstream;

using namespace std;
using namespace citygml;


/**
 * vérifier l'existence et l'extension du fichier
 */
bool is_readable( const std::string & file )
{
	string ext;
	string::size_type pos;
	pos=file.find_last_of( '.' );
	 if (pos != string::npos)
	       {
	          ext = file.substr( pos );
	       }
	ifstream fichier( file.c_str() );
	if(ext!=".gml"){
		cout<<"**** ERROR : argument is not a CityGml File"<<endl;
	}
	if(fichier.fail()){
		cout<<"**** ERROR : " << file<<" no such file or directory "<<endl;
	}
    return (!fichier.fail() && (ext==".gml") );
}

int main(int argc, char** argv )
{

	//VERIFICATION ARGC/ARGV
	//TODO Faire la gestion du nom de fichier et arguments et aide
	if (argc!=2){
		cout<<"**** Warning :enter absolute or relative file path as an argument"<<endl;
		return 0;
	}
	string filePath=argv[1];

	if (is_readable(filePath))
	{
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

	//FIXME Hotfix pour empecher une appel blocant thread ui: à améliorer
	cout<<"!!IMPORTANT!!:Press Entrée to close the program"<<endl;
	UiThread::instance()->cancel();

	cout<<"Exiting program"<<endl;

}
//	else
//		cout<<"     enter absolute or relative file path as an argument "<<endl;

	return 0;
}



// -----FONCTIONS POUR RECUPERER LES DONNES DE LA SCENE ICI










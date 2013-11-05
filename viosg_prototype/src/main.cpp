// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include "CityGmlReader.h"

using namespace std;
using namespace citygml;

//Déclarations fonctions de tests
void showCitySceneGraph(osg::ref_ptr<osg::Group> cityGroup);
void showMetadata(osg::ref_ptr<osg::Group> cityGroup);

int main()
{
	/*FICHIER A CHARGER*/
	//string filePath="/home/blam/samples/Frankfurt_Street_Setting_LOD3/Frankfurt_Street_Setting_LOD3.citygml";
	string filePath="/home/blam/samples/1.citygml";


	/*CHARGEMENT DU FICHIER CITYGML */
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);


	/*TESTS SUR LA SCENE*/
	//Afficherl'arbre de la scène sur 3 niveaux dans la console
	// showCitySceneGraph(cityGroup);
	//Afficher des métadonnées contenues dans les géodes
	//showMetaData(cityGroup);


	//-----Partie habituelle osg-----
	/* OBJECTS CREATION */
	osgViewer::Viewer viewer ;
	osg::ref_ptr<osg::Group> root (new osg::Group);
	/* SCENE DATA */
	root->addChild(cityGroup.get());
	viewer.setSceneData( root.get() );
	/* KEYBOARD INPUT */
	//Stats Event Handler s key
	viewer.addEventHandler(new osgViewer::StatsHandler);
	//Windows size handler
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	// add the state manipulator
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	/* START VIEWER */
	return (viewer.run());
}

void showCitySceneGraph(osg::ref_ptr<osg::Group> cityGroup){
	osg::ref_ptr <osg::Group> subGroup1;
	osg::ref_ptr <osg::Group> subGroup2;
	osg::ref_ptr <osg::Group> subGroup3;

	cout<<endl<<"AFFICHAGE DE L'ARBRE DU FICHIER:"<<endl;
	for (unsigned int i = 0; i < cityGroup->getNumChildren(); i++) {
		cout << "\t" << cityGroup->getChild(i)->className() << endl;
		subGroup1 = (osg::Group *) cityGroup->getChild(i);
		for (unsigned int j = 0; j < subGroup1->getNumChildren(); j++) {
			cout << "\t\t" << subGroup1->getChild(j)->className() << endl;
			subGroup2 = (osg::Group *) subGroup1->getChild(j);
			for (unsigned int k = 0; k < subGroup2->getNumChildren(); k++) {
				cout << "\t\t\t" << subGroup2->getChild(k)->className() << endl;
				subGroup3 = (osg::Group *) subGroup2->getChild(k);
				for (unsigned int l = 0; l < subGroup3->getNumChildren(); l++) {
					cout << "\t\t\t\t" << subGroup3->getChild(l)->className() << endl;
				}
			}
		}
	}
}

void showMetadata(osg::ref_ptr<osg::Group> cityGroup){
	//TODO Regarde dans la géode les métadonnées
}


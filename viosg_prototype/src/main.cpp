// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include "CityGmlReader.h"
#include "GeodeFinder.h"

using namespace std;
using namespace citygml;

//Déclarations fonctions de tests, voir en bas pour leur implémentation
void showCitySceneGraph(osg::ref_ptr<osg::Group> cityGroup);
void showMetadata(osg::Object* geode);
void showMetadataAllGeodes(osg::ref_ptr<osg::Group> cityGroup);

int main()
{
	/*-----FICHIER A CHARGER*/
	string filePath="/home/blam/samples/Frankfurt_Street_Setting_LOD3/Frankfurt_Street_Setting_LOD3.citygml";
	//string filePath="/home/blam/samples/1.citygml";


	/*-----CHARGEMENT DU FICHIER CITYGML */
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);


	/*-----TESTS SUR LA SCENE*/
	//Afficher l'arbre de la scène sur 3 niveaux dans la console
	showCitySceneGraph(cityGroup);
	//Récupérer toutes les géodes de la scène et afficher leurs métadonnées
	showMetadataAllGeodes(cityGroup);


	//-----PARTIE HABITUELLE OSG

	osgViewer::Viewer viewer ;
	osg::ref_ptr<osg::Group> root (new osg::Group);

	root->addChild(cityGroup.get());
	viewer.setSceneData( root.get() );

	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );

	return (viewer.run());
}



// -----FONCTIONS POUR RECUPERER LES DONNES DE LA SCENE ICI

//Imprime les métadonnées d'un objet de l'arbre osg
void showMetadata(osg::Object* osgObject){
	//On récupère le userdata de l'object
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData() );

	//Si il est de type Metadata alors c'est qu'on y a stocké des métadonnées
	if(metadata)
	{
		//metadata.attributes contient les métadonnées sous la forme forme Hashmap
		//On cherche une valeur en cherchant la clé dans le hashmap
//TODO à faire
		//ou on affiche toutes les métadonnées avec un iterator
		AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			//On affiche le nom et la valeur
			cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
		}
		cout<<endl;
	}
}


//Afficher les métadonnées pour tous les objets de la scène
void showMetadataAllGeodes(osg::ref_ptr<osg::Group> cityGroup){
	cout<<endl<<"AFFICHAGE DES METADONNEES DES GEODES"<<endl;
	GeodeFinder geodeFinder;
	//On utilise le geodeFinder pour récupérer tous les objets type géode
	cityGroup->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		showMetadata(geodes[i]);
	}


}

void showCitySceneGraph(osg::ref_ptr<osg::Group> cityGroup){
	osg::ref_ptr <osg::Group> subGroup1;
	osg::ref_ptr <osg::Group> subGroup2;
	osg::ref_ptr <osg::Group> subGroup3;

//Parcours manuel de la scène de graphe

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






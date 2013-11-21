// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>


#include "CityGmlReader.h"
#include "GeodeFinder.h"
#include "KeyEventHandler.h"



using namespace std;
using namespace citygml;


void showMetadata(osg::Object* object);
void showAllMetadata(osg::ref_ptr<osg::Group> cityGroup);


int main(int argc, char** argv )
{

	/*-----FICHIER A CHARGER*/
	//TODO Faire la gestion du nom de fichier et arguments et aide
	if (argc!=2){
		cout<<"Mettre le chemin absolu ou relatif du fichier en argument!"<<endl;
		return 0;
	}
	string filePath=argv[1];

	//Chargement du fichier
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);
	osgViewer::Viewer viewer ;
	viewer.setSceneData(cityGroup);
	//Event Handlers
	viewer.addEventHandler(new KeyEventHandler(&viewer)); //Event handler actions clavier
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );


	/*-----TESTS SUR LA SCENE*/
		//Récupérer toutes les géodes de la scène et afficher leurs métadonnées
		//showAllMetadata(cityGroup);
	return viewer.run();
}



// -----FONCTIONS POUR RECUPERER LES DONNES DE LA SCENE ICI

/**
 * Affiche les métadonnées contenu dans un objet osg
 * Il charge le userData, si il est de type Metadata, c'est qu'il contient des métadonnées
 */
void showMetadata(osg::Object* osgObject){
	//On récupère le userdata de l'object
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData() );

	if(metadata)
	{
		//metadata->attributes contient les métadonnées sous une forme dérivée de Hashmap
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

/**
 * Affiche toutes les métadonnées présentes
 * On utilise le geodeFinder pour récupérer tous les objets type géode de l'arbre
 * Car les métadonnées sont stockées uniquement dans les objets Géode pour l'instant
 */
void showAllMetadata(osg::ref_ptr<osg::Group> cityGroup){
	cout<<endl<<"AFFICHAGE DES METADONNEES"<<endl;
	GeodeFinder geodeFinder;

	cityGroup->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		showMetadata(geodes[i]);

	}
}








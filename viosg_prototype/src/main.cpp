// base


#include <QApplication>
#include <osgDB/ReadFile>

//Chargement fichier
#include "CityGmlReader.h"
#include "GeodeFinder.h"


//Interface
#include "MainWindow.h"

using namespace std;
using namespace citygml;

//Déclarations des fonctions de tests, voir en bas pour leur implémentation
void showCitySceneGraph(osg::ref_ptr<osg::Group> cityGroup);
void showMetadata(osg::Object* object);
void showAllMetadata(osg::ref_ptr<osg::Group> cityGroup);
//TODO Essayer de la déplacer quelque part, fonction importante
osg::Camera* createCamera( int x, int y, int w, int h )
{
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowDecoration = false;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );
    camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    return camera.release();
}

int main(int argc, char** argv )
{
	//Initialition de QT App en premier
	QApplication app( argc, argv );

	/*-----FICHIER A CHARGER*/
	//string filePath="/home/blam/samples/Frankfurt_Street_Setting_LOD3/Frankfurt_Street_Setting_LOD3.citygml";
	string filePath="/home/blam/samples/1.citygml";

	/*-----CHARGEMENT DU FICHIER CITYGML */
	CityGmlReader cityGmlReader;
	osg::ref_ptr<osg::Group> cityGroup =cityGmlReader.readCityGmlFile(filePath);


	/*-----TESTS SUR LA SCENE*/
	//Afficher l'arbre de la scène sur 3 niveaux dans la console
	//showCitySceneGraph(cityGroup);
	//Récupérer toutes les géodes de la scène et afficher leurs métadonnées
	//showAllMetadata(cityGroup);


	//Création de la fenêtre principale
	 osg::Camera* camera = createCamera( 50, 50, 640, 480 );
	 MainWindow mainWindow(cityGroup,camera);
	 mainWindow.showMaximized();
	return app.exec();
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

/**
 * Parcoure manuellement l'arbre de scène pour afficher la structure de l'arbre
 */
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






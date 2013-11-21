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
void colorBuildings(osg::ref_ptr<osg::Group> cityGroup);
std::string yearOfConstruction(osg::Object* osgObject);
std::string measuredHeight(osg::Object* osgObject);

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
		showAllMetadata(cityGroup);
		colorBuildings(cityGroup);
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

void colorBuildings(osg::ref_ptr<osg::Group> cityGroup){
GeodeFinder geodeFinder;
	cityGroup->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	static const char* fragSource =
	"uniform vec4 color;"
	"void main()"
	"{"
	" gl_FragColor = color;"
	"}";

	osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
	f->setShaderSource( fragSource );

	osg::ref_ptr<osg::Program> rPrg = new osg::Program;
	rPrg->setName( "myShader" );
	rPrg->addShader( f.get() );

	//on met en couleur bleu clair les batiments de taille 70 et construits en 1989
	//on met en rouge ceux de taille 70 et construits en 1999
for(unsigned int i=0;i<geodes.size();i++){
	std::string test2=measuredHeight(geodes[i]);
	if(test2.compare("70") == 0){
	std::string test=yearOfConstruction(geodes[i]);
		if(test.compare("1989") == 0){
			geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
               geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(0.0,191.0,255.0,1.0)));
}
		if(test.compare("1999") == 0){
					geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
		               geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(1.0,0.0,0.0,1.0)));
		}
	}}

}


std::string measuredHeight(osg::Object* osgObject){
	osg::ref_ptr<Metadata> metadata =
				dynamic_cast<Metadata*> (osgObject->getUserData());

		if(metadata)
		{
			AttributesMap::const_iterator iterator;
			for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
			{
				std::string test=iterator->first;
				std::string test2=iterator->second;
				if (test.compare("measuredHeight")==0)
					return test2;
			}

		}
}

std::string yearOfConstruction(osg::Object* osgObject){
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData());

	if(metadata)
	{
		AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			std::string test=iterator->first;
			std::string test2=iterator->second;
			if (test.compare("yearOfConstruction")==0)
				return test2;
		}

	}

}









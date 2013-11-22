/*
 * UserCommands.cpp
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#include "UserCommands.h"
using namespace std;

UserCommands::UserCommands(osg::ref_ptr<osg::Group> root){
	UserCommands::root=root;
}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}
	if(command.compare("colorBuildings1989")==0) {colorBuildings1989();return;}
	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}


void UserCommands::showHelp(){
cout<<"Commands List:"<<endl;
cout<<"\t"<<"'help'"<<"\t"<<"Show commands list"<<endl;
cout<<"\t"<<"'showAll'"<<"\t"<<"Show all metadata stored on all geodes"<<endl;
}


/**
 * Affiche toutes les métadonnées présentes
 * On utilise le geodeFinder pour récupérer tous les objets type géode de l'arbre
 * Car les métadonnées sont stockées uniquement dans les objets Géode pour l'instant
 */
void UserCommands::showAllMetadata(){
	cout<<endl<<"AFFICHAGE DES METADONNEES"<<endl;
	GeodeFinder geodeFinder;

	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		showMetadata(geodes[i]);
	}
}

/**
 * Affiche les métadonnées contenu dans un objet osg
 * Il charge le userData, si il est de type Metadata, c'est qu'il contient des métadonnées
 */
void UserCommands::showMetadata(osg::Object* osgObject){
	//On récupère le userdata de l'object
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData() );

	if(metadata)
	{
		citygml::AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			//On affiche le nom et la valeur
			cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
		}
		cout<<endl;
	}
}

void UserCommands::colorBuildings1989(){
GeodeFinder geodeFinder;
root->accept(geodeFinder);
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

	//on met en couleur bleu clair les batiments construits en 1989
for(unsigned int i=0;i<geodes.size();i++){
	std::string test=yearOfConstruction(geodes[i]);
		if(test.compare("1989") == 0){
			geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
               geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(0.0,191.0,255.0,1.0)));
		}
	}

}



std::string UserCommands::yearOfConstruction(osg::Object* osgObject){
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData());

	if(metadata)
	{
		citygml::AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			std::string test=iterator->first;
			std::string test2=iterator->second;
			if (test.compare("yearOfConstruction")==0)
				return test2;
		}

	}

}





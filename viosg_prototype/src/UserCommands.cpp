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
	createColors();
	createStateSets();
	//test();


}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}
	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}


void UserCommands::createColors(){

	//Création des state sets
	osg::ref_ptr<osg::Vec4Array> colors(new osg::Vec4Array);
	colors->push_back(osg::Vec4(0.0,0.0,1.0,1.0));//index 0 bleu
	colors->push_back(osg::Vec4(0.0,1.0,0.0,1.0));//index 1 vert
	colors->push_back(osg::Vec4(0.0,191.0,255.0,1.0));//index 2 azur
	colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));//index 3 rouge
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0));//index 4 jaune
	colors->push_back(osg::Vec4(0.0,0.0,0.0,1.0));//index 5 noir
	colors->push_back(osg::Vec4(0.0,238.0,0.0,1.0));//index 6 vert fluo
	colors->push_back(osg::Vec4(255.0,0.0,255.0,1.0));//index 7 magenta
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0));// index 8 rose


	for(unsigned int i=0;i<9;i++){
		osg::ref_ptr<osg::Material>material(new osg::Material);
		material->setDiffuse(osg::Material::FRONT_AND_BACK,colors->at(i));
		materials.push_back(material.get());
	}

}

void UserCommands::createStateSets(){

	//StateSet par défault
	stateSetDefault=new osg::StateSet;
	//StateSet Transparence
	stateSetTransparent=new osg::StateSet;


	osg::ref_ptr<osg::BlendFunc> bf(new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA ));
	stateSetTransparent->setAttributeAndModes(bf);

	//StateSetColors
	cout<<materials.size()<<endl;
	for(unsigned int i=0;i<9;i++){
		osg::ref_ptr<osg::StateSet>stateSet(new osg::StateSet);
			stateSet->setAttribute(materials[0]);
		}


}


void UserCommands::test(){
	/*
	 GeodeFinder geodeFinder;
	 root->accept(geodeFinder);
	 vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	 for(unsigned int i=0;i<geodes.size();i++){
	                geodes[i]->setStateSet(stateSetsColors[0]);
	        }
	        */

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






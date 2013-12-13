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
	setArrayColor();
	}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}
	if(command.compare("colorLegendYear")==0) {displayInColor("yearOfConstruction");return;}
	if(command.compare("colorLegendHeight")==0){displayInColor("measuredHeight");return;}
	if(command.compare("invisibleBut1996")==0) {displayOnly("yearOfConstruction", 1996);return;}
	if(command.compare("transparentBut1996")==0) {transparenceBut("yearOfConstruction", 1989);return;}


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

//Pour une géode donnée, il renvoit la valeur de la clé passée en paramètre
std::string UserCommands::valueOfKey(osg::Object* osgObject, std::string key){
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData());

	if(metadata)
	{
		citygml::AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			std::string test=iterator->first;
			std::string test2=iterator->second;

			if (test.compare(key)==0)
				return test2;
		}

	}
}

void UserCommands::setArrayColor(){
	colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0,0.0,1.0,1.0));//index 0 bleu
	colors->push_back(osg::Vec4(0.0,1.0,0.0,1.0));//index 1 vert
	colors->push_back(osg::Vec4(0.0,191.0,255.0,1.0));//index 2 azur
	colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));//index 3 rouge
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0));//index 4 jaune
	colors->push_back(osg::Vec4(0.0,0.0,0.0,1.0));//index 5 noir
	colors->push_back(osg::Vec4(0.0,238.0,0.0,1.0));//index 6 vert fluo
	colors->push_back(osg::Vec4(255.0,0.0,255.0,1.0));//index 7 magenta
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0));// index 8 rose
this->colors=colors;
}

//Permet l'affichage couleur d'une clé
void UserCommands::displayInColor(std::string key){
	int valMin;
	int valMax;
	int tableau[10];

	GeodeFinder geodeFinder;
	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	valMax = 0;
	valMin = 2500;
	for(unsigned int i=0;i<geodes.size();i++){
		std::string test1 = valueOfKey(geodes[i], key);
		int test = atoi(test1.c_str());
		if (test>valMax) valMax = test;
		if (test<valMin) valMin = test;}
	cout<<valMin<<endl;
	cout<<valMax<<endl;

	int e = (int)((valMax-valMin+1)/10+1);

	for(unsigned int j=0; j<9; j++){
		tableau[j] = valMin + j*e;
		cout<<tableau[j]<<endl;}
	    tableau[9] = valMax + 1;
	    cout<<tableau[9]<<endl;

	for(unsigned int i=0;i<geodes.size();i++){
		std::string test =valueOfKey(geodes[i],key);
		int valeur = atoi(test.c_str());
		osg::ref_ptr<osg::Material> material = new osg::Material;

		for(unsigned int j=0;j<9;j++){
			if((valeur>=tableau[j]) && (valeur<tableau[j+1]))
				material->setDiffuse(osg::Material::FRONT,colors->at(j));
		}

		geodes[i]->getOrCreateStateSet()->setAttribute(material.get());
	}

}

//Affiche uniquement les géodes qui correspondent au couple (clé, valeur) donné en paramètre
//Les autres sont invisibles
void UserCommands::displayOnly(std::string key, int value){
	GeodeFinder geodeFinder;
	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		std::string test1 =valueOfKey(geodes[i],key);
		int test = atoi(test1.c_str());
		osg::ref_ptr<osg::Material> material = new osg::Material;

		if (test!=value){
			material->setTransparency(osg::Material::FRONT_AND_BACK, 1.0);
			geodes[i]->getOrCreateStateSet()->setAttribute(material.get());
			geodes[i]->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}
	}
}

//Met en transparence les géodes ne correspondant pas au coule (clé,valeur) donné en paramètre
void UserCommands::transparenceBut(std::string key, int value){
	GeodeFinder geodeFinder;
		root->accept(geodeFinder);
		vector<osg::Geode*> geodes=geodeFinder.getNodeList();

		for(unsigned int i=0;i<geodes.size();i++){
			std::string test1 =valueOfKey(geodes[i],key);
			int test = atoi(test1.c_str());
			osg::ref_ptr<osg::Material> material = new osg::Material;

			material->setAlpha(osg::Material::FRONT_AND_BACK, 0.1);
			osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );

					if (test!=value){
					geodes[i]->getOrCreateStateSet()->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
					geodes[i]->getOrCreateStateSet()->setAttributeAndModes(bf);
					}
}
}



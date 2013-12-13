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
	if(command.compare("legendYear")==0) {colorLegend("yearOfConstruction");return;}
	if(command.compare("Transparency")==0) {setTransparence();return;}
	if(command.compare("changeColor")==0) {changeColor();return;}



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

//pour assigner les couleurs selon la valeur du type choisi en paramètre
void UserCommands::colorLegend(std::string key){
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

int e = (int)((valMax-valMin+1)/10+1);

for(unsigned int j=0; j<10; j++)
tableau[j] = valMin + j*e;

for(unsigned int i=0;i<geodes.size();i++){
	std::string test =valueOfKey(geodes[i],key);
	int valeur = atoi(test.c_str());
	osg::ref_ptr<osg::Material> material = new osg::Material;

		if((valeur >= tableau[0]) && (valeur < tableau[1]))
			material->setDiffuse(osg::Material::FRONT,osg::Vec4(0.0,0.0,255.0,1.0));//bleu
else if((valeur >= tableau[1]) && (valeur < tableau[2]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(191.0,62.0,255.0,1.0));//violet
else if((valeur >= tableau[2]) && (valeur < tableau[3]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(0.0,191.0,255.0,1.0));//azur
else if((valeur >= tableau[3]) && (valeur < tableau[4]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(1.0,0.0,0.0,1.0));//rouge
else if((valeur >= tableau[4]) && (valeur < tableau[5]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(255.0,105.0,180.0,1.0));//rose fonce
else if((valeur >= tableau[5]) && (valeur < tableau[6]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(255.0,182.0,193.0,1.0));//rose clair
else if((valeur >= tableau[6]) && (valeur < tableau[7]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(127.0,255.0,0.0,1.0));//vert clair
else if((valeur >= tableau[7]) && (valeur < tableau[8]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(0.0,100.0,0.0,1.0));//vert foncé
else if((valeur >= tableau[8]) && (valeur <= tableau[9]))
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(255.0,215.0,0.0,1.0));//jaune

	geodes[i]->getOrCreateStateSet()->setAttribute(material.get());

	}
}

void UserCommands::changeColor(){
	GeodeFinder geodeFinder;
		root->accept(geodeFinder);
		vector<osg::Geode*> geodes=geodeFinder.getNodeList();

		osg::ref_ptr<osg::Material> material = new osg::Material;
		material->setDiffuse(osg::Material::FRONT,osg::Vec4(1.0f,0.0f,0.0f,1.0f));
		geodes[1]->getOrCreateStateSet()->setAttribute(material.get());
}

void UserCommands::setTransparence(){
	GeodeFinder geodeFinder;
			root->accept(geodeFinder);
			vector<osg::Geode*> geodes=geodeFinder.getNodeList();

			osg::ref_ptr<osg::Material> material = new osg::Material;
			material->setTransparency(osg::Material::FRONT, 1.0);
			geodes[1]->getOrCreateStateSet()->setAttribute(material.get());
			geodes[1]->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}


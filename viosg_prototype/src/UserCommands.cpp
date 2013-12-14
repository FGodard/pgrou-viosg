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
	root->accept(geodeFinder);
	createColors();
	createStateSets();


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
		osg::Material* material = new osg::Material;
		material->setDiffuse(osg::Material::FRONT_AND_BACK,colors->at(i));
		materials.push_back(material);
	}

}

void UserCommands::createStateSets(){

	//StateSet Transparence
	osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
	stateSetTransparent=new osg::StateSet;
	osg::Material* material = new osg::Material;
	material->setAlpha(osg::Material::FRONT_AND_BACK, 0.1);
	stateSetTransparent->setAttribute(material);
	stateSetTransparent->setAttributeAndModes(bf, osg::StateAttribute::ON);

	//StateSetColors
	//StateSetColors[0] est la couleur par défault
	stateSetsColors.push_back(new osg::StateSet);
	//SateSetColors[1->9] sont les différentes couleurs
	for(unsigned int i=0;i<9;i++){
		osg::StateSet* stateSet = new osg::StateSet;
		stateSet->setAttribute(materials[i]);
		stateSetsColors.push_back(stateSet);
	}


}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}

	if(command.compare("reset")==0){showMetadataReset();return;}
	if(command.compare("testColor1")==0){showMetadataByColor("yearOfConstruction");return;}
	if(command.compare("testColor2")==0){showMetadataByColor("measuredHeight");return;}
	if(command.compare("testTransparence1")==0){showMetadataByTransparency("measuredHeight","49");return;}
	if(command.compare("testTransparence2")==0){showMetadataByTransparency("description","FZK/IAI TestCases Only");return;}
	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}

void UserCommands::showMetadataByTransparency(const string key,const string value){
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (geodes[i]->getUserData() );
		if(metadata){
			updateTransparencyState(metadata, key,value);
			updateStateSet(geodes[i],metadata);
		}
	}
}

void UserCommands::updateTransparencyState(Metadata* metadata, const string key,const string value){
	citygml::AttributesMap::const_iterator iterator;
	if((iterator=metadata->attributes.find(key))!=metadata->attributes.end()&&
					iterator->second.compare(value)==0)
			{
				metadata->isTransparent=false;
			}else{
				metadata->isTransparent=true;
			}
}

void UserCommands::showMetadataByColor(const string key){
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
		for(unsigned int i=0;i<geodes.size();i++){
			osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (geodes[i]->getUserData() );
			if(metadata){
				updateColorState(metadata, key);
				updateStateSet(geodes[i],metadata);
			}
		}
}
void UserCommands::updateColorState(Metadata* metadata, const string key){
	citygml::AttributesMap::const_iterator iterator;
		if((iterator=metadata->attributes.find(key))!=metadata->attributes.end())
				{
					metadata->colorState=calculateColorState(key, iterator->second);
				}else{
					metadata->colorState=0;
				}
}

int UserCommands::calculateColorState(const string key,const string value){
	//Si le type de données n'est pas numérique, on ne peut l'afficher en couleur

	//Sinon on calcule l'intervalle auquel appartient value et on retourne la valeur de couleur correspondante
	return 1;
}

void UserCommands::showMetadataReset(){
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
			for(unsigned int i=0;i<geodes.size();i++){
				osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (geodes[i]->getUserData() );
				if(metadata){
					metadata->isTransparent=false;
					metadata->colorState=0;
					updateStateSet(geodes[i],metadata);
				}
			}
}

void UserCommands::updateStateSet(osg::Geode* geode,Metadata* metadata){
	if(metadata->isTransparent==true){
		geode->setStateSet(stateSetTransparent);
	}else{
		geode->setStateSet(stateSetsColors[metadata->colorState]);
	}
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






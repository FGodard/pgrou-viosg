/*
 * UserCommands.cpp
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#include "UserCommands.h"
using namespace std;

UserCommands::UserCommands(osg::ref_ptr<osg::Group> root,MetadataMap* metadataMap):metadataMap(metadataMap){
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



	//StateSets
	//StateSetColors[0] est la couleur par défault
	stateSets.push_back(new osg::StateSet);
	//SateSetColors[1->9] sont les différentes couleurs
	for(unsigned int i=0;i<9;i++){
		osg::StateSet* stateSet = new osg::StateSet;
		stateSet->setAttribute(materials[i]);
		stateSets.push_back(stateSet);
	}

	//StateSet Transparence
		osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
		osg::StateSet* stateSetTransparent=new osg::StateSet;
		osg::Material* material = new osg::Material;
		material->setAlpha(osg::Material::FRONT_AND_BACK, 0.1);
		stateSetTransparent->setAttribute(material);
		stateSetTransparent->setAttributeAndModes(bf, osg::StateAttribute::ON);
		stateSets.push_back(stateSetTransparent);
}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	vector<string> parsedCommand=parseCommand(command);
	if(parsedCommand[0].compare("help")==0){printHelp();return;}
	if(parsedCommand[0].compare("printAll")==0){printAllMetadata();return;}
	if(parsedCommand[0].compare("printTypes")==0){printTypes();return;}
	if(parsedCommand[0].compare("printValues")==0){printValues(parsedCommand);return;}
	//if(parsedCommand[0].compare("showColor")==0){testColor(parsedCommand);return;}
	if(parsedCommand[0].compare("showTransparency")==0){testTransparency(parsedCommand);return;}
	if(parsedCommand[0].compare("resetAll")==0){resetDisplay();return;}
	cout<<"Type 'help' for commands list or close the osgViewer to quit the program"<<endl;
}


vector<string> UserCommands::parseCommand(string command){
	string::size_type pos_start = 0, pos_end;
	vector <string> command_options;
	do
	{
		pos_end = command.find (" ", pos_start);
		string option;
		if (pos_end != command.length())
		{
			int i = pos_end - pos_start ;
			option=command.substr(pos_start, i);
			command_options.push_back(option);
			pos_start = pos_end + 1;
		}
		else
		{
			option = command.substr (pos_start);
		}
	}
	while (pos_end != string::npos);
	return command_options;
}

//TODO ICI
void UserCommands::printHelp(){

	cout<<" ________________________________________________________________________________________"<<endl;
	cout<<"|																						|"<<endl;
	cout<<"| USER GUIDE 																			|"<<endl;
	cout<<"|________________________________________________________________________________________|"<<endl;
	cout<<" ________________________________________________________________________________________"<<endl;
	cout<<"|    COMMAND 			            |					 DESCRIPTION 					|"<<endl;
	cout<<"|____________________________________|___________________________________________________|"<<endl;
	cout<<"| help 								| Opens the user guide 								|"<<endl;
	cout<<"| printAll 							| Prints all metadata stored on all geodes			|"<<endl;
	cout<<"| printTypes 						| Prints a list of present metadata types 			|"<<endl;
	cout<<"| printValues [TYPE] 				| Prints a list of values present for given type	|"<<endl;
	cout<<"| showColor [TYPE] 					| Shows metatadata values for given type by color	|"<<endl;
	cout<<"| 									|  (works only if numeric type)						|"<<endl;
	cout<<"| showTransparency [TYPE] [VALUE] 	| Hides all objects who don't have the metadata		|"<<endl;
	cout<<"|									|  given in arguments								|"<<endl;
	cout<<"| resetColor							| Reset the display colors of the objects			|"<<endl;
	cout<<"| resetTransparency					| Reset the transparency of the objects				|"<<endl;
	cout<<"| resetAll							| Reset all display parameters of the objects		|"<<endl;
	cout<<"|____________________________________|___________________________________________________|"<<endl;
}

void UserCommands::printAllMetadata(){
	cout<<endl<<"AFFICHAGE DES METADONNEES"<<endl;
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		printGeodeMetadata(geodes[i]);
	}
}

void UserCommands::printGeodeMetadata(osg::Object* osgObject){
	//On récupère le userdata de l'object
	osg::ref_ptr<GeodeData> geodeData =
			dynamic_cast<GeodeData*> (osgObject->getUserData() );

	if(geodeData)
	{
		citygml::AttributesMap::const_iterator iterator;
		for (iterator=geodeData->attributes.begin(); iterator != geodeData->attributes.end();++iterator)
		{
			//On affiche le nom et la valeur
			cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
		}
		cout<<endl;
	}
}

void UserCommands::printTypes(){
	std::map<std::string,ValuesData>::iterator itr;
	for (itr=metadataMap->metadataMap.begin(); itr != metadataMap->metadataMap.end();++itr){
		cout<<(itr->second.isNumeric? "Numeric Type":"Literal Type")<<":\t";
		cout<<itr->first<<endl;
	}
}

void UserCommands::printValues(vector<string> parsedCommand){
	if(parsedCommand.size()!=2){cout<<"printValues takes exatly one argument (metadata type)"<<endl;return;}
	if(!metadataMap->hasType(parsedCommand[1])){cout<<"specified type is not present for this scene"<<endl;return;}
	std::map<std::string,ValuesData>::iterator itr;
	itr=metadataMap->metadataMap.find(parsedCommand[1]);
	if(itr!=metadataMap->metadataMap.end()){
	cout<<"Values for metadata type: "<<parsedCommand[1]<<endl;
		for(unsigned int i=0;i<itr->second.values.size();i++){
			cout<<"\t"<<itr->second.values[i]<<endl;
		}
	}
}

void UserCommands::testColor(vector<string>parsedCommand){
if(parsedCommand.size()!=2){cout<<"showColor takes exactly 1 argument (type to display)<<";return;}
if(!metadataMap->hasType(parsedCommand[1])){cout<<"specified type is not present for this scene";return;}
//TODO Change color of the objects
}

void UserCommands::testTransparency(vector<string>parsedCommand){
	if(parsedCommand.size()!=3){cout<<"showTransparency takes exactly 2 arguments (type and value to display)"<<endl;;return;}
	if(!metadataMap->hasType(parsedCommand[1])){cout<<"specified type is not present for this scene"<<endl;return;}
	if(!metadataMap->hasValue(parsedCommand[1],parsedCommand[2])){cout<<"specified value is not present for the specified type"<<endl;return;}
	showMetadataByTransparency(parsedCommand[1],parsedCommand[2]);
}
void UserCommands::resetDisplay(){
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
		for(unsigned int i=0;i<geodes.size();i++){
			osg::ref_ptr<GeodeData> geodeData =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
			if(geodeData){
				geodeData->isTransparent=false;
				geodeData->colorState=0;
				updateStateSet(geodes[i],geodeData);
			}
		}
}

void UserCommands::showMetadataByTransparency(const string key,const string value){
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		osg::ref_ptr<GeodeData> metadata =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
		if(metadata){
			updateTransparencyState(metadata, key,value);
			updateStateSet(geodes[i],metadata);
		}
	}
}

void UserCommands::updateTransparencyState(GeodeData* metadata, const string key,const string value){
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
		osg::ref_ptr<GeodeData> metadata =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
		if(metadata){
			updateColorState(metadata, key);
			updateStateSet(geodes[i],metadata);
		}
	}
}

void UserCommands::updateColorState(GeodeData* geodeData, const string key){
	citygml::AttributesMap::const_iterator iterator;
	if((iterator=geodeData->attributes.find(key))!=geodeData->attributes.end())
	{
		geodeData->colorState=calculateColorState(key, iterator->second);
	}else{
		geodeData->colorState=0;
	}
}

int UserCommands::calculateColorState(const string key,const string value){
	//Si le type de données n'est pas numérique, on ne peut l'afficher en couleur

	//Sinon on calcule l'intervalle auquel appartient value et on retourne la valeur de couleur correspondante
	return 1;
}

void UserCommands::showMetadataReset(){

}

void UserCommands::updateStateSet(osg::Geode* geode,GeodeData* geodeData){
	if(geodeData->isTransparent==true){
		geode->setStateSet(stateSets[stateSets.size()-1]);
	}else{
		geode->setStateSet(stateSets[geodeData->colorState]);
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

}

/**
 * Affiche les métadonnées contenu dans un objet osg
 * Il charge le userData, si il est de type Metadata, c'est qu'il contient des métadonnées
 */
void UserCommands::showMetadata(osg::Object* osgObject){

}






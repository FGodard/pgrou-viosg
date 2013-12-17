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
	storeStateSets();
	createColorsIntervalles();
}




void UserCommands::createColors(){
	//Création des state sets
	osg::ref_ptr<osg::Vec4Array> colors(new osg::Vec4Array);
	/*
	colors->push_back(osg::Vec4(0.0,0.0,1.0,1.0));//index 0 bleu
	colors->push_back(osg::Vec4(0.0,1.0,0.0,1.0));//index 1 vert
	colors->push_back(osg::Vec4(0.0,191.0,255.0,1.0));//index 2 azur
	colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));//index 3 rouge
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0));//index 4 jaune
	colors->push_back(osg::Vec4(0.0,0.0,0.0,1.0));//index 5 noir
	colors->push_back(osg::Vec4(0.0,238.0,0.0,1.0));//index 6 vert fluo
	colors->push_back(osg::Vec4(255.0,0.0,255.0,1.0));//index 7 magenta
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0));// index 8 rose
	 */

	colors->push_back(osg::Vec4(1.0,0.0,1.0,1.0)); //violet
	colors->push_back(osg::Vec4(0.0,0.0,0.5,1.0)); //blue foncé
	colors->push_back(osg::Vec4(0.0,0.0,1.0,1.0)); //bleu marine
	colors->push_back(osg::Vec4(0.0,0.5,0.0,1.0)); //vert
	colors->push_back(osg::Vec4(0.0,238.0,0.0,1.0)); //vert fluo
	colors->push_back(osg::Vec4(255.0,215.0,0.0,1.0)); //jaune
	colors->push_back( osg::Vec4(1.0f, 0.55f, 0.0f, 1.0f) );// Orange
	colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));//rouge



	for(unsigned int i=0;i<colors->size();i++){
		osg::Material* material = new osg::Material;
		material->setDiffuse(osg::Material::FRONT_AND_BACK,colors->at(i));
		materials.push_back(material);
	}

}

void UserCommands::createStateSets(){
	//StateSets
	//StateSetColors[0] est la couleur par défault
	colorsStateSets.push_back(new osg::StateSet);
	//SateSetColors[1->9] sont les différentes couleurs
	for(unsigned int i=0;i<materials.size();i++){
		osg::StateSet* stateSet = new osg::StateSet;
		stateSet->setAttribute(materials[i]);
		colorsStateSets.push_back(stateSet);
	}

	//StateSet Transparence
	osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
	transparentStateSet=new osg::StateSet;
	osg::Material* material = new osg::Material;
	material->setAlpha(osg::Material::FRONT_AND_BACK, 0.1);
	transparentStateSet->setAttribute(material);
	transparentStateSet->setAttributeAndModes(bf, osg::StateAttribute::ON);

}
/**
 * Stockage des StateSets dans le group factice stateSetsTree pour empêcher la suppresion des StateSets si déréfenrecement dans root
 */
void UserCommands::storeStateSets(){
	stateSetsTree=new osg::Group;
	for(unsigned int i=0;i<colorsStateSets.size();i++){
		osg::ref_ptr<osg::Geode> geode(new osg::Geode);
		geode->setStateSet(colorsStateSets[i]);
		stateSetsTree->addChild(geode);
	}
	osg::ref_ptr<osg::Geode> geode(new osg::Geode);
	geode->setStateSet(transparentStateSet);
	stateSetsTree->addChild(geode);
}

void UserCommands::createColorsIntervalles(){
	for(unsigned int i=0;i<materials.size();i++){
		Intervalle intervalle;
		intervalle.isDisplayed=false;
		intervalle.minValue=0.0;
		intervalle.maxValue=0.0;
		switch(i){
		case 0: intervalle.color="purple";break;
		case 1: intervalle.color="dark blue";break;
		case 2: intervalle.color="blue ";break;
		case 3: intervalle.color="green";break;
		case 4: intervalle.color="light green";break;
		case 5: intervalle.color="yellow";break;
		case 6: intervalle.color="orange";break;
		case 7: intervalle.color="red";break;
		default: intervalle.color="?"; break;
		}
		colorsIntervalles.push_back(intervalle);
	}
}

void UserCommands::resetColorsIntervalles(){
	for(unsigned int i=0;i<colorsIntervalles.size();i++){
		colorsIntervalles[i].isDisplayed=false;
		colorsIntervalles[i].maxValue=0.0;
		colorsIntervalles[i].minValue=0.0;
	}
}
void UserCommands::calculateColorsIntervalles(vector<string> values){
	if(values.size()==0)return;
	int valuesPerIntervalles=values.size()/colorsIntervalles.size();
	if(valuesPerIntervalles==0){// il y a moins de valeurs que de couleurs proposées
		for (unsigned int i =0; i<values.size(); i++ ){
			colorsIntervalles[i].isDisplayed=true;
			colorsIntervalles[i].minValue=atof(values[i].c_str());
			colorsIntervalles[i].maxValue=atof(values[i].c_str());
		}
	}else{
		int j =-1;
		for(unsigned int i=0;i<colorsIntervalles.size()-1;i++){
			colorsIntervalles[i].isDisplayed=true;
			j=j+1;
			colorsIntervalles[i].minValue=atof(values[j].c_str());
			j=j+(valuesPerIntervalles-1);
			colorsIntervalles[i].maxValue=atof(values[j].c_str());
		}
		colorsIntervalles[colorsIntervalles.size()-1].isDisplayed=true;
		colorsIntervalles[colorsIntervalles.size()-1].minValue=atof(values[j+1].c_str());
		colorsIntervalles[colorsIntervalles.size()-1].maxValue=atof(values[values.size()-1].c_str());
	}
}

void UserCommands::printColorsIntervalles(){
	cout<<"Colors Legend:"<<endl;
	for(unsigned int i=0;i<colorsIntervalles.size();i++){
		if(colorsIntervalles[i].isDisplayed==true){
			cout<<colorsIntervalles[i].color<<":\t\t";
			if(colorsIntervalles[i].maxValue-colorsIntervalles[i].minValue<0.1){
				cout<<(int)colorsIntervalles[i].minValue<<endl;
			}else{
				cout<<(int)colorsIntervalles[i].minValue<<"\t to \t"<<(int)colorsIntervalles[i].maxValue<<endl;
			}
		}
	}
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
	if(parsedCommand[0].compare("showColor")==0){testColor(parsedCommand);return;}
	if(parsedCommand[0].compare("showTransparency")==0){testTransparency(parsedCommand);return;}
	if(parsedCommand[0].compare("resetAll")==0){resetDisplay();return;}
	if(parsedCommand[0].compare("resetColor")==0){resetColor();return;}
	if(parsedCommand[0].compare("resetTransparency")==0){resetTransparency();return;}
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
	if(parsedCommand.size()!=2){cout<<"showColor takes exactly 1 argument (type to display)"<<endl;;return;}
	if(!metadataMap->hasType(parsedCommand[1])){cout<<"specified type is not present for this scene"<<endl;return;}
	if(!metadataMap->isNumericType(parsedCommand[1])){cout<<"specified type doesn't contain numeric values, cannot display in colors"<<endl;return;}
	showMetadataByColor(parsedCommand[1]);
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

void UserCommands::resetColor(){
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
		for(unsigned int i=0;i<geodes.size();i++){
			osg::ref_ptr<GeodeData> geodeData =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
			if(geodeData){
				geodeData->colorState=0;
				updateStateSet(geodes[i],geodeData);
			}
		}
}

void UserCommands::resetTransparency(){
vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		osg::ref_ptr<GeodeData> geodeData =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
		if(geodeData){
			geodeData->isTransparent=false;
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
	std::map<std::string,ValuesData>::iterator itr;
	itr=metadataMap->metadataMap.find(key);

	resetColorsIntervalles();
	calculateColorsIntervalles(itr->second.values);
	printColorsIntervalles();

	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		osg::ref_ptr<GeodeData> geodeData =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
		if(geodeData){
			updateColorState(geodeData, key);
			updateStateSet(geodes[i],geodeData);
		}
	}

}

void UserCommands::updateColorState(GeodeData* geodeData, const string key){
	citygml::AttributesMap::const_iterator iterator;
	if((iterator=geodeData->attributes.find(key))!=geodeData->attributes.end())
	{
		float geodeValue=atof(iterator->second.c_str());
		geodeData->colorState=calculateColorState(geodeValue);
	}else{
		geodeData->colorState=0;
	}
}

int UserCommands::calculateColorState(float geodeValue){
	for(unsigned int i=0;i<colorsIntervalles.size();i++){
		if(geodeValue<=colorsIntervalles[i].maxValue){
			return i+1;
		}
	}
	return 0;
}



void UserCommands::updateStateSet(osg::Geode* geode,GeodeData* geodeData){
	if(geodeData->isTransparent==true){
		geode->setStateSet(transparentStateSet);
	}else{
		geode->setStateSet(colorsStateSets[geodeData->colorState]);
	}

}











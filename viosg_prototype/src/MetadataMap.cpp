/*
 * MetadataMap.cpp
 *
 *  Created on: 14 déc. 2013
 *      Author: blam
 */

#include "MetadataMap.h"
using namespace std;

MetadataMap::MetadataMap(osg::ref_ptr<osg::Group> root){
	this->root=root;
	vector<string> sceneTypes=getSceneTypes();
	for(unsigned int i=0;i<sceneTypes.size();i++){
		ValuesData valuesData=getValuesData(sceneTypes[i]);
		metadataMap[sceneTypes[i]]=valuesData;
	}
	printAllMetadataTypesAndValues();

}

vector<string> MetadataMap::getSceneTypes(){
	vector<string> sceneTypes;
	GeodeFinder geodeFinder;
	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		osg::ref_ptr<GeodeData> metadata =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
		if(metadata){
			vector<string> geodeTypes=getGeodeTypes(metadata);
			sceneTypes=updateWithNewData(sceneTypes,geodeTypes);
		}
	}

	return sceneTypes;
}

vector<string> MetadataMap::getGeodeTypes(GeodeData* metadata){
	vector <std::string> geodeTypes;
	citygml::AttributesMap::const_iterator iterator;
	for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator){
		geodeTypes.push_back(iterator->first);
	}
	return geodeTypes ;
}

std::vector<std::string> MetadataMap::updateWithNewData(std::vector<std::string> origin,std::vector<std::string> newData){
	for(unsigned int i=0; i<newData.size();i++){
		bool isNew=true;
		for(unsigned int j=0;j<origin.size();j++){
			if(newData[i].compare(origin[j])==0)
				isNew=false;
		}
		const string newString=newData[i];
		if(isNew) origin.push_back(newString);
	}
	return origin;
}

ValuesData MetadataMap::getValuesData(string type){
	vector<string> values=getSceneValues(type);
	sort(values.begin(),values.end());
	bool isNumeric=checkIfNumericType(values);
	ValuesData valuesData;
	valuesData.isNumeric=isNumeric;
	valuesData.values=values;
	return valuesData;
}

vector<string> MetadataMap::getSceneValues(string type){
	vector<string> sceneValues;
	GeodeFinder geodeFinder;
	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		osg::ref_ptr<GeodeData> metadata =dynamic_cast<GeodeData*> (geodes[i]->getUserData() );
		if(metadata){
			vector<string> geodeValues=getGeodeValues(metadata, type);
			sceneValues=updateWithNewData(sceneValues,geodeValues);
		}
	}
	return sceneValues;
}

vector<string> MetadataMap::getGeodeValues(GeodeData* metadata, string type){
	vector<string> geodeValues;
	citygml::AttributesMap::const_iterator iterator;
	iterator=metadata->attributes.find(type);
	if(iterator!=metadata->attributes.end()){
		geodeValues.push_back(iterator->second);
	}
	return geodeValues;
}

bool MetadataMap::checkIfNumericType(vector<string> values){
	bool isNumeric=true;
	for(unsigned int i =0;i<values.size();i++){
		std::istringstream iss(values[i]);
		float f;
		iss >> noskipws >> f;
		isNumeric=iss.eof() && !iss.fail();
		if(!isNumeric)break;
	}
	return isNumeric;
}




void MetadataMap::printAllMetadataTypesAndValues(){
	std::map<std::string,ValuesData>::iterator itr;
	cout<<"TEST:AFFICHAGE DE TOUS LES TYPES ET VALEURS DE LA SCENE"<<endl;
	for (itr=metadataMap.begin(); itr != metadataMap.end();++itr){
		cout<<"Nom type:"<<itr->first<<endl;
		cout<<(itr->second.isNumeric? "Type Numérique":"Type Non Numérique")<<".Valeurs trouvées:"<<endl;
		for(unsigned int i =0; i<itr->second.values.size();i++){
			cout<<"\t"<<itr->second.values[i]<<endl;
		}
		cout<<endl;
	}

}





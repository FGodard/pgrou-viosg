/*
 * ExternalDataLoader.cpp
 *
 *  Created on: 8 déc. 2013
 *      Author: blam
 */

#include "ExternalDataLoader.h"

using namespace std;
ExternalDataLoader::ExternalDataLoader(osg::ref_ptr<osg::Group> externalDataGroup){
	this->externalDataGroup=externalDataGroup;
}

void ExternalDataLoader::loadData(std::string filePathCityGML){
	if(loadCityGMLData(filePathCityGML)){
		string filePathExtern="/home/blam/VIOSGdata/CERDSOLH.xml";
		//loadExternalFileData(filePathExtern);
	}
	xmlCleanupParser();
}





bool ExternalDataLoader::loadCityGMLData(string filePath){
	xmlDoc *doc = NULL;
		doc = xmlReadFile(filePath.c_str(), NULL, 0);
		if (doc == NULL) {
			cout<<"Erreur lors du passage du fichier cityGML dans le parser"<<endl;
			return false;
		}
		xmlNode *root = NULL;
		root = xmlDocGetRootElement(doc);
		//Récupérer les attributs de citygml interessants
		printTestCityGML(root);
		xmlFreeDoc(doc);
		return true;
}

bool ExternalDataLoader::loadExternalFileData(string filePath){
	xmlDoc *doc = NULL;
			doc = xmlReadFile(filePath.c_str(), NULL, 0);
			if (doc == NULL) {
				cout<<"Erreur lors du passage du fichier cityGML dans le parser"<<endl;
				return false;
			}
			xmlNode *root = NULL;
			root = xmlDocGetRootElement(doc);
			//Récupérer les attributs de xml
			printTestExtern(root);
			xmlFreeDoc(doc);
			return true;
}

void ExternalDataLoader::printTestCityGML(xmlNode* rootNode){
	xmlNode *currentNode = NULL;
	for (currentNode = rootNode; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_ELEMENT_NODE&&currentNode->properties) {
				xmlChar * id=xmlGetProp(currentNode,(const xmlChar*)"id");
				if(id){
				cout<<id<<endl;
				}
				xmlFree(id);
			}
			printTestCityGML(currentNode->children);
		}
}

void ExternalDataLoader::printTestCityGMLPosition(xmlNode* rootNode){

}

void ExternalDataLoader::printTestExtern(xmlNode * rootNode){
	xmlNode *currentNode = NULL;
	for (currentNode = rootNode; currentNode; currentNode = currentNode->next) {
		if (currentNode->type == XML_ELEMENT_NODE&&currentNode->properties) {
			xmlChar * varKey=xmlGetProp(currentNode,(const xmlChar*)"VarKey");
			xmlChar * varValue=xmlGetProp(currentNode,(const xmlChar*)"VarValue");
			xmlChar * cityGMLId=xmlGetProp(currentNode,(const xmlChar*)"CityGMLId");
			if(varKey && varValue && cityGMLId)
			cout<<cityGMLId<<"\t"<<varKey<<"\t"<<varValue<<endl;
			xmlFree(varKey);xmlFree(cityGMLId);xmlFree(varValue);
		}
		printTestExtern(currentNode->children);
	}
}




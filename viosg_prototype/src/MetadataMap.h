/*
 * MetadataMap.h
 *
 *  Created on: 14 déc. 2013
 *      Author: blam
 */

#ifndef METADATAMAP_H_
#define METADATAMAP_H_
#include <osg/Group>

#include "GeodeFinder.h"
#include "GeodeData.h"
typedef struct {
	bool isNumeric;
	std::vector<std::string> values;
} ValuesData;

class MetadataMap {
public:
	std::map<std::string,ValuesData> metadataMap;
	MetadataMap(osg::ref_ptr<osg::Group> root);

	//--FONCTIONS UTILISEES PAR USERCOMMANDS
	//Fonctions pour vérifier les paramètres fournis dans les commandes utilisateurs
	bool hasType(std::string type);
	bool hasValue(std::string type, std::string value);

private:
	//--FONCTIONS INTERNES CONSTRUCTEUR--
	//Fonctions pour récupérer les types de métadonnées présentes dans la scène
	std::vector<std::string> getSceneTypes();
	std::vector<std::string> getGeodeTypes(GeodeData* metadata);

	//Fonctions pour récupérer pour chaque type, les valeurs présentes et si elles sont numériques
	ValuesData getValuesData(std::string type);
	std::vector<std::string> getSceneValues(std::string type);
	std::vector<std::string> getGeodeValues(GeodeData* metadata, std::string type);
	bool checkIfNumericType(vector<string> values);

	//Fonction générique de comparaison de vecteurs
	std::vector<std::string> updateWithNewData(std::vector<std::string> origin,std::vector<std::string> newData);


	//Fonction test pour afficher tous les types et valeurs récupérées
	void printAllMetadataTypesAndValues();

	osg::ref_ptr<osg::Group> root;
};

#endif /* METADATAMAP_H_ */

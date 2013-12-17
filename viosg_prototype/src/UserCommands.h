/*
 * UserCommands.h
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#ifndef USERCOMMANDS_H_
#define USERCOMMANDS_H_

#include <osg/Group>
#include <string>
#include <iostream>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osg/Material>
#include <osg/Depth>
#include <osg/StateSet>

#include "GeodeData.h"
#include "GeodeFinder.h"
#include "citygml.h"
#include "MetadataMap.h"
typedef struct {
	string color;
        float minValue;
        float maxValue;
        bool isDisplayed;
} Intervalle;


class UserCommands {
public:
	UserCommands(osg::ref_ptr<osg::Group> root,MetadataMap* metadataMap);
	void executeCommand(std::string command);
private:
	//--FONCTIONS CONSTRUCTEUR--
	void createColors();
	void createStateSets();
	void storeStateSets();
	void createColorsIntervalles();

	//--FONCTIONS DE TRAITEMENT DE COMMANDES
	std::vector<std::string> parseCommand(std::string command);

	//--FONCTIONS RELATIVES AUX COMMANDES UTILISATEUR--
	void printHelp();
	void printAllMetadata();
	void printGeodeMetadata(osg::Object* geode);
	void printTypes();
	void printValues(std::vector<string> parsedCommand);
	void testColor(std::vector<string>parsedCommand);
	void testTransparency(std::vector<string>parsedCommand);


	//--FONCTIONS INTERNES D'AFFICHAGE DES METADONEES DANS LA SCENE 3D--
	void updateStateSet(osg::Geode* geode, GeodeData* geodeData);
	//Fonctions de transparence
	void showMetadataByTransparency(const string key, const string value);
	void updateTransparencyState(GeodeData* geodeData, const string key,const string value);

	//Fonctions de couleurs
	void showMetadataByColor(const string key);

	void calculateColorsIntervalles(std::vector<std::string> values);
	void printColorsIntervalles();
	void resetColorsIntervalles();

	void updateColorState(GeodeData* geodeData, const string key);
	int calculateColorState(float geodeValue);

	void resetDisplay();
		void resetColor();
		void resetTransparency();




	osg::ref_ptr<osg::Group> root;
	MetadataMap* metadataMap;

	GeodeFinder geodeFinder;

	vector<osg::Material*> materials;
	vector<osg::StateSet*> colorsStateSets;
	osg::StateSet* transparentStateSet;
	osg::ref_ptr<osg::Group> stateSetsTree;

	vector<Intervalle> colorsIntervalles;

};

#endif /* USERCOMMANDS_H_ */

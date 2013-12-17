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
#include <osgText/Text>
#include <osgText/TextBase>

#include "GeodeData.h"
#include "GeodeFinder.h"
#include "citygml.h"
#include "MetadataMap.h"
typedef struct {
	osg::Vec4 colorcode;
	string color;
	float minValue;
	float maxValue;
	bool isDisplayed;
	string label;
} Intervalle;


typedef struct {
	std::string nomLegende;
	std::string nomTransparence;
	std::vector<Intervalle> libelles;
} Legende;

class UserCommands {
public:
	UserCommands(osg::ref_ptr<osg::Group> root,MetadataMap* metadataMap);
	void executeCommand(std::string command);
	Legende legend;

private:
	//--FONCTIONS CONSTRUCTEUR--
	void createColors();
	void createStateSets();
	void storeStateSets();
	void createColorsIntervalles();

	//--FONCTIONS DE TRAITEMENT DE COMMANDES
	std::vector<std::string> parseCommand(std::string command);

	//--FONCTIONS DE LA LEGENDE
	int hudIndex;
	osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size ) ;
	osg::Drawable* createLegendPolygon(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, const osg::Vec4& color, osg::Image* image);
	osg::Camera* createHUDCamera();
	void showLegend();

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

	int updateColorState(GeodeData* geodeData, const string key);
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
	osg::ref_ptr<osg::Vec4Array> colors;
	vector<Intervalle> colorsIntervalles;

};

#endif /* USERCOMMANDS_H_ */

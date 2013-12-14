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

#include "Metadata.h"
#include "GeodeFinder.h"
#include "citygml.h"

class UserCommands {
public:
	UserCommands(osg::ref_ptr<osg::Group> root);
	void executeCommand(std::string command);
private:
	//Fonctions constructeur
	void createColors();
	void createStateSets();

	//Fonctions d'affichage des métadonnées sur la scène
	void showMetadataByTransparency(const string key, const string value);
	void updateTransparencyState(Metadata* metadata, const string key,const string value);

	void showMetadataByColor(const string key);
	void updateColorState(Metadata* metadata, const string key);
	int calculateColorState(const string key, const string value);

	void showMetadataReset();
	void updateStateSet(osg::Geode* geode, Metadata* metadata);


	void test();
	void showHelp();
	void showAllMetadata();
	void showMetadata(osg::Object* object);






	osg::ref_ptr<osg::Group> root;
	GeodeFinder geodeFinder;
	vector<osg::Material*> materials;
	vector<osg::StateSet*> stateSetsColors;
	osg::StateSet* stateSetTransparent;
};

#endif /* USERCOMMANDS_H_ */

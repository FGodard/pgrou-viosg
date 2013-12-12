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

#include "Metadata.h"
#include "GeodeFinder.h"
#include "citygml.h"

class UserCommands {
public:
	UserCommands(osg::ref_ptr<osg::Group> root);
	void executeCommand(std::string command);
private:
	osg::ref_ptr<osg::Group> root;
	void showHelp();
	void showAllMetadata();
	void showAllTypeMetadata();
	//affiche tous les types
	void showType();
	//affiche tous les valeurs possibles pour un type choisie
	void showValues();
	void showMetadata(osg::Object* object);
	std::vector<std::string> showTypeMetadata(osg::Object* osgObject);
	std::vector<std::string> showValueMetadata(osg::Object* osgObject, string type);

};

#endif /* USERCOMMANDS_H_ */

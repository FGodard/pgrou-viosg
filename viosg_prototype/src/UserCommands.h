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
	void showMetadata(osg::Object* object);

	std::string valueOfKey(osg::Object* object, std::string key);
	void displayInColor(std::string key);
	void displayOnly(std::string key, int value);
	void setArrayColor();
	void transparenceBut(std::string key, int value);

	osg::Vec4Array* colors;

};

#endif /* USERCOMMANDS_H_ */

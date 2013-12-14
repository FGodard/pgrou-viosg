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
	void createColors();
	void createStateSets();
	void test();
	void showHelp();
	void showAllMetadata();
	void showMetadata(osg::Object* object);
	void transparenceTest();
	void defaultTest();

	osg::ref_ptr<osg::Group> root;
	vector<osg::Material*> materials;
	vector<osg::StateSet*> stateSetsColors;
	osg::StateSet* stateSetDefault;
	osg::StateSet* stateSetTransparent;
};

#endif /* USERCOMMANDS_H_ */

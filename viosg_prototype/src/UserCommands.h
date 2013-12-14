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
	void updateGeodeColor(osg::Geode*,int color);
	void updateGeodeTransparency(osg::Geode*,bool isTransparent);
	void transparenceTest();
	void defaultTest();
	void blueTest();

	osg::ref_ptr<osg::Group> root;
	vector<osg::Material*> materials;
	vector<osg::StateSet*> stateSetsColors;

	osg::StateSet* stateSetTransparent;
};

#endif /* USERCOMMANDS_H_ */

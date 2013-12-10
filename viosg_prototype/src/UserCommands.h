/*
 * UserCommands.h
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#ifndef USERCOMMANDS_H_
#define USERCOMMANDS_H_

#include <osg/Group>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osgText/TextBase>
#include <string>
#include <iostream>
#include <osgUtil/Tessellator>
#include <osg/LineWidth>

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
	void showLegend();
	void hideLegend();

};

#endif /* USERCOMMANDS_H_ */

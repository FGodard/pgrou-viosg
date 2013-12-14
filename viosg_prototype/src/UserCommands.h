/*
 * UserCommands.h
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#ifndef USERCOMMANDS_H_
#define USERCOMMANDS_H_

#include <osg/Group>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osg/Material>
#include <osg/Depth>
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
	vector <string> metadataTypes;
	osg::ref_ptr<osg::Group> root;
	void showHelp();
	void printType();
	void printAllMetadata();
	void printMetadata(osg::Object* object);
	vector<string>readCommand(string command);
	vector <string>getTypes();
	vector <string>showTypeMetadata(osg::Object* object);
	void showType();
	bool testCommand(string command );
	bool testType(string type);
	bool testValue(string value);
	void printOneMetadata(osg::Object* object,string type);
	void printAMetadata(vector <string> meta);
	void showTransparence(string type, string value);
	string valueOfKey(osg::Object* osgObject,string type);
	vector <string> showValues(string type);

};

#endif /* USERCOMMANDS_H_ */

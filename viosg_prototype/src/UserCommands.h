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
	bool testCommand( vector <string> options );
	void printOneMetadata(osg::Object* object, std::string meta);
	void printAMetadata(vector <string> meta);

};

#endif /* USERCOMMANDS_H_ */

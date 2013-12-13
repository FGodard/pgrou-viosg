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
	void echange(int& val1, int& val2);
	std::vector<std::string> showTypeMetadata(osg::Object* osgObject);
	std::vector<int> triTableau( vector <string> tab,int l);
	std::vector<std::string> showValueMetadata(osg::Object* osgObject, string type);
	std::vector<std::string> testCommand( vector <string> table_gde, vector <string> table_tot);
	std::vector<int> StringToInt( vector <string> tab);


};

#endif /* USERCOMMANDS_H_ */

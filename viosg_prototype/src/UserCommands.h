/*
* UserCommands.h
*
* Created on: 22 nov. 2013
* Author: blam
*/

//#ifndef USERCOMMANDS_H_
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


        //functions
        void printHelp();
        void printAllMetadata();
        void printType();
        void printValues(string type);
        void printAMetadata(vector <string> meta);

        void showTransparence(string type, string value);

        //TODO
        //void showColor*****
        //void backToDefult***
        //void showLegend****


        //dépendances

        vector<string>readCommand(string command);
        bool testCommand(string command );

        vector <string>getTypes();
        vector <string>getTypeMetadata(osg::Object* object);
        bool testType(string type);

        string valueOfKey(osg::Object* osgObject,string type);

        void printOneMetadata(osg::Object* object,string type);
        void printMetadata(osg::Object* object);
        vector <string>getValues(string type);
        bool testTypeValue(string type,string value);

        vector<string> showValueMetadata(osg::Object* osgObject, string type);
        vector<string>testUnic( vector <string> table_gde, vector <string> table_tot);

         std::vector<int> triTableau( vector <string> tab);
         std::vector<int> StringToInt( vector <string> tab);
     bool valide_entier(string t,int &e);
     vector<int>NumericValuesTri(vector<string> table_donne);
     bool NumericOrNot(vector<string> table_donne);

};


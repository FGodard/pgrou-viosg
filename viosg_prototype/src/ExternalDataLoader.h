/*
 * ExternalDataLoader.h
 *
 *  Created on: 8 déc. 2013
 *      Author: blam
 */

#ifndef EXTERNALDATALOADER_H_
#define EXTERNALDATALOADER_H_

#include <iostream>
#include <osg/Group>
#include <libxml2/libxml/DOCBparser.h>
#include <libxml2/libxml/tree.h>


class ExternalDataLoader {
public:
	ExternalDataLoader(osg::ref_ptr<osg::Group> externalDataGroup);
	void loadData(std::string filePath);

private:
	bool loadCityGMLData(std::string filePathCityGML);
	bool loadExternalFileData(std::string filePath);

	void printTestCityGML(xmlNode * rootNode);
	void printTestCityGMLPosition(xmlNode * rootNode);
	void printTestExtern(xmlNode * rootNode);

	osg::ref_ptr<osg::Group> externalDataGroup;
};

#endif /* EXTERNALDATALOADER_H_ */

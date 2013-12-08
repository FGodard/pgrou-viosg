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
	void print_element_names(xmlNode * a_node);
	void print_id_attributes(xmlNode * a_node);
	void printSpecificAttribute(xmlNode * rootNode, const xmlChar * attributeName);
	osg::ref_ptr<osg::Group> externalDataGroup;
};

#endif /* EXTERNALDATALOADER_H_ */

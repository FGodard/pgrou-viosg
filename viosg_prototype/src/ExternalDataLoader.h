/*
 * ExternalDataLoader.h
 *
 *  Created on: 8 déc. 2013
 *      Author: blam
 */

#ifndef EXTERNALDATALOADER_H_
#define EXTERNALDATALOADER_H_

#include "string.h"

class ExternalDataLoader {
public:
	ExternalDataLoader(osg::ref_ptr<osg::Group> externalDataGroup);
	void loadData(std::string filePath);

private:
	osg::ref_ptr<osg::Group> externalDataGroup;
};

#endif /* EXTERNALDATALOADER_H_ */

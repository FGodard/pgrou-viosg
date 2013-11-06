/*
 * Metadata.h
 *
 *  Created on: 6 nov. 2013
 *      Author: blam
 */

#ifndef METADATA_H_
#define METADATA_H_

#include <algorithm>
#include <cctype>
#include <osg/Referenced>
#include "citygml.h"
//Dérive de osg::Referenced pour pouvoir stocker Metadata dans la géode avec setUserData
class Metadata :public osg::Referenced {
public:
	Metadata(const citygml::AttributesMap& _attributes):attributes(_attributes){};
	//Hashmap contenant les métadonnéees
	const citygml::AttributesMap& attributes;
};

#endif /* METADATA_H_ */

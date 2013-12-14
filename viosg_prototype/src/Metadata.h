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

/**
 * Classe pour stocker les attributs des métadonnées
 * Dérive de osg::Referenced pour pouvoir les gérer depuis les objets osg avec set/getUserData
 */
class Metadata :public osg::Referenced {
public:
	Metadata(const citygml::AttributesMap& _attributes):attributes(_attributes){};
	//Hashmap contenant les métadonnéees
	const citygml::AttributesMap& attributes;
	bool isTransparent;
	int colorState;
};

#endif /* METADATA_H_ */

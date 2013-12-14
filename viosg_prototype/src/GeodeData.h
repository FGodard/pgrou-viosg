/*
 * GeodeData.h
 *
 *  Created on: 6 nov. 2013
 *      Author: blam
 */

#ifndef GEODEDATA_H_
#define GEODEDATA_H_

#include <algorithm>
#include <cctype>
#include <osg/Referenced>
#include "citygml.h"

/**
 * Classe pour stocker les attributs des métadonnées
 * Dérive de osg::Referenced pour pouvoir les gérer depuis les objets osg avec set/getUserData
 */
class GeodeData :public osg::Referenced {
public:
	GeodeData(const citygml::AttributesMap& _attributes):attributes(_attributes){
		isTransparent=false;
		colorState=0;
	};

	const citygml::AttributesMap& attributes;
	bool isTransparent;
	int colorState;
};

#endif /* GEODEDATA_H_ */

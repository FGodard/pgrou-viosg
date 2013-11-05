/*
 * CityGmlReader.h
 *
 *  Created on: 5 nov. 2013
 *      Author: blam
 */

#ifndef CITYGMLREADER_H_
#define CITYGMLREADER_H_


#include "citygml.h"

#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <iostream>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/Depth>
#include <osg/LightModel>

#include <osgText/Font>
#include <osgText/Text>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <algorithm>
#include <cctype>




class CityGmlReader {
public:
	CityGmlReader();
	virtual ~CityGmlReader();
	osg::MatrixTransform* readCityGmlFile(std::string FilePath);
private:
	citygml::CityModel* openFile(std::string filePath);
	osg::MatrixTransform* readCity(citygml::CityModel* city);
	bool createCityObjectGeode( const citygml::CityObject* object, osg::Group* parent );

};

#endif /* CITYGMLREADER_H_ */

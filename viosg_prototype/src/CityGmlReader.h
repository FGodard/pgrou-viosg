/*
 * CityGmlReader.h
 *
 *  Created on: 5 nov. 2013
 *      Author: blam
 */

#ifndef CITYGMLREADER_H_
#define CITYGMLREADER_H_




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

#include "citygml.h"
#include "Metadata.h"

/**
 * Classe utilisée pour charger les fichiers citygml et générer un arbre de scène
 */
class CityGmlReader {
public:

	CityGmlReader();
	//Génération de l'objet osg à partir de citygml
	osg::MatrixTransform* readCityGmlFile(std::string FilePath);

private:
	//Paramètres utilisateurs pour le niveau de détail
	bool _useMaxLODOnly;
	unsigned int minimumLODToConsider;

	//Variables internes pour le rendu des géométries
	std::string filePath;
	bool isFirstRender;
	osg::Vec3 _origin;
	std::map< std::string, osg::Texture2D* > _textureMap;

	//Méthodes internes pour générer la scène
	citygml::CityModel* openFile(std::string filePath);
	osg::MatrixTransform* readCity(citygml::CityModel* city);
	bool createCityObjectGeode( const citygml::CityObject* object, osg::Group* parent );
	void fetchCityObjectMetadata(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode);
	void createCityObjectDrawable(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode);
	void createCityObjectDrawableGeometry(const citygml::Polygon* p, osg::Geometry* geom);
	void createCityObjectDrawableMaterial(const citygml::CityObject* object,const citygml::Polygon* p, osg::Geometry* geom,const citygml::Geometry& geometry);
	void manageTransparencyifWindows(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode);


	unsigned int getHighestLodForObject( const citygml::CityObject * object);


	std::string parseFilePathToFileFolder(std::string _filePath);
};

#endif /* CITYGMLREADER_H_ */

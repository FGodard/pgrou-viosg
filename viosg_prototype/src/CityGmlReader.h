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


class CityGmlReader {
public:
	/**
	 * Constructeur de base avec les paramètres de rendu graphique par défaut
	 */
	CityGmlReader();

	/**
	 * Lit un fichier citygml au chemin fourni et renvoie le graphe de scène complet
	 */
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

	/**
	 * Ouvre le fichier avec le parser libcitygml
	 */
	citygml::CityModel* openFile(std::string filePath);

	/**
	 * Génère le graphe de scène correspondant à l'arbre des objets citygml
	 */
	osg::MatrixTransform* readCity(citygml::CityModel* city);

	/**
	 * Génère la géode correspondant à un objet citygml et le contenu
	 */
	bool createCityObjectGeode( const citygml::CityObject* object, osg::Group* parent );

	/**
	 * Récupère les metadonnées de l'objet citygml et les associe à la géode crée
	 */
	void fetchCityObjectMetadata(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode);

	/**
	 * Crée le contenu graphique de la géode à partir de l'objet citygml
	 */
	void createCityObjectDrawable(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode);

	/**
	 * Crée le contenu géométrique du drawable à partir de l'objet citygml
	 */
	void createCityObjectDrawableGeometry(const citygml::Polygon* p, osg::Geometry* geom);

	/**
	 * Crée la partie surface du drawable (couleurs/textures) à partir de l'objet citygml
	 */
	void createCityObjectDrawableMaterial(const citygml::CityObject* object,const citygml::Polygon* p, osg::Geometry* geom,const citygml::Geometry& geometry);

	/**
	 * Change la transparence de la géode si l'objet citygml associé est de type fenêtre
	 */
	void manageTransparencyifWindows(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode);

	/**
	 * Renvoie le plus haut niveau de détail parmi l'objet et ses enfants
	 */
	unsigned int getHighestLodForObject( const citygml::CityObject * object);

	/**
	 * Renvoie le chemin du dossier du fichier chargé
	 */
	std::string parseFilePathToFileFolder(std::string _filePath);
};

#endif /* CITYGMLREADER_H_ */

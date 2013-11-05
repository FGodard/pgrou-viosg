/*
 * CityGmlReader.cpp
 *
 *  Created on: 5 nov. 2013
 *      Author: blam
 */

#include "CityGmlReader.h"

using namespace std;
using namespace citygml;

CityGmlReader::CityGmlReader() {
	// TODO Auto-generated constructor stub

}

CityGmlReader::~CityGmlReader() {
	// TODO Auto-generated destructor stub
}

osg::MatrixTransform* CityGmlReader::readCityGmlFile(string filePath){
	citygml::CityModel* city=openFile(filePath);
	osg::MatrixTransform* cityScene=readCity(city);
	delete city;
	return cityScene;
}


CityModel* CityGmlReader::openFile(string filePath){
	//TODO Que peut on faire avec les paramètres?
	citygml::ParserParams params;
	std::ifstream file;
	file.open(filePath.c_str(),std::ifstream::in);
	CityModel* city = load( file, params );
	file.close();

	if ( !city ) osg::notify(osg::NOTICE)<<"Error while loading file"<<endl;
	else osg::notify(osg::NOTICE) << "Parsing CityGML file " << filePath << "..." << std::endl;

	return city;
}

osg::MatrixTransform* CityGmlReader::readCity(CityModel* city){
	if(!city)return NULL;
	osg::notify(osg::NOTICE) << city->size() << " city objects read." << std::endl;
	osg::notify(osg::NOTICE) << "Creation of the OSG city objects' geometry..." << std::endl;

	//Création de la matrice
	osg::MatrixTransform *root = new osg::MatrixTransform();
	const TVec3d& t = city->getTranslationParameters();
	root->setMatrix(osg::Matrixd::translate(t.x , t.y , t.z ));
	root->setName( city->getId() );

	//Création des objets de la scène
	#define RECURSIVE_DUMP

	#ifndef RECURSIVE_DUMP
	const citygml::CityObjectsMap& cityObjectsMap = city->getCityObjectsMap();
	citygml::CityObjectsMap::const_iterator it = cityObjectsMap.begin();
	for ( ; it != cityObjectsMap.end(); ++it )
	{
		const citygml::CityObjects& v = it->second;
		osg::notify(osg::NOTICE) << " Creation of " << v.size() << " " << citygml::getCityObjectsClassName( it->first ) << ( ( v.size() > 1 ) ? "s" : "" ) << "..." << std::endl;
		osg::Group* grp = new osg::Group;
		grp->setName( citygml::getCityObjectsClassName( it->first ) );
		root->addChild( grp );
		for ( unsigned int i = 0; i < v.size(); ++i )
			createCityObject( v[i], settings, grp ) );
	}
#else
	const citygml::CityObjects& roots = city->getCityObjectsRoots();
	for ( unsigned int i = 0; i < roots.size(); ++i ) createCityObjectGeode( roots[i], root );
#endif


	osg::notify(osg::NOTICE) << "Done." << std::endl;
	return root;
}

bool CityGmlReader::createCityObjectGeode( const citygml::CityObject* object, osg::Group* parent ){
	// Skip objects without geometry
		if ( !object || !parent ) return false;

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->setName( object->getId() );
		//TODO Ici ajouter les métadonnées à la géode

#ifdef RECURSIVE_DUMP
	osg::Group* grp = new osg::Group;
	grp->setName( object->getId() );
	grp->addChild( geode );
	parent->addChild( grp );
#else
	parent->addChild( geode )
#endif

		//TODO Creer les géométries pour chaque géode
return true;
}


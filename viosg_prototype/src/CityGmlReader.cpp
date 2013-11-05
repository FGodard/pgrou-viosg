/*
 * CityGmlReader.cpp
 *
 *  Created on: 5 nov. 2013
 *      Author: blam
 */

#include "CityGmlReader.h"

using namespace std;
using namespace citygml;


/**
 * Renvoie un graphe de scène du fichier spécifié dans filePath
 */
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

	root->setName( city->getId() );

	//Préparations des variables pour le rendu des géométries
	isFirstRender=true;
	_origin.set(0,0,0);

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

	root->setMatrix(osg::Matrixd::translate(t.x+_origin.x() , t.y+_origin.y() , t.z+_origin.z() ));
	osg::notify(osg::NOTICE) << "Done." << std::endl;
	return root;
}

bool CityGmlReader::createCityObjectGeode( const citygml::CityObject* object, osg::Group* parent ){
	// Skip objects without geometry
		if ( !object || !parent ) return false;

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->setName( object->getId() );

#ifdef RECURSIVE_DUMP
	osg::Group* grp = new osg::Group;
	grp->setName( object->getId() );
	grp->addChild( geode );
	parent->addChild( grp );
#else
	parent->addChild( geode )
#endif

	// Creer les géométries pour chaque géode
	createCityObjectGeometry(object,geode);
	//TODO Ici ajouter les métadonnées à la géode

#ifdef RECURSIVE_DUMP
			for ( unsigned int i = 0; i < object->getChildCount(); ++i )
		        createCityObjectGeode( object->getChild(i), grp );
		#endif


return true;
}

void CityGmlReader::createCityObjectGeometry(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode){
	// Get the default color for the whole city object
		osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
		shared_colors->push_back( osg::Vec4( object->getDefaultColor().r, object->getDefaultColor().g, object->getDefaultColor().b, object->getDefaultColor().a ) );

		osg::ref_ptr<osg::Vec4Array> roof_color = new osg::Vec4Array;
		roof_color->push_back( osg::Vec4( 0.9f, 0.1f, 0.1f, 1.0f ) );

		for ( unsigned int i = 0; i < object->size(); i++ )
		{
			const citygml::Geometry& geometry = *object->getGeometry( i );
			for ( unsigned int j = 0; j < geometry.size(); j++ )
			{
				const citygml::Polygon* p = geometry[j];

				if ( p->getIndices().size() == 0 ) continue;

				// Geometry management

				osg::Geometry* geom = new osg::Geometry;

				// Vertices
				osg::Vec3Array* vertices = new osg::Vec3Array;
				const std::vector<TVec3d>& vert = p->getVertices();
				vertices->reserve( vert.size() );
				for ( unsigned int k = 0; k < vert.size(); k++ )
							{
								osg::Vec3d pt( vert[k].x, vert[k].y, vert[k].z );
				                if ( isFirstRender )
								{
									_origin.set( pt );
									isFirstRender = false;
								}
								vertices->push_back( pt - _origin );
							}

				geom->setVertexArray( vertices );

				// Indices
				osg::DrawElementsUInt* indices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
				const std::vector<unsigned int>& ind = p->getIndices();
				indices->reserve( ind.size() );
				for ( unsigned int i = 0 ; i < ind.size() / 3; i++ )
				{
					indices->push_back( ind[ i * 3 + 0 ] );
					indices->push_back( ind[ i * 3 + 1 ] );
					indices->push_back( ind[ i * 3 + 2 ] );
				}

				geom->addPrimitiveSet( indices );

				// Normals
				osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
				const std::vector<TVec3f>& norm = p->getNormals();
				normals->reserve( norm.size() );
				for ( unsigned int k = 0; k < norm.size(); k++ )
					normals->push_back( osg::Vec3( norm[k].x, norm[k].y, norm[k].z ) );

				geom->setNormalArray( normals.get() );
				geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );


				//TODO Material Management


				//Add geometry to the geode
				geode->addDrawable(geom);
			}



			// Manage transparency for windows
			if ( object->getType() == citygml::COT_Window )
			{
				osg::StateSet *geodeSS( geode->getOrCreateStateSet() );

		        osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA,osg::BlendFunc::CONSTANT_ALPHA);
				geodeSS->setAttributeAndModes( blendFunc.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );

		        osg::ref_ptr<osg::BlendColor> blendColor = new osg::BlendColor(osg::Vec4( 1., 1., 1., object->getDefaultColor().a ));
				geodeSS->setAttributeAndModes( blendColor.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );

		        osg::ref_ptr<osg::Depth> depth = new osg::Depth;
		        depth->setWriteMask( false );
		        geodeSS->setAttributeAndModes( depth.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );

				geodeSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			}


		}

}


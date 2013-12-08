/*
 * CityGmlReader.cpp
 *
 *  Created on: 5 nov. 2013
 *      Author: blam
 */

#include "CityGmlReader.h"

using namespace std;
using namespace citygml;

CityGmlReader::CityGmlReader(){
	//On fixe les paramètres utilisateurs par défaut
	//TODO pouvoir proposer à l'utilisateur de modifier le niveau de détail
	_useMaxLODOnly=false;
	minimumLODToConsider=0;
	isFirstRender=true;
}

/**
 * Lit un fichier citygml au chemin fourni et renvoie le graphe de scène complet
 */
osg::MatrixTransform* CityGmlReader::readCityGmlFile(string filePath){
	osg::notify(osg::NOTICE)<<"Trying to open given file..."<<endl;
	citygml::CityModel* city=openFile(filePath);
	osg::MatrixTransform* cityScene=readCity(city);

	return cityScene;
}


/**
 * Ouvre le fichier avec le parser libcitygml
 */
CityModel* CityGmlReader::openFile(string _filePath){
	filePath=_filePath;
	citygml::ParserParams params;
	std::ifstream file;
	file.open(filePath.c_str(),std::ifstream::in);
	CityModel* city = load( file, params );
	file.close();

	if ( !city ) osg::notify(osg::NOTICE)<<"Error while loading file"<<endl;
	else osg::notify(osg::NOTICE) << "Parsing CityGML file " << filePath << "..." << std::endl;

	return city;
}

/**
 * Génère le graphe de scène correspondant à l'arbre des objets citygml
 */
osg::MatrixTransform* CityGmlReader::readCity(CityModel* city){
	if(!city)return NULL;
	osg::notify(osg::NOTICE) << city->size() << " city objects read." << std::endl;
	osg::notify(osg::NOTICE) << "Creation of the OSG city objects' geometry..." << std::endl;

	//Création de la matrice root
	osg::MatrixTransform *root = new osg::MatrixTransform();
	root->setName( city->getId() );

	//Préparations des variables pour le rendu des géométries
	isFirstRender=true;
	_origin.set(0,0,0);

	//Dump des objets de la scène
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

	const TVec3d& t = city->getTranslationParameters();
	root->setMatrix(osg::Matrixd::translate(t.x+_origin.x() , t.y+_origin.y() , t.z+_origin.z() ));

	osg::notify(osg::NOTICE) << "Done." << std::endl;
	return root;
}

/**
 * Génère la géode correspondant à un objet citygml et le contenu
 */
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

	// Creer le contenu graphique de la géode
	createCityObjectDrawable(object,geode);

	//Ajout des métadonnées à la géode
	fetchCityObjectMetadata(object,geode);


#ifdef RECURSIVE_DUMP
	for ( unsigned int i = 0; i < object->getChildCount(); ++i )
		createCityObjectGeode( object->getChild(i), grp );
#endif


	return true;
}


/**
 * Récupère les metadonnées de l'objet citygml et les associe à la géode crée
 */
void CityGmlReader::fetchCityObjectMetadata(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode){
	const AttributesMap& attributes=object->getAttributes();
	Metadata* metadata=new Metadata(attributes);
	geode->setUserData(metadata);
}

/**
 * Crée le contenu graphique de la géode à partir de l'objet citygml
 */
void CityGmlReader::createCityObjectDrawable(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode){

	for ( unsigned int i = 0; i < object->size(); i++ )
	{
		const citygml::Geometry& geometry = *object->getGeometry( i );
		//Vérification du niveau de détail

		const unsigned int currentLOD = geometry.getLOD();
		unsigned int highestLOD = getHighestLodForObject(object);
		if (_useMaxLODOnly && (currentLOD < highestLOD || currentLOD < minimumLODToConsider )){
			continue;
		}

		for ( unsigned int j = 0; j < geometry.size(); j++ )
		{
			const citygml::Polygon* p = geometry[j];
			if ( p->getIndices().size() == 0 ) continue;
			osg::Geometry* geom = new osg::Geometry;

			// Geometry management
			createCityObjectDrawableGeometry(p,geom);

//Le modèle est crée sans couleurs pour pouvoir afficher les légendes
#ifdef COLOR
			// Material Management
			createCityObjectDrawableMaterial(object,p,geom,geometry);
			//Gérer la transparence si l'objet est une fenêtre
			manageTransparencyifWindows(object,geode);
#endif
			//Add geometry to the geode
			geode->addDrawable(geom);
		}

	}

}

/**
 * Crée le contenu géométrique du drawable à partir de l'objet citygml
 */
void CityGmlReader::createCityObjectDrawableGeometry(const citygml::Polygon* p, osg::Geometry* geom){
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
}

/**
 * Crée la partie surface du drawable (couleurs/textures) à partir de l'objet citygml
 */
void CityGmlReader::createCityObjectDrawableMaterial(const citygml::CityObject* object,const citygml::Polygon* p, osg::Geometry* geom,const citygml::Geometry& geometry){
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back( osg::Vec4( object->getDefaultColor().r, object->getDefaultColor().g, object->getDefaultColor().b, object->getDefaultColor().a ) );

	osg::ref_ptr<osg::Vec4Array> roof_color = new osg::Vec4Array;
	roof_color->push_back( osg::Vec4( 0.9f, 0.1f, 0.1f, 1.0f ) );

	osg::ref_ptr<osg::StateSet> stateset = geom->getOrCreateStateSet();

	const citygml::Appearance *mat = p->getAppearance();

	bool colorset = false;

	if ( mat )
	{
		shared_colors->clear();
		shared_colors->push_back( osg::Vec4( 1.f, 1.f, 1.f, 1.f ) );

		if ( const citygml::Material* m = dynamic_cast<const citygml::Material*>( mat ) )
		{
#define TOVEC4(_t_) osg::Vec4( _t_.r, _t_.g, _t_.b, _t_.a )
			osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
			TVec4f diffuse( m->getDiffuse(), 0.f );
			TVec4f emissive( m->getEmissive(), 0.f );
			TVec4f specular( m->getSpecular(), 0.f );
			float ambient = m->getAmbientIntensity();

			osg::Material* material = new osg::Material;
			material->setColorMode( osg::Material::OFF );
			material->setDiffuse( osg::Material::FRONT_AND_BACK, TOVEC4( diffuse ) );
			material->setSpecular( osg::Material::FRONT_AND_BACK, TOVEC4( specular ) );
			material->setEmission( osg::Material::FRONT_AND_BACK, TOVEC4( emissive ) );
			material->setShininess( osg::Material::FRONT_AND_BACK, m->getShininess() );
			material->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4( ambient, ambient, ambient, 1.0 ) );
			material->setTransparency( osg::Material::FRONT_AND_BACK, m->getTransparency() );
			stateset->setAttributeAndModes( material, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );
			stateset->setMode( GL_LIGHTING, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

			colorset = true;
		}
		else if ( const citygml::Texture* t = dynamic_cast<const citygml::Texture*>( mat ) )
		{
			const citygml::TexCoords& texCoords = p->getTexCoords();

			if ( texCoords.size() > 0 )
			{
				osg::Texture2D* texture = 0;

				if ( _textureMap.find( t->getUrl() ) == _textureMap.end() )
				{
					// Load a new texture
					osg::notify(osg::NOTICE) << "  Loading texture " << t->getUrl() << "..." << std::endl;

					if ( osg::Image* image = osgDB::readImageFile(parseFilePathToFileFolder(filePath)+ t->getUrl() ) )
					{
						texture = new osg::Texture2D;
						texture->setImage( image );
						texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR );
						texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::NEAREST );
						texture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
						texture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
						texture->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
					}
					else
						osg::notify(osg::NOTICE) << "  Warning: Texture " << t->getUrl() << " not found!" << std::endl;

					_textureMap[ t->getUrl() ] = texture;
				}
				else
					texture = _textureMap[ t->getUrl() ];

				if ( texture )
				{
					osg::ref_ptr<osg::Vec2Array> tex = new osg::Vec2Array;

					tex->reserve( texCoords.size() );
					for ( unsigned int k = 0; k < texCoords.size(); k++ )
						tex->push_back( osg::Vec2( texCoords[k].x, texCoords[k].y ) );

					geom->setTexCoordArray( 0, tex );

					stateset->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );

					colorset = true;
				}
			}
			else
				osg::notify(osg::NOTICE) << "  Warning: Texture coordinates not found for poly " << p->getId() << std::endl;
		}
	}
	// Color management
	geom->setColorArray( ( !colorset && geometry.getType() == citygml::GT_Roof ) ? roof_color.get() : shared_colors.get() );
	geom->setColorBinding( osg::Geometry::BIND_OVERALL );
#if 0
	// Set lighting model to two sided
	osg::ref_ptr< osg::LightModel > lightModel = new osg::LightModel;
	lightModel->setTwoSided( true );
	stateset->setAttributeAndModes( lightModel.get(), osg::StateAttribute::ON );
#endif
}


/**
 * Change la transparence de la géode si l'objet citygml associé est de type fenêtre
 */
void CityGmlReader::manageTransparencyifWindows(const citygml::CityObject* object, osg::ref_ptr<osg::Geode> geode){
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

/**
 * Renvoie le plus haut niveau de détail parmi l'objet et ses enfants
 */
unsigned int CityGmlReader::getHighestLodForObject( const citygml::CityObject * object){
	unsigned int highestLOD = 0;
	// first find out highest LOD for this object
	for (unsigned int i = 0; i < object->size(); i++) {
		const citygml::Geometry &geometry = *object->getGeometry(i);

		if (geometry.getLOD() > highestLOD){
			highestLOD = geometry.getLOD();
		}
	}

#ifdef RECURSIVE_DUMP
	//check for the highest LODs of Children
	for (unsigned int i = 0; i < object->getChildCount(); ++i){
		unsigned int tempHighestLOD = getHighestLodForObject(object->getChild(i));
		if (tempHighestLOD > highestLOD){
			tempHighestLOD = highestLOD;
		}
	}
#endif
	return highestLOD;
}

/**
 * Renvoie le chemin du dossier du fichier chargé
 */
string CityGmlReader::parseFilePathToFileFolder(string _filePath){
	//TODO pas terrible car ne marchera que sous Linux
	const string::size_type p = _filePath.find_last_of("/");
	return _filePath.substr(0, p)+"/";
}

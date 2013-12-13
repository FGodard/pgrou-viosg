/*
 * UserCommands.h
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#ifndef USERCOMMANDS_H_
#define USERCOMMANDS_H_

#include <osg/Group>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osgText/TextBase>
#include <string>
#include <iostream>
#include <osgUtil/Tessellator>
#include <osg/LineWidth>
#include <osg/Uniform>

#include "Metadata.h"
#include "GeodeFinder.h"
#include "citygml.h"

typedef struct {
	std::string nomLibelle;
	osg::Vec4 couleurLibelle;
} Libelle;
typedef struct {
	std::string nomLegende;
	std::vector<Libelle> libelles;
} Legende;


class UserCommands {
public:
	UserCommands(osg::ref_ptr<osg::Group> root);
	void executeCommand(std::string command);
	Legende legend;
private:
	int hudIndex;
	osg::ref_ptr<osg::Group> root;
	void showHelp();
	osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size ) ;
	osg::Drawable* createLegendPolygon(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, const osg::Vec4& color, osg::Image* image);
	osg::Camera* createHUDCamera();

	void showAllMetadata();
	void showMetadata(osg::Object* object);
	void showLegend();

	void showOneMetadata(osg::Object* object);
};

#endif /* USERCOMMANDS_H_ */

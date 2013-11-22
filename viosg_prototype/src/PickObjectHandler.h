/*
 * PickObjectHandler.h
 *
 *  Created on: 19 nov. 2013
 *      Author: blam
 */

#ifndef PICKOBJECTHANDLER_H_
#define PICKOBJECTHANDLER_H_

#include <osgUtil/LineSegmentIntersector>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <iostream>

#include "citygml.h"
#include "Metadata.h"

class IntersectionSelector{
public:
	bool handle(osgUtil::LineSegmentIntersector::Intersection &intersection);
};

class PickObjectHandler:public osgGA::GUIEventHandler{
public:
	PickObjectHandler(osgViewer::Viewer *node);
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
protected:
   osgViewer::Viewer *m_viewer;
   IntersectionSelector *m_selector;
};
#endif /* PickObjectHandler */

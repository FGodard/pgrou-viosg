/*
 * KeyEventHandler.h
 *
 *  Created on: 19 nov. 2013
 *      Author: blam
 */

#ifndef KEYEVENTHANDLER_H_
#define KEYEVENTHANDLER_H_

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

class KeyEventHandler:public osgGA::GUIEventHandler{
public:
	KeyEventHandler(osgViewer::Viewer *node);
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
protected:
   osgViewer::Viewer *m_viewer;
   IntersectionSelector *m_selector;
};
#endif /* KeyEventHandler */

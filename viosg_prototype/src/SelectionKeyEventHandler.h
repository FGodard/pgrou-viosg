/*
 * SelectionKeyEventHandler.h
 *
 *  Created on: 19 nov. 2013
 *      Author: blam
 */

#ifndef SELECTIONKEYEVENTHANDLER_H_
#define SELECTIONKEYEVENTHANDLER_H_

#include <osgUtil/LineSegmentIntersector>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <QtGui>
#include <iostream>

#include "citygml.h"
#include "Metadata.h"

class IntersectionSelector{
public:
	IntersectionSelector(QLabel* _metadataLabel);
	~IntersectionSelector();
	bool handle(osgUtil::LineSegmentIntersector::Intersection &intersection);
protected:
	QLabel* metadataLabel;
};

class SelectionKeyEventHandler:public osgGA::GUIEventHandler{
public:
	SelectionKeyEventHandler(osgViewer::Viewer *node, IntersectionSelector *selector);
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
protected:
   osgViewer::Viewer *m_viewer;
   IntersectionSelector *m_selector;
};
#endif /* SelectionKeyEventHandler */

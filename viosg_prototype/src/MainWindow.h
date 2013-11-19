/*
 * MainWindow.h
 *
 *  Created on: 19 nov. 2013
 *      Author: blam
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

//Selection des objets dans la scène
#include "SelectionKeyEventHandler.h"
#include "SelectionKeyEventHandler.cpp"

class MainWindow : public QMainWindow
{
	public:
	   MainWindow(osg::Node* scene,osg::Camera* camera);
};

class ViewerWidget : public QWidget {
public:
    ViewerWidget(osg::Node* scene,osg::Camera* camera, QLabel* metadataLabel);
   // osg::Camera* createCamera( int x, int y, int w, int h );
protected:
    virtual void paintEvent( QPaintEvent* event )
    { viewer.frame(); }

    osgViewer::Viewer viewer;
    QTimer timer;
};

#endif /* MAINWINDOW_H_ */

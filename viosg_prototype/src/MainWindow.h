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
#include "CityGmlReader.h"

/**
 * QtWidget contenant la scène 3D OpenSceneGraph
 */
class ViewerWidget : public QWidget {
public:
    ViewerWidget(osg::Node* scene,osg::Camera* camera, QLabel* metadataLabel);
protected:
    void paintEvent( QPaintEvent* event );

    osgViewer::Viewer viewer;
    QTimer timer;
};

class MainWindow : public QMainWindow
{
//Q_OBJECT
	public:
	   MainWindow(osg::Camera* camera);
	   ~MainWindow();
	protected:
	   osg::ref_ptr<osg::Group> root;
	   QLabel* metadataLabel;

	private:
	   void createMenu();
	   QVBoxLayout* createLeftLayout();
	   QVBoxLayout* createMiddleLayout(ViewerWidget* osgWidget);
	   QVBoxLayout* createRightLayout();

	   //Actions de l'interface
private slots:
	   void openFile();
};



#endif /* MAINWINDOW_H_ */

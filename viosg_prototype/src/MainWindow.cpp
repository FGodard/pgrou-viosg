/*
 * MainWindow.cpp
 *
 *  Created on: 19 nov. 2013
 *      Author: blam
 */

#include "MainWindow.h"

MainWindow::MainWindow(osg::Node* scene,osg::Camera* camera)
{
	//Layout Left
	 QVBoxLayout *leftLayout = new QVBoxLayout;
	 QPushButton *test1openFile = new QPushButton;
	 QPushButton *test2openFile = new QPushButton;
	 QPushButton *test3openFile = new QPushButton;
	 QLabel *testMetadata=new QLabel;
	 leftLayout->addWidget(test1openFile);
	 leftLayout->addWidget(test2openFile);
	 leftLayout->addWidget(test3openFile);
	 leftLayout->addWidget(testMetadata);

	 	 	 //Layout Middle
		   QVBoxLayout *middleLayout = new QVBoxLayout;
		   ViewerWidget* osgWidget = new ViewerWidget(scene,camera,testMetadata);
		   middleLayout->addWidget(osgWidget);

		   //Layout Right
		   QVBoxLayout *rightLayout = new QVBoxLayout;

		   //Main Layout
		   QHBoxLayout *mainLayout=new QHBoxLayout;
		   mainLayout->addLayout(leftLayout);
		   mainLayout->addLayout(middleLayout);
		   mainLayout->addLayout(rightLayout);

		   	QWidget *container = new QWidget;
		   	      container->setLayout(mainLayout);
		   	      setCentralWidget(container);
}

ViewerWidget::ViewerWidget(osg::Node* scene, osg::Camera* camera, QLabel* testMetadata):QWidget(){

	 	 	 viewer.setCamera(camera);
			viewer.setSceneData(scene);
//TODO Recupérer les réponses de l'intersector dans l'interface
			IntersectionSelector* myInterSectionSelector=new IntersectionSelector(testMetadata);
			 SelectionKeyEventHandler* eventHandler=new SelectionKeyEventHandler(&viewer,myInterSectionSelector);
			 viewer.addEventHandler(eventHandler);
			 viewer.addEventHandler( new osgViewer::StatsHandler );
			 viewer.setCameraManipulator( new osgGA::TrackballManipulator );
			 viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
			 viewer.setThreadingModel( osgViewer::Viewer::SingleThreaded );

osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
if ( gw )
        {
            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget( gw->getGLWidget() );
            setLayout( layout );
        }

        connect( &timer, SIGNAL(timeout()), this, SLOT(update()) );
        timer.start( 40 );
}





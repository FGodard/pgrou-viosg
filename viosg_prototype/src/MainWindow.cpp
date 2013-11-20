/*
 * MainWindow.cpp
 *
 *  Created on: 19 nov. 2013
 *      Author: blam
 */

#include "MainWindow.h"

MainWindow::~MainWindow(){
	delete metadataLabel;
}

MainWindow::MainWindow(osg::Node* scene,osg::Camera* camera):metadataLabel(new QLabel)
{
	//Create Menu
	createMenu();
	QHBoxLayout *mainLayout=new QHBoxLayout;
	mainLayout->addLayout(createLeftLayout());

	//Create osgWidget for middleLayout;
	ViewerWidget* osgWidget = new ViewerWidget(scene,camera,metadataLabel);
	mainLayout->addLayout(createMiddleLayout(osgWidget));

	mainLayout->addLayout(createRightLayout());
	QWidget *container = new QWidget;
	container->setLayout(mainLayout);
	setCentralWidget(container);
}



void MainWindow::createMenu(){
	 QMenu *menu = menuBar()->addMenu("&Menu");
			 QAction *actionOpenFile = new QAction("&Open File", this);
			 QAction *actionQuit = new QAction("&Quit", this);
			 menu->addAction(actionOpenFile);
			 menu->addAction(actionQuit);
}

QVBoxLayout* MainWindow::createLeftLayout(){
	//Layout Left
		 QVBoxLayout *leftLayout = new QVBoxLayout;
		 QPushButton *test1openFile = new QPushButton;
		 QPushButton *test2openFile = new QPushButton;
		 QPushButton *test3openFile = new QPushButton;

		 leftLayout->addWidget(test1openFile);
		 leftLayout->addWidget(test2openFile);
		 leftLayout->addWidget(test3openFile);
		 leftLayout->addWidget(metadataLabel);
		 return leftLayout;
}

QVBoxLayout* MainWindow::createMiddleLayout(ViewerWidget *osgWidget){
	  QVBoxLayout *middleLayout = new QVBoxLayout;
			   middleLayout->addWidget(osgWidget);
			   return middleLayout;
}

QVBoxLayout* MainWindow::createRightLayout(){
			   QVBoxLayout *rightLayout = new QVBoxLayout;
			   return rightLayout;
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

void ViewerWidget::paintEvent(QPaintEvent* event){
	viewer.frame();
}




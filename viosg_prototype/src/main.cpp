// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include <osgDB/ReadFile>

int main()
{
/* OBJECTS CREATION */


        osgViewer::Viewer viewer ;
        osg::ref_ptr<osg::Group> root (new osg::Group);
        //Creating the model node
        osg::ref_ptr<osg::Node> fileNode (osgDB::readNodeFile("/home/blam/samples/Frankfurt_Street_Setting_LOD3/Frankfurt_Street_Setting_LOD3.citygml"));

        // Set the scene data
        root->addChild(fileNode.get());
        viewer.setSceneData( root.get() );

/* KEYBOARD INPUT */

         //Stats Event Handler s key
        viewer.addEventHandler(new osgViewer::StatsHandler);
        //Windows size handler
        viewer.addEventHandler(new osgViewer::WindowSizeHandler);
        // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );


/* START VIEWER */

        return (viewer.run());
}

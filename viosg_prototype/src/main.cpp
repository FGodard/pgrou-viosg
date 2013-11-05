// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <iostream>
USE_OSGPLUGIN(citygml)

using namespace std;
int main()
{
/* OBJECTS CREATION */


        osgViewer::Viewer viewer ;
        osg::ref_ptr<osg::Group> root (new osg::Group);
/* TESTS */

        //Chargement du fichier
        //osg::ref_ptr<osg::Node> fileNode (osgDB::readNodeFile("/home/blam/samples/1.citygml"));
          osg::ref_ptr<osg::Node> fileNode (osgDB::readNodeFile("/home/blam/samples/Frankfurt_Street_Setting_LOD3/Frankfurt_Street_Setting_LOD3.citygml"));
        //Objets pour tester le fichier
        osg::ref_ptr <osg::Group> fileGroup;
        osg::ref_ptr <osg::Group> subGroup1;
        osg::ref_ptr <osg::Group> subGroup2;
        osg::ref_ptr <osg::Group> subGroup3;


        //Donne comme résultat MatrixTransform, on peut donc le caster en group
        cout << fileNode->className() << endl;
        // Caste le node en group pour pouvoir le décomposer
        fileGroup = (osg::Group*) fileNode.get();
        // Parcoure les enfants de la scène à 3 niveaux par des boucles
        for (int i = 0; i < fileGroup->getNumChildren(); i++) {
           cout << "\t" << fileGroup->getChild(i)->className() << endl;

           subGroup1 = (osg::Group *) fileGroup->getChild(i);
           for (int j = 0; j < subGroup1->getNumChildren(); j++) {
              cout << "\t\t" << subGroup1->getChild(j)->className() << endl;

              subGroup2 = (osg::Group *) subGroup1->getChild(j);
              for (int k = 0; k < subGroup2->getNumChildren(); k++) {
                 cout << "\t\t\t" << subGroup2->getChild(k)->className() << endl;

                 subGroup3 = (osg::Group *) subGroup2->getChild(k);
                 for (int l = 0; l < subGroup3->getNumChildren(); l++) {
                    cout << "\t\t\t\t" << subGroup3->getChild(l)->className() << endl;
                 }
              }
           }
        }

        /* SCENE DATA */

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

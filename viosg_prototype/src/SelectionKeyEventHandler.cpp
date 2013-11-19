#include "SelectionKeyEventHandler.h"

typedef osgUtil::LineSegmentIntersector::Intersection Intersection;
using namespace citygml;
using namespace std;
class IntersectionSelector
{
public:
   virtual bool handle(Intersection &intersection)
   {
       osg::NodePath &nodes = intersection.nodePath;
//TODO Verifier que nodes.size-1 est bien la géode
	cout<<"Objet trouvé:"<<nodes[nodes.size()-1]->getName()<<nodes[nodes.size()-1]->className()<<endl;

		osg::ref_ptr<Metadata> metadata =
					dynamic_cast<Metadata*> (nodes[nodes.size()-1]->getUserData() );

			if(metadata)
			{
				AttributesMap::const_iterator iterator;
						for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
						{
							//On affiche le nom et la valeur
							std::cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
						}
						cout<<endl;
			}

       if (nodes.size()>0) return true;
       else return false;
   }


};

class SelectionKeyEventHandler
   : public osgGA::GUIEventHandler
{
protected:

   osgViewer::Viewer *m_viewer;
   IntersectionSelector *m_selector;

public:

   SelectionKeyEventHandler(osgViewer::Viewer *node, IntersectionSelector *selector)
   : osgGA::GUIEventHandler()
   {
       m_viewer = node;
       m_selector = selector;
   }

   virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
   {
   switch (ea.getEventType()){

	   case(osgGA::GUIEventAdapter::KEYDOWN):
        {

		   switch(ea.getKey())
		   {
		   	   	//TODO Utiliser la touche espace
		   	   case 'w':
		   	   {
		   		   osg::ref_ptr<osgUtil::LineSegmentIntersector> ray = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized());
		   		   osgUtil::IntersectionVisitor visitor(ray);
		   		   m_viewer->getCamera()->accept(visitor);
		   		   if (ray->containsIntersections())
		   		   {
		   			   Intersection intersection = ray->getFirstIntersection();
		   			   if (m_selector->handle(intersection)) return true;
		   		   }
		   	   }

		   default:
		   return false;
		   }

	       default:
		   return false;
	   }
   }
   //Fin de la fonction
   }
};

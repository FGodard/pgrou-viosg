#include "PickObjectHandler.h"

typedef osgUtil::LineSegmentIntersector::Intersection Intersection;
using namespace citygml;
using namespace std;




bool IntersectionSelector::handle(Intersection &intersection)
{
	osg::NodePath &nodes = intersection.nodePath;
	//TODO Verifier que nodes.size-1 est bien la géode
	osg::ref_ptr<GeodeData> geodeData =
			dynamic_cast<GeodeData*> (nodes[nodes.size()-1]->getUserData() );

	if(geodeData)
	{
		cout<<"Objet trouvé:"<<endl;
		AttributesMap::const_iterator iterator;
		for (iterator=geodeData->attributes.begin(); iterator != geodeData->attributes.end();++iterator)
		{
			 std::cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
		}
	}

	if (nodes.size()>0) return true;
	else return false;
}


PickObjectHandler::PickObjectHandler(osgViewer::Viewer *node)
: osgGA::GUIEventHandler()
{
	m_viewer = node;
	m_selector = new IntersectionSelector();
}

bool PickObjectHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch (ea.getEventType()){
	case(osgGA::GUIEventAdapter::KEYDOWN):
        	{
		switch(ea.getKey())
		{
		//TODO Utiliser la touche espace
		case osgGA::GUIEventAdapter::KEY_Space:
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
}


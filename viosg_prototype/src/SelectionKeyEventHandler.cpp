#include "SelectionKeyEventHandler.h"

typedef osgUtil::LineSegmentIntersector::Intersection Intersection;
using namespace citygml;
using namespace std;


IntersectionSelector::IntersectionSelector(QLabel* _metadataLabel):
					metadataLabel(_metadataLabel){};

IntersectionSelector::~IntersectionSelector()
{
	delete metadataLabel;
}

bool IntersectionSelector::handle(Intersection &intersection)
{
	osg::NodePath &nodes = intersection.nodePath;

	//TODO Verifier que nodes.size-1 est bien la géode
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (nodes[nodes.size()-1]->getUserData() );

	if(metadata)
	{
		AttributesMap::const_iterator iterator;
		string data="";
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			data=data+iterator->first+"\t"+iterator->second+"\n";
		}

		const string dataf=data;
		QString qstring=QString::fromStdString(dataf);
		metadataLabel->setText(qstring);
	}

	if (nodes.size()>0) return true;
	else return false;
}


SelectionKeyEventHandler::SelectionKeyEventHandler(osgViewer::Viewer *node, IntersectionSelector *selector)
: osgGA::GUIEventHandler()
{
	m_viewer = node;
	m_selector = selector;
}

bool SelectionKeyEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
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
}


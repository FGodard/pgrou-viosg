#ifndef GEODE_FINDER_H
#define GEODE_FINDER_H

#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Geode>

#include <stdio.h>
#include <string.h>
using namespace std;

/**
 * NodeVisitor pour récupérer toutes les géodes dans l'arbre traversé
 */
class GeodeFinder : public osg::NodeVisitor {
   public:

   // Constructor - sets the traversal mode to TRAVERSE_ALL_CHILDREN
   // and Visitor type to NODE_VISITOR
   GeodeFinder();

   // The 'apply' method for 'node' type instances.
   // See if a className() call to searchNode returns "Geode."
   // If so, add this node to our list.
   void apply(osg::Node &searchNode);

   // Return a pointer to the first node in the list
   // with a matching name
   osg::Geode* getFirst();

   // return a the list of nodes we found
   std::vector<osg::Geode*> getNodeList();

   private:
   // List of nodes with names that match the searchForName string
   std::vector<osg::Geode*> foundGeodes;
};

#endif

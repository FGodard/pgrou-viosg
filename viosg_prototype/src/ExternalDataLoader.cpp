/*
 * ExternalDataLoader.cpp
 *
 *  Created on: 8 déc. 2013
 *      Author: blam
 */

#include "ExternalDataLoader.h"

using namespace std;
ExternalDataLoader::ExternalDataLoader(osg::ref_ptr<osg::Group> externalDataGroup){
	this->externalDataGroup=externalDataGroup;
}

void ExternalDataLoader::loadData(std::string filePath){
	xmlDoc *doc = NULL;
	doc = xmlReadFile(filePath.c_str(), NULL, 0);
	if (doc == NULL) {
        cout<<"Erreur lors du passage du fichier dans le parser"<<endl;
        return;
	    }


	//Print Data Inside
	xmlNode *root_element = NULL;
	root_element = xmlDocGetRootElement(doc);
	//Attribut recherché:
	const xmlChar * attributeName=(xmlChar*)"id";

	printSpecificAttribute(root_element,attributeName);


	    xmlFreeDoc(doc);
	    xmlCleanupParser();

}

void
ExternalDataLoader::print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}

void ExternalDataLoader::print_id_attributes(xmlNode * a_node){
xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
          if(cur_node->properties){
        	  xmlAttr* attribute = cur_node->properties;
        	     while(attribute && attribute->name && attribute->children)
        	     {
        	       xmlChar* value = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        	       printf ("Atributo %s: %s\n",attribute->name, value);

        	       xmlFree(value);
        	       attribute = attribute->next;
        	     }
          }
        }

        print_id_attributes(cur_node->children);
    }
}

void ExternalDataLoader::printSpecificAttribute(xmlNode * rootNode,const xmlChar * attributeName){
xmlNode *currentNode = NULL;
    for (currentNode = rootNode; currentNode; currentNode = currentNode->next) {
        if (currentNode->type == XML_ELEMENT_NODE&&currentNode->properties) {
        	xmlChar * attributeValue=xmlGetProp(currentNode,attributeName);
        	//print
        	if(attributeValue){
        		cout<<attributeValue<<endl;
        		xmlFree(attributeValue);
        	}
        }
        printSpecificAttribute(currentNode->children,attributeName);
    }
}

/*
 * UserCommands.cpp
 *
 *  Created on: 22 nov. 2013
 *  Modified on: dec. 2013
 *  Author: blam , wafa
 *
 */

#include "UserCommands.h"

#include <algorithm>
#include <Geode>
#include <map>
#include <ref_ptr>
#include <utility>
#include <vector>

using namespace std;

UserCommands::UserCommands(osg::ref_ptr<osg::Group> root)
{
	UserCommands::root=root;
}

/**
 * Execute la commande entrée dans le terminal
 */
/*******************************************************************************************/
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}
	if(command.compare("showAllType")==0) {showAllTypeMetadata();return;}
	if(command.compare("showAllTypeMetadata")==0) {showType();return;}
	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}

/*****************************************************************************************/
void UserCommands::showHelp(){
cout<<"Commands List:"<<endl;
cout<<"\t"<<"'help'"<<"\t"<<"Show commands list"<<endl;
cout<<"\t"<<"'showAll'"<<"\t"<<"Show all metadata stored on all geodes"<<endl;
cout<<"\t"<<"'showAllType'"<<"\t"<<"Show all types of metadata stored on all geodes"<<endl;
cout<<"\t"<<"'showAllTypeMetadata'"<<"\t"<<"Show all types of metadata possible "<<endl;

}

/*****************************************************************************************/
/**
 * Affiche toutes les métadonnées présentes
 * On utilise le geodeFinder pour récupérer tous les objets type géode de l'arbre
 * Car les métadonnées sont stockées uniquement dans les objets Géode pour l'instant
 */
void UserCommands::showAllMetadata(){
	cout<<endl<<"AFFICHAGE DES METADONNEES"<<endl;
	GeodeFinder geodeFinder;
	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	for(unsigned int i=0;i<geodes.size();i++){
		showMetadata(geodes[i]);
	}
}

/************************************************************************************************/
/*
 *Affiche toutes les types de metadonnees qu'on peut manipuler
 */

void UserCommands::showAllTypeMetadata(){
	cout<<endl<<"AFFICHAGE DES TYPES DE METADONNEES POUR CHAQUE GEODE"<<endl;
		GeodeFinder geodeFinder;
		vector <std::string> table_geode , table_globale;
		root->accept(geodeFinder);
		vector<osg::Geode*> geodes=geodeFinder.getNodeList();
for(unsigned int i=0;i<geodes.size();i++)
	{
	 vector <std::string> table_geode;
	 table_geode=showTypeMetadata(geodes[i]);
	 //affiche les types de cette geode
     for(unsigned int k=0;k<table_geode.size();k++)
	 {cout<<table_geode[k]<<endl;}
   }
}
/**********************************************************************/
void UserCommands::showType(){
	cout<<endl<<"AFFICHAGE DES TYPES POSSIBLES"<<endl;
		GeodeFinder geodeFinder;
		vector <std::string> table_geode , table_globale;
		root->accept(geodeFinder);
		vector<osg::Geode*> geodes=geodeFinder.getNodeList();
	 table_globale=showTypeMetadata(geodes[0]);
	 //affiche les types de cette geode

     for(unsigned int p=0;p<geodes.size();p++){
    	 table_geode=showTypeMetadata(geodes[p]);
     for(unsigned int j=0;j<table_geode.size();j++){
    	 bool find=false;
    	 unsigned int i=0;
    	 while (!find && i<table_globale.size()){
    		 if(table_globale.at(i).compare(table_geode[j])==0){
    			 find = true;
    		 }
    		 else {
    			 i++;
    		 }
    	         }
    	 if(!find){ table_globale.push_back(table_geode[j]);}
     }
   }
     for(unsigned int k=0;k<table_globale.size();k++)
    	 {cout<<table_globale[k]<<endl;}
     cout<<table_globale.size()<<endl;
}
/***************************************************************************************************************/
/**
 * Affiche les métadonnées contenu dans un objet osg
 * Il charge le userData, si il est de type Metadata, c'est qu'il contient des métadonnées
 */
void UserCommands::showMetadata(osg::Object* osgObject){

	//On récupère le userdata de l'object
	osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (osgObject->getUserData() );

	if(metadata)
	{
		citygml::AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			//On affiche le nom et la valeur
			cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
		}
		cout<<endl;
	}
}

/*****************************************************************************************************************/
/**
 * fonction qui retourne les types de donnees pour chaque geode
 * sous forme d'un tableau qui sera ensuite stocker en tant que table_geode
 */
std::vector<std::string> UserCommands::showTypeMetadata(osg::Object* osgObject)
{	osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (osgObject->getUserData() );
    vector <std::string> tab;
	if(metadata)
		{
		citygml::AttributesMap::const_iterator iterator;
		//extraction des types de metadonnees et enregistrement dans un tableau
	    for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		  tab.push_back(iterator->first);
	    }
		cout<<endl;
		return tab ;
}

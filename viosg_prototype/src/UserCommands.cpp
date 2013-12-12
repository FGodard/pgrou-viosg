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
	if(command.compare("Valeur")==0) {showValues();return;}
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
cout<<"\t"<<"'showAllValuesOfAllTypeMetadata'"<<"\t"<<"Show all values of each type of metadata "<<endl;

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
 *Affiche toutes les types de metadonnees qu'on peut manipuler pour chaque geode
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
/**
 * void permettant de trouver la liste final de tous les types de metadonnees possibles
 */
void UserCommands::showType(){
	cout<<endl<<"AFFICHAGE DES TYPES POSSIBLES"<<endl;
		GeodeFinder geodeFinder;
		vector <std::string> table_geode , table_globale;
		root->accept(geodeFinder);
		vector<osg::Geode*> geodes=geodeFinder.getNodeList();
		//initialisation de la table_globale par les types de la premieres geode
	 table_globale=showTypeMetadata(geodes[0]);
	 //parcours de toutes les geodes
     for(unsigned int p=0;p<geodes.size();p++){
    	 //creation de table contenant les types à chaque fois
    	 table_geode=showTypeMetadata(geodes[p]);
    	 //parcours de la table de chaque geode pour verifier si ce type existe deja ou non
     for(unsigned int j=0;j<table_geode.size();j++){
    	 bool find=false;
    	 unsigned int i=0;
    	 while (!find && i<table_globale.size()){
    		 //comparaison entre se qui existe et le type de la table de chaque geode
    		 if(table_globale.at(i).compare(table_geode[j])==0){
    			 find = true;
    		 }
    		 else {
    			 i++;
    		 }
    	         }
    	 //si c'est un nouveau type on  doit l'ajouter à la fin du tableau globale
    	 if(!find){ table_globale.push_back(table_geode[j]);}
     }
   }
     //affichage du tableau globale contenant tous les types possibles
         for(unsigned int k=0;k<table_globale.size();k++)
    	 {cout<<table_globale[k]<<endl;}
         cout<<table_globale.size()<<endl;
}
/****************************************************************************************************/
void UserCommands::showValues(){
	string type ="yearOfConstruction";
	GeodeFinder geodeFinder;
		vector <std::string> table_valeur_geode , table_donne;
		root->accept(geodeFinder);
		vector<osg::Geode*> geodes=geodeFinder.getNodeList();
        cout<<"Affiche les valeurs possibles pour chaque types"<<endl;
       //parcours de toutes les valeurs pour un type
        cout<< "Le type: "<<type<<"peut avoir comme valeur"<<endl;
        for(unsigned int p=0;p<geodes.size();p++){
    	 //creation de table contenant les valeurs à chaque fois
    	 table_valeur_geode=showValueMetadata(geodes[p],type);
    	 //parcours de la table de chaque geode pour verifier si cette valeur existe deja ou non
    	 table_donne=testCommand(table_valeur_geode, table_donne);
        }
     //affichage du tableau globale contenant tous les valeurs possibles de ce type
         for(unsigned int k=0;k<table_donne.size();k++)
    	 {cout<<table_donne[k]<<endl;}
         cout<<table_donne.size()<<endl;
}
/***********************************************************************************************************/
std::vector<std::string>UserCommands::testCommand( vector <string> table_gde, vector <string> table_tot){
	 for(unsigned int j=0;j<table_gde.size();j++){
	    	 bool find=false;
	    	 unsigned int i=0;
	    	 while (!find && i<table_tot.size()){
	    		 //comparaison entre se qui existe et la nouvelle valeur dans la table de chaque geode
	    		 if(table_tot.at(i).compare(table_gde[j])==0){
	    			 find = true;
	    		 }
	    		 else {
	    			 i++;
	    		 }
	    	         }
	    	 //si c'est une nouvelle valeur on  doit l'ajouter à la fin du tableau de donnees
	    	 if(!find){ table_tot.push_back(table_gde[j]);}
	     }
	 return table_tot;
}
/*****************************************************************************************************************/
/** retourne un tableau de donnees
 * fonction qui retourne les  donnees de toutes les geodes selon le type choisie qui est passé en parametre
 * sous forme d'un tableau qui sera ensuite stocker en tant que table_geode_donnees
 */
std::vector<std::string> UserCommands::showValueMetadata(osg::Object* osgObject, string type)
{	osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (osgObject->getUserData() );
    vector <std::string> tab;
	if(metadata)
		{
		citygml::AttributesMap::const_iterator iterator;
		//trouver les donnees d'un type donne
	    for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
	    	// trouver les donnees de ce type
	    	if (iterator->first.compare(type)==0)
		  tab.push_back(iterator->second);
	    }
		return tab ;
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
/** retourne un tableau de type
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


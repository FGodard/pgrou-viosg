/*
 * UserCommands.cpp
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#include "UserCommands.h"

#include <Geode>
#include <ref_ptr>
#include <vector>
#include <list>
using namespace std;

UserCommands::UserCommands(osg::ref_ptr<osg::Group> root){
	UserCommands::root=root;
}
/**
 * help
 * print all
 * print type
 * print value (type)
 * showcolor(type)
 * showtransparence(type)
 * show reset
 * show legend
 */


/**
 * fonction pour la lecture de la commande utilisateur ,
 * retourne un tableau ou sont stockés toutes les  commandes et les options
 */
	vector<string> UserCommands::readCommand(string command){

		string::size_type pos_start = 0, pos_end;
		vector <string> command_options;
		do
		{
		    pos_end = command.find (" ", pos_start);
		    string option;
		    if (pos_end != command.length())
		    {
		    	int i = pos_end - pos_start ;
		        option=command.substr(pos_start, i);
		        command_options.push_back(option);
		        pos_start = pos_end + 1;
		    }
		    else
		    {
		        option = command.substr (pos_start);
		    }
		}
		while (pos_end != string::npos);

	return command_options;
	}
/**
 * test command: fonction pour tester si les commandes utilisateurs sont valides
 * la liste des commandes possibles est chargée à partir du ficher .gml  d'entrée
 */

	bool test_option( vector <string> options )
	{
		vector <string> cmd;
		//les differentes commandes possibles  stockés dans cmd
		cmd.push_back("description");
		cmd.push_back("name");
		//Todo appel fonction Wafa pour initialiser cmd

		cmd.push_back("help");
		cmd.push_back("printAll");
		cmd.push_back("print");
		cmd.push_back("showLegend");
		cmd.push_back("showColor");
		cmd.push_back("showReset");
		cmd.push_back("showTransparence");
		//
		for(unsigned int j=0;j<options.size();j++){
		  bool find = false;
		  unsigned int i=0;
		  while (!find && i<cmd.size()){
			  if(cmd.at(i).compare(options[j])==0){
				  find=true;
			  }
			  else {
				  i++;
			  }
		  }
		  //si une commande n'existe pas dans la liste des commandes possible => quitter avec un message d'erreur
		  if(!find){
			  cout<<"**** ERROR : Command '"<<options[j]<<"' does not exist "<<endl;
			  return false;
		  }
		 }
		return true;
	}


/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){

	//lectute de la commande
	vector <string> _command=readCommand(command);
	for (unsigned int i=0;i<_command.size();i++){
			cout<<"*"<<_command[i]<<"*"<<endl;
		}

	//appel de la fonction test pour vérifier la saisie de l'utilisateur
	bool okToContinue= test_option(_command);

	//si toutes les commandes et les options sont valides => affichage
	if(okToContinue){
		//commande entrée : help => afficher le menu help
		if(_command[0].compare("help")==0){showHelp();return;}

		//commande d'entrée: showAll => afficher toutes les metadonnées dans le ficher
		if(_command[0].compare("printAll")==0) {showAllMetadata();return;}

		//print type: afficher les options saisies
		if(_command[0].compare("print")==0){
			if(_command.size()==1){
				cout<<"**** ERROR : "<<"print Command needs at least one argument"<<endl;
				}
				else{
					for (unsigned int j=1;j<_command.size();j++){
						printAMetadata(_command[j]);
						}
					}
			return;
			}
		//TODO les autres commandes
		}
	else{
	cout<<"Type 'help' for commands list or close the osgViewer to close the program"<<endl;
	}
}


void UserCommands::showHelp(){
cout<<"Commands List:"<<endl;
cout<<"\t"<<"'help'"<<"\t"<<"Show commands list"<<endl;
cout<<"\t"<<"'printAll'"<<"\t"<<"Show all metadata stored on all geodes"<<endl;
cout <<"\t"<<"'print-option'"<<"\t"<<"Show optional informations stored on each geode "<<endl;
cout<<"\t"<<"'showColor-option'"<<"\t"<<"Show metadata in color"<<endl;

}


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



/**
 * Affiche les métadonnées contenu dans un objet osg
 * Il charge le userData, si il est de type Metadata, c'est qu'il contient des métadonnées
 */
void UserCommands::showMetadata(osg::Object* osgObject){
	//On récupère le userdata de l'object
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast <Metadata*> (osgObject->getUserData() );

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


/**
 * affiche une seule metadonnée
 *
 */


void UserCommands::printAMetadata(string meta){
	cout<<endl<<"AFFICHAGE DES METADONNEES"<<endl;
	GeodeFinder geodeFinder;

	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		printOneMetadata(geodes[i],meta);
	}
}


/**
 * affiche une methadonnée
 *
 */
void UserCommands::printOneMetadata(osg::Object* osgObject,string meta){

	//On récupère le userdata de l'object
		osg::ref_ptr<Metadata> metadata =
				dynamic_cast<Metadata*> (osgObject->getUserData() );

		if(metadata)
		{
			citygml::AttributesMap::const_iterator iterator=metadata->attributes.find(meta);
			if(iterator!=metadata->attributes.end()){//il a trouvé l'attribut
			cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
			}
		}
}


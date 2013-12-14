/*
 * UserCommands.cpp
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

//CIN 08754170

//INSCRIPTION 3307

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
 * Test sur la valeur
 */
	bool UserCommands::testValue(string value ){

		//TODO metadataTypes vector of all types in class userCommands
		//vector <string> metadataValues=getValues();

		/*pour tester */
		vector <string> metadataValues;
		metadataValues.push_back("1999");

		bool find = false;
		unsigned int i=0;
		while (!find && i<metadataValues.size()){
			if(metadataValues.at(i).compare(value)==0){
				find=true;
			}
				else {
						i++;
					  }
		}
		//si tout le parcour effectué et que la valeur n existe pas => erreur
		if(!find){
			cout<<"**** ERROR : invalid Value '"<<value<<"'"<<endl;
			cout<<"Type 'printValues' to see which Values exists"<<endl;
			return false;
			}
		return true ;
		}

/**
 * Test sur le type
 */
	bool UserCommands::testType(string type){

		//TODO metadataTypes vector of all types in class userCommands
		vector <string> metadataTypes=getTypes();

		bool find = false;
		unsigned int i=0;
		while (!find && i<metadataTypes.size()){
				if(metadataTypes.at(i).compare(type)==0){
					find=true;
				}
				else {
					   i++;
					  }
		 }
		//si tout le parcour effectué et que la valeur n existe pas => erreur
			if(!find){
				cout<<"**** ERROR : invalid Type '"<<type<<"'"<<endl;
				cout<<"Type 'printType' to see which type exists"<<endl;
				return false;
				}
		return true;
	}
/**
 * Test sur la commande
 */
	bool UserCommands::testCommand( string command )
	{
		//liste des commandes
		vector <string> cmd;
		cmd.push_back("help");
		cmd.push_back("printAll");
		cmd.push_back("printType");
		cmd.push_back("printValues");
		cmd.push_back("print");
		cmd.push_back("showLegend");
		cmd.push_back("showColor");
		cmd.push_back("showReset");
		cmd.push_back("showTransparence");

		bool find = false;
		unsigned int i=0;
		while (!find && i<cmd.size()){
			if(cmd.at(i).compare(command)==0){
				find=true;
			}
			else {
					i++;
				}
		 }
		//si tout le parcour effectué et que la valeur n existe pas => erreur
			if(!find){
				cout<<"**** ERROR : invalid Command '"<<command<<"'"<<endl;
				cout<<"Type 'help' to see list of command"<<endl;
				return false;
				}
		return true;
	}


/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){

	//lectute de la commande
	vector <string> _command=readCommand(command);

	//commande entrée : help => afficher le menu principal
	if(_command[0].compare("main")==0){showHelp();return;}

	bool okToContinue=true;
	//appel des fonctions test pour vérifier la commande
	okToContinue=testCommand(_command[0]);

	if(okToContinue){
		//commande entrée : help => afficher le menu help
		if(_command[0].compare("help")==0 && _command.size()==1){showHelp();return;}

		//commande d'entrée: printAll => afficher toutes les metadonnées dans le ficher
		if(_command[0].compare("printAll")==0 && _command.size()==1) {printAllMetadata();return;}

		//commande d'entrée: printType => afficher tout les types existant dans le fichier
		if (_command[0].compare("printType")==0 && _command.size()==1){printType();return;}

		//commande d'entrée: printValues => afficher toutes les valeurs existantes
		//if (_command[0].compare("printValues")==0){printValues();return;}

		if(_command[0].compare("print")==0){
			if(_command.size()==1){
				cout<<"**** ERROR : "<<"print Command needs at least one argement"<<endl;
				}
				else{
						bool  ok=true;
						unsigned int k=1;
						while (ok && k<_command.size())
						{
							ok=testType(_command[k]);
							k++;
						}
						if (ok){printAMetadata(_command);}
					}
			return;
			}

		//commande d'entrée: showTransparence
		if(_command[0].compare("showTransparence")==0){
			if(_command.size()==1 || _command.size()>3){
							cout<<"**** ERROR : "<<"showTransparence Command needs two argements"<<endl;
							//cout<<"     showTransparence [TYPE] [VALUE]"<<endl;
							}
							else{
									bool  ok=testType(_command[1])&& testValue(_command[2]);
									if (ok){showTransparence(_command[1],_command[2]);}
								}
						return;
		}

			/**
				cmd.push_back("showLegend");
				cmd.push_back("showColor");
				cmd.push_back("showReset");

		 */
		cout<<"**** ERROR : invalid Command '"<<command<<"'"<<endl;
		cout<<"Type 'help' to see list of command"<<endl;
		}


}
/**
 * afficher tout les types de metadonnées existants
 */
void UserCommands::printType(){

	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"|                           DISPLAY EXISTING TYPES                                     |"<<endl;
	cout<<"|______________________________________________________________________________________|"<<endl;

	//TODO Types vector of all types
	vector <string> metadataTypes=getTypes();
	for(unsigned int k=0;k<metadataTypes.size();k++)
            {cout<<"\t"<<metadataTypes[k]<<endl;}

}
/** retourne un tableau de type
* fonction qui retourne les types de donnees pour chaque geode
* sous forme d'un tableau qui sera ensuite stocker en tant que table_geode
*/
vector<string> UserCommands::showTypeMetadata(osg::Object* osgObject)
{        osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (osgObject->getUserData() );
    vector <string> tab;
        if(metadata)
                {
                citygml::AttributesMap::const_iterator iterator;
                //extraction des types de metadonnees et enregistrement dans un tableau
         for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
                 tab.push_back(iterator->first);
         }

                return tab ;
}
vector <string> UserCommands::getTypes(){

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
         //si c'est un nouveau type on doit l'ajouter à la fin du tableau globale
         if(!find){ table_globale.push_back(table_geode[j]);}
 }
}
 return table_globale;
}


void UserCommands::showHelp(){

	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"|                                                                                      |"<<endl;
	cout<<"|                                    USER GUIDE                                        |"<<endl;
	cout<<"|______________________________________________________________________________________|"<<endl;
	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"| COMMAND                  |   DESCRIPTION                                             |"<<endl;
	cout<<"|__________________________|___________________________________________________________|"<<endl;
	cout<<"| help                     |   Open the user guide                                     |"<<endl;
	cout<<"| printAll                 |   Show all metadata stored on all geodes                  |"<<endl;
	cout<<"| printType                |   Show types of metadata stored on all geodes             |"<<endl;
	cout<<"| print [TYPE]             |   Show optional informations stored on all geodes         |"<<endl;
	cout<<"|                          |     --you can select more than one TYPE--                 |"<<endl;
	cout<<"| showColor [TYPE]         |   Show data classified by TYPE                            |"<<endl;
	cout<<"|                          |     --you can select only one TYPE --                     |"<<endl;
	cout<<"| showTransparence [TYPE]  |   Show TYPE only of selected PTION                        |"<<endl;
	cout<<"|                  [OPTION]|                                                           |"<<endl;
	cout<<"| showReset                |   *****                                                   |"<<endl;
	cout<<"| showLegend               |   *****                                                   |"<<endl;
	cout<<"|__________________________|___________________________________________________________|"<<endl;

}


/**
 * Affiche toutes les métadonnées présentes
 * On utilise le geodeFinder pour récupérer tous les objets type géode de l'arbre
 * Car les métadonnées sont stockées uniquement dans les objets Géode pour l'instant
 */
void UserCommands::printAllMetadata(){

	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"|                                   DISPLAY METADATA                                   |"<<endl;
	cout<<"|______________________________________________________________________________________|"<<endl;

	GeodeFinder geodeFinder;

	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		printMetadata(geodes[i]);
	}
}



/**
 * Affiche les métadonnées contenu dans un objet osg
 * Il charge le userData, si il est de type Metadata, c'est qu'il contient des métadonnées
 */

	void UserCommands::printMetadata(osg::Object* osgObject){
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
 * affiche seulement les metadonnées sélectionnées
 *
 */

void UserCommands::printAMetadata(vector <string> meta){


	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"|                                   DISPLAY METADATA                                   |"<<endl;
	cout<<"|______________________________________________________________________________________|"<<endl;
	GeodeFinder geodeFinder;

	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	for(unsigned int i=0;i<geodes.size();i++){
		for(unsigned int j=0;j<meta.size();j++){
			printOneMetadata(geodes[i],meta[j]);
		}
			cout<<endl;
	}
}


/**
 * affiche une metadonnée
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

//Pour une géode donnée, il renvoit la valeur de la clé passée en paramètre
string UserCommands::valueOfKey(osg::Object* osgObject, std::string key){
        osg::ref_ptr<Metadata> metadata =
                        dynamic_cast<Metadata*> (osgObject->getUserData());
        if(metadata)
        {
                citygml::AttributesMap::const_iterator iterator;
                for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
                {
                        std::string test=iterator->first;
                        std::string test2=iterator->second;

                        if (test.compare(key)==0)
                                return test2;
                }
        }
        return "";
}


/**
 *
 * Met en transparence les géodes ne correspondant pas au couple (clé,valeur) donné en paramètre
 */

//
void UserCommands::showTransparence(string key, string value){
        GeodeFinder geodeFinder;
                root->accept(geodeFinder);
                vector<osg::Geode*> geodes=geodeFinder.getNodeList();

                for(unsigned int i=0;i<geodes.size();i++){
                       string test =valueOfKey(geodes[i],key);

                       osg::ref_ptr<osg::Material> material = new osg::Material;

                        material->setAlpha(osg::Material::FRONT_AND_BACK, 0.1);
                        osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );

                                        if (test!=value){
                                        geodes[i]->getOrCreateStateSet()->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
                                        geodes[i]->getOrCreateStateSet()->setAttributeAndModes(bf);
                                        }
}
}

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
	 * test si la valeur donnée existe pour le type saisis
	 */

	bool UserCommands::testTypeValue(string type,string value){

		vector <string> metadataValues=getValues(type);

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
				cout<<"**** ERROR : invalid value for the type: '"<<type<<"'"<<endl;
				cout<<"Type 'printValue "<<type<<"' to see which values you can select"<<endl;
				return false;
				}

		return true;
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
		cmd.push_back("printValue");
		cmd.push_back("print");
		cmd.push_back("showLegend");
		cmd.push_back("showColor");
		cmd.push_back("showReset");
		cmd.push_back("showTransparence");
		cmd.push_back("backToDefault");

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
	if(_command[0].compare("main")==0){printHelp();return;}

	bool okToContinue=true;
	//appel des fonctions test pour vérifier la commande
	okToContinue=testCommand(_command[0]);

	if(okToContinue){
		//commande entrée : help => afficher le menu help
		if(_command[0].compare("help")==0 && _command.size()==1){printHelp();return;}

		//commande d'entrée: printAll => afficher toutes les metadonnées dans le ficher
		if(_command[0].compare("printAll")==0 && _command.size()==1) {printAllMetadata();return;}

		//commande d'entrée: printType => afficher tout les types existant dans le fichier
		if (_command[0].compare("printType")==0 && _command.size()==1){printType();return;}

		//commande d'entrée: printValue => afficher toutes les valeurs existantes

		if (_command[0].compare("printValue")==0 && _command.size()==2){
			if(_command.size()==1){
				cout<<"**** ERROR : "<<"printValue Command needs one argument"<<endl;
				}
			else{
					bool ok=testType(_command[1]);
					if (ok){
							printValues(_command[1]);
							}
				}
			return;
		}


		if(command.compare("backToDefault")==0) {
			cout<<"ici appel de la fonction default test"<<endl;
			//defaultTest();
			return;
		}

		if(_command[0].compare("showLegend")==0 && _command.size()==1) {
			cout << "ici appel de show legend"<<endl;
			//showLegend();
			return;
		}

		if(_command[0].compare("print")==0){
			if(_command.size()==1){
				cout<<"**** ERROR : "<<"print Command needs at least one argument"<<endl;
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

		//
		if (_command[0].compare("showColor")==0){
			if(_command.size()==1){
				cout<<"**** ERROR : "<<"showColor Command needs one argument"<<endl;
				}
			else{
					bool ok=testType(_command[1]);
					if (ok){
							cout<<"ici appel de show color "<<endl;
							//showColor(_command[1]);
							}
				}
			return;
		}


		//commande d'entrée: showTransparence
		if(_command[0].compare("showTransparence")==0){
			if(_command.size()==3){
				bool  ok=testTypeValue(_command[1],_command[2]);
				if (ok){showTransparence(_command[1],_command[2]);}
				}
				else{cout<<"**** ERROR : "<<"showTransparence Command needs two arguments"<<endl;}
			return;
		}
		cout<<"**** ERROR : invalid Command '"<<command<<"'"<<endl;
	}
		cout<<"Type 'help' to see list of command or close the viewer to exit the programm "<<endl;
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



void UserCommands::printValues(string type){

	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"|                     DISPLAY EXISTING VALUES FOR SELECTED TYPE                        |"<<endl;
	cout<<"|______________________________________________________________________________________|"<<endl;

	vector<int> table2;
	vector<string> metadataValues=getValues(type);
	bool b=NumericOrNot(metadataValues);
	if(b==true){
		table2=triTableau(metadataValues);
		for(unsigned int k=0;k<table2.size();k++)
		            {cout<<"\t"<<table2[k]<<endl;}
	}
	else{
		for(unsigned int k=0;k<metadataValues.size();k++)
		            {cout<<"\t"<<metadataValues[k]<<endl;}
	}

}




/** retourne un tableau de type
* fonction qui retourne les types de donnees pour chaque geode
* sous forme d'un tableau qui sera ensuite stocker en tant que table_geode
*/

vector<string> UserCommands::getTypeMetadata(osg::Object* osgObject)
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
     table_globale=getTypeMetadata(geodes[0]);
     //parcours de toutes les geodes
     for(unsigned int p=0;p<geodes.size();p++){
         //creation de table contenant les types à chaque fois
         table_geode=getTypeMetadata(geodes[p]);
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


void UserCommands::printHelp(){

	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"|                                                                                      |"<<endl;
	cout<<"|                                    USER GUIDE                                        |"<<endl;
	cout<<"|______________________________________________________________________________________|"<<endl;
	cout<<" ______________________________________________________________________________________"<<endl;
	cout<<"| COMMAND                  |   DESCRIPTION                                             |"<<endl;
	cout<<"|__________________________|___________________________________________________________|"<<endl;
	cout<<"| help                     |   Open the user guide                                     |"<<endl;
	cout<<"| printAll                 |   Show all metadata stored on all geodes                  |"<<endl;
	cout<<"| printValue [TYPE]        |   Show all values                                         |"<<endl;
	cout<<"| printType                |   Show types of metadata stored on all geodes             |"<<endl;
	cout<<"| print [TYPE]             |   Show optional informations stored on all geodes         |"<<endl;
	cout<<"|                          |     --you can select more than one TYPE--                 |"<<endl;
	cout<<"| showColor [TYPE]         |   Show data classified by TYPE                            |"<<endl;
	cout<<"|                          |     --you can select only one TYPE --                     |"<<endl;
	cout<<"| showTransparence [TYPE]  |   Show TYPE only of selected PTION                        |"<<endl;
	cout<<"|                  [OPTION]|                                                           |"<<endl;
	cout<<"| showReset                |   *****                                                   |"<<endl;
	cout<<"| showLegend               |   *****                                                   |"<<endl;
	cout<<"| backToDefault            |   *****                                                   |"<<endl;
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

/*
* fonction prend en parametre deux tableau
* donne en resultat un tableau global contenant tout les elements
* */
vector<string>UserCommands::testUnic( vector <string> table_gde, vector <string> table_tot){
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
                  //si c'est une nouvelle valeur on doit l'ajouter à la fin du tableau de donnees
                  if(!find){ table_tot.push_back(table_gde[j]);}
         }
         return table_tot;
}

/**
 * valeurs d'un type donnée
 */


vector<string>UserCommands::getValues(string type){

        GeodeFinder geodeFinder;
                vector <std::string> table_donne ;
                vector<int>table_final;
                root->accept(geodeFinder);
                vector<osg::Geode*> geodes=geodeFinder.getNodeList();

        for(unsigned int p=0;p<geodes.size();p++){

             vector <std::string> table_valeur_geode ;
             //creation de table contenant les valeurs à chaque fois
             table_valeur_geode=showValueMetadata(geodes[p],type);
             //parcours de la table de chaque geode pour verifier si cette valeur existe deja ou non
             table_donne=testUnic(table_valeur_geode, table_donne);
         }

        return table_donne;
}



/***********************************************************************************************************/
/*
* retourne true si le type est numerique
*/
 bool UserCommands::NumericOrNot(vector<string> table_donne){
 vector<int> table_final;
  bool test=true;

      for(unsigned int p=0;p<table_donne.size();p++){
              int a;
              bool ib =valide_entier(table_donne[p],a);
              if(ib==false)
                      {test=false;
                      break;}
       }
      return test;
 }


 /*
 * Convertin d'un tableau de chaine de caractaire en tableau d'entier
 */
 std::vector<int>UserCommands::StringToInt( vector <string> tab){
	 vector<int> tableau;
	 int x;
     for(unsigned int i=0;i<tab.size();i++)
         {
              string myStream = tab[i];
              istringstream buffer(myStream);
              buffer >> x;
              tableau.push_back(x);

         }
     return tableau;
 }

 /*
 * fonction tri dans le cas des valeurs numeriques
 */
 std::vector<int>UserCommands::triTableau( vector <string> tab){
         vector<int> tabl;

         //changer le type du tableau en tableau de int
         tabl=StringToInt(tab);

         for(unsigned int i = 0; i < tab.size(); i++) {
        	 for(unsigned int j = 0; j < tab.size()-1 ; j++)
        		 if(tabl[j] > tabl[j+1])
        			 std::swap(tabl[j], tabl[j+1]);
         }
         return tabl;
 }



 /*
 * determiner le type d'une valeur passé en parametre
 * retourne unboolean
 * true si le type est numeirque
 * false sinon
 */
 bool UserCommands::valide_entier(string t,int &e)
 {
     bool valide=true;
     int i=0;
     e=0;
     if (t[0]<'1' || t[0]>'9') valide = false;

     while (valide && t[i]!='\0')
     {
         if (t[i]>='0' && t[i]<='9') { e = 10*e + (t[i]-'0'); i++; }
         else valide = false;
     }
     return valide;
 }




/** retourne un tableau de donnees
* fonction qui retourne les donnees de toutes les geodes selon le type choisie qui est passé en parametre
* sous forme d'un tableau qui sera ensuite stocker en tant que table_geode_donnees
*/
vector<string> UserCommands::showValueMetadata(osg::Object* osgObject, string type)
{

        osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (osgObject->getUserData() );
    vector <std::string> tab;
        if(metadata)
                {
                citygml::AttributesMap::const_iterator iterator;
                //trouver les donnees d'un type donne
         for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
                 // trouver les donnees de ce type
         { if (iterator->first.compare(type)==0)
                 tab.push_back(iterator->second);
       }
         }
                return tab ;
}

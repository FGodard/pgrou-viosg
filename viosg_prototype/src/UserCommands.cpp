/*
* UserCommands.cpp
*
* Created on: 22 nov. 2013
* Modified on: dec. 2013
* Author: blam , wafa
*
*/

#include "UserCommands.h"

//#include <algorithm>
#include <Geode>
//#include <iostream>
#include <map>
#include <ref_ptr>
#include <utility>
#include <vector>
#include<sstream>

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
        std::vector<std::string> tab;
        if(command.compare("help")==0){showHelp();return;}
        if(command.compare("showAll")==0) {showAllMetadata();return;}
        if(command.compare("showAllType")==0) {showAllTypeMetadata();return;}
        if(command.compare("showAllTypeMetadata")==0) {tab=showType();return;}
        if(command.compare("Valeur")==0) {printvaleur();return;}
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
std::vector<std::string> UserCommands::showType(){
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
             //si c'est un nouveau type on doit l'ajouter à la fin du tableau globale
             if(!find){ table_globale.push_back(table_geode[j]);}
     }
   }
     //affichage du tableau globale contenant tous les types possibles
         for(unsigned int k=0;k<table_globale.size();k++)
             {cout<<table_globale[k]<<endl;}
         cout<<table_globale.size()<<endl;
         return table_globale;
}
/**************************************************************************************************************/
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
    cout<<valide<<"valide";
    return valide;
}
/****************************************************************************************************/
vector<string>UserCommands::showValues(){
      //string type ="name";
       string type ="yearOfConstruction";
        GeodeFinder geodeFinder;
                vector <std::string>  table_donne ;
                vector<int>table_final;
                root->accept(geodeFinder);
                vector<osg::Geode*> geodes=geodeFinder.getNodeList();
       cout<<"Affiche les valeurs possibles pour chaque types"<<endl;
       //parcours de toutes les valeurs pour un type
       cout<< "Le type: "<<type<<"peut avoir comme valeur"<<endl;
      //  cout<<"**********************************************************"<<endl;

        cout<<geodes.size()<<endl;

        for(unsigned int p=0;p<geodes.size();p++){
        //	cout<<p<<endl;
        	vector <std::string> table_valeur_geode ;
             //creation de table contenant les valeurs à chaque fois
             table_valeur_geode=showValueMetadata(geodes[p],type);
             //parcours de la table de chaque geode pour verifier si cette valeur existe deja ou non
             table_donne=testCommand(table_valeur_geode, table_donne);
         }
        cout<<"**********************************************************"<<endl;

        for(unsigned int k=0;k<table_donne.size();k++)
                                 {cout<<table_donne[k]<<endl;}

        return table_donne;}
/****************************************************************************************************************/
/**
 *
 *procedure qui appelle la fonction sho wvalue et teste si le retour est numerique elle fait le tri
 */
void UserCommands::printvaleur(){
	vector<int> table2;
vector<string> table1=showValues();
bool b=NumericOrNot(table1);
if(b==true)
	table2=PrintNumericValuesTri(table1);
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
              cout<<ib;
              if(ib==false)
                      {test=false;
                      break;}
       }
      return test;
 }
/*********************************************************************************************/
 /**
  * tri les donnes dans le cas ou les donnees sont de type numerique
  */
 vector<int>UserCommands::PrintNumericValuesTri(vector<string> table_donne)
 {
	 vector<int> table_final;
	table_final=triTableau(table_donne, table_donne.size());
   //affichage du tableau globale contenant tous les valeurs possibles de ce type
       for(unsigned int k=0;k<table_final.size();k++)
           { cout<<table_final[k]<<endl;}
       return table_final;
 }


/*********************************************************************************************************/
/*
* fonction prend en parametre deux tableau
* donne en resultat un tableau global contenant tout les elements
* */
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
                  //si c'est une nouvelle valeur on doit l'ajouter à la fin du tableau de donnees
                  if(!find){ table_tot.push_back(table_gde[j]);}
         }
         return table_tot;
}
/******************************************************************************************************************/
/*
* fonction tri dans le cas des valeurs numeriques
*/
std::vector<int>UserCommands::triTableau( vector <string> tab,int l){
        vector<int> tabl;

        //changer le type du tableau en tableau de int
        tabl=StringToInt(tab);
        cout<<"tableau changer en tableau de int "<<endl;
        for(int i = 0; i < l; i++) { // on va jusqu'a taille du tableau - 1
      for(int i = 0; i < l; i++) // car tab[3 + 1] = tab[4] n'est pas défini !
           if(tabl[i] > tabl[i+1])
            std::swap(tabl[i], tabl[i+1]);
}
        cout<<"tableau trié "<<endl;
 return tabl;
}
/******************************************************************************************************************/
/*
* Convertin d'un tableau de chaine de caractaire en tableau d'entier
*/
std::vector<int>UserCommands::StringToInt( vector <string> tab)
{ vector<int> tableau;
int x;
cout<<tab.size()<<endl;
        for(unsigned int i=0;i<tab.size();i++)
        {
                try {
                        string myStream = tab[i];
                        istringstream buffer(myStream);
                        buffer >> x;
                        tableau.push_back(x);
                } catch (exception& e) {
                        cout<<"type non numeric"<<endl;
                }
        }
        return tableau;
}
/*****************************************************************************************************************/
/** retourne un tableau de donnees
* fonction qui retourne les donnees de toutes les geodes selon le type choisie qui est passé en parametre
* sous forme d'un tableau qui sera ensuite stocker en tant que table_geode_donnees
*/
std::vector<std::string> UserCommands::showValueMetadata(osg::Object* osgObject, string type)
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

        }
}

/*****************************************************************************************************************/
/** retourne un tableau de type
* fonction qui retourne les types de donnees pour chaque geode
* sous forme d'un tableau qui sera ensuite stocker en tant que table_geode
*/
std::vector<std::string> UserCommands::showTypeMetadata(osg::Object* osgObject)
{        osg::ref_ptr<Metadata> metadata =dynamic_cast<Metadata*> (osgObject->getUserData() );
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


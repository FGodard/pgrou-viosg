/*
 * UserCommands.cpp
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#include "UserCommands.h"
using namespace std;

UserCommands::UserCommands(osg::ref_ptr<osg::Group> root){
	UserCommands::root=root;
}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}
	if(command.compare("legendYear")==0) {legend("yearOfConstruction");return;}
	if(command.compare("eraseLegend")==0) {eraseLegend();return;}

	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}


void UserCommands::showHelp(){
cout<<"Commands List:"<<endl;
cout<<"\t"<<"'help'"<<"\t"<<"Show commands list"<<endl;
cout<<"\t"<<"'showAll'"<<"\t"<<"Show all metadata stored on all geodes"<<endl;
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
			dynamic_cast<Metadata*> (osgObject->getUserData() );

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



int UserCommands::valueOfKey(osg::Object* osgObject, std::string key){
	osg::ref_ptr<Metadata> metadata =
			dynamic_cast<Metadata*> (osgObject->getUserData());

	if(metadata)
	{
		citygml::AttributesMap::const_iterator iterator;
		for (iterator=metadata->attributes.begin(); iterator != metadata->attributes.end();++iterator)
		{
			std::string test=iterator->first;
			std::string test2=iterator->second;
			int value = atoi(test2.c_str());
			if (test.compare(key)==0)
				return value;
		}

	}
}

//pour assigner les couleurs selon la valeur du type choisi en paramètre
void UserCommands::legend(std::string key){
int valMin;
int valMax;
int tableau[10];

GeodeFinder geodeFinder;
root->accept(geodeFinder);
vector<osg::Geode*> geodes=geodeFinder.getNodeList();

valMax = 0;
valMin = 2500;
for(unsigned int i=0;i<geodes.size();i++){
	int test = valueOfKey(geodes[i], key);
	if (test>valMax) valMax = test;
	if (test<valMin) valMin = test;}

int e = (int)((valMax-valMin+1)/10+1);

for(unsigned int j=0; j<10; j++)
tableau[j] = valMin + j*e;

	static const char* fragSource =
	"uniform vec4 color;"
	"void main()"
	"{"
	" gl_FragColor = color;"
	"}";

	osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
	f->setShaderSource( fragSource );

	osg::ref_ptr<osg::Program> rPrg = new osg::Program;
	rPrg->setName( "myShader" );
	rPrg->addShader( f.get() );

for(unsigned int i=0;i<geodes.size();i++){
	int valeur=valueOfKey(geodes[i],key);
		if((valeur >= tableau[0]) && (valeur < tableau[1])){
geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
               geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(0.0,0.0,255.0,1.0)));}//bleu
else if((valeur >= tableau[1]) && (valeur < tableau[2])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(191.0,62.0,255.0,1.0)));}//violet
else if((valeur >= tableau[2]) && (valeur < tableau[3])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(0.0,191.0,255.0,1.0)));}//azur
else if((valeur >= tableau[3]) && (valeur < tableau[4])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(1.0,0.0,0.0,1.0)));}//rouge
else if((valeur >= tableau[4]) && (valeur < tableau[5])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(255.0,105.0,180.0,1.0)));}//rose fonce
else if((valeur >= tableau[5]) && (valeur < tableau[6])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(255.0,182.0,193.0,1.0)));}//rose clair
else if((valeur >= tableau[6]) && (valeur < tableau[7])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(127.0,255.0,0.0,1.0)));}//vert clair
else if((valeur >= tableau[7]) && (valeur < tableau[8])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(0.0,100.0,0.0,1.0)));}//vert foncé
else if((valeur >= tableau[8]) && (valeur <= tableau[9])){
	geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(255.0,215.0,0.0,1.0)));}//jaune
else geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(139.0,131.0,134.0,1.0)));
	}
cout << "Les batiments construits entre " << tableau[0] <<" et" << tableau[1] << " sont en bleu"<<endl;
cout << "Les batiments construits entre " << tableau[1] <<" et" << tableau[2] << " sont en violet"<<endl;
cout << "Les batiments construits entre " << tableau[2] <<" et" << tableau[3] << " sont en azur"<<endl;
cout << "Les batiments construits entre " << tableau[3] <<" et" << tableau[4] << " sont en rouge"<<endl;
cout << "Les batiments construits entre " << tableau[4] <<" et" << tableau[5] << " sont en rose fonce"<<endl;
cout << "Les batiments construits entre " << tableau[5] <<" et" << tableau[6] << " sont en rose clair"<<endl;
cout << "Les batiments construits entre " << tableau[6] <<" et" << tableau[7] << " sont en vert clair"<<endl;
cout << "Les batiments construits entre " << tableau[7] <<" et" << tableau[8] << " sont en vert fonce"<<endl;
cout << "Les batiments construits entre " << tableau[8] <<" et" << tableau[9] << " sont en jaune"<<endl;
}

//pour effacer la legende, on colore toutes les geodes en gris
void UserCommands::eraseLegend(){
	GeodeFinder geodeFinder;
	root->accept(geodeFinder);
	vector<osg::Geode*> geodes=geodeFinder.getNodeList();

	static const char* fragSource =
		"uniform vec4 color;"
		"void main()"
		"{"
		" gl_FragColor = color;"
		"}";

		osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
		f->setShaderSource( fragSource );

		osg::ref_ptr<osg::Program> rPrg = new osg::Program;
		rPrg->setName( "myShader" );
		rPrg->addShader( f.get() );

	for(unsigned int i=0;i<geodes.size();i++){
		geodes[i]->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );
		geodes[i]->getOrCreateStateSet()->addUniform( new osg::Uniform("color",osg::Vec4(139.0,131.0,134.0,1.0)));}
}


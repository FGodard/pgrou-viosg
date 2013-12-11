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
	UserCommands::hudIndex = NULL;
}

/**
 * Execute la commande entrée dans le terminal
 */
void UserCommands::executeCommand(string command){
	if(command.compare("help")==0){showHelp();return;}
	if(command.compare("showAll")==0) {showAllMetadata();return;}
	if(command.compare("showLegend")==0) {showLegend();return;}
	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}


void UserCommands::showHelp(){
cout<<"Commands List:"<<endl;

cout<<"\t"<<"'help'"<<"\t"<<"Show commands list."<<endl;
cout<<"\t"<<"'showAll'"<<"\t"<<"Show all metadata stored on all geodes."<<endl;
cout<<"\t"<<"'showLegend'"<<"\t"<<"Displays the legend."<<endl;
cout<<"\t"<<"'hideLegend'"<<"\t"<<"Hides the legend."<<endl;

cout<<"\t"<<"'help'"<<"\t"<<"Show commands list"<<endl;
cout<<"\t"<<"'showAll'"<<"\t"<<"Show all metadata stored on all geodes"<<endl;
cout<<"Pointez dans le Viewer3D un objet et appuyez sur espace pour récupérer les métadonnées contenues"<<endl;

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
		//showOneMetadata(geodes[i]);
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

osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
osgText::Text* UserCommands::createText( const osg::Vec3& pos, const std::string& content, float size )
{
	osgText::Text* text = new osgText::Text;
	text->setDataVariance( osg::Object::DYNAMIC );
	text->setFont( g_font.get() );
	text->setCharacterSize( size );
	text->setAxisAlignment( osgText::TextBase::XY_PLANE );
	text->setPosition( pos );
	text->setText( content );
	return text;
}

void UserCommands::showOneMetadata(osg::Object* osgObject){
	//On récupère le userdata de l'object
		osg::ref_ptr<Metadata> metadata =
				dynamic_cast<Metadata*> (osgObject->getUserData() );

		if(metadata)
		{
			citygml::AttributesMap::const_iterator iterator=metadata->attributes.find("measuredHeight");
			if(iterator!=metadata->attributes.end()){//il a trouvé l'attribut
			cout<<"\t"<<iterator->first<<":"<<iterator->second<<endl;
			}
		}
}


osg::Drawable* UserCommands::createLegendPolygon(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, const osg::Vec4& color, osg::Image* image=NULL)
{
    // set up the Geometry.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(4);
    (*coords)[0] = corner;
    (*coords)[1] = corner+width;
    (*coords)[2] = corner+width+height;
    (*coords)[3] = corner+height;



    geom->setVertexArray(coords);

    osg::Vec3Array* norms = new osg::Vec3Array(1);
    (*norms)[0] = width^height;
    (*norms)[0].normalize();

    geom->setNormalArray(norms);

    osg::Vec2Array* tcoords = new osg::Vec2Array(4);
    (*tcoords)[0].set(0.0f,0.0f);
    (*tcoords)[1].set(1.0f,0.0f);
    (*tcoords)[2].set(1.0f,1.0f);
    (*tcoords)[3].set(0.0f,1.0f);
    geom->setTexCoordArray(0,tcoords);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

    if (image)
    {
        osg::StateSet* stateset = new osg::StateSet;
        osg::Texture2D* texture = new osg::Texture2D;
        texture->setImage(image);
        stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
        geom->setStateSet(stateset);
    }
    geom->getStateSet()->addUniform(new osg::Uniform("color",osg::Vec4(0.0,191.0,255.0,1.0));

    return geom;
}

osg::Camera* UserCommands::createHUDCamera() {
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	camera->setClearMask( GL_DEPTH_BUFFER_BIT );
	camera->setRenderOrder( osg::Camera::POST_RENDER );
	camera->setAllowEventFocus( false );
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1024, 0, 768) );
	camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF );

	// A geometry node for our HUD:
	osg::Geode* HUDGeode = new osg::Geode();

	// Set up geometry for the HUD and add it to the HUD
	osg::Geometry* HUDBackgroundGeometry = new osg::Geometry();

	osg::Vec3Array* HUDBackgroundVertices = new osg::Vec3Array;
	HUDBackgroundVertices->push_back( osg::Vec3( 0,    0,-1) );
	HUDBackgroundVertices->push_back( osg::Vec3(1024,  0,-1) );
	HUDBackgroundVertices->push_back( osg::Vec3(1024,200,-1) );
	HUDBackgroundVertices->push_back( osg::Vec3(   0,200,-1) );

	osg::DrawElementsUInt* HUDBackgroundIndices =
	  new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
	HUDBackgroundIndices->push_back(0);
	HUDBackgroundIndices->push_back(1);
	HUDBackgroundIndices->push_back(2);
	HUDBackgroundIndices->push_back(3);

	osg::Vec4Array* HUDcolors = new osg::Vec4Array;
	HUDcolors->push_back(osg::Vec4(0.1f,0.1f,0.1f,0.1f));

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(1.0f,0.0f);
	(*texcoords)[2].set(1.0f,1.0f);
	(*texcoords)[3].set(0.0f,1.0f);

	HUDBackgroundGeometry->setTexCoordArray(0,texcoords);
	osg::Texture2D* HUDTexture = new osg::Texture2D;
	HUDTexture->setDataVariance(osg::Object::DYNAMIC);
	osg::Vec3Array* HUDnormals = new osg::Vec3Array;
	HUDnormals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	HUDBackgroundGeometry->setNormalArray(HUDnormals);
	HUDBackgroundGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
	HUDBackgroundGeometry->addPrimitiveSet(HUDBackgroundIndices);
	HUDBackgroundGeometry->setVertexArray(HUDBackgroundVertices);
	HUDBackgroundGeometry->setColorArray(HUDcolors);
	HUDBackgroundGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	HUDGeode->addDrawable(HUDBackgroundGeometry);
	// Create and set up a state set using the texture from above:
	osg::StateSet* HUDStateSet = new osg::StateSet();
	HUDGeode->setStateSet(HUDStateSet);
	HUDStateSet->setTextureAttributeAndModes(0,HUDTexture,osg::StateAttribute::ON);

	// For this state set, turn blending on (so alpha texture looks right)
	HUDStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);

	// Disable depth testing so geometry is draw regardless of depth values
	// of geometry already draw.
	HUDStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	HUDStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );


	camera->addChild(HUDGeode);
	return camera.release();
}

/**
 * Ajoute un HUD qui contient la légende.
 */
void UserCommands::showLegend(){
	if (hudIndex == NULL) {
		//Creates colored figures and a text legend and places them in the HUD
		osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
		//TODO Bug incompréhensible d'affichage, il faut en créer deux
		textGeode->addDrawable(createText(osg::Vec3(20, 140, 0.0f), "Affichage des tests", 30.0f));
		textGeode->addDrawable(createText(osg::Vec3(20, 220, 0.0f), "Affichage des tests", 30.0f));
		for (int i = 0; i < 13; i++) {
			//Coordonnée verticale
			int b = 165 - 50*(i%4);
			//Coordonnée horizontale
			int a = 35 + 60*(i-i%4);
			textGeode->addDrawable(createLegendPolygon(osg::Vec3(a - 25, b, 0.0f), osg::Vec3(20, 0, 0.0f), osg::Vec3(0, 20, 0.0f), osg::Vec4(100, 100, 100, 1)));
			textGeode->addDrawable(createText(osg::Vec3(a, b, 0.0f), "Test", 20.0f));

		}



		osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera();
		hudCamera->addChild( textGeode.get() );
		hudCamera->setName("HUD");
		root->addChild(hudCamera);
		hudIndex = root->getChildIndex(hudCamera);
	}
	else {
		root->removeChild(hudIndex, 1);
		hudIndex = NULL;
	}
}


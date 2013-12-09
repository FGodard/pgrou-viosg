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
	if(command.compare("showLegend")==0) {showLegend();return;}
	//Si aucune commande trouvée
	cout<<"Type 'help' for commands list or close the osgViewer to close program"<<endl;
}


void UserCommands::showHelp(){
cout<<"Commands List:"<<endl;
cout<<"\t"<<"'help'"<<"\t"<<"Show commands list."<<endl;
cout<<"\t"<<"'showAll'"<<"\t"<<"Show all metadata stored on all geodes."<<endl;
cout<<"\t"<<"'showLegend'"<<"\t"<<"Displays the legend."<<endl;
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

osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");
osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setDataVariance( osg::Object::DYNAMIC );
	text->setFont( g_font.get() );
	text->setCharacterSize( size );
	text->setAxisAlignment( osgText::TextBase::XY_PLANE );
	text->setPosition( pos );
	text->setText( content );
	return text.release();
}

osg::Drawable* createLegendPolygon() {
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(8);
	(*vertices)[0].set( 0.0f, 0.0f, 0.0f );
	(*vertices)[1].set( 3.0f, 0.0f, 0.0f );
	(*vertices)[2].set( 3.0f, 0.0f, 3.0f );
	(*vertices)[3].set( 0.0f, 0.0f, 3.0f );
	(*vertices)[4].set( 1.0f, 0.0f, 1.0f );
	(*vertices)[5].set( 2.0f, 0.0f, 1.0f );
	(*vertices)[6].set( 2.0f, 0.0f, 2.0f );
	(*vertices)[7].set( 1.0f, 0.0f, 2.0f );

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
	(*colors)[0].set( 1.0f, 1.0f, 0.0f, 1.0f );
	osg::ref_ptr<osg::Geometry> border = new osg::Geometry;
	border->setVertexArray( vertices.get() );
	border->setColorArray( colors.get() );
	border->setColorBinding( osg::Geometry::BIND_OVERALL );
	border->addPrimitiveSet( new osg::DrawArrays(
	GL_LINE_LOOP, 0, 4) );
	border->addPrimitiveSet( new osg::DrawArrays(
	GL_LINE_LOOP, 4, 4) );
	border->getOrCreateStateSet()->setAttribute(new osg::LineWidth(5.0f) );

	return border.get();
}

osg::Camera* createHUDCamera() {
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
	HUDcolors->push_back(osg::Vec4(0.8f,0.8f,0.8f,0.8f));

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(1.0f,0.0f);
	(*texcoords)[2].set(1.0f,1.0f);
	(*texcoords)[3].set(0.0f,1.0f);

	HUDBackgroundGeometry->setTexCoordArray(0,texcoords);
	osg::Texture2D* HUDTexture = new osg::Texture2D;
	HUDTexture->setDataVariance(osg::Object::DYNAMIC);
//	osg::Image* hudImage;
//	hudImage = osgDB::readImageFile("HUDBack2.tga");
//	HUDTexture->setImage(hudImage);
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
	HUDStateSet->
	  setTextureAttributeAndModes(0,HUDTexture,osg::StateAttribute::ON);

	// For this state set, turn blending on (so alpha texture looks right)
	HUDStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);

	// Disable depth testing so geometry is draw regardless of depth values
	// of geometry already draw.
	HUDStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	HUDStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	// Need to make sure this geometry is draw last. RenderBins are handled
	// in numerical order so set bin number to 11
	HUDStateSet->setRenderBinDetails( 11, "RenderBin");

	camera->addChild(HUDGeode);
	return camera.release();
}

/**
 * Ajoute un HUD qui contient la légende.
 */
void UserCommands::showLegend(){

	//Create random texts and place them in the HUD
	osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
	for (int i = 0; i < 5; ++i) {
		textGeode->addDrawable(createText(osg::Vec3(rand() % 100, rand() % 100, 0.0f), "Test", 20.0f));
		//textGeode->addDrawable(createLegendPolygon());
	}

	osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera();
	hudCamera->addChild( textGeode.get() );
	root->addChild(hudCamera);


}

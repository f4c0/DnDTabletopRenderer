#include "AppDemarrage.h"

AppDemarrage::AppDemarrage()
:mRoot(NULL),
mWindow(NULL),
mSceneMgr(NULL),
mCamera(NULL),
mFrameListener(NULL),
m_battleground(NULL)
{
}

AppDemarrage::~AppDemarrage()
{
    OGRE_DELETE m_battleground;
    OGRE_DELETE mRoot;
}

bool
AppDemarrage::start(const Ogre::String& pluginFile, const Ogre::String& ogreFile,
                         const Ogre::String& logFile, const Ogre::String& resourceFile)
{
    mRoot = OGRE_NEW Ogre::Root(pluginFile, ogreFile, logFile);
    Ogre::ConfigFile configFile;
    configFile.load(resourceFile);

    Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }

    //if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
    if(!mRoot->showConfigDialog())
    {
        return false;
    }

    mWindow = mRoot->initialise(true, APP_WIN_NAME);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "Mon Scene Manager");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);


    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(10*1.5f, 1*1.5f, 10*1.5f));
    pointLight->setDiffuseColour(0.7, 0.7, 0.7);
    pointLight->setSpecularColour(0.7, 0.7, 0.7);

    mCamera = mSceneMgr->createCamera("MainCam");
    #define MYTILESIZE 1.5f
    mCamera->setPosition(5*MYTILESIZE,8,10*MYTILESIZE);
    //mCamera->setDirection(Ogre::Vector3::)
    //mCamera->setOrientation();
    mCamera->lookAt(5*MYTILESIZE,0,10*MYTILESIZE);
    mCamera->setDirection(0,0,0);

    mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(15);

    mCamera->setOrthoWindow(20 * MYTILESIZE, 20 * MYTILESIZE / 2.7f);


    //mCamera->setOrthoWindow(15.f, 6.f);//50 * 20

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));



    //mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    //mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    std::cout << "vp->getActualWidth(): " << vp->getActualWidth() << "\n";
    std::cout << "vp->getActualHeight(): " << vp->getActualHeight() << std::endl;
    //mCamera->setOrthoWindow(Ogre::Real(vp->getActualWidth()), Ogre::Real(vp->getActualHeight()));

    //mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

    mFrameListener= new InputListener(mCamera);
    mRoot->addFrameListener(mFrameListener);

//    Ogre::Entity *ent = mSceneMgr->createEntity("pinguin", "penguin.mesh");
//    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//    node->attachObject(ent);

//    Ogre::Plane plan(Ogre::Vector3::UNIT_Y, 0);
//    Ogre::MeshManager::getSingleton().createPlane("Sol", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//                                            plan, 5, 5, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
//                                            Ogre::Entity* ent= mSceneMgr->createEntity("EntiteSol", "Sol");
//    Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//    node->attachObject(ent);
//    ent->setMaterialName("Metal_Embossed");

//    Ogre::Plane plane;
//	plane.normal = Ogre::Vector3::UNIT_Y;
//	plane.d = 0;
//
//	//Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 150.0f, 150.0f, 10, 10, true, 1, 10.0f, 10.0f, Ogre::Vector3::UNIT_Z);
//	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 15.0f, 15.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Z);
//	Ogre::Entity* planeEnt = mSceneMgr->createEntity("plane", "floor");
//	planeEnt->setMaterialName("Examples/BumpyMetal");
//	planeEnt->setCastShadows(false);
//	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEnt);

//    Ogre::ViewportGrid myGrid(mSceneMgr, vp);
//    myGrid.setColour(Ogre::ColourValue (1,1,1));
//    myGrid.setRenderLayer(Ogre::ViewportGrid::RL_INFRONT);
//    myGrid.setDivision(1.5f);
//    //myGrid.setRenderScale(true);
//    myGrid.enable();




    #define SIZE_EDGE 1.5f
    #define COLONNE 4
    #define LIGNE 5
//    MaterialPtr pMat = MaterialManager::getSingleton().create("test", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//    Pass* textPass = pMat->getTechnique(0)->getPass(0);
//    textPass->createTextureUnitState( NomImage );
//    textPass->setSceneBlending(SBT_TRANSPARENT_ALPHA);

//Ogre::Terrain terrain(mSceneMgr);
    Battleground bg("plop", mSceneMgr);
    bg.load("battlegnd.png");
//mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
//	Ogre::ManualObject* table = mSceneMgr->createManualObject("table");
//
//	table->begin("Tile/ClearWaterStream", Ogre::RenderOperation::OT_TRIANGLE_LIST, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//	 for(unsigned int i = 0; i < (LIGNE + 1); i++)
//    {
//        for(unsigned int j = 0; j < (COLONNE+1); j++)
//        {
//            table->position(j*SIZE_EDGE, 0, i*SIZE_EDGE);
//            table->textureCoord(i,j);
//            std::cout << Ogre::Vector3(j*SIZE_EDGE, 0, i*SIZE_EDGE) << " ";
//        }
//    }
//    table->quad(1,16,19,4);
//	table->end();
//	// specify the material (by name) and rendering type
//    table->begin("Tile/Minitiles", Ogre::RenderOperation::OT_TRIANGLE_LIST, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//  //   define start and end point
//
//
//    for(unsigned int i = 0; i < (LIGNE + 1); i++)
//    {
//        for(unsigned int j = 0; j < (COLONNE+1); j++)
//        {
//            table->position(j*SIZE_EDGE, 0, i*SIZE_EDGE);
//            table->textureCoord(i,j);
//            std::cout << Ogre::Vector3(j*SIZE_EDGE, 0, i*SIZE_EDGE) << " ";
//        }
//    }
//    table->quad(0,15,16,1);
//    table->quad(15,25,29,19);
//    //std::cout << std::endl;
////
////    int cornerTL = 0;
////    for(unsigned int i = 0; i < COLONNE*LIGNE; i++)
////    {
////        //table->quad(cornerTL,cornerTL +1,cornerTL+COLONNE+2,cornerTL+COLONNE+1);0132 0231
////        //std::cout << "[ " << cornerTL << "," << (cornerTL +1) << "," << (cornerTL+COLONNE+2) << "," << (cornerTL+COLONNE+1) << "] ; ";
////
////        table->quad(cornerTL,cornerTL+COLONNE+1,cornerTL+COLONNE+2,cornerTL +1);
////        std::cout << "[ " << cornerTL << "," << (cornerTL+COLONNE+1) << "," << (cornerTL+COLONNE+2) << "," << (cornerTL+1) << "] ; ";
////
////        if(((cornerTL + 2) % (COLONNE+1)) == 0)
////        {
////            cornerTL += 2;
////        }else
////        {
////            cornerTL ++;
////        }
////    }
////     std::cout << std::endl;
////
//
//    //table->quad(10,15,16,11);
////    table->quad(1,2,5,4);
////    table->quad(1,2,5,4);
////    table->quad(1,2,5,4);
////    table->quad(1,2,5,4);
////    table->quad(1,2,5,4);
////    table->quad(1,2,5,4);
////
////
////    // tell Ogre, your definition has finished
//    table->end();
//
//    // add ManualObject to the RootSceneNode (so it will be visible)
//    mSceneMgr->getRootSceneNode()->attachObject(table);

//   m_battleground = OGRE_NEW Battleground(mSceneMgr, "battlegnd.png");
//m_battleground->prepare(m_battleground->m_mapInData);
//m_battleground->load();
    mWindow->setDeactivateOnFocusChange(false);


    while(true)
    {
        Ogre::WindowEventUtilities::messagePump();

        if(mWindow->isClosed())
        {
            return false;
        }

        if(!mRoot->renderOneFrame())
        {
            return false;
        }
    }

    return true;
}

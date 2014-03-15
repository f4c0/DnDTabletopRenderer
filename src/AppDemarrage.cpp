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
    if(m_battleground != NULL)
    {
       delete m_battleground;
    }

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

                if(secName == "TileResourceID")
                {
                    resourceIdMapper[Ogre::StringConverter::parseInt(typeName)] = archName;
                }else
                {
                    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
                }

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


//    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
//    pointLight->setType(Ogre::Light::LT_POINT);
//    pointLight->setPosition(Ogre::Vector3(10*1.5f, 1*1.5f, 10*1.5f));
//    pointLight->setDiffuseColour(0.7, 0.7, 0.7);
//    pointLight->setSpecularColour(0.7, 0.7, 0.7);

    mCamera = mSceneMgr->createCamera(APP_MAIN_CAM_NAME);
    #define MYTILESIZE 1.5f
    mCamera->setPosition(5*MYTILESIZE,8,10*MYTILESIZE);
    //mCamera->setDirection(Ogre::Vector3::)
    //mCamera->setOrientation();
    mCamera->lookAt(5*MYTILESIZE,0,10*MYTILESIZE);
    mCamera->setDirection(0,0,0);

    mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    mCamera->setNearClipDistance(0.5);
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

    //m_battleground = new Battleground("plop", mSceneMgr, &resourceIdMapper);
    //m_battleground->load("battlegnd.png");

    //m_battleground Battleground(const xml_node &xmlBattlefieldNode, Ogre::SceneManager* mSceneMgr, std::map<int, Ogre::String>* resourceIdMapper);

    mFrameListener= new InputListener(mSceneMgr, &resourceIdMapper, m_battleground);
    mRoot->addFrameListener(mFrameListener);

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

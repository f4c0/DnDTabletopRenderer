#ifndef APPDEMARRAGE_H
#define APPDEMARRAGE_H

#include <Ogre.h>
#include <map>
#include "InputListener.h"
#include "Battleground.h"

#define APP_WIN_NAME "DnD Tabletop v0.2.0 by Faco"

class AppDemarrage
{
    public:
        AppDemarrage();
        ~AppDemarrage();

        bool start( const Ogre::String& pluginFile,
                    const Ogre::String& ogreFile,
                    const Ogre::String& logFile,
                    const Ogre::String& resourceFile);


    private:
        Ogre::Root* mRoot;
        Ogre::RenderWindow* mWindow;
        Ogre::SceneManager* mSceneMgr;
        Ogre::Camera* mCamera;
        std::map<int, Ogre::String> resourceIdMapper;


        InputListener* mFrameListener;
        Battleground* m_battleground;
};

#endif // APPDEMARRAGE_H

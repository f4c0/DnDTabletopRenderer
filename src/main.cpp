#define BOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN

#include "AppDemarrage.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    AppDemarrage app;

    try {
        #ifdef DEBUG
            app.start("plugins_d.cfg", "ogre.cfg", "Ogre.log", "resources_d.cfg");
        #else
            app.start("plugins.cfg", "ogre.cfg", "Ogre.log", "resources.cfg");
        #endif
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
    }




    return 0;
}

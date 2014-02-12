#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <Ogre.h>
#include <boost/thread/thread.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
#include <sstream>
#include "NetworkManager.h"

#define DISPLAY_RCV_MSG(msg) std::cout<<"[RCV MSG]> "<<msg<<"\n"

class InputListener : public Ogre::FrameListener
{
public:
    InputListener(Ogre::Camera* camera, const int port = 33333);
    virtual ~InputListener(void);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

private:
    boost::thread* th_networkManager;
    NetworkManager m_nm;

    Ogre::Camera* m_camera;



};
#endif // INPUTLISTENER_H

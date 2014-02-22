#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <Ogre.h>
#include <boost/thread/thread.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
#include <sstream>
#include <pugixml/pugixml.hpp>
#include "Battleground.h"
#include "NetworkManager.h"

#define CMD_SIZE 4
#define CMD_BG_TRANSFERT "[A0]"
#define CMD_SHUTDOWN "[A1]"
#define CMD_SIZEWIN "sizewin"
#define CMD_CAM_MOVE "move"
#define CMD_WIRE "wire"
#define CMD_GRID "grid"
#define DISPLAY_RCV_MSG(msg) std::cout<<"[RCV CMD]> "<<msg<<"\n"

using namespace pugi;

class InputListener : public Ogre::FrameListener
{
public:
    InputListener(Ogre::SceneManager* sceneMgr, Battleground* battleground, const int port = 33333);
    virtual ~InputListener(void);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

private:
    boost::thread* th_networkManager;
    NetworkManager m_nm;
    Battleground* m_battleground;

    Ogre::SceneManager* m_sceneMgr;

    std::stringstream m_ssCurrentStringFlow;
    std::string m_previousState;

    void parse_CMD_BG_TRANSFERT_Begin(const std::string chunk);
    void parse_CMD_BG_TRANSFERT_fill(const std::string chunk);
    void parse_CMD_BG_TRANSFERT_end(void);


};
#endif // INPUTLISTENER_H

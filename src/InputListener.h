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

#define APP_MAIN_CAM_NAME "MainCam"

#define CMD_SIZE 4
#define CMD_TRANSFERT_BATTLEFIELD   "[A0]"
#define CMD_SHUTDOWN                "[A1]"
#define CMD_MOVE_CAM_LEFT           "[M1]"
#define CMD_MOVE_CAM_RIGHT          "[M2]"
#define CMD_MOVE_CAM_UP             "[M3]"
#define CMD_MOVE_CAM_DOWN           "[M4]"
#define CMD_PROJ_ORTHO              "[C1]"
#define CMD_PROJ_PERSPECTIVE        "[C2]"
#define CMD_POLYGON_MODE_SOLID      "[P1]"
#define CMD_POLYGON_MODE_WIRE       "[P2]"

#define CMD_SIZEWIN "sizewin"
#define CMD_CAM_MOVE "move"
#define CMD_WIRE "wire"
#define CMD_GRID "grid"
#define DISPLAY_RCV_MSG(msg) std::cout<<"[RCV CMD]> "<<msg<<"\n"

using namespace pugi;

class InputListener : public Ogre::FrameListener
{
public:
    InputListener(Ogre::SceneManager* sceneMgr, std::map<int, Ogre::String>* resourceIdMapper, Battleground* battleground, const int port = 33333);
    virtual ~InputListener(void);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

private:
    boost::thread* th_networkManager;
    NetworkManager m_nm;
    Battleground* m_battleground;

    Ogre::SceneManager* m_sceneMgr;
    std::map<int, Ogre::String>* m_resourceIdMapper;

    std::stringstream m_ssCurrentStringFlow;
    std::string m_previousState;

    void parse_CMD_BG_TRANSFERT_Begin(const std::string chunk);
    void parse_CMD_BG_TRANSFERT_fill(const std::string chunk);
    void parse_CMD_BG_TRANSFERT_end(void);


};
#endif // INPUTLISTENER_H

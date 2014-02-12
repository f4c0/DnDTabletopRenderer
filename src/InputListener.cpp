#include "InputListener.h"

InputListener::InputListener(Ogre::Camera* camera, const int port)
:FrameListener(),
m_nm(port),
m_camera(camera)
{
    th_networkManager = new boost::thread(&NetworkManager::listenData, &m_nm);
}

InputListener::~InputListener(void)
{
    m_nm.kill();
    th_networkManager->join();
    delete th_networkManager;
}

bool
InputListener::frameEnded(const Ogre::FrameEvent& evt)
{
    bool b_continue = true;
    std::string msg = m_nm.messageReceived();
    std::vector<std::string> tokens;

    if(msg == NM_QUEUE_EMPTY)
    {
        // Do nothing
    }else
    {
        DISPLAY_RCV_MSG(msg);

        boost::tokenizer<> tok(msg);
        for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
            tokens.push_back(*beg);
        }

        if(tokens[0] == MSG_SHUTDOWN)
        {
            b_continue = false;
        }else if(tokens[0] == MSG_CAM_MOVE)
        {

            if(tokens.size() == 3)
            {
                std::stringstream ss;
                Ogre::Real moveValue;

                if(tokens[1] == "left")
                {
                    ss.str(tokens[2]);
                    ss >> moveValue;
                    m_camera->move(Ogre::Vector3(-moveValue,0,0));
                }else if(tokens[1] == "right")
                {
                    ss.str(tokens[2]);
                    ss >> moveValue;
                    m_camera->move(Ogre::Vector3(moveValue,0,0));
                }else if(tokens[1] == "top")
                {
                    ss.str(tokens[2]);
                    ss >> moveValue;
                    m_camera->move(Ogre::Vector3(0,0,-moveValue));
                }else if(tokens[1] == "bottom")
                {
                    ss.str(tokens[2]);
                    ss >> moveValue;
                    m_camera->move(Ogre::Vector3(0,0,moveValue));
                }else
                {
                    std::cerr << "Unknown parameter: " << tokens[1] << "\n";
                }
            }else
            {
                std::cerr << "wrong size: " << tokens.size() << "\n";
            }


        }else if(tokens[0] == MSG_SIZEWIN)
        {
            if(tokens.size() == 3)
            {
                if(tokens[1] == "w")
                {
                    std::stringstream ss;
                    Ogre::Real sizeValue;
                    ss.str(tokens[2]);
                    ss >> sizeValue;

                    const Ogre::Real resizedWidth = m_camera->getOrthoWindowWidth() + sizeValue;
                    m_camera->setOrthoWindowWidth(resizedWidth);
                }else if(tokens[1] == "h")
                {
                    std::stringstream ss;
                    Ogre::Real sizeValue;
                    ss.str(tokens[2]);
                    ss >> sizeValue;

                    const Ogre::Real resizedHeight = m_camera->getOrthoWindowHeight() + sizeValue;
                    m_camera->setOrthoWindowHeight(resizedHeight);
                }else
                {
                    std::cerr << "Unknown parameter: " << tokens[1] << "\n";
                }

            }else
            {
                std::cerr << "wrong size: " << tokens.size() << "\n";
            }

        }else if(tokens[0] == MSG_WIRE)
        {
            if(tokens.size() == 2)
            {
                if(tokens[1] == "on")
                {
                    m_camera->setPolygonMode(Ogre::PM_WIREFRAME);
                }else if(tokens[1] == "off")
                {
                    m_camera->setPolygonMode(Ogre::PM_SOLID);
                }else
                {
                    std::cerr << "Unknown parameter: " << tokens[1] << "\n";
                }

            }else
            {
                std::cerr << "wrong size: " << tokens.size() << "\n";
            }

        }else if(tokens[0] == MSG_GRID)
        {
            if(tokens.size() == 2)
            {
                if(tokens[1] == "on")
                {
                    m_camera->getSceneManager()->getManualObject("Grid")->setVisible(true);
                }else if(tokens[1] == "off")
                {
                    m_camera->getSceneManager()->getManualObject("Grid")->setVisible(false);
                }else
                {
                    std::cerr << "Unknown parameter: " << tokens[1] << "\n";
                }

            }else
            {
                std::cerr << "wrong size: " << tokens.size() << "\n";
            }

        }else
        {
            std::cerr << "Unknown message !\n";
        }
    }



    return b_continue;
}

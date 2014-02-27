#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H

#include <Ogre.h>
#include <cmath>
#include <map>
#include <sstream>
#include <pugixml/pugixml.hpp>

#include "Grid.h"

#define BATTLEGND_TILE_SIZE 1.5f
#define BATTLEGND_NODE_NAME "bf_node"

using namespace pugi;

class Battleground
{
    public:
        //Battleground(const Ogre::String &name, Ogre::SceneManager* mSceneMgr, std::map<int, Ogre::String>* resourceIdMapper);
        Battleground(const xml_node &xmlBattlefieldNode, Ogre::SceneManager* mSceneMgr, std::map<int, Ogre::String>* resourceIdMapper);
        //void load(const Ogre::String &fileName);

        //void load(const xml_node &xmlBattlefieldNode);
        virtual ~Battleground();
    protected:
    private:
        Ogre::SceneManager* m_SceneMgr;
        Ogre::SceneNode*    m_bfNode;
        Ogre::ManualObject* m_battleGnd;
        Ogre::uint32        m_width;
        Ogre::uint32        m_height;
        Grid* m_grid;
        std::map<int, Ogre::String>* m_resourceIdMapper;

        Ogre::String getMaterialById(const int id) const;
        void createWall(const std::string wallId, const Ogre::Vector3 p1, const Ogre::Vector3 p2);

};

#endif // BATTLEGROUND_H

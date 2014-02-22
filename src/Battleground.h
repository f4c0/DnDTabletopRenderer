#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H

#include <Ogre.h>
#include <map>
#include <sstream>
#include <pugixml/pugixml.hpp>

#include "Grid.h"

#define BATTLEGND_TILE_SIZE 1.5f

using namespace pugi;

class Battleground
{
    public:
        Battleground(const Ogre::String &name, Ogre::SceneManager* mSceneMgr);
        Battleground(const xml_node &xmlBattlefieldNode, Ogre::SceneManager* mSceneMgr);
        void load(const Ogre::String &fileName);

        void load(const xml_node &xmlBattlefieldNode);
        virtual ~Battleground();
    protected:
    private:
        Ogre::SceneManager* m_SceneMgr;
        Ogre::ManualObject* m_battleGnd;
        Ogre::uint32        m_width;
        Ogre::uint32        m_height;

        Grid* m_grid;

        Ogre::String getMaterialById(const unsigned long long id) const;

};

#endif // BATTLEGROUND_H

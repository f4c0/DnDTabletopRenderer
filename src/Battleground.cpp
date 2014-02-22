#include "Battleground.h"

Battleground::Battleground(const Ogre::String &name, Ogre::SceneManager* mSceneMgr)
:m_SceneMgr(mSceneMgr),
m_battleGnd(NULL),
m_width(0),
m_height(0),
m_grid(NULL)
{
    m_grid = new Grid(mSceneMgr);
    m_battleGnd = mSceneMgr->createManualObject(name);
    m_battleGnd->setCastShadows(false);
}

Battleground::Battleground(const xml_node &xmlBattlefieldNode, Ogre::SceneManager* mSceneMgr)
:m_SceneMgr(mSceneMgr),
m_battleGnd(NULL),
m_width(0),
m_height(0),
m_grid(NULL)
{
    m_grid = new Grid(mSceneMgr);
    m_battleGnd = mSceneMgr->createManualObject("Battleground");
    m_battleGnd->setCastShadows(false);

    std::map<unsigned long long, std::vector<unsigned int> > indexRoomMap;
    std::map<unsigned long long, std::vector<unsigned int> >::iterator indexRoomMapItr;
    std::vector<unsigned int>::const_iterator tilesVectItr;

    unsigned int tileIndex;
    unsigned long long ResourceId;

    xml_node TilesNode = xmlBattlefieldNode.child("Tiles");

    std::istringstream (TilesNode.attribute("width").value()) >> m_width;
    std::istringstream (TilesNode.attribute("height").value()) >> m_height;

    tileIndex = 0L;
    for (xml_node tileNode = TilesNode.first_child(); tileNode; tileNode = tileNode.next_sibling())
    {
        std::istringstream (tileNode.attribute("id").value()) >> ResourceId;

        //TEST !!!!
        if(ResourceId == 24310463750)
        {
            ResourceId = 1;
        }else
        {
            ResourceId = 255;
        }


        indexRoomMap[ResourceId].push_back(tileIndex);
        tileIndex ++;
    }

    Ogre::Vector2 vectTopLeft;
    Ogre::Vector2 vectBottomLeft;
    Ogre::Vector2 vectTopRight;
    Ogre::Vector2 vectBottomRight;
    for(indexRoomMapItr = indexRoomMap.begin(); indexRoomMapItr != indexRoomMap.end(); ++indexRoomMapItr)
    {
        std::sort(indexRoomMapItr->second.begin(), indexRoomMapItr->second.end());

        const Ogre::String materialName = getMaterialById(indexRoomMapItr->first);
        m_battleGnd->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST , Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        for(tilesVectItr = indexRoomMapItr->second.begin(); tilesVectItr != indexRoomMapItr->second.end(); ++tilesVectItr)
        {
            vectTopLeft.x = (*tilesVectItr % m_width)  * BATTLEGND_TILE_SIZE;
            vectTopLeft.y = (*tilesVectItr / m_width) * BATTLEGND_TILE_SIZE;

            vectBottomLeft.x = vectTopLeft.x;
            vectBottomLeft.y = vectTopLeft.y + BATTLEGND_TILE_SIZE;

            vectTopRight.x = vectTopLeft.x + BATTLEGND_TILE_SIZE;
            vectTopRight.y = vectTopLeft.y;

            vectBottomRight.x = vectTopLeft.x + BATTLEGND_TILE_SIZE;
            vectBottomRight.y = vectTopLeft.y + BATTLEGND_TILE_SIZE;

            m_battleGnd->position(vectTopLeft.x, 0, vectTopLeft.y);
            m_battleGnd->textureCoord(0,0);
            m_battleGnd->position(vectBottomLeft.x, 0, vectBottomLeft.y);
            m_battleGnd->textureCoord(0,1);
            m_battleGnd->position(vectTopRight.x, 0, vectTopRight.y);
            m_battleGnd->textureCoord(1,0);
            m_battleGnd->position(vectTopRight.x, 0, vectTopRight.y);
            m_battleGnd->textureCoord(1,0);
            m_battleGnd->position(vectBottomLeft.x, 0, vectBottomLeft.y);
            m_battleGnd->textureCoord(0,1);
            m_battleGnd->position(vectBottomRight.x, 0, vectBottomRight.y);
            m_battleGnd->textureCoord(1,1);

        }
        m_battleGnd->end();
    }

    m_battleGnd->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1 - 1);
    m_SceneMgr->getRootSceneNode()->attachObject(m_battleGnd);

    m_grid->create(m_width, m_height, BATTLEGND_TILE_SIZE);

}

Battleground::~Battleground()
{
    if(m_grid != NULL)
    {
        delete m_grid;
    }

    if(m_battleGnd != NULL)
    {
        m_SceneMgr->getRootSceneNode()->detachObject(m_battleGnd);
        m_SceneMgr->destroyManualObject(m_battleGnd);
    }

}

void
Battleground::load(const Ogre::String &fileName)
{
    Ogre::Image img;
    std::map<unsigned int, std::vector<unsigned int> > indexRoomMap;
    unsigned int tileIndex;
    img.load(fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    m_width = img.getWidth();
    m_height = img.getHeight();
    //unsigned int roomId = 0;
    std::map<unsigned int, std::vector<unsigned int> >::iterator mapItr;
    std::vector<unsigned int>::const_iterator vectItr;

    for(unsigned int roomId = 0; roomId <= 255; roomId++)
    {
        for(unsigned int y = 0; y < m_height; y++)
        {
            for(unsigned int x = 0; x < m_width; x++)
            {
                if(((img.getColourAt(x,y,0).getAsRGBA()) >> 24) == roomId)
                {
                    tileIndex = y * m_width + x;
                    indexRoomMap[roomId].push_back(tileIndex);
                }
            }
        }
    }

    Ogre::Vector2 vectTopLeft;
    Ogre::Vector2 vectBottomLeft;
    Ogre::Vector2 vectTopRight;
    Ogre::Vector2 vectBottomRight;
    for(mapItr = indexRoomMap.begin(); mapItr != indexRoomMap.end(); ++mapItr)
    {
        std::sort(mapItr->second.begin(), mapItr->second.end());

        const Ogre::String materialName = getMaterialById(mapItr->first);
        m_battleGnd->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST , Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        for(vectItr = mapItr->second.begin(); vectItr != mapItr->second.end(); ++vectItr)
        {
            vectTopLeft.x = (*vectItr % m_width)  * BATTLEGND_TILE_SIZE;
            vectTopLeft.y = (*vectItr / m_width) * BATTLEGND_TILE_SIZE;

            vectBottomLeft.x = vectTopLeft.x;
            vectBottomLeft.y = vectTopLeft.y + BATTLEGND_TILE_SIZE;

            vectTopRight.x = vectTopLeft.x + BATTLEGND_TILE_SIZE;
            vectTopRight.y = vectTopLeft.y;

            vectBottomRight.x = vectTopLeft.x + BATTLEGND_TILE_SIZE;
            vectBottomRight.y = vectTopLeft.y + BATTLEGND_TILE_SIZE;

            m_battleGnd->position(vectTopLeft.x, 0, vectTopLeft.y);
            m_battleGnd->textureCoord(0,0);
            m_battleGnd->position(vectBottomLeft.x, 0, vectBottomLeft.y);
            m_battleGnd->textureCoord(0,1);
            m_battleGnd->position(vectTopRight.x, 0, vectTopRight.y);
            m_battleGnd->textureCoord(1,0);
            m_battleGnd->position(vectTopRight.x, 0, vectTopRight.y);
            m_battleGnd->textureCoord(1,0);
            m_battleGnd->position(vectBottomLeft.x, 0, vectBottomLeft.y);
            m_battleGnd->textureCoord(0,1);
            m_battleGnd->position(vectBottomRight.x, 0, vectBottomRight.y);
            m_battleGnd->textureCoord(1,1);

        }
        m_battleGnd->end();
    }

    m_battleGnd->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1 - 1);
    m_SceneMgr->getRootSceneNode()->attachObject(m_battleGnd);

    m_grid->create(m_width, m_height, BATTLEGND_TILE_SIZE);
}

void
Battleground::load(const xml_node& xmlBattlefieldNode)
{

}


Ogre::String
Battleground::getMaterialById(const unsigned long long id) const
{
    if(id != 255)
    {
       return  "Tile/Minitiles";
    }else
    {
       return  "Tile/ClearMoltenStream";
    }
}

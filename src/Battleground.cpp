#include "Battleground.h"

Battleground::Battleground(const xml_node &xmlBattlefieldNode, Ogre::SceneManager* mSceneMgr, std::map<int, Ogre::String>* resourceIdMapper)
:m_SceneMgr(mSceneMgr),
m_bfNode(NULL),
m_battleGnd(NULL),
m_width(0),
m_height(0),
m_grid(NULL),
m_resourceIdMapper(resourceIdMapper)
{
    m_battleGnd = mSceneMgr->createManualObject("Battleground");
    m_battleGnd->setCastShadows(false);

    std::map<int, std::vector<unsigned int> > indexRoomMap;
    std::map<int, std::vector<unsigned int> >::iterator indexRoomMapItr;
    std::vector<unsigned int>::const_iterator tilesVectItr;

    unsigned int tileIndex;
    int ResourceId;

    xml_node TilesNode = xmlBattlefieldNode.child("Tiles");

    std::istringstream (TilesNode.attribute("width").value()) >> m_width;
    std::istringstream (TilesNode.attribute("height").value()) >> m_height;

    tileIndex = 0;
    for (xml_node tileNode = TilesNode.first_child(); tileNode; tileNode = tileNode.next_sibling())
    {
        std::istringstream (tileNode.attribute("id").value()) >> ResourceId;

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

    m_bfNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(BATTLEGND_NODE_NAME, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
    m_bfNode->attachObject(m_battleGnd);
    m_grid = new Grid(m_bfNode, m_SceneMgr, m_width, m_height, BATTLEGND_TILE_SIZE);

    for(xml_node movableObjNode = xmlBattlefieldNode.child("MovableObject"); movableObjNode; movableObjNode = movableObjNode.next_sibling("MovableObject"))
    {
        Ogre::Real objX;
        Ogre::Real objY;
        int refId;
        int resId;

        std::istringstream (movableObjNode.attribute("resourceId").value()) >> resId;

        std::istringstream (movableObjNode.attribute("x").value()) >> objX;
        std::istringstream (movableObjNode.attribute("y").value()) >> objY;
        std::istringstream (movableObjNode.attribute("refId").value()) >> refId;

        std::cout << "resourceId: " << movableObjNode.attribute("resourceId").value() << "\n";
        std::cout << "refId: " << movableObjNode.attribute("refId").value() << "\n";
        std::cout << "x: " << objX << "\n";
        std::cout << "y: " << objY << "\n";

        Ogre::Vector3 objPos(objX, 0, objY);


       std::string entityName = "entity_mvObj_";
        entityName.append(movableObjNode.attribute("refId").value());

        std::string nodeName = "node_mvObj_";
        nodeName.append(movableObjNode.attribute("refId").value());

        Ogre::Entity*       objEntity = m_SceneMgr->createEntity(entityName, "Barrel.mesh");
        Ogre::SceneNode*    objNode = m_bfNode->createChildSceneNode(nodeName, objPos, Ogre::Quaternion::IDENTITY);
        objNode->scale(0.25, 0.25, 0.25);
        objNode->attachObject(objEntity);


        if(resId == 5)
        {
            std::string lightName = "light_mvObj_";
            lightName.append(movableObjNode.attribute("refId").value());

            Ogre::Light* pointLight = mSceneMgr->createLight(lightName);
            pointLight->setType(Ogre::Light::LT_POINT);
            //objPos.y = 5 * BATTLEGND_TILE_SIZE;
            pointLight->setPosition(objPos);
            pointLight->setDiffuseColour(1.0, 0.5, 0.0);
            pointLight->setSpecularColour(1.0, 0.5, 0.0);

            Ogre::Real lightRange = 40.0 * BATTLEGND_TILE_SIZE;
            pointLight->setAttenuation( lightRange, 1.0f, 4.5/lightRange, 75.0f/(lightRange*lightRange) );
            //pointLight->setAttenuation( 13, 1.0, 0.35, 0.44);
        }
    }

}

Battleground::~Battleground()
{
    if(m_grid != NULL)
    {
        delete m_grid;
    }

    if(m_battleGnd != NULL)
    {
        m_bfNode->removeAndDestroyAllChildren();
        m_SceneMgr->destroyAllEntities();
        m_SceneMgr->destroyAllLights();
        m_SceneMgr->destroyManualObject(m_battleGnd);
        m_SceneMgr->destroySceneNode(m_bfNode);

    }

}

Ogre::String
Battleground::getMaterialById(const int id) const
{
    if(m_resourceIdMapper->count(id))
    {
       return (*m_resourceIdMapper)[id];
    }else
    {
        return  "Tile/Minitiles";
    }

//    if(id != 255)
//    {
//       return  "Tile/Minitiles";
//    }else
//    {
//       return  "Tile/ClearMoltenStream";
//    }
}

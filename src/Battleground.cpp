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

    /* Tiles */
    m_bfNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(BATTLEGND_NODE_NAME, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
    m_bfNode->attachObject(m_battleGnd);

    /* Grid */
    m_grid = new Grid(m_bfNode, m_SceneMgr, m_width, m_height, BATTLEGND_TILE_SIZE);

    /* 3D Objects */
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

//        std::cout << "resourceId: " << movableObjNode.attribute("resourceId").value() << "\n";
//        std::cout << "refId: " << movableObjNode.attribute("refId").value() << "\n";
//        std::cout << "x: " << objX << "\n";
//        std::cout << "y: " << objY << "\n";

        Ogre::Vector3 objPos(objX, 0, objY);


        std::string entityName = "entity_mvObj_";
        entityName.append(movableObjNode.attribute("refId").value());

        std::string nodeName = "node_mvObj_";
        nodeName.append(movableObjNode.attribute("refId").value());



        if(resId == 5)
        {
            std::string lightName = "light_mvObj_";
            lightName.append(movableObjNode.attribute("refId").value());

            Ogre::Light* pointLight = mSceneMgr->createLight(lightName);
            pointLight->setType(Ogre::Light::LT_POINT);
            objPos.y = 1 * BATTLEGND_TILE_SIZE;
            pointLight->setPosition(objPos);
            pointLight->setDiffuseColour(1.0, 1.0, 1.0);
            pointLight->setSpecularColour(1.0, 1.0, 1.0);

            Ogre::Entity*       objEntity = m_SceneMgr->createEntity(entityName, Ogre::SceneManager::PT_SPHERE);
            objEntity->setMaterialName("Light/ClearMoltenStream");
            Ogre::SceneNode*    objNode = m_bfNode->createChildSceneNode(nodeName, objPos, Ogre::Quaternion::IDENTITY);
            objNode->scale(0.005, 0.005, 0.005);
            objNode->attachObject(objEntity);
            //Ogre::Real lightRange = 5.0 * BATTLEGND_TILE_SIZE;
            //pointLight->setAttenuation( lightRange, 1.0f, 4.5/lightRange, 75.0f/(lightRange*lightRange) );
            //pointLight->setAttenuation( 13, 1.0, 0.35, 0.44);
        }else
        {
            Ogre::Entity*       objEntity = m_SceneMgr->createEntity(entityName, "Barrel.mesh");
            Ogre::SceneNode*    objNode = m_bfNode->createChildSceneNode(nodeName, objPos, Ogre::Quaternion::IDENTITY);
            objNode->scale(0.25, 0.25, 0.25);
            objNode->attachObject(objEntity);

        }
    }

//    const Ogre::Vector3 p1(4.5, 0, 1.5);
//    const Ogre::Vector3 p2(7.5, 0, 4.5);
//    createWall("0", p1, p2);

    /* Walls */
    int wallId = 0;

    //Ogre::Entity*       objEntity = m_SceneMgr->createEntity(entityName, "cube.mesh");

    Ogre::Vector3 p1;
    p1.y = 0.0f;
    Ogre::Vector3 p2;
    p2.y = 0.0f;
    for(xml_node wallNode = xmlBattlefieldNode.child("Wall"); wallNode; wallNode = wallNode.next_sibling("Wall"))
    {

        //Ogre::Entity*       objEntity = m_SceneMgr->createEntity(wallName, "cube.mesh");
        //Ogre::SceneNode*    wallNode    = m_bfNode->createChildSceneNode(wallNodeName, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
//        wallNode->attachObject(wall);
//
//        wall->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST , Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        //std::cout << "wall !!!\n";
        for (xml_node_iterator it = wallNode.begin(); it != wallNode.end(); ++it)
        {
            Ogre::Real pointX;
            Ogre::Real pointZ;
            std::istringstream (it->attribute("x").value()) >> pointX;
            std::istringstream (it->attribute("y").value()) >> pointZ;

//            std::cout << "Point[" << pointX << " ; " << pointZ << "]\n";

            if(it == wallNode.begin())
            {
                p1.x = pointX;
                p1.z = pointZ ;//+ 0.5 * BATTLEGND_TILE_SIZE;
            }else
            {
                p2.x = pointX;
                p2.z = pointZ ;//+ 0.5 * BATTLEGND_TILE_SIZE;
                std::string strWallId = static_cast<std::ostringstream*>( &(std::ostringstream() << wallId) )->str();

                //create3DWall(strWallId, p1, p2);
                create2DWall(strWallId, p1, p2);
                wallId ++;

                p1.x = pointX;
                p1.z = pointZ;
            }

            //createWall(i, p1, p2);
        }
//        wall->end();

    }

//    p1.x = 6 * BATTLEGND_TILE_SIZE;
//    p1.z = 5 * BATTLEGND_TILE_SIZE;
//    p2.x = 7 * BATTLEGND_TILE_SIZE;
//    p2.z = 5 * BATTLEGND_TILE_SIZE;
//    createWall("MurFmi1", p1, p2);
//
//    p1.x = 8 * BATTLEGND_TILE_SIZE;
//    p1.z = 5 * BATTLEGND_TILE_SIZE;
//    p2.x = 10 * BATTLEGND_TILE_SIZE;
//    p2.z = 5 * BATTLEGND_TILE_SIZE;
//    createWall("MurFmi2", p1, p2);

    std::cout << "Number of created walls: " << wallId << "\n";

//    Ogre::Light* pointLight = mSceneMgr->createLight("DungeonLight");
//    pointLight->setType(Ogre::Light::LT_POINT);
//    pointLight->setPosition(Ogre::Vector3(10*1.5f, 1*1.5f, 10*1.5f));
//    pointLight->setDiffuseColour(0.1, 0.1, 0.1);
//    pointLight->setSpecularColour(0.1, 0.1, 0.1);
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
        m_SceneMgr->destroyAllManualObjects();
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

void
Battleground::create3DWall(const std::string& wallId, const Ogre::Vector3& p1, const Ogre::Vector3& p2)
{
    const Ogre::Real wallCenterX = (std::abs(p1.x - p2.x) / 2.0) + std::min(p1.x, p2.x);
    const Ogre::Real wallCenterY = 0.0f;
    const Ogre::Real wallCenterZ = (std::abs(p1.z - p2.z) / 2.0) + std::min(p1.z, p2.z);
    const Ogre::Vector3 wallCenter(wallCenterX, wallCenterY, wallCenterZ);
    const Ogre::Real wallLength = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.z - p2.z) * (p1.z - p2.z));
    const Ogre::Real angle  = - atan(std::abs(p1.z - p2.z) / std::abs(p1.x - p2.x));

    const Ogre::Real wallHeight = 2.5f;
    const Ogre::Real wallThickness = 0.5f;

    const Ogre::Quaternion rotation(Ogre::Radian(angle), Ogre::Vector3::UNIT_Y);
    std::string wallEntityName  = "WallEnt_";
    wallEntityName.append(wallId);
    std::string wallNodeName    = "WallNode_";
    wallNodeName.append(wallId);
    std::string wallMeshName    = "Mesh_";
    wallMeshName.append(wallId);

    Ogre::ManualObject* manObjWall = m_SceneMgr->createManualObject(wallEntityName);
    Ogre::SceneNode*    wallNode = m_bfNode->createChildSceneNode(wallNodeName, wallCenter, rotation);
    wallNode->attachObject(manObjWall);

    manObjWall->setCastShadows(true);
    manObjWall->begin("Wall/brick", Ogre::RenderOperation::OT_TRIANGLE_LIST, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    /*0*/manObjWall->position(-wallLength/2,    wallHeight, -wallThickness/2);
    manObjWall->textureCoord(0.0, 0.0);
    /*1*/manObjWall->position(wallLength/2,     wallHeight, -wallThickness/2);
    manObjWall->textureCoord(wallLength/2.5f, 1.0);
    /*2*/manObjWall->position(wallLength/2,     0.0f, -wallThickness/2);
    manObjWall->textureCoord(wallLength/2.5f, 1.0);
    /*3*/manObjWall->position(-wallLength/2,    0.0f, -wallThickness/2);
    manObjWall->textureCoord(0.0, 1.0);

    /*4*/manObjWall->position(-wallLength/2,    wallHeight, wallThickness/2);
    manObjWall->textureCoord(0.0, 1.0);
    /*5*/manObjWall->position(wallLength/2,     wallHeight, wallThickness/2);
    manObjWall->textureCoord(wallLength/2.5f, 1.0);
    /*6*/manObjWall->position(wallLength/2,     0.0f, wallThickness/2);
    manObjWall->textureCoord(wallLength/2.5f, 0.0);
    /*7*/manObjWall->position(-wallLength/2,    0.0f, wallThickness/2);
    manObjWall->textureCoord(0.0, 0.0);

    // face behind / front
    manObjWall->triangle(0,1,2);
    manObjWall->triangle(2,3,0);
    manObjWall->triangle(4,6,5);
    manObjWall->triangle(6,4,7);

    // face top / down
    manObjWall->triangle(0,4,5);
    manObjWall->triangle(5,1,0);
    manObjWall->triangle(2,6,7);
    manObjWall->triangle(7,3,2);

    // face left / right
    manObjWall->triangle(0,7,4);
    manObjWall->triangle(7,0,3);
    manObjWall->triangle(1,5,6);
    manObjWall->triangle(6,2,1);

    manObjWall->end();

//    Ogre::MeshPtr meshPtr = manObjWall->convertToMesh(wallMeshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//    unsigned short src, dest;
//    if (!meshPtr->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
//    {
//     meshPtr->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
//    }
}

void
Battleground::create2DWall(const std::string& wallId, const Ogre::Vector3& p1, const Ogre::Vector3& p2)
{
    const Ogre::Real wallCenterX = (std::abs(p1.x - p2.x) / 2.0) + std::min(p1.x, p2.x);
    const Ogre::Real wallCenterY = 0.5f;
    const Ogre::Real wallCenterZ = (std::abs(p1.z - p2.z) / 2.0) + std::min(p1.z, p2.z);
    const Ogre::Vector3 wallCenter(wallCenterX, wallCenterY, wallCenterZ+0.3f*BATTLEGND_TILE_SIZE);
    const Ogre::Real wallLength = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.z - p2.z) * (p1.z - p2.z));
    const Ogre::Real angle  = - atan(std::abs(p1.z - p2.z) / std::abs(p1.x - p2.x));

    const Ogre::Real wallHeight = 2.5f;
    const Ogre::Real wallThickness = BATTLEGND_TILE_SIZE * 0.5f;

    const Ogre::Quaternion rotation(Ogre::Radian(angle), Ogre::Vector3::UNIT_Y);
    std::string wallEntityName  = "WallEnt_";
    wallEntityName.append(wallId);
    std::string wallNodeName    = "WallNode_";
    wallNodeName.append(wallId);
    std::string wallMeshName    = "Mesh_";
    wallMeshName.append(wallId);

    Ogre::ManualObject* manObjWall = m_SceneMgr->createManualObject(wallEntityName);
    Ogre::SceneNode*    wallNode = m_bfNode->createChildSceneNode(wallNodeName, wallCenter, rotation);
    wallNode->attachObject(manObjWall);

    manObjWall->setCastShadows(true);
    manObjWall->begin("Wall/brick", Ogre::RenderOperation::OT_TRIANGLE_LIST, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    /*0*/manObjWall->position(-wallLength/2,    wallCenterY, -wallThickness/2);
    manObjWall->textureCoord(0.0, 1.0);
    /*1*/manObjWall->position(wallLength/2,     wallCenterY, -wallThickness/2);
    manObjWall->textureCoord(wallLength/2.5f, 1.0);
    /*4*/manObjWall->position(-wallLength/2,    wallCenterY, wallThickness/2);
    manObjWall->textureCoord(0.0, 0.0);
    /*5*/manObjWall->position(wallLength/2,     wallCenterY, wallThickness/2);
    manObjWall->textureCoord(wallLength/2.5f, 0.0);

    manObjWall->triangle(0,2,3);
    manObjWall->triangle(3,1,0);

    manObjWall->end();
}

void
Battleground::createWall(const std::string& wallId, const xml_node& xmlWallNode)
{
    const Ogre::String materialName  = "Wall/brick";
    const Ogre::Real wallThickness  = 0.5f;

    Ogre::String wallEntityName  = "WallEnt_";
    Ogre::String wallNodeName    = "WallNode_";
    wallEntityName.append(wallId);
    wallNodeName.append(wallId);

    Ogre::ManualObject* wallObject   = m_SceneMgr->createManualObject(wallEntityName);
    Ogre::SceneNode*    wallNode     = m_bfNode->createChildSceneNode(wallNodeName, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
    wallNode->attachObject(wallObject);

    wallObject->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST , Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    for(xml_node_iterator it = xmlWallNode.begin(); it != xmlWallNode.end(); ++it)
    {

    }
    wallObject->end();
}

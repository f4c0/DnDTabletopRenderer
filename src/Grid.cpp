#include "Grid.h"

Grid::Grid(Ogre::SceneNode* node, Ogre::SceneManager* mSceneMgr,
           const unsigned int width, const unsigned int height, const Ogre::Real tileSize,
           const Ogre::ColourValue &m_colour)
:m_SceneMgr(mSceneMgr),
m_Node(node),
m_pGrid(NULL),
m_colour(m_colour)
{
    m_pGrid = m_SceneMgr->createManualObject(GRID_ENTITY_NAME);
    create(width, height, tileSize);

    m_Node->attachObject(m_pGrid);
}

Grid::~Grid()
{
    m_Node->detachObject(m_pGrid);
    m_SceneMgr->destroyManualObject(m_pGrid);
}

void
Grid::setColour(const Ogre::ColourValue &colour)
{
   m_colour  = colour;
}

void
Grid::create(const unsigned int width, const unsigned int height, const Ogre::Real tileSize)
{
    m_pGrid->begin("Grid/default", Ogre::RenderOperation::OT_LINE_LIST);

    for(unsigned int currentLine = 0; currentLine <= height; currentLine++)
    {
        m_pGrid->position(0, GRID_HEIGHT, currentLine * tileSize);
		m_pGrid->colour(m_colour);
		m_pGrid->position(width* tileSize, GRID_HEIGHT, currentLine * tileSize);
		m_pGrid->colour(m_colour);
    }

    for(unsigned int currentColumn = 0; currentColumn <= width; currentColumn++)
    {
        m_pGrid->position(currentColumn * tileSize, GRID_HEIGHT, 0);
		m_pGrid->colour(m_colour);
		m_pGrid->position(currentColumn * tileSize, GRID_HEIGHT, height* tileSize);
		m_pGrid->colour(m_colour);
    }
    m_pGrid->end();

    // Render just before the overlays
    m_pGrid->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
}

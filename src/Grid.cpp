#include "Grid.h"

Grid::Grid(Ogre::SceneManager* mSceneMgr, const Ogre::ColourValue &m_colour)
:mSceneMgr(mSceneMgr),
m_colour(m_colour),
m_pGrid(NULL)
{
    m_pGrid = mSceneMgr->createManualObject("Grid");
}

Grid::~Grid()
{
    mSceneMgr->getRootSceneNode()->detachObject(m_pGrid);
    mSceneMgr->destroyManualObject(m_pGrid);
}

void
Grid::setColour(const Ogre::ColourValue &colour)
{
   m_colour  = colour;
}

void
Grid::create(const unsigned int width, const unsigned int height, const Ogre::Real tileSize)
{
//    #define LIGNE 10
//    #define COLONNE 10
    #define HAUTEUR 0.005f

    m_pGrid->begin("Grid/default", Ogre::RenderOperation::OT_LINE_LIST);



    for(unsigned int currentLine = 0; currentLine <= height; currentLine++)
    {
        m_pGrid->position(0, HAUTEUR, currentLine * tileSize);
		m_pGrid->colour(m_colour);
		m_pGrid->position(width* tileSize, HAUTEUR, currentLine * tileSize);
		m_pGrid->colour(m_colour);
    }


    for(unsigned int currentColumn = 0; currentColumn <= width; currentColumn++)
    {
        m_pGrid->position(currentColumn * tileSize, HAUTEUR, 0);
		m_pGrid->colour(m_colour);
		m_pGrid->position(currentColumn * tileSize, HAUTEUR, height* tileSize);
		m_pGrid->colour(m_colour);
    }
    m_pGrid->end();

    // Render just before the overlays
    m_pGrid->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);

    mSceneMgr->getRootSceneNode()->attachObject(m_pGrid);
}

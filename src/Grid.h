#ifndef GRID_H
#define GRID_H

#include <Ogre.h>

#define GRID_HEIGHT 0.005f
#define GRID_ENTITY_NAME    "Grid"

class Grid
{
    public:
        Grid(Ogre::SceneNode* node, Ogre::SceneManager* mSceneMgr,
           const unsigned int width, const unsigned int height, const Ogre::Real tileSize,
           const Ogre::ColourValue &m_colour = Ogre::ColourValue::Black);

        virtual ~Grid();

        //! Grid colour
		const Ogre::ColourValue &getColour() const { return m_colour; }
		void setColour(const Ogre::ColourValue &colour);

    protected:
    private:
        Ogre::SceneManager* m_SceneMgr;
        Ogre::SceneNode* m_Node;
        Ogre::ManualObject* m_pGrid;
        Ogre::ColourValue m_colour;

        void create(const unsigned int width, const unsigned int height, const Ogre::Real tileSize);


};

#endif // GRID_H

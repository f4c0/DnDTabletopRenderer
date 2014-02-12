#ifndef GRID_H
#define GRID_H

#include <Ogre.h>
class Grid
{
    public:
        Grid(Ogre::SceneManager* mSceneMgr, const Ogre::ColourValue &m_colour = Ogre::ColourValue::Black);
        virtual ~Grid();

        //! Grid colour
		const Ogre::ColourValue &getColour() const { return m_colour; }
		void setColour(const Ogre::ColourValue &colour);

		void create(const unsigned int width, const unsigned int height, const Ogre::Real tileSize);

    protected:
    private:
        Ogre::SceneManager* mSceneMgr;
        Ogre::ColourValue m_colour;
        Ogre::ManualObject* m_pGrid;


};

#endif // GRID_H

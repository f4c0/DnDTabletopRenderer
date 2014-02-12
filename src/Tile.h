#ifndef TILE_H
#define TILE_H

#include <Ogre.h>

class Tile
{
    public:
        Tile();
        Tile(const unsigned int x, const unsigned int y, const Ogre::String& materialName);
        Tile(const Tile& cpy);
        virtual ~Tile();

        inline unsigned int getX(void) const;
        inline unsigned int getY(void) const;
        inline Ogre::String getMaterialName(void) const;

        void setXY(const unsigned int x, const unsigned int y);
        void setMaterialName(const Ogre::String& materialName);
    protected:
    private:
        unsigned int  m_x;
        unsigned int  m_y;
        Ogre::String  m_materialName;

};

#endif // TILE_H

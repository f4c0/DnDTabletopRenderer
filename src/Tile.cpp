#include "Tile.h"

Tile::Tile()
:m_x(0),
m_y(0),
m_materialName("")
{

}

Tile::Tile(const unsigned int x, const unsigned int y, const Ogre::String& materialName)
:m_x(x),
m_y(y),
m_materialName(materialName)
{
    //ctor
}

Tile::Tile(const Tile& cpy)
:m_x(cpy.getX()),
m_y(cpy.getY()),
m_materialName(cpy.getMaterialName())
{

}

Tile::~Tile()
{
    //dtor
}

inline unsigned int
Tile::getX(void) const
{
    return m_x;
}

inline unsigned int
Tile::getY(void) const
{
    return m_y;
}

inline Ogre::String
Tile::getMaterialName(void) const
{
    return m_materialName;
}

void
Tile::setXY(const unsigned int x, const unsigned int y)
{
    m_x = x;
    m_y = y;
}

void
Tile::setMaterialName(const Ogre::String& materialName)
{
    m_materialName = materialName;
}

#ifndef MAP_HPP
#define MAP_HPP

#include "OgreFramework.hpp"

class Map
{
private:
    Ogre::TerrainGlobalOptions* m_pTerrainGlobals;
    Ogre::TerrainGroup* m_pTerrainGroup;
    bool m_pTerrainsImported;

    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
public:
    Map(void);
    virtual ~Map(void);
    void createMap();
};

#endif // MAP_HPP

#include "Map.hpp"

Map::Map(void)
{
}

Map::~Map(void)
{

}

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();

}

void Map::defineTerrain(long x, long y)
{
    Ogre::String filename = m_pTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_pTerrainGroup->getResourceGroup(), filename))
    {
        m_pTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        m_pTerrainGroup->defineTerrain(x, y, &img);
        m_pTerrainsImported = true;
    }
}

void Map::initBlendMaps(Ogre::Terrain *terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void Map::configureTerrainDefaults(Ogre::Light *light)
{

    m_pTerrainGlobals->setMaxPixelError(8);
    m_pTerrainGlobals->setCompositeMapDistance(3000);

    m_pTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    m_pTerrainGlobals->setCompositeMapAmbient(OgreFramework::getSingletonPtr()->m_pSceneMgr->getAmbientLight());
    m_pTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    Ogre::Terrain::ImportData& defaultimp = m_pTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600; // due terrain.png is 8 bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void Map::createMap()
{
Ogre::Vector3 dirlight(0.55,-0.3,0.75);
Ogre::Light* Complight = OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Complight");
Complight->setType(Ogre::Light::LT_DIRECTIONAL);
Complight->setDirection(dirlight);
Complight->setDiffuseColour(Ogre::ColourValue::White);
Complight->setSpecularColour(Ogre::ColourValue(0.4,0.4,0.4));
OgreFramework::getSingletonPtr()->m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

m_pTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

m_pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(OgreFramework::getSingletonPtr()->m_pSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
    m_pTerrainGroup->setFilenameConvention(Ogre::String("GameMap"), Ogre::String("dat"));
    m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

configureTerrainDefaults(Complight);

for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);

    m_pTerrainGroup->loadAllTerrains(true);

    if (m_pTerrainsImported)
        {
            Ogre::TerrainGroup::TerrainIterator ti = m_pTerrainGroup->getTerrainIterator();
            while(ti.hasMoreElements())
            {
                Ogre::Terrain* t = ti.getNext()->instance;
                initBlendMaps(t);
            }
        }
m_pTerrainGroup->freeTemporaryResources();

OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyDome(true, "Examples/CloudySky");

}

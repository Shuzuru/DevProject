#include "Avion.hpp"

Avion::Avion(void)
{
}

void Avion::createAvion(bool Faction)
{
    AvionEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("AvionEntity", "razor.mesh");
    AvionNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("AvionNode");
    AvionNode->attachObject(AvionEntity);
    if(Faction)
    {
    AvionNode->setPosition(0,300,0);
    }else
    {
        AvionNode->setPosition(300,300,400);
    }
}

void Avion::accelerate()
{

}

void Avion::decelerate()
{

}

void Avion::changeDirection()
{
}

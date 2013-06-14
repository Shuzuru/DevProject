#ifndef AVION_HPP
#define AVION_HPP

#include "OgreFramework.hpp"

class Avion
{
private:
    Ogre::SceneNode*      AvionNode;
    Ogre::Entity*    AvionEntity;
    Ogre::Vector3   AvionVector;
    bool            Faction;
    int             Speed;
    int             MaxSpeed;
    int             EnginePower;
public:
    Avion();
    void createAvion(bool Faction);
    void accelerate();
    void decelerate();
    void changeDirection();
};

#endif // AVION_HPP

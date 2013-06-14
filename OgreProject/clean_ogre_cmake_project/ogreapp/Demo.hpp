//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_DEMO_HPP
#define OGRE_DEMO_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.hpp"
#include "Map.hpp"
#include "Avion.hpp"
//|||||||||||||||||||||||||||||||||||||||||||||||

class DemoApp : public OIS::KeyListener, public OIS::MouseListener
{
public:
    DemoApp();
    ~DemoApp();

    void startDemo();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &mouseEventRef);
    bool mousePressed(const OIS::MouseEvent &mouseEventRef, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &mouseEventRef, OIS::MouseButtonID id);

private:
        void setupDemoScene();
    void runDemo();

    Ogre::SceneNode*			m_pAvionNode;
    Ogre::Entity*				m_pAvionEntity;
    Map                         Gamemap;
    Avion                       Player;

    bool					m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

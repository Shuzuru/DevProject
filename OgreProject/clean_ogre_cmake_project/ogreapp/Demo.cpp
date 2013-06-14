#include "Demo.hpp"

DemoApp::DemoApp()
{
    m_pAvionNode		= 0;
    m_pAvionEntity		= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
       delete OgreFramework::getSingletonPtr();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::startDemo()
{
    new OgreFramework();
    if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, 0))
        return;

    m_bShutdown = false;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

    setupDemoScene();
    runDemo();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::setupDemoScene()
{
    OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,75,75);
    Gamemap.createMap();
    Player.createAvion(true);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::runDemo()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");

    double timeSinceLastFrame = 0;
    double startTime = 0;

      OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();

    while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown())
    {
        if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

        Ogre::WindowEventUtilities::messagePump();

        if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
        {
            startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

            OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
            OgreFramework::getSingletonPtr()->m_pMouse->capture();

            OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
            OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

            timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
        }
        else
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
        }
    }

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Z))
    {
         //do something
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::mouseMoved(const OIS::MouseEvent &mouseEventRef)
{
    OgreFramework::getSingletonPtr()->mouseMoved(mouseEventRef);
    return true;
}

bool DemoApp::mousePressed(const OIS::MouseEvent &mouseEventRef, OIS::MouseButtonID id)
{
    OgreFramework::getSingletonPtr()->mousePressed(mouseEventRef, id);
    return true;
}

bool DemoApp::mouseReleased(const OIS::MouseEvent &mouseEventRef, OIS::MouseButtonID id)
{
    OgreFramework::getSingletonPtr()->mouseReleased(mouseEventRef, id);
    return true;
}
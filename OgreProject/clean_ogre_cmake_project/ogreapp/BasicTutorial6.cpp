#include "BasicTutorial6.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

//-------------------------------------------------------------------------------------
BasicTutorial6::BasicTutorial6(void)
    : mRoot(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mWindow(0),
    mSceneMgr(0),
    mCamera(0)
{
}
//-------------------------------------------------------------------------------------
BasicTutorial6::~BasicTutorial6(void)
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

bool BasicTutorial6::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    // construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg);

    // setup resources
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
    // configure
    // Show the configuration dialog and initialise the system
    if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
    {
        return false;
    }

    mWindow = mRoot->initialise(true, "BasicTutorial6 Render Window");

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    // initialise all resource groups
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);

    Ogre::Entity* Avion = mSceneMgr->createEntity("Avion", "razor.mesh");
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("AvionNode");
    node->attachObject(Avion);
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Ogre::Vector3(0, 0, 0));
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Ogre::Vector3(0, 50, 150));
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


    Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode->attachObject(ogreHead);

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);

    mRoot->startRendering();

    return true;
}

//Adjust mouse clipping area
void BasicTutorial6::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BasicTutorial6::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void BasicTutorial6::createFrameListener(void){
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);

    // Populate the camera container
    mCamNode = mCamera->getParentSceneNode();

    // set the rotation and move speed
    mRotate = 0.13;
    mMove = 250;

    mDirection = Ogre::Vector3::ZERO;

}

bool BasicTutorial6::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
    if(mShutdown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    return true;
}

bool BasicTutorial6::keyPressed( const OIS::KeyEvent &arg ){
    switch (arg.key)
    {
        case OIS::KC_O:
        mCamera->getParentSceneNode()->detachObject(mCamera);
        mCamNode = mSceneMgr->getSceneNode("CamNode1");
        mCamNode->attachObject(mCamera);
            break;

        case OIS::KC_P:
        mCamera->getParentSceneNode()->detachObject(mCamera);
        mCamNode = mSceneMgr->getSceneNode("CamNode2");
        mCamNode->attachObject(mCamera);
            break;
        case OIS::KC_UP:
        case OIS::KC_Z:
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            break;

        case OIS::KC_LEFT:
        case OIS::KC_Q:
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            break;

        case OIS::KC_PGDOWN:
        case OIS::KC_E:
            break;

        case OIS::KC_PGUP:
        case OIS::KC_A:
            break;

        case OIS::KC_ESCAPE:
            mShutdown = true;
            break;
        default:
            break;
    }
    return true;
}

bool BasicTutorial6::keyReleased( const OIS::KeyEvent &arg ){

    switch (arg.key)
    {
        case OIS::KC_UP:
        case OIS::KC_W:
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            break;

        case OIS::KC_PGDOWN:
        case OIS::KC_E:
            break;

        case OIS::KC_PGUP:
        case OIS::KC_Q:
            break;

        default:
            break;
        }
        return true;
}
// OIS::MouseListener
bool BasicTutorial6::mouseMoved( const OIS::MouseEvent &arg ){
    if (arg.state.buttonDown(OIS::MB_Right))
        {
         //   mCamNode->yaw(Ogre::Degree(-mRotate * arg.state.X.rel), Ogre::Node::TS_WORLD);
         //   mCamNode->pitch(Ogre::Degree(-mRotate * arg.state.Y.rel), Ogre::Node::TS_LOCAL);
        }
    return true;
}
bool BasicTutorial6::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
    Ogre::Light *light = mSceneMgr->getLight("Light1");
    switch (id)
    {
    case OIS::MB_Left:
        light->setVisible(! light->isVisible());
        break;
    default:
        break;
    }
    return true;
}
bool BasicTutorial6::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){return true;}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        BasicTutorial6 app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

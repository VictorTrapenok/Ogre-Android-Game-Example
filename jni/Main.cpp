#define OGRE_STATIC_GLES2
#define OGRE_STATIC_ParticleFX
#define OGRE_STATIC_OctreeSceneManager

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <android/input.h>
#include <android/sensor.h>



#include "Ogre.h"
#include "OgreRenderWindow.h"
#include "OgreStringConverter.h"
#include "RTShaderHelper.h"
#include "OgreAndroidEGLWindow.h"
#include "OgreAPKFileSystemArchive.h"
#include "OgreAPKZipArchive.h"

#include "OgreOverlayManager.h"
#include "OgreOverlayContainer.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreOverlaySystem.h"

#include "include/Build/OgreStaticPluginLoader.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "", __VA_ARGS__))

static Ogre::RenderWindow* gRenderWnd = NULL;
static Ogre::Root* gRoot = NULL;
static AAssetManager* gAssetMgr = NULL; 
static Ogre::SceneManager* gSceneMgr = NULL;
static Ogre::ShaderGeneratorTechniqueResolverListener* gMatListener = NULL;
static Ogre::StaticPluginLoader* gStaticPluginLoader = NULL;



static Ogre::DataStreamPtr openAPKFile(const Ogre::String& fileName)
{
	Ogre::DataStreamPtr stream;
    AAsset* asset = AAssetManager_open(gAssetMgr, fileName.c_str(), AASSET_MODE_BUFFER);
    if(asset)
    {
		off_t length = AAsset_getLength(asset);
        void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
        memcpy(membuf, AAsset_getBuffer(asset), length);
        AAsset_close(asset);
                
        stream = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(membuf, length, true, true));
    }
    return stream;
}
		
Ogre::Camera* camera = NULL;
Ogre::SceneNode* pNode = NULL;

/**
 * Загрузка ресурсов
 */
static void loadResources(const char *name)
{
	Ogre::ConfigFile cf;
	cf.load(openAPKFile(name));

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		Ogre::String sec, type, arch;
		sec = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); i++)
		{
			type = i->first;
			arch = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();
}

static void setupScene()
{
	Ogre::RTShader::ShaderGenerator::initialize();
	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->setTargetLanguage("glsles");
	gMatListener = new Ogre::ShaderGeneratorTechniqueResolverListener();
	Ogre::MaterialManager::getSingleton().addListener(gMatListener);

	LOGW("Create SceneManager");
	gSceneMgr = gRoot->createSceneManager(Ogre::ST_GENERIC);
	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(gSceneMgr);

	camera = gSceneMgr->createCamera("MyCam");
	camera->setNearClipDistance(1.0f);
	camera->setFarClipDistance(100000.0f);
	camera->setPosition(0,0,20.0f);
	camera->lookAt(0,0,0);
	camera->setAutoAspectRatio(true);

	Ogre::Viewport* vp = gRenderWnd->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(0.90f,0.99f,0.0f));
	vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	/**
	 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Creating+Overlays+via+Code
	 * http://www.ogre3d.org/forums/viewtopic.php?f=2&t=78278#p492027
	 */
	LOGW("Create OverlaySystem");
	Ogre::OverlaySystem *mOverlaySystem = OGRE_NEW Ogre::OverlaySystem();
	gSceneMgr->addRenderQueueListener(mOverlaySystem);

	LOGW("Create overlayManager");
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();


	loadResources("resources.cfg");

	/**
	 * Инициализация сцены
	 */
	Ogre::Entity* pEntity = gSceneMgr->createEntity("SinbadInstance", "Sinbad.mesh");
	Ogre::SceneNode* pNode = gSceneMgr->getRootSceneNode()->createChildSceneNode();
	pNode->attachObject(pEntity);

	Ogre::Light* pDirLight = gSceneMgr->createLight();
	pDirLight->setDirection(Ogre::Vector3(0,-1,0));
	pDirLight->setType(Ogre::Light::LT_DIRECTIONAL);
	pNode->attachObject(pDirLight);

	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);


	 LOGW("Create overlay");
	 Ogre::Overlay* overlay = overlayManager.create( "OverlayName" );

	 LOGW("Create a panel");
	 Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelName" ) );

	 LOGW("panel->setPosition");
	 panel->setPosition( 0.0, 0.0 );
	 panel->setDimensions( 300, 50 );

	 LOGW("panel->setMaterialName");
	 panel->setMaterialName("overlay_image_material");
	 panel->setMetricsMode(Ogre::GMM_PIXELS);

	 // Add the panel to the overlay
	 LOGW("overlay->add2D( panel )");
	 overlay->add2D( panel );


	 LOGW("Create TextAreaOverlayElement");
	 // Create a text area
	 Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "TextAreaName"));
	 textArea->setMetricsMode(Ogre::GMM_PIXELS);
	 textArea->setPosition(0, 0);
	 textArea->setDimensions(100, 100);
	 textArea->setCaption("Hello, World!");
	 textArea->setCharHeight(16);
	 textArea->setFontName("QWcuckoo");
	 textArea->setColourBottom(Ogre::ColourValue(0.3, 0.5, 0.3));
	 textArea->setColourTop(Ogre::ColourValue(0.5, 0.7, 0.5));

	 LOGW("addChild Element");
	 panel->addChild(textArea);

	 LOGW("Show the overlay");
	 // Show the overlay
	 overlay->show();


}

class AppState{
	public:
};

class NativeApp{

	public:
		struct android_app* app;

		ASensorManager* sensorManager;
		ASensorEventQueue* sensorEventQueue;

		AppState state;
};

Ogre::Vector3 CameraRot;
Ogre::Vector3 lastPos;
static int32_t handleInput(struct android_app* app, AInputEvent* event)
{
	    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
	        float x = AMotionEvent_getX(event, 0);
	        float y = AMotionEvent_getY(event, 0);

	        LOGW("MOTION: x=%f y=%f", x, y);

	        if( abs(x - lastPos.x) < 25)
	        {
	        	CameraRot.x += (x - lastPos.x) * 0.01;
	        }

	        if( abs(y - lastPos.y)< 25)
	        {
	        	CameraRot.y += (y - lastPos.y) * 0.01;
	        }

	        camera->setPosition( cos( CameraRot.x) * 20,  sin( CameraRot.y) * 20 , -sin( CameraRot.x) * 20 );
	    	camera->lookAt(0,0,0);

	        lastPos.x = x;
	        lastPos.y = y;

	        char text[300];
	        sprintf(text, "X:%.2f Y:%.2f", x, y);

	        //textArea->setCaption( text );


	        return 1;
	    }
	return 0;
}

static void handleCmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) 
    {
        case APP_CMD_SAVE_STATE:
        	// http://developer.android.com/reference/android/app/NativeActivity.html
            break;
        case APP_CMD_INIT_WINDOW:
            if(app->window && gRoot)
            {
                AConfiguration* config = AConfiguration_new();
                AConfiguration_fromAssetManager(config, app->activity->assetManager);
                gAssetMgr = app->activity->assetManager;
				
                if(!gRenderWnd)
                {
				    Ogre::ArchiveManager::getSingleton().addArchiveFactory( new Ogre::APKFileSystemArchiveFactory(app->activity->assetManager) );
					Ogre::ArchiveManager::getSingleton().addArchiveFactory( new Ogre::APKZipArchiveFactory(app->activity->assetManager) );
				
                    Ogre::NameValuePairList opt;
                    opt["externalWindowHandle"] = Ogre::StringConverter::toString((int)app->window);
                    opt["androidConfig"] = Ogre::StringConverter::toString((int)config);
                           
					gRenderWnd = gRoot->createRenderWindow("OgreWindow", 0, 0, false, &opt); 
		
					setupScene();
                }
                else
                {
					static_cast<Ogre::AndroidEGLWindow*>(gRenderWnd)->_createInternalResources(app->window, config);
                }
                AConfiguration_delete(config);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            if(gRoot && gRenderWnd)
				static_cast<Ogre::AndroidEGLWindow*>(gRenderWnd)->_destroyInternalResources();
            break;
        case APP_CMD_GAINED_FOCUS:
        	// When our app gains focus, we start monitoring the accelerometer.

            break;
        case APP_CMD_LOST_FOCUS:
			// Also stop animating.
            break;
    }
}

void android_main(struct android_app* state)
{
	NativeApp app;
    app_dummy();

	state->userData = &app;

	// Prepare to monitor accelerometer
	app.sensorManager = ASensorManager_getInstance();
	app.sensorEventQueue = ASensorManager_createEventQueue(app.sensorManager, state->looper, LOOPER_ID_USER, NULL, NULL);



	if (state->savedState != NULL)
	{
		// We are starting with a previous saved state; restore from it.
		app.state = *(AppState*)state->savedState;
	}

	if(gRoot == NULL)
	{
		gRoot = new Ogre::Root();
		#ifdef OGRE_STATIC_LIB
			gStaticPluginLoader = new Ogre::StaticPluginLoader();
			gStaticPluginLoader->load();
		#endif
        gRoot->setRenderSystem(gRoot->getAvailableRenderers().at(0));
        gRoot->initialise(false);	
	}


    state->onAppCmd = &handleCmd;
    state->onInputEvent = &handleInput;

    int ident, events;
    struct android_poll_source* source;
    
    while (true)
    {
        while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
        {
            if (source != NULL)
            {
            	source->process(state, source);
            }
            
            if (state->destroyRequested != 0)
            {
            	return;
            }
        }
        
		if(gRenderWnd != NULL && gRenderWnd->isActive())
		{
			gRenderWnd->windowMovedOrResized();
			gRoot->renderOneFrame();
		}
    }
}

#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "Sphere.hpp"
// Include the 'Renderer.hpp' which deteremines what
// the graphics API is going to be for OpenGL
#include "Renderer.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
	// The window we'll be rendering to
	m_window = NULL;

    m_width = w;
    m_height = h;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
	}
    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //Create window
    m_window = SDL_CreateWindow( "Lab",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            m_width,
                            m_height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    // Check if Window did not create.
    if( m_window == NULL ){
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    //Create an OpenGL Graphics Context
    m_openGLContext = SDL_GL_CreateContext( m_window );
    if( m_openGLContext == NULL){
        std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    // Initialize GLAD Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        std::cerr << "Failed to iniitalize GLAD\n";
        exit(EXIT_FAILURE);
    }

    // If initialization succeeds then print out a list of errors in the constructor.
    SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL errors detected during initialization\n\n");

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}

Object* sphere3;
SceneNode* Moon;
SceneNode* Moon2;
SceneNode* Moon3;
SceneNode* Moon4;
SceneNode* Moon5;
SceneNode* Moon6;
// Create the Earth
Object* sphere2;
SceneNode* Earth;
SceneNode* Earth2;
SceneNode* Earth3;
// Create the Sun
Object* sphere;
SceneNode* Sun;
//Loops forever!
void SDLGraphicsProgram::SetLoopCallback(std::function<void(void)> callback){
    
    // Create a renderer
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(m_width,m_height);    

    // Create our terrain
    //std::shared_ptr<Terrain> myTerrain = std::make_shared<Terrain>(512,512,"./assets/textures/terrain2.ppm");
    //myTerrain->LoadTextures("./assets/textures/colormap.ppm","./assets/textures/detailmap.ppm");

    // Create a node for our terrain 
    //std::shared_ptr<SceneNode> terrainNode;
    //terrainNode = std::make_shared<SceneNode>(myTerrain,"./shaders/vert.glsl","./shaders/frag.glsl");

    sphere3 = new Sphere();
    sphere3->LoadTexture("./media/rock.ppm");
    // Create a new node using sphere3 as the geometry
    Moon = new SceneNode(sphere3);
    Moon->GetLocalTransform().Translate(5, 0, 0);
    Moon->GetLocalTransform().Scale(0.5, 0.5, 0.5);

    Moon2 = new SceneNode(sphere3);
    Moon2->GetLocalTransform().Translate(5, 0, 2);
    Moon2->GetLocalTransform().Scale(0.2, 0.2, 0.2);

    Moon3 = new SceneNode(sphere3);
    Moon3->GetLocalTransform().Translate(0, 0, 7);
    Moon3->GetLocalTransform().Scale(0.3, 0.3, 0.3);

    Moon4 = new SceneNode(sphere3);
    Moon4->GetLocalTransform().Translate(1, 0, 4);
    Moon4->GetLocalTransform().Scale(0.3, 0.3, 0.3);

    Moon5 = new SceneNode(sphere3);
    Moon5->GetLocalTransform().Translate(3, 0, 3);
    Moon5->GetLocalTransform().Scale(0.6, 0.6, 0.6);

    Moon6 = new SceneNode(sphere3);
    Moon6->GetLocalTransform().Translate(4, 0, 2);
    Moon6->GetLocalTransform().Scale(0.3, 0.3, 0.3);
    // Create the Earth
    sphere2 = new Sphere();
    sphere2->LoadTexture("./media/earth.ppm");
    Earth = new SceneNode(sphere2);
    Earth->GetLocalTransform().Translate(-7, 0, -15);
    Earth->GetLocalTransform().Scale(0.5, 0.5, 0.5);

    Earth2 = new SceneNode(sphere2);
    Earth2->GetLocalTransform().Translate(8, 0, 15);
    Earth2->GetLocalTransform().Scale(0.5, 0.5, 0.5);

    Earth3 = new SceneNode(sphere2);
    Earth3->GetLocalTransform().Translate(2, 0, 4);
    Earth3->GetLocalTransform().Scale(0.5, 0.5, 0.5);
    // Create the Sun
    sphere = new Sphere();
    sphere->LoadTexture("./media/sun.ppm");
    Sun = new SceneNode(sphere);
    Sun->GetLocalTransform().Translate(-5, 0, -25);

    // Render our scene starting from the sun.
    renderer->setRoot(Sun);
    // Make the Earth a child of the Sun
    Sun->AddChild(Earth);
    Sun->AddChild(Earth2);
    Sun->AddChild(Earth3);
    // Make the Moon a child of the Earth
    Earth->AddChild(Moon);
    Earth->AddChild(Moon6);
    Earth2->AddChild(Moon2);
    Earth2->AddChild(Moon3);
    Earth2->AddChild(Moon4);
    Earth3->AddChild(Moon5);
    // Set our SceneTree up
    //renderer->setRoot(terrainNode);

    // Set a default position for our camera
    renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.0f,0.0f);
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    bool rotating = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 5.0f;

    // Center our mouse
    SDL_WarpMouseInWindow(m_window,m_width/2,m_height/2);

    // Get a pointer to the keyboard state
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    std::cout << "Welcome to my final project for CS4300\n";
    std::cout << "Press W to show the wireframe (it should just be a single quad)\n";
    std::cout << "Press R to toggle the earths rotating around the sun\n";
    std::cout << "Press 0 to view motion blur (this is the starting view)\n";
    std::cout << "Press 1 to use the depth of field effect\n";
    std::cout << "      -Pressing E will move the focus depth backwards, Q will move it forwards\n";
    std::cout << "Press 2 to view a greyscale representation of the depth buffer\n";
    std::cout << "Press 3 to view a representation of the velocity buffer";
    std::cout << "      - red is horizontal velocity, green is vertical\n";
    std::cout << "Press 4 to view the scene without any effects\n";
    std::cout << "Press 5 to view the scene with an attempt to combine motion blur and depth of field\n";
    std::cout << "Press 6 to view depth of field, but the section in focus is colored red\n";


    // While application is running
    while(!quit){
        // For our terrain setup the identity transform each frame
        // By default set the terrain node to the identity
        // matrix.
        //terrainNode->GetLocalTransform().LoadIdentity();
        // Invoke(i.e. call) the callback function
        callback();

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboard input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
            }
        } // End SDL_PollEvent loop.

        // Move left or right
        if(keyboardState[SDL_SCANCODE_LEFT]){
            renderer->GetCamera(0)->MoveLeft(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_RIGHT]){
            renderer->GetCamera(0)->MoveRight(cameraSpeed);
        }

        // Move forward or back
        if(keyboardState[SDL_SCANCODE_UP]){
            renderer->GetCamera(0)->MoveForward(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_DOWN]){
            renderer->GetCamera(0)->MoveBackward(cameraSpeed);
        }

        // Move up or down
        if(keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT])   {
            //renderer->GetCamera(0)->MoveUp(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_LCTRL] || keyboardState[SDL_SCANCODE_RCTRL]){
            renderer->GetCamera(0)->MoveDown(cameraSpeed);
        }else if(keyboardState[SDL_SCANCODE_R]){
            rotating = !rotating;
        }
        if(rotating)
        {
            Sun->GetLocalTransform().Rotate(0.01, 0, 1, 0);
        }	
        // ... transform the Sun

        Moon->GetLocalTransform().Rotate(0.1, 0, 1, 0);
        Moon2->GetLocalTransform().Rotate(0.1, 0, 1, 0);
        Moon3->GetLocalTransform().Rotate(0.1, 0, 1, 0);
        Moon4->GetLocalTransform().Rotate(0.1, 0, 1, 0);
        Moon5->GetLocalTransform().Rotate(0.1, 0, 1, 0);		
        Moon6->GetLocalTransform().Rotate(0.1, 0, 1, 0);		
        // ... transform the Moon

        Earth->GetLocalTransform().Rotate(0.05, 1, 1, 0);

        Earth2->GetLocalTransform().Rotate(0.3, 0, 1, 0);

        Earth3->GetLocalTransform().Rotate(0.2, -1, 1, 0);
        // Update our scene through our renderer
        renderer->Update();
        // Render our scene using our selected renderer
        renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

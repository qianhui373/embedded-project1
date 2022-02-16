
#ifndef APPLICATION_H_
#define APPLICATION_H_



#include <HAL/HAL.h>
//////
#include <HAL/Graphics.h>
#include <HAL/Timer.h>
///////

///////////////////////////////////////////////////////////////////////////////
#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds

#define NUM_TEST_OPTIONS    12
#define NUM_RANDOM_NUMBERS  5

char rxChar;
char txChar;
//////////////////////////////////////////////////////////////////////////
int Energy = 1;
    int Exp = 0;
    int lvl = 0;
    int tonext = 1;
    int Move = 0;




enum _GameState
{
    TITLE_SCREEN, INSTRUCTIONS_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2, CURSOR_3 = 3, CURSOR_4 = 4, CURSOR_5 = 5, CURSOR_6 = 6, CURSOR_7 = 7, CURSOR_8 = 8, CURSOR_9 = 9,
    CURSOR_10 = 10, CURSOR_11 = 11,
};
typedef enum _Cursor Cursor;

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////
enum _printState
{
    m_state, mo_state, s_state, move_state, e_state
};
typedef enum _printState printState;



struct _Application
{
 // Put your application members and FSM state variables here!
    // =========================================================================
    UART_Baudrate baudChoice;
    bool firstCall;
    bool consumeEnergy;
    bool original;


    //////////////////////////
    GameState state;  // Determines which screen is currently shown
        SWTimer timer;    // General-purpose timer for when screens must disappear
     printState printState;
        // Booleans to determine which colors the user has picked
        bool redSelected;
        bool greenSelected;
        bool blueSelected;

        // Cursor tracker variable
        Cursor cursor;
        //////////////////////////////////////////////////////////////////////////////

};


typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed

// Called once per super-loop of the main application.


////////////////////////////////////////////////////////
Application Application_construct(HAL* hal_p);
void Application_loop(Application* app_, HAL* hal_p);


void Application_handleTitleScreen(Application* app_, HAL* hal);
void Application_handleInstructionsScreen(Application* app_, HAL* hal_p);
void Application_handleGameScreen(Application* app_, HAL* hal_p);
void Application_handleResultScreen(Application* app_, HAL* hal_p);

// Helper functions which clear the screen and draw the text for each state
void Application_showTitleScreen(GFX* gfx_p);
void Application_showInstructionsScreen(Application* app_p, GFX* gfx_p);
void Application_showGameScreen(Application* app_p, GFX* gfx_p);
void Application_showResultScreen(Application* app_p, HAL* hal_p);



void Application_updateGameScreen(Application* app_p, GFX* gfx_p);

// Used to reset the internal game variables
void Application_initGameVariables(Application* app_p, HAL* hal_p);

////////////////////////////////the new functions i set
void qianhui(Application* app_p, HAL* hal_p);
void qianhui1(Application* app_p, HAL* hal_p);
void Application_input(Application* app_p, HAL* hal_p);
void ecolor(Application* app_p, HAL* hal_p);



// Called whenever the UART module needs to be updated
void Application_updateCommunications(Application* app_, HAL* hal_p);

// Interprets an incoming character and echoes back to terminal what kind of
// character was received (number, letter, or other)


// Generic circular increment function
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);


#endif /* APPLICATION_H_ */


///////////////////////////////////////////////////////////
#ifndef APPLICATION_H_
#define APPLICATION_H_



#include <HAL/HAL.h>
//////
#include <HAL/Graphics.h>
#include <HAL/Timer.h>
///////
#include <HAL/ADC_HAL.h>
///////////////////////////////////////////////////////////////////////////////
#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds

#define NUM_TEST_OPTIONS    12
#define NUM_RANDOM_NUMBERS  5





enum _GameState
{
    TITLE_SCREEN, MENU_SCREEN, HOWTOPLAY_SCREEN, HIGHSCORES_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2, CURSOR_3 = 3, CURSOR_4 = 4, CURSOR_5 = 5, CURSOR_6 = 6, CURSOR_7 = 7, CURSOR_8 = 8, CURSOR_9 = 9,
    CURSOR_10 = 10, CURSOR_11 = 11,
};
typedef enum _Cursor Cursor;

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////
enum _printState
{
    m_state, mo_state, s_state, move_state, e_state
};
typedef enum _printState printState;



struct _Application
{
 // Put your application members and FSM state variables here!
    // =========================================================================
    //UART_Baudrate baudChoice;
    bool firstCall;
    bool consumeEnergy;
    bool original;


    //////////////////////////
    GameState state;  // Determines which screen is currently shown
        SWTimer timer;    // General-purpose timer for when screens must disappear
     printState printState;
        // Booleans to determine which colors the user has picked
        bool redSelected;
        bool greenSelected;
        bool blueSelected;

        // Cursor tracker variable
        Cursor cursor;
        //////////////////////////////////////////////////////////////////////////////
        int randomNumbers[NUM_RANDOM_NUMBERS];
        int randomNumberChoice;

        ////////////////////////////////////////////////////////////////////////////
        GFX gfx;  //gfx stands for grahics!!!

        unsigned int frameIndex;
        unsigned int frameOffset;

        //SWTimer Launchpad_LED2_blinkingTimer;

};


typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed

// Called once per super-loop of the main application.


////////////////////////////////////////////////////////
Application Application_construct(HAL* hal_p);
void Application_loop(Application* app_p, HAL* hal_p);


void Application_handleTitleScreen(Application* app_p, HAL* hal_p);
void Application_handleMenuScreen(Application* app_p, HAL* hal_p);
void Application_handleGameScreen(Application* app_p, HAL* hal_p);
void Application_handleResultScreen(Application* app_p, HAL* hal_p);
////////////////////////////////////////////////////////////////////////////////
void Application_handleHowToPlayScreen(Application* app_p, HAL* hal_p);
void Application_handleHighScoresScreen(Application* app_p, HAL* hal_p);


// Helper functions which clear the screen and draw the text for each state
void Application_showTitleScreen(GFX* gfx_p);
void Application_showMenuScreen(Application* app_p, GFX* gfx_p);
void Application_showGameScreen(Application* app_p, GFX* gfx_p);
void Application_showResultScreen(Application* app_p, HAL* hal_p);
///////////////////////////////////////////////////////////////////////////////
void Application_showHowToPlayScreen(Application* app_p, GFX* gfx_p);
void Application_showHighScoresScreen(Application* app_p, GFX* gfx_p);



void Application_updateGameScreen(Application* app_p, GFX* gfx_p);

// Used to reset the internal game variables
void Application_initGameVariables(Application* app_p, HAL* hal_p);

////////////////////////////////the new functions i set
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);



#endif /* APPLICATION_H_ */


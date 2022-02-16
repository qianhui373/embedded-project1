/**
 * project1 by Qianhui Xie
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>

//////
//define three image functions whihc will be displayed on LCD
extern const Graphics_Image colors8BPP_UNCOMP;
extern const Graphics_Image sad8BPP_UNCOMP;
extern const Graphics_Image happy8BPP_UNCOMP;

//change LED2 color based on energy change
void ecolor(Application* app_p, HAL* hal_p){
                                            if (Energy == 0){//energy = 0, no light
                                                LED_turnOff(&hal_p->launchpadLED2Red);
                                                LED_turnOff(&hal_p->launchpadLED2Green);
                                                LED_turnOff(&hal_p->launchpadLED2Blue);
                                            }
                                            else if (Energy == 1){//energy = 1, light red
                                                LED_turnOn(&hal_p->launchpadLED2Red);
                                                LED_turnOff(&hal_p->launchpadLED2Green);
                                                LED_turnOff(&hal_p->launchpadLED2Blue);
                                            }
                                            else if (Energy == 2){//energy = 2, light yellow
                                                LED_turnOn(&hal_p->launchpadLED2Red);
                                                LED_turnOn(&hal_p->launchpadLED2Green);
                                                LED_turnOff(&hal_p->launchpadLED2Blue);
                                            }
                                            else if (Energy == 3){//energy = 3, light green
                                                LED_turnOff(&hal_p->launchpadLED2Red);
                                                LED_turnOn(&hal_p->launchpadLED2Green);
                                                LED_turnOff(&hal_p->launchpadLED2Blue);
                                            }
                                            else if (Energy == 4){//energy = 4, light cyan
                                                LED_turnOn(&hal_p->launchpadLED2Green);
                                                LED_turnOn(&hal_p->launchpadLED2Blue);
                                                LED_turnOff(&hal_p->launchpadLED2Red);
                                            }
                                            else if (Energy == 5){//energy = 5, blue
                                                LED_turnOn(&hal_p->launchpadLED2Blue);
                                                LED_turnOff(&hal_p->launchpadLED2Red);
                                                LED_turnOff(&hal_p->launchpadLED2Green);
                                            }
}


//in this function, it controls energy decrease, each two moves decreaseone energy.
void qianhui(Application* app_p, HAL* hal_p)
{

    if (Move %2 != 0){
        Energy = Energy - 1;
    }
}

void qianhui1(Application* app_p, HAL* hal_p)
{
    Move = Move + 1;
    Exp = Exp + 1;
}


// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();

    InitSystemTiming();

        // Initialize the main Application object and HAL object
        HAL hal_p = HAL_construct();
        Application app_p = Application_construct(&hal_p);
        Application_showTitleScreen(&hal_p.gfx);

        // Main super-loop! In a polling architecture, this function should call
        // your main FSM function over and over.
        while (true)
        {
            Application_loop(&app_p, &hal_p);
            HAL_refresh(&hal_p);
            PollNonBlockingLED();
        }
        ////////////////////////////////////////////////////////

}




//Generic circular increment function   this is from previous lecture source on GIT hub which is an assignment example-App_GuessTheColor.c.
uint32_t CircularIncrement(uint32_t value, uint32_t maximum)
{
    return (value + 1) % maximum;
}


Application Application_construct(HAL* hal_p)
{
    Application app_p;

     //Initialize local application state variables here!
    app_p.baudChoice = BAUD_9600;
    app_p.firstCall = true;
    //app_p.consumeEnergy = true;


        // Initialization of FSM variables
        app_p.state = TITLE_SCREEN;
        app_p.timer = SWTimer_construct(TITLE_SCREEN_WAIT);
        SWTimer_start(&app_p.timer);
        app_p.printState= m_state;

        //Application_initGameVariables(&app_p, hal_p);

        app_p.cursor = CURSOR_0;

        // Return the completed Application struct to the user
        return app_p;
}





/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time.
 */
void Application_loop(Application* app_p, HAL* hal_p)
{
    // The obligatory non-blocking check. At any time in your code, pressing and
    // holding Launchpad S1 should always turn on Launchpad LED1.
    //

    if (Button_isPressed(&hal_p->launchpadS1))
        LED_turnOn(&hal_p->launchpadLED1);
    else
        LED_turnOff(&hal_p->launchpadLED1);

    // Restart/Update communications if either this is the first time the application is
    // run or if Boosterpack S1 is pressed (which means a new baudrate is being set up)
    if (Button_isTapped(&hal_p->boosterpackS1) || app_p->firstCall) {
        Application_updateCommunications(app_p, hal_p);
    }

    /////////////////////////////////


    //////////////////////////////////////////////////////////
    switch (app_p->state)
        {
            case TITLE_SCREEN:
                Application_handleTitleScreen(app_p, hal_p);
                break;

            case INSTRUCTIONS_SCREEN:
                Application_handleInstructionsScreen(app_p, hal_p);
                break;

            case GAME_SCREEN:
                Application_handleGameScreen(app_p, hal_p);
                break;

            case RESULT_SCREEN:
                Application_handleResultScreen(app_p, hal_p);
                break;

            default:
                break;
        }

}



////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is used to show the title of the screen which includes all contents.
void Application_showTitleScreen(GFX* gfx_p)
{

    GFX_clear(gfx_p);
    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);//set the back ground color as black
    Graphics_drawImage((const Graphics_Context *)gfx_p, &colors8BPP_UNCOMP, 0, 0);//display a image which i copied from previous class examples.

    GFX_print(gfx_p, "Explorer fire fighting  ", 0, 0);
    GFX_print(gfx_p, "Playing now", 1, 0);
    GFX_print(gfx_p, "By: Qianhui Xie", 2, 0);
    GFX_print(gfx_p, "Edit: Qianhui Xie  ", 3, 0);
}

/**
 * A helper function which resets all the game variables to their unselected
 * states and resets the cursor position.
 */
//initialize all variables.
void Application_initGameVariables(Application* app_p, HAL* hal_p)
{
    // Reset the cursor
    app_p->cursor = CURSOR_0;

    // Deselect each option
    app_p->redSelected = false;
    app_p->greenSelected = false;
   app_p->blueSelected = false;

    // Turn off all LEDs - they don't turn on until a random number is generated
    LED_turnOff(&hal_p->boosterpackRed);
    LED_turnOff(&hal_p->boosterpackGreen);
    LED_turnOff(&hal_p->boosterpackBlue);

    LED_turnOff(&hal_p->launchpadLED2Red);
    LED_turnOff(&hal_p->launchpadLED2Green);
    LED_turnOff(&hal_p->launchpadLED2Blue);

    //////////////////////////////////////////////////////////////
    //initialize these variable, or inother definition, reset them.
    //this part in the function is for reset the game over screen to title screen.
    *app_p = Application_construct(hal_p);
    int Energy = 1;
        int Exp = 0;
        int lvl = 0;
        int tonext = 1;
        int Move = 0;
}

/**
 * Callback function for when the game is in the TITLE_SCREEN state. Used to
 * break down the main App_GuessTheColor_loop() function into smaller
 * sub-functions.
 */

//control the title screen of the board.
void Application_handleTitleScreen(Application* app_p, HAL* hal_p)
{
    if (SWTimer_expired(&app_p->timer))
    {
        app_p->state = INSTRUCTIONS_SCREEN;
        Application_showInstructionsScreen(app_p, &hal_p->gfx);

    }
}

//control the instruction screen of the board which includes instructions of how to get to game screen.
void Application_handleInstructionsScreen(Application* app_p, HAL* hal_p)
{
    // Transition to start the game when B2 is pressed
    if (Button_isTapped(&hal_p->boosterpackS2))
    {
        // Update internal logical state
        app_p->state = GAME_SCREEN;


        Application_showGameScreen(app_p, &hal_p->gfx);
    }

    while (UART_hasChar(&hal_p->uart)){
                       if (UART_getChar(&hal_p->uart)){
                           app_p->state = GAME_SCREEN;
                           Application_showGameScreen(app_p, &hal_p->gfx);
                       }
                   }
}

/**
 * Callback function for when the game is in the GAME_SCREEN state. Used to
 * break down the main Application_loop() function into smaller
 * sub-functions.
 */
void Application_handleGameScreen(Application* app_p, HAL* hal_p)
{
    if (UART_hasChar(&hal_p->uart)) { // send characters to mobaxterm and display on it.
           rxChar = UART_getChar(&hal_p->uart);


           UART_putChar(&hal_p->uart, rxChar);
           Application_input(app_p, hal_p);// Only send a character if the UART module can send it

           if ((txChar == 'U') || (txChar == 'M') || (txChar == 'F')){//on mobaxterm, after the three characters, it need to get to next line automatically.
                                  UART_putChar(&hal_p->uart, txChar);
                                  UART_putChar(&hal_p->uart, '\n');
                                  UART_putChar(&hal_p->uart, '\r');
           }

           txChar = 'q';

         //type f on mobaXterm, energy will increase up to 5.
           if (rxChar == 'f' && Energy < 5 && Energy >= 0)
              {
                  Energy = Energy + 1;
              }
              char buffer[10];
              snprintf(buffer, 10, "%02d", Energy);
              GFX_print(&hal_p->gfx, buffer, 0, 17);

//i built a switch case to display all my 12 balls on the board. there are 12 cursors,
//each represent 12 different location in side the rectangle on the LCD. this is to make the ball looks like it is moving around in the rectangle.
    switch (app_p->cursor)
                {
                    // In the first three choices, we need to re-display the game screen
                    // to reflect updated choices.
                    // -----------------------------------------------------------------
                    case CURSOR_0: //first location
                        if (rxChar == 'r'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                        {
                            app_p->cursor = CURSOR_1;
                            GFX_removeSolidCircle(&hal_p->gfx, 43, 55, 5);//remove the original ball
                            GFX_drawSolidCircle(&hal_p->gfx, 65, 55, 5);//the ball appear at a new location, which will looks like the ball just moved to the location

                            qianhui1(app_p, hal_p);
                            qianhui(app_p,hal_p);
                            app_p->printState = m_state;

                        }

                        else if (rxChar == 'l'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                        {
                            app_p->cursor = CURSOR_7;
                            GFX_removeSolidCircle(&hal_p->gfx, 43, 55, 5);
                            GFX_drawSolidCircle(&hal_p->gfx, 21, 55, 5);//the ball appear at a new location, which will looks like the ball just moved to the location

                            qianhui1(app_p, hal_p);
                            qianhui(app_p,hal_p);
                            app_p->printState = m_state;
                        }

                        else if (rxChar == 'u'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                        {
                            app_p->cursor = CURSOR_5;
                            GFX_removeSolidCircle(&hal_p->gfx, 43, 55, 5);
                            GFX_drawSolidCircle(&hal_p->gfx, 43, 31, 5);//the ball appear at a new location, which will looks like the ball just moved to the location

                            qianhui1(app_p, hal_p);
                            qianhui(app_p,hal_p);
                            app_p->printState = m_state;
                        }
                        else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                        {
                            app_p->cursor = CURSOR_9;
                            GFX_removeSolidCircle(&hal_p->gfx, 43, 55, 5);
                            GFX_drawSolidCircle(&hal_p->gfx, 43, 78, 5);//the ball appear at a new location, which will looks like the ball just moved to the location

                            qianhui1(app_p, hal_p);
                            qianhui(app_p,hal_p);
                            app_p->printState = m_state;

                        }


                        break;

                    case CURSOR_1: //second location
                                                if (rxChar == 'r' && app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_2;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 55, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'l'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_0;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 55, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'u'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_4;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 31, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }
                                                else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_10;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 78, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }





                        break;

                    case CURSOR_2: // third location
                                                if (rxChar == 'l'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_1;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 55, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'u'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_3;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 31, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_11;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 78, 5);//the ball appear at a new location, which will looks like the ball just moved to the location
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }





                        break;


                    case CURSOR_3://fourth location
                                                if (rxChar == 'l'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_4;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_2;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }



                        break;

                    case CURSOR_4: //fifth location
                                                if (rxChar == 'r'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_3;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'l'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_5;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_1;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                        break;

                    case CURSOR_5: //sixth location
                                                if (rxChar == 'r'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_4;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 43, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'l'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_6;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 43, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 21, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_0;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 43, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                        break;

                    case CURSOR_6://seventh location
                                                if (rxChar == 'r'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_5;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 31, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'd'&& app_p->printState == e_state)//the ball will move to required location of other balls.
                                                {
                                                    app_p->cursor = CURSOR_7;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 31, 5);//the ball will move to required location of other balls.
                                                    GFX_drawSolidCircle(&hal_p->gfx, 21, 55, 5);//the ball will move to required location of other balls.
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                      break;




                    case CURSOR_7: // eighth location
                                                if (rxChar == 'r'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_0;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                                                else if (rxChar == 'u'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_6;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 21, 31, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }
                                                else if (rxChar == 'd'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_8;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 55, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 21, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                     break;

                    case CURSOR_8://ninth location
                                                if (rxChar == 'r'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_9;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }



                                                else if (rxChar == 'u'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_7;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 21, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 21, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }



                        break;

                    case CURSOR_9://tenth location
                                                if (rxChar == 'r'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_10;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 43, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'l'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_8;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 43, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 21, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'u'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_0;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 43, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                        break;

                    case CURSOR_10://eleventh location
                                                if (rxChar == 'r'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_11;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'l'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_9;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 43, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }

                                                else if (rxChar == 'u'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_1;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 65, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;
                                                }


                         break;

                    case CURSOR_11://twelves location
                                                if (rxChar == 'l'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_10;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 65, 78, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;

                                                }

                                                else if (rxChar == 'u'&& app_p->printState == e_state)
                                                {
                                                    app_p->cursor = CURSOR_2;
                                                    GFX_removeSolidCircle(&hal_p->gfx, 87, 78, 5);
                                                    GFX_drawSolidCircle(&hal_p->gfx, 87, 55, 5);
                                                    qianhui1(app_p, hal_p);
                                                    qianhui(app_p,hal_p);
                                                    app_p->printState = m_state;

                                                }

                        break;

                    default:
                        break;
                }

               // display the ypdated variable value on setted location on LCD
               char buffer4[15];
               snprintf(buffer4, 15, "%02d", Move);
               GFX_print(&hal_p->gfx, buffer4, 14, 18);


               // reset experience value since get into the next level. the calculation based on {(2 * level) + 1} = experience.
               if (Exp == (2 * lvl + 1))
               {
                   Exp = 0;
                   lvl = lvl + 1;
               }

               // display the ypdated variable value on setted location on LCD
                  char buffer3[15];
                  snprintf(buffer3, 15, "%02d", lvl);
                  GFX_print(&hal_p->gfx, buffer3, 1, 4);

                  // display the ypdated variable value on setted location on LCD
                  char buffer2[15];
                  snprintf(buffer2, 15, "%02d", Exp);
                  GFX_print(&hal_p->gfx, buffer2, 0, 4);

                  // display the ypdated variable value on setted location on LCD
                  tonext =  (2 * lvl) + 1;
                  char buffer6[15];
                  snprintf(buffer6, 15, "%02d", tonext);
                  GFX_print(&hal_p->gfx, buffer6, 1, 17);





                  //as energy = 0 finally, the game screen will automatically get into game over screen
                   if (Energy == 0){
                       app_p->state = RESULT_SCREEN;
                       Application_showResultScreen(app_p, hal_p);
                   }

                   ////////////////////////

                   ecolor(app_p, hal_p);



    }

}




//////////////////////////////////////////

/**
 * Callback function for when the game is in the RESULT_SCREEN state. Used to
 * break down the main App_GuessTheColor_loop() function into smaller
 * sub-functions.
 */
void Application_handleResultScreen(Application* app_p, HAL* hal_p)
{
    // Transition to title and reset game variables when the timer expires

    ////////////////////////////////////////////////////////////////////////////////////
    if (UART_hasChar(&hal_p->uart))
    {
        app_p->state = TITLE_SCREEN;
        Application_initGameVariables(app_p, hal_p);

        Application_showTitleScreen(&hal_p->gfx);
    }

}

/**
 * A helper function which clears the screen and prints the instructions for how
 * to play the game.
 */
void Application_showInstructionsScreen(Application* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);

    // Display the text
    GFX_print(gfx_p, "Now play the game   ", 0, 0);
    GFX_print(gfx_p, "press any key to     ", 1, 0);
    GFX_print(gfx_p, "start  ", 2, 0);
    GFX_print(gfx_p, "now enter game", 3, 0);


}

/**
 * A helper function which clears the screen and draws an updated display of
 * each color and its selection, intended for use when setting up the
 * GAME_SCREEN state.
 */
void Application_showGameScreen(Application* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //show the initial game screen content
    GFX_print(gfx_p, "Exp:00    ", 0, 0);
    GFX_print(gfx_p, "lvl:00     ", 1, 0);
    GFX_print(gfx_p, "Energy:01  ", 0, 10);
    GFX_print(gfx_p, "tonext:01     ", 1, 10);
    GFX_print(gfx_p, "input:   ", 13, 0);
    GFX_print(gfx_p, "Move:00   ", 14, 13);

    // this is to built a rectangle in the center of the LCD
    Graphics_Rectangle tRectangle;
    tRectangle.xMax=100;
    tRectangle.xMin=10;
    tRectangle.yMax=90;
    tRectangle.yMin=20;
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(&gfx_p->context, &tRectangle);

    //the initial circle location
    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    Graphics_fillCircle(&gfx_p->context, 43, 55, 5);
}

// this is for update of my game screen
void Application_updateGameScreen(Application* app_p, GFX* gfx_p)
{
    switch (app_p->cursor)
            {
                case CURSOR_0://first circle location, set the background and fill a cirlcle to make it looks like a ball
                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                    Graphics_fillCircle(&gfx_p->context, 43, 55, 5);
                    break;

                case CURSOR_1://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 43, 55, 5);
                                    break;

                case CURSOR_2://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 65, 55, 5);
                                    break;

                case CURSOR_3://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 87, 55, 5);
                                    break;

                case CURSOR_4://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 87, 31, 5);
                                    break;

                case CURSOR_5://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 65, 31, 5);
                                    break;

                case CURSOR_6://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 21, 31, 5);
                                    break;

                case CURSOR_7://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 21, 55, 5);
                                    break;

                case CURSOR_8://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 21, 78, 5);
                                    break;

                case CURSOR_9://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 43, 78, 5);
                                    break;

                case CURSOR_10://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 65, 78, 5);
                                    break;

                case CURSOR_11://first circle location, set the background and fill a cirlcle to make it looks like a ball
                                    Graphics_setBackgroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                                    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
                                    Graphics_fillCircle(&gfx_p->context, 87, 78, 5);
                                    break;



                default:
                    break;
            }

}

void Application_showResultScreen(Application* app_p, HAL* hal_p)
{
    // Print the splash text
    GFX_clear(&hal_p->gfx);
    GFX_print(&hal_p->gfx, "GAME OVER            ", 2, 0);
    GFX_print(&hal_p->gfx, "Highest Level: ", 3, 0);
    char buffer3[15];
                      snprintf(buffer3, 15, "%02d", lvl);
                      GFX_print(&hal_p->gfx, buffer3, 3, 17);



    // Determine if each selection matched correctly
    bool match = app_p->redSelected   == LED_isLit(&hal_p->boosterpackRed  )
              && app_p->greenSelected == LED_isLit(&hal_p->boosterpackGreen)
              && app_p->blueSelected  == LED_isLit(&hal_p->boosterpackBlue );


        Graphics_drawImage((const Graphics_Context *)&hal_p->gfx, &sad8BPP_UNCOMP, 60, 60);// i copied this from the previous example.



}




/**
 * Updates which LEDs are lit and what baud rate the UART module communicates
 * with, based on what the application's baud choice is at the time this
 * function is called.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_updateCommunications(Application* app_p, HAL* hal_p)// this function is what we finished in homework6.
{
    // When this application first loops, the proper LEDs aren't lit. The
    // firstCall flag is used to ensure that the
    /*if (app_p->firstCall) {
        app_p->firstCall = false;
    }

    // When Boosterpack S1 is tapped, circularly increment which baud rate is used.
    else
    {
        uint32_t newBaudNumber = CircularIncrement((uint32_t) app_p->baudChoice, NUM_BAUD_CHOICES);
        app_p->baudChoice = (UART_Baudrate) newBaudNumber;
    }

    // Start/update the baud rate according to the one set above.
    UART_updateBaud(&hal_p->uart, app_p->baudChoice);*/






    // Based on the new application choice, turn on the correct LED.
    // To make your life easier, we recommend turning off all LEDs before
    // selectively turning back on only the LEDs that need to be relit.
    // -------------------------------------------------------------------------
    LED_turnOff(&hal_p->boosterpackRed);
    LED_turnOff(&hal_p->boosterpackGreen);
    LED_turnOff(&hal_p->boosterpackBlue);


    switch (app_p->baudChoice)
    {
        // When the baud rate is 9600, turn on Boosterpack LED Red
        case BAUD_9600:
            LED_turnOn(&hal_p->boosterpackRed);
            break;


        case BAUD_19200:
            LED_turnOn(&hal_p->boosterpackGreen);
            break;


        case BAUD_38400:
            LED_turnOn(&hal_p->boosterpackBlue);
            break;


        case BAUD_57600:
            LED_turnOn(&hal_p->boosterpackRed);
            LED_turnOn(&hal_p->boosterpackGreen);
            LED_turnOn(&hal_p->boosterpackBlue);

            break;

        // In the default case, this program will do nothing.
        default:
            break;
    }
}

/**
 * Interprets a character which was incoming and returns an interpretation of
 * that character. If the input character is a letter, it return L for Letter, if a number
 * return N for Number, and if something else, it return O for Other.
 *
 * @param rxChar: Input character
 * @return :  Output character
 */




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this the function i built to type orders on MobaXterm which will move the ball in the rectangle.
void Application_input(Application* app_p, HAL* hal_p){
    switch(app_p->printState){
    case m_state: //in the first case, this is the first location i need to fill is m.
    if (rxChar == 'm'){
        app_p->printState = mo_state; //show location for next state
    }
    else if (rxChar == 'f') { // if type in f, the explorer will feed in energy and F automatically appear on mobaXterm in follow
        txChar = 'F';//then the state back to initial m_state.
    }
    else {//if no required character typed, display U on mobaXterm.
        txChar = 'U'; app_p->printState = m_state; // U appeared, back to the first state.
    }
    break;

    case mo_state: //second state
        if (rxChar == 'v'){ //type in v,
            app_p->printState = s_state; //show location for next state
        }
        else if(rxChar != 'v'){//if not v, U appear
            txChar = 'U'; app_p->printState = m_state;//back to first state.
        }
        break;

    case s_state:// third state
        if (rxChar == ' '){
            app_p->printState = move_state;//show location for next state
        }
        else {//if not v, U appear
            txChar = 'U';
            app_p->printState = m_state;//back to first state.
        }
        break;

    case move_state: // in this state, i will type four different characters which represent four direction of the ball.
        if (rxChar == 'l'){//type the required character, M appear
            txChar = 'M'; app_p->printState = e_state; //type in character correctly, move to e_state which will lead to another line to start in first state
        }
        else if (rxChar == 'r'){
            txChar = 'M'; app_p->printState = e_state;//type in character correctly, move to e_state which will lead to another line to start in first state
        }
        else if (rxChar == 'u'){
            txChar = 'M'; app_p->printState = e_state;//type in character correctly, move to e_state which will lead to another line to start in first state
        }
        else if (rxChar == 'd'){
                txChar = 'M'; app_p->printState = e_state;//type in character correctly, move to e_state which will lead to another line to start in first state
            }
            else {
                        txChar = 'U'; app_p->printState = m_state;//type in character correctly, move to e_state which will lead to another line to start in first state
                    }
        break;
    case e_state:
         break;
    default:
        break;
     }
}


#include <stdio.h>
#include "cprocessing.h"

#define GOL_GRID_COLS 30
#define GOL_GRID_ROWS 30
#define GOL_GRID_BUFFERS 2

#define GOL_ALIVE 1
#define GOL_DEAD 0

#define TRUE 1
#define FALSE 0

//Declaring Variables
int gIsPaused;
int gGrids[GOL_GRID_BUFFERS][GOL_GRID_ROWS][GOL_GRID_COLS];
CP_Color Grey; CP_Color White; 
float mouse_positionX; float mouse_positionY; CP_Vector mouse_position;
int cell_row; int cell_col;

//Defining function of neighbours
int neighbours(int row, int col) {
    int x1; int x2; int x3; int Ncells;
    //if ((((col - 1) >= 0) && ((row - 1) >= 0) && ((col + 1) < 30) && ((row + 1) < 30))) {
    if (((col >= 0) && (row >= 0) && (col < 30) && (row < 30))) {
        x1 = gGrids[0][row - 1][col] + gGrids[0][row - 1][col - 1] + gGrids[0][row - 1][col + 1];
        x2 = gGrids[0][row][col - 1] + gGrids[0][row][col + 1];
        x3 = gGrids[0][row + 1][col] + gGrids[0][row + 1][col - 1] + gGrids[0][row + 1][col + 1];
        Ncells = x1 + x2 + x3;
        return Ncells;
    }
}
    
//diplay and render grid variables
int display = 1;
int reference = 0;

/* Feel free to declare your own variables here */
// declaring width and height
int w; int h;
void game_init(void)
{
    // Color of cells
    Grey = CP_Color_Create(128, 128, 128, 255);
    White = CP_Color_Create(255, 255, 255, 255);
    /* Set every grids' cells as 'dead' */
    for (int row = 0; row < GOL_GRID_ROWS; ++row) {
        for (int col = 0; col < GOL_GRID_COLS; ++col) {
            for (int i = 0; i < GOL_GRID_BUFFERS; ++i) {
                gGrids[i][row][col] = GOL_DEAD;
            }
        }
    }

    /*********************************************************
    *  Let's start with gGrids[0] as the 'reference grid'
    *  and gGrids[1] as the 'displaying grid'.
    *
    *  We initialize gGrids[0] with a simple 'glider'.
    *********************************************************/

    //Setting the glider in the display.
    gGrids[1][1][2] = GOL_ALIVE;
    gGrids[1][2][3] = GOL_ALIVE;
    gGrids[1][3][1] = GOL_ALIVE;
    gGrids[1][3][2] = GOL_ALIVE;
    gGrids[1][3][3] = GOL_ALIVE;

    CP_System_SetWindowSize(900, 900);
    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */
    //cell width and height
    w = CP_System_GetWindowWidth() / GOL_GRID_COLS;
    h = CP_System_GetWindowHeight() / GOL_GRID_ROWS;
}

void game_update(void)
{
    //For mouse postions
    mouse_positionX = CP_Input_GetMouseX();
    mouse_positionY = CP_Input_GetMouseY();
    mouse_position = CP_Vector_Set(mouse_positionX,mouse_positionY);
    
    // Toggle pause state
    if (CP_Input_KeyTriggered(KEY_ANY)) {
        gIsPaused = !gIsPaused;
    }
    // Toggle cell state
    if (gIsPaused)
    {
            if (0 <= mouse_positionX && mouse_positionX <= 900 && 0 <= mouse_positionY && mouse_positionY <= 900)
            {
                cell_row = CP_Input_GetMouseY() / h;
                cell_col = CP_Input_GetMouseX() / w;
                if (CP_Input_MouseClicked())
                    gGrids[1][cell_row][cell_col] = !gGrids[1][cell_row][cell_col];
            }
    }
    else {
        //Simulation case
        for (int row = 0; row < GOL_GRID_ROWS; ++row) {
            for (int col = 0; col < GOL_GRID_COLS; ++col) {
                if (gGrids[0][row][col] == GOL_ALIVE) {
                    if (neighbours(row, col) < 2) {
                        gGrids[1][row][col] = GOL_DEAD;
                    }
                    else {
                        if (neighbours(row, col) > 3) {
                            gGrids[1][row][col] = GOL_DEAD;
                        }
                    }
                }
                else {
                    if (gGrids[0][row][col] == GOL_DEAD) {
                        if (neighbours(row, col) == 3) {
                            gGrids[1][row][col] = GOL_ALIVE;
                        }
                    }
                }
            }
        }
    }
    // Draw rectangles
    for (int row = 0; row < GOL_GRID_ROWS; ++row) 
    {
        for (int col = 0; col < GOL_GRID_COLS; ++col) 
        {
            gGrids[0][row][col] = gGrids[1][row][col];
            if (gGrids[1][row][col] == GOL_DEAD)
            {
                CP_Settings_Fill(Grey);
            }
            else if (gGrids[1][row][col] == GOL_ALIVE)
            {
                CP_Settings_Fill(White);
            }
            CP_Graphics_DrawRect(col* w, row* h, w, h);
        }
    }
}

void game_exit(void)
{

}
// Include necessary headers
#include <iostream>
#include <ctime> // for time()
#include "util.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <fstream>
#include <string>
#define PI 3.14159265358979323846
using namespace std;

//const int GRID_SIZE = 8;
struct GameState {
    int arr[8][8];
    int selectedIndex = 0;
    int userscore = 0;
    int moves=30;
    string playerName;
};

// Function declarations
void drawMenu();

void handleKeypress(unsigned char key, int x, int y);
void handleSpecialKeypress(int key, int x, int y);
void levels();
void SetCanvasSize(int width, int height);
int getRandomValue();
void DrawGrid();
bool CheckForMatches();
void MousePressedAndMoved(int x, int y);
void GameDisplay();
void MouseClicked(int button, int state, int x, int y);
void NonPrintableKeys(int key, int x, int y);
void PrintableKeys(unsigned char key, int x, int y);
void Timer(int m);
void scoring();
int getRandomValue();
void playername();
void DrawHexagon(int pixelsX, int pixelsY, int size, float color[]);
void DrawDiamond(int pixelsX, int pixelsY, int size, float color[]);
void moveCandiesDown();
void saveGameState();
void loadGameState();



// Global variable (minimized to one)
GameState gameState;
int main(int argc, char *argv[])
{
    int SCREEN_WIDTH = 1020, SCREEN_HEIGHT = 840; // i have set my window size to be 1020 x 840
    srand(time(nullptr));                         // Seed the random number generator with current time
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("YOUR CRUSH");
    SetCanvasSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    glutDisplayFunc(drawMenu);
    // Mouse function
    // glutMouseFunc(handleMouseClick);
    glutMotionFunc(MousePressedAndMoved);

    // keyboard function
    glutKeyboardFunc(handleKeypress);       // tell library which function to call for non-printable ASCII characters
    glutSpecialFunc(handleSpecialKeypress); // tell library which function to call for printable ASCII characters
    // This function tells the library to call our Timer function after 1000.0/12.0 FPS milliseconds...
    // The game is working on 12 FPS
    glutTimerFunc(1000.0 / 12.0, Timer, 0);
    glutMainLoop();
    return 0;

    //
    // glutDisplayFunc(drawMenu);
    // glutKeyboardFunc(handleKeypress);
    // glutTimerFunc(1000.0 / 12.0, Timer, 0);
    // glutMainLoop();
}

void DrawDiamond(int pixelsX, int pixelsY, int size, float color[])
{
    // Set the color
    glColor3fv(color);

    // Calculate the vertices of the diamond
    float halfSize = size / 2.0f;
    float topVertex[2] = {pixelsX, pixelsY + halfSize};
    float bottomVertex[2] = {pixelsX, pixelsY - halfSize};
    float leftVertex[2] = {pixelsX - halfSize, pixelsY};
    float rightVertex[2] = {pixelsX + halfSize, pixelsY};

    // Draw the diamond with two intersecting triangles
    glBegin(GL_TRIANGLES);
    // Triangle 1 (top-left)
    glVertex2fv(topVertex);
    glVertex2fv(leftVertex);
    glVertex2fv(bottomVertex);
    // Triangle 2 (top-right)
    glVertex2fv(topVertex);
    glVertex2fv(rightVertex);
    glVertex2fv(bottomVertex);
    glEnd();
}
void DrawHexagon(int pixelsX, int pixelsY, int size, float color[])
{
    // Set the color
    glColor3fv(color);

    // Calculate the vertices of the hexagon
    float angle = 0;
    float centerX = pixelsX;
    float centerY = pixelsY;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i)
    {
        float x = centerX + size * cos(angle);
        float y = centerY + size * sin(angle);
        glVertex2f(x, y);
        angle += 2 * PI / 6;
    }
    glEnd();
}

void SetCanvasSize(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int getRandomValue()
{
    static bool seeded = false; // Static variable to ensure seeding happens only once
    if (!seeded)
    {
        srand(time(0)); // Seed the random number generator with current time
        seeded = true;  // Mark the random number generator as seeded
    }
    return rand() % 5 + 1; // Generate a random value between 1 and 2
}

void redraw()
{
    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 600;
    const int GRID_SIZE = 8;
    const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int pixelsX = (i * CELL_SIZE) + 200;
            int pixelsY = (j * CELL_SIZE) + 200;

            if (gameState.arr[i][j] > 5 || gameState.arr[i][j] < 1)
            {
                int k = getRandomValue();
                gameState.arr[i][j] = k;
                if (k == 1)
                {
                    DrawSquare(pixelsX - 15, pixelsY - 15, 35, colors[BLUE]);
                }
                else if (k == 2)
                {
                    DrawTriangle(pixelsX - 20, pixelsY - 10, pixelsX, pixelsY + 30, pixelsX + 20, pixelsY - 10, colors[GREEN]);
                }
                else if (k == 3)
                {
                    DrawDiamond(pixelsX, pixelsY + 10, 40, colors[RED]);
                }
                else if (k == 4)
                {
                    DrawHexagon(pixelsX, pixelsY, 20, colors[MEDIUM_ORCHID]);
                }
                else if (k == 5)
                {
                    DrawCircle(pixelsX, pixelsY, 20, colors[BROWN]);
                }
            }
        }
    }
}

void scoring()
{
    // static int userscore=0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            // Horizontal check
            if ((gameState.arr[i][j] == gameState.arr[i][j + 1]) && (gameState.arr[i][j + 1] == gameState.arr[i][j + 2]))
            {
                if (i == 7 || i == 6)
                {
                    //cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                    gameState.userscore += 20;
                }

                else if (i == 5)
                {
                    //cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                
                    gameState.arr[i + 1][j] = gameState.arr[i + 2][j];
                    gameState.arr[i + 1][j + 1] = gameState.arr[i + 2][j + 1];
                    gameState.arr[i + 1][j + 2] = gameState.arr[i + 2][j + 2];

                    gameState.userscore += 20;
                }

                else if (i == 4)
                {
                  //  cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                    
                    gameState.arr[i + 1][j] = gameState.arr[i + 2][j];
                    gameState.arr[i + 1][j + 1] = gameState.arr[i + 2][j + 1];
                    gameState.arr[i + 1][j + 2] = gameState.arr[i + 2][j + 2];

                    gameState.arr[i + 2][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 2][j + 1] = gameState.arr[i + 3][j + 1];
                    gameState.arr[i + 2][j + 2] = gameState.arr[i + 3][j + 2];

                    gameState.userscore += 20;
                }

                else if (i == 3)
                {
                   // cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                
                    gameState.arr[i + 1][j] = gameState.arr[i + 2][j];
                    gameState.arr[i + 1][j + 1] = gameState.arr[i + 2][j + 1];
                    gameState.arr[i + 1][j + 2] = gameState.arr[i + 2][j + 2];

                    gameState.arr[i + 2][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 2][j + 1] = gameState.arr[i + 3][j + 1];
                    gameState.arr[i + 2][j + 2] = gameState.arr[i + 3][j + 2];

                    gameState.arr[i + 3][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 3][j + 1] = gameState.arr[i + 4][j + 1];
                    gameState.arr[i + 3][j + 2] = gameState.arr[i + 4][j + 2];

                    gameState.userscore += 20;
                }

                else if (i == 2)
                {
                    //cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                 
                    gameState.arr[i + 1][j] = gameState.arr[i + 2][j];
                    gameState.arr[i + 1][j + 1] = gameState.arr[i + 2][j + 1];
                    gameState.arr[i + 1][j + 2] = gameState.arr[i + 2][j + 2];

                    gameState.arr[i + 2][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 2][j + 1] = gameState.arr[i + 3][j + 1];
                    gameState.arr[i + 2][j + 2] = gameState.arr[i + 3][j + 2];

                    gameState.arr[i + 3][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 3][j + 1] = gameState.arr[i + 4][j + 1];
                    gameState.arr[i + 3][j + 2] = gameState.arr[i + 4][j + 2];

                    gameState.arr[i + 4][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 4][j + 1] = gameState.arr[i + 5][j + 1];
                    gameState.arr[i + 4][j + 2] = gameState.arr[i + 5][j + 2];

                    gameState.userscore += 20;
                }

                else if (i == 1)
                {
                   // cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                  
                    gameState.arr[i + 1][j] = gameState.arr[i + 2][j];
                    gameState.arr[i + 1][j + 1] = gameState.arr[i + 2][j + 1];
                    gameState.arr[i + 1][j + 2] = gameState.arr[i + 2][j + 2];

                    gameState.arr[i + 2][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 2][j + 1] = gameState.arr[i + 3][j + 1];
                    gameState.arr[i + 2][j + 2] = gameState.arr[i + 3][j + 2];

                    gameState.arr[i + 3][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 3][j + 1] = gameState.arr[i + 4][j + 1];
                    gameState.arr[i + 3][j + 2] = gameState.arr[i + 4][j + 2];

                    gameState.arr[i + 4][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 4][j + 1] = gameState.arr[i + 5][j + 1];
                    gameState.arr[i + 4][j + 2] = gameState.arr[i + 5][j + 2];

                    gameState.arr[i + 5][j] = gameState.arr[i + 6][j];
                    gameState.arr[i + 5][j + 1] = gameState.arr[i + 6][j + 1];
                    gameState.arr[i + 5][j + 2] = gameState.arr[i + 6][j + 2];

                    gameState.userscore += 20;
                }

                else if (i == 0)
                {
                  //  cout << "three Same" << endl;
                    gameState.arr[i][j] = gameState.arr[i + 1][j];
                    gameState.arr[i][j + 1] = gameState.arr[i + 1][j + 1];
                    gameState.arr[i][j + 2] = gameState.arr[i + 1][j + 2];
                    // temp=gameState.arr[i+1][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 2][j];
                    gameState.arr[i + 1][j + 1] = gameState.arr[i + 2][j + 1];
                    gameState.arr[i + 1][j + 2] = gameState.arr[i + 2][j + 2];

                    gameState.arr[i + 2][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 2][j + 1] = gameState.arr[i + 3][j + 1];
                    gameState.arr[i + 2][j + 2] = gameState.arr[i + 3][j + 2];

                    gameState.arr[i + 3][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 3][j + 1] = gameState.arr[i + 4][j + 1];
                    gameState.arr[i + 3][j + 2] = gameState.arr[i + 4][j + 2];

                    gameState.arr[i + 4][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 4][j + 1] = gameState.arr[i + 5][j + 1];
                    gameState.arr[i + 4][j + 2] = gameState.arr[i + 5][j + 2];

                    gameState.arr[i + 5][j] = gameState.arr[i + 6][j];
                    gameState.arr[i + 5][j + 1] = gameState.arr[i + 6][j + 1];
                    gameState.arr[i + 5][j + 2] = gameState.arr[i + 6][j + 2];

                    gameState.arr[i + 6][j] = gameState.arr[i + 7][j];
                    gameState.arr[i + 6][j + 1] = gameState.arr[i + 7][j + 1];
                    gameState.arr[i + 6][j + 2] = gameState.arr[i + 7][j + 2];

                    gameState.userscore += 20;
                }
            }

            // Vertical check
            if ((gameState.arr[i][j] == gameState.arr[i + 1][j]) && (gameState.arr[i + 1][j] == gameState.arr[i + 2][j]))
            {

                if (i == 5)
                {

                    gameState.arr[i][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 2][j] = gameState.arr[i + 5][j];

                    gameState.userscore += 20;
                }

                else if (i == 4)
                {

                    gameState.arr[i][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 2][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 3][j] = gameState.arr[i + 6][j];
                    gameState.userscore += 20;
                }

                else if (i == 3)
                {

                    gameState.arr[i][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 2][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 3][j] = gameState.arr[i + 6][j];
                    gameState.arr[i + 4][j] = gameState.arr[i + 7][j];
                    gameState.userscore += 20;
                }

                else if (i == 2)
                {

                    gameState.arr[i][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 2][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 3][j] = gameState.arr[i + 6][j];
                    gameState.arr[i + 4][j] = gameState.arr[i + 7][j];
                    gameState.arr[i + 5][j] = gameState.arr[i + 8][j];
                    gameState.userscore += 20;
                }

                else if (i == 1)
                {

                    gameState.arr[i][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 2][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 3][j] = gameState.arr[i + 6][j];
                    gameState.arr[i + 4][j] = gameState.arr[i + 7][j];
                    gameState.arr[i + 5][j] = gameState.arr[i + 8][j];
                    gameState.arr[i + 6][j] = gameState.arr[i + 9][j];
                    gameState.userscore += 20;
                }

                else if (i == 0)
                {

                    gameState.arr[i][j] = gameState.arr[i + 3][j];
                    gameState.arr[i + 1][j] = gameState.arr[i + 4][j];
                    gameState.arr[i + 2][j] = gameState.arr[i + 5][j];
                    gameState.arr[i + 3][j] = gameState.arr[i + 6][j];
                    gameState.arr[i + 4][j] = gameState.arr[i + 7][j];
                    gameState.arr[i + 5][j] = gameState.arr[i + 8][j];
                    gameState.arr[i + 6][j] = gameState.arr[i + 9][j];
                    gameState.arr[i + 7][j] = gameState.arr[i + 10][j];
                    gameState.userscore += 20;
                }
            }
        }
    }
    redraw();
    DrawString(46, 800, "[P]", colors[CRIMSON]);
    string scoreText = "Score: " + to_string(gameState.userscore);
    DrawRectangle(31, 720, 125, 35, colors[CRIMSON]);
    DrawString(35, 730, scoreText, colors[WHITE]);
    DrawRectangle(730, 774, 255, 35, colors[CRIMSON]);
    DrawString(740, 780, "Name:  " + gameState.playerName, colors[WHITE]);
    
    if (gameState.userscore >= 0 && gameState.userscore <= 1500)
    {   
       
        DrawRectangle(787, 720, 100, 35, colors[CRIMSON]);
        DrawString(790, 730, "Level 1", colors[WHITE]);
        DrawRectangle(310, 774, 255, 35, colors[CRIMSON]);
        DrawString(312, 780, "SCORE NEEDED : 1500", colors[WHITE]);
        
        string moves = "MOVES: " + to_string(gameState.moves);
        DrawRectangle(30, 620, 125, 35, colors[CRIMSON]);
        DrawString(32, 630, moves, colors[WHITE]);
    }


    if (gameState.userscore > 1500 && gameState.userscore <= 3600)
    {   
        DrawRectangle(787, 720, 100, 35, colors[CRIMSON]);
        DrawString(790, 730, "Level 2", colors[WHITE]);
        DrawRectangle(310, 774, 255, 35, colors[CRIMSON]);
        DrawString(312, 780, "SCORE NEEDED : 3600", colors[WHITE]);
        string moves = "MOVES: " + to_string(gameState.moves);
        DrawRectangle(30, 620, 125, 35, colors[CRIMSON]);
        DrawString(32, 630, moves, colors[WHITE]);
    }
    static int count = 0;
    if (gameState.userscore > 1500 && count == 0)
    {
        count++;
        cout << "COUNTER HAS BEEN UPDATED" << endl;
        gameState.selectedIndex = 7;
        drawMenu();
    }

}

void levels()
{

    GameDisplay();
    glutMouseFunc(MouseClicked);
}

void DrawGrid()
{
    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 600;
    const int GRID_SIZE = 8;
    const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

    static int count = 0;
    if (count > 0)
        scoring();
    int pixX[8];
    int pixY[8];

    if (count == 0)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            for (int i = 0; i < GRID_SIZE; i++)
            {
                int pixelsX = (i * CELL_SIZE) + 200;
                int pixelsY = (j * CELL_SIZE) + 200;
                pixX[i] = pixelsX;
                pixY[j] = pixelsY;

                int d = getRandomValue(); // Get a random value using our function
                gameState.arr[j][i] = d;
                if (d == 1)
                {
                    DrawSquare(pixelsX - 15, pixelsY - 15, 35, colors[BLUE]);
                }
                else if (d == 2)
                {
                    DrawTriangle(pixelsX - 20, pixelsY - 10, pixelsX, pixelsY + 30, pixelsX + 20, pixelsY - 10, colors[GREEN]);
                }
                else if (d == 3)
                {
                    DrawDiamond(pixelsX, pixelsY + 10, 40, colors[RED]);
                }
                else if (d == 4)
                {
                    DrawHexagon(pixelsX, pixelsY, 20, colors[MEDIUM_ORCHID]);
                }
                else if (d == 5)
                {
                    DrawCircle(pixelsX, pixelsY, 20, colors[BROWN]);
                }
            }
        }
    }
    if (count > 0)
    {

        for (int j = 0; j < GRID_SIZE; j++)
        {
            for (int i = 0; i < GRID_SIZE; i++)
            {
                int pixelsX = (i * CELL_SIZE) + 200;
                int pixelsY = (j * CELL_SIZE) + 200;
                if (gameState.arr[j][i] == 1)
                {
                    DrawSquare(pixelsX - 15, pixelsY - 15, 35, colors[BLUE]);
                }
                else if (gameState.arr[j][i] == 2)
                {
                    DrawTriangle(pixelsX - 20, pixelsY - 10, pixelsX, pixelsY + 30, pixelsX + 20, pixelsY - 10, colors[GREEN]);
                }
                else if (gameState.arr[j][i] == 3)
                {
                    DrawDiamond(pixelsX, pixelsY + 10, 40, colors[RED]);
                }
                else if (gameState.arr[j][i] == 4)
                {
                    DrawHexagon(pixelsX, pixelsY, 20, colors[MEDIUM_ORCHID]);
                }
                else if (gameState.arr[j][i] == 5)
                {
                    DrawCircle(pixelsX, pixelsY, 20, colors[BROWN]);
                }
            }
        }

        // scoring();
    }
    scoring();

    count++;
    // vertical lines of the grid
    DrawLine(165, 173, 165, 760, 5, colors[BROWN]);
    DrawLine(235, 173, 235, 760, 5, colors[BROWN]);
    DrawLine(315, 173, 315, 760, 5, colors[BROWN]);
    DrawLine(385, 173, 385, 760, 5, colors[BROWN]);
    DrawLine(460, 173, 460, 760, 5, colors[BROWN]);
    DrawLine(540, 173, 540, 760, 5, colors[BROWN]);
    DrawLine(610, 173, 610, 760, 5, colors[BROWN]);
    DrawLine(685, 173, 685, 760, 5, colors[BROWN]);
    DrawLine(765, 173, 765, 760, 5, colors[BROWN]);

    // horizontal lines of grid
    DrawLine(162, 173, 767, 173, 5, colors[BROWN]);
    DrawLine(0, 760, 1020, 760, 5, colors[BROWN]);

    // making a Pause Button
    DrawSquare(10, 790, 32, colors[PALE_GREEN]);
    DrawLine(17, 818, 17, 793, 7, colors[BROWN]);
    DrawLine(33, 818, 33, 793, 7, colors[BROWN]);
    
             //MKAING A HINT BUTTON
     DrawRectangle(787, 620, 100, 35, colors[CRIMSON]);
     DrawString(790, 630, "HINT[I]", colors[WHITE]);

    //TELLING THE USER START OF THE ARRAY FOR HINT
     DrawString( 140, 190, "1", colors[CRIMSON]);
     DrawString( 190, 135, "1", colors[CRIMSON]);
    
    if(gameState.moves==0 && gameState.userscore!=3600){
        gameState.selectedIndex=8;
        drawMenu();
    }

    glutSwapBuffers();
}

void MousePressedAndMoved(int x, int y)
{
    cout << x << " " << y << endl;
    glutPostRedisplay();
}

bool CheckForMatches()
{
    // horizontal matches
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8 - 2; ++j)
        {
            if (gameState.arr[i][j] == gameState.arr[i][j + 1] && gameState.arr[i][j] == gameState.arr[i][j + 2])
            {
                gameState.moves--;
                return true;
            }
        }
    }

    // vertical matches
    for (int i = 0; i < 8 - 2; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (gameState.arr[i][j] == gameState.arr[i + 1][j] && gameState.arr[i][j] == gameState.arr[i + 2][j])
            {
                gameState.moves--;
                return true;
            }
        }
    }

        gameState.moves--;
    return false;
}

void MouseClicked(int button, int state, int x, int y)
{

    static int selectedRow = -1;
    static int selectedCol = -1;
    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 600;
    const int GRID_SIZE = 8;
    const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

        int col = ((x / CELL_SIZE)) - 2;
        int row = (GRID_SIZE - 1 - (y / CELL_SIZE)) + 1;

        cout << "Clicked on row: " << row << ", col: " << col << endl<<endl<<endl;

        if (selectedRow == -1 && selectedCol == -1)
        {
            selectedRow = row;
            selectedCol = col;
        }
        else
        {
            // selected cells are adjacent
            if (abs(row - selectedRow) + abs(col - selectedCol) == 1)
            {

                int temp = gameState.arr[selectedRow][selectedCol];
                gameState.arr[selectedRow][selectedCol] = gameState.arr[row][col];
                gameState.arr[row][col] = temp;

                if (!CheckForMatches())
                {
                    // no matches found, swap back
                    temp = gameState.arr[selectedRow][selectedCol];
                    gameState.arr[selectedRow][selectedCol] = gameState.arr[row][col];
                    gameState.arr[row][col] = temp;
                }
            }

            selectedRow = -1;
            selectedCol = -1;
        }

        glutPostRedisplay();
    }
}

void GameDisplay() /**/
{

    glClearColor(0 /*Red Component*/, 0,                         // 148.0/255/*Green Component*/,
                 0.0 /*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
    glClearColor(0.0, 0.0, 0.0, 0.0);                            // Set the background color to black

    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    DrawRectangle(0, 0, 1200, 1000, colors[CYAN]);
    DrawGrid();
    glutSwapBuffers(); // do not modify this line..
}

void Timer(int m)
{

    // implement your functionality here

    // once again we tell the library to call our Timer function after next 1000/12 FPS
    // The game will be working on 12 FPS
    glutPostRedisplay();
    glutTimerFunc(1000.0 / 12.0, Timer, 0);
}

void drawMenu() {
    

		
		if(gameState.selectedIndex == 0) //Splash Screen
    {
        glClear(GL_COLOR_BUFFER_BIT);
        DrawString(440, 400,"GAMERZZ", colors[CRIMSON]);
        DrawString(460, 100, "Loading ...", colors[GOLDEN_ROD]);
        DrawCircle(435, 106, 10,colors[GOLDEN_ROD]);
        glutSwapBuffers();
        sleep(1);
        gameState.selectedIndex = 1;
       
    }
    
	 if(gameState.selectedIndex==1){
    glClear(GL_COLOR_BUFFER_BIT);


            glClear(GL_COLOR_BUFFER_BIT);
            DrawRectangle(0, 0, 1200, 1000, colors[ANTIQUE_WHITE]);
            DrawString( 440, 600, "Main Menu", colors[CRIMSON]);
            DrawLine(435, 595, 560, 595, 4, colors[CRIMSON]);
            DrawRectangle(745, 429, 201, 40, colors[DEEP_SKY_BLUE]);
            DrawString( 120, 400, "YOUR CRUSH", colors[FIREBRICK]);
            DrawString( 127, 360, "... Swap to Win ...", colors[DARK_OLIVE_GREEN]);
            DrawString ( 750, 440, "Load Game [L]",colors[WHITE]);
            DrawString( 750, 390, "New Game [G]", colors[LIGHT_SALMON]);
            DrawString( 750, 340, "HighScores [H]", colors[LIGHT_SALMON]);
            DrawString( 750, 290, "Help [T]", colors[LIGHT_SALMON]);
            DrawString( 750, 240, "Exit [E]", colors[LIGHT_SALMON]);
            DrawString( 270, 15, "Designed and Developed by i230795 AND i230713", colors[BLACK]);

    glutSwapBuffers();
    
    
}

if(gameState.selectedIndex==6){
                
            glClear(GL_COLOR_BUFFER_BIT);
            DrawRectangle(0, 0, 1200, 1000, colors[DEEP_SKY_BLUE]);
            DrawString( 400, 500, "WELCOME TRAVELER!!", colors[LIGHT_SALMON]);
            DrawString( 401, 500, "WELCOME TRAVELER!!", colors[LIGHT_SALMON]);
            DrawString( 402, 500, "WELCOME TRAVELER!!", colors[LIGHT_SALMON]);
            DrawString( 403, 500, "WELCOME TRAVELER", colors[LIGHT_SALMON]);
            DrawString( 465, 450, "LEVEL 1", colors[LIGHT_SALMON]);
            DrawString( 463, 450, "LEVEL 1", colors[LIGHT_SALMON]);
            DrawString( 464, 450, "LEVEL 1", colors[LIGHT_SALMON]);
            DrawString( 466, 450, "LEVEL 1", colors[LIGHT_SALMON]);
            DrawString( 170, 55, "YOUR GOAL IS TO REACH 1500 POINTS FOR THE FIRST LEVEL", colors[WHITE]);
            glutSwapBuffers();
            sleep(1);
            gameState.selectedIndex=4;
        }

	else if(gameState.selectedIndex == 2)
        {
            glClear(GL_COLOR_BUFFER_BIT);
           DrawRectangle(0, 0, 1200, 1000, colors[GREEN_YELLOW]);
        DrawString(500, 600, "Create a new Profile", colors[WHITE]);
        DrawLine(495, 595, 700, 595, 4, colors[WHITE]);
        DrawString(300, 400, "Enter Name: " + gameState.playerName, colors[WHITE]);
        DrawRectangle(407, 200, 185, 45, colors[CRIMSON]);
        DrawString(410, 215, "Back [Backspace]", colors[WHITE]);
        DrawRectangle(595, 200, 110, 45, colors[CRIMSON]);
        DrawString(597, 215, "Ok [Enter]", colors[WHITE]);
        DrawString(410, 80, "Carefully type in your name and then,", colors[WHITE]);
        DrawString(445, 50, "Press [Enter] to Continue ...", colors[WHITE]);
            glutSwapBuffers();
            
        }		
		
		
		
	else if(gameState.selectedIndex==3){
            
            glClear(GL_COLOR_BUFFER_BIT);
            DrawRectangle(0, 0, 1200, 1000, colors[PAPAYA_WHIP]);
            DrawString( 420, 650, "HOW TO PLAY!", colors[CRIMSON]);
            DrawLine(415, 642, 600, 642, 4, colors[CRIMSON]);
            DrawString( 120, 540, "General Instructions:", colors[DARK_SLATE_BLUE]);
            DrawString( 145, 470, "1. Player can swap gems by drag and drop using a mouse.", colors[CORAL]);
            DrawString( 145, 410, "2. Players main goal should be to form chains of 3 or more of gems by swapping.", colors[CORAL]);
            DrawString( 145, 350, "3. Press [P] to pause game during the gameplay.", colors[CORAL]);
            DrawString( 145, 290, "4. Each level has a target score required to proceed to next level.", colors[CORAL]);
            DrawString( 145, 220, "5. There is a hint button in the game but it will deduct 400pts.", colors[CORAL]);
            DrawRectangle(443, 40, 110, 45, colors[CRIMSON]);
            DrawString( 448, 55, "Back [B]", colors[WHITE]);
            glutSwapBuffers();
        }

    
    
    else if(gameState.selectedIndex==5){
            glClearColor(0.98, 0.98, 0.98, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            DrawRectangle(0, 0, 1200, 1000, colors[CYAN]);
            DrawString( 465, 650, "PAUSE", colors[CRIMSON]);
            DrawLine(455, 642, 550, 642, 4, colors[CRIMSON]);
            DrawString( 145, 470, "1. CONTINUE[C]", colors[DARK_GREEN]);
            DrawString( 145, 410, "2. MAIN MENU[M]", colors[DARK_GREEN]);
            DrawString( 145, 350, "3. EXIT[E]", colors[DARK_GREEN]);
            DrawRectangle(443, 40, 110, 45, colors[CRIMSON]);
            DrawString( 450, 55, "Back [B]", colors[WHITE]);
            glutSwapBuffers();
        }
    if(gameState.selectedIndex==7){
        glClear(GL_COLOR_BUFFER_BIT);
            DrawRectangle(0, 0, 1200, 1000, colors[DEEP_SKY_BLUE]);
            DrawString( 400, 500, "Congratulation on completing Level 1", colors[LIGHT_SALMON]);
            DrawString( 401, 500, "Congratulation on completing Level 1", colors[LIGHT_SALMON]);
            DrawString( 402, 500, "Congratulation on completing Level 1", colors[LIGHT_SALMON]);
            DrawString( 465, 450, "LEVEL 2", colors[LIGHT_SALMON]);
            DrawString( 463, 450, "LEVEL 2", colors[LIGHT_SALMON]);
            DrawString( 464, 450, "LEVEL 2", colors[LIGHT_SALMON]);
            DrawString( 450, 55, "Saving your Prograss...", colors[WHITE]);
            glutSwapBuffers();
            sleep(1);
            gameState.selectedIndex=4;
            drawMenu();
    }

        if(gameState.selectedIndex==8){
        glClear(GL_COLOR_BUFFER_BIT);
            DrawRectangle(0, 0, 1200, 1000, colors[DEEP_SKY_BLUE]);
            DrawString( 435, 420, "Game Over", colors[LIGHT_SALMON]);
            DrawString( 436, 420, "Game Over", colors[LIGHT_SALMON]);
            DrawString( 437, 420, "Game Over", colors[LIGHT_SALMON]);
            DrawString( 450, 55, "RETURNING TO MAIN MENU...", colors[WHITE]);
            glutSwapBuffers();
            sleep(1);
            gameState.selectedIndex=1;
            drawMenu();
    }

        if(gameState.selectedIndex==4){
        levels();

    }
}

void hint(){
         // horizontal matches
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8 - 2; ++j) {
            if (gameState.arr[i][j] == gameState.arr[i][j+1]) {
                //Check if any side of the have the same candy
                if(gameState.arr[i][j] == gameState.arr[i][j-1]){
                    cout<<"\nCHECK AROUND THE ROW "<<i+1<<" Coloumn "<<j<<endl;
                }
                else if(gameState.arr[i][j]==gameState.arr[i+1][j-1]){
                    cout<<"\nCHECK AROUND THE ROW "<<i+2<<" Coloumn "<<j<<endl;
                }
                else if(gameState.arr[i][j]==gameState.arr[i-1][j-1]){
                    cout<<"\nCHECK AROUND THE ROW "<<i<<" Coloumn "<<j<<endl;
                }

         
            }
        }
    }
    

}


// Function to handle key presses
void handleKeypress(unsigned char key, int x, int y)
{
    if (gameState.selectedIndex == 2 && ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || key == ' '))
    {
        gameState.playerName += key;
        drawMenu();
        return;
    }
    switch (key)
    {
    case 27: // ESC key
        exit(0);
        break;

    case 'G': // New Game
    case 'g':
        gameState.selectedIndex = 2;
        gameState.playerName = ""; // Reset player name
        gameState.userscore = 0;   // Reset score
        drawMenu();
        break;

    case 'L': // Load Game
    case 'l':
        loadGameState();
        gameState.selectedIndex = 4;
        drawMenu();
        break;

    case 'H': // High Scores
    case 'h':
    	if(gameState.selectedIndex==1)
        cout << "HIGH SCORE IS PRESSED!" << endl;
        // Additional functionality to show high scores
        break;

    case 'T': // Help
    case 't':
        gameState.selectedIndex = 3;
        drawMenu();
        break;

    case 8: // Backspace for player name
        if (!gameState.playerName.empty())
            gameState.playerName.pop_back();
        gameState.selectedIndex=1;
        drawMenu();
        
        break;

    case 13: // Enter to start the game
        if (gameState.selectedIndex == 2 && !gameState.playerName.empty())
        {
            gameState.selectedIndex = 4;
            drawMenu();
        }
        break;
        
    case 'I': // High Scores
    case 'i':
    	if(gameState.userscore>400)
        gameState.userscore-=400;
		hint();
        // Additional functionality to show high scores
        break;
        
       

    case 'B': // Back
    case 'b':
        if (gameState.selectedIndex == 3)
        {
            gameState.selectedIndex = 1;
            drawMenu();
        }
        if (gameState.selectedIndex == 5)
        {
            gameState.selectedIndex = 4;
            drawMenu();
        }
        break;

    case 'C': // Back
    case 'c':
        if (gameState.selectedIndex == 5)
        {
            gameState.selectedIndex = 4;
            drawMenu();
        }
        break;

    case 'P': // Pause
    case 'p':
        if (gameState.selectedIndex == 4)
        {
            gameState.selectedIndex = 5;
            drawMenu();
        }
        break;

    case 'M': // Main Menu
    case 'm':
        gameState.selectedIndex = 1;
        drawMenu();
        break;

    case 'S': // Save Game
    case 's':
        saveGameState();
        cout << "Game state saved!" << endl;
        break;

    case 'E': // Exit Game
    case 'e':
        if (gameState.selectedIndex == 1 || gameState.selectedIndex == 5)
            exit(0);
        break;

    } // switch closingbracket

    glutPostRedisplay();
}

// Function to handle special key presses (arrow keys)
void handleSpecialKeypress(int key, int x, int y)
{

    glutPostRedisplay(); // Request a redraw
}

// Save game state to file
void saveGameState()
{
	const int GRID_SIZE=8;
    ofstream outFile("gameState.txt");
    if (outFile)
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                outFile << gameState.arr[i][j] << " ";
            }
            outFile << endl;
        }
        outFile << gameState.userscore << endl;
        outFile << gameState.playerName << endl;
        outFile.close();
    }
    else
    {
        cout << "Unable to save game state." << endl;
    }
}

// Load game state from file
void loadGameState()
{	
const int GRID_SIZE=8;
    ifstream inFile("gameState.txt");
    if (inFile)
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                inFile >> gameState.arr[i][j];
            }
        }
        inFile >> gameState.userscore;
        inFile >> gameState.playerName;
        inFile.close();
        cout << "Game state loaded." << endl;
        cout << "Score: " << gameState.userscore << ", Player Name: " << gameState.playerName << endl;
    }
    else
    {
        cout << "Unable to load game state." << endl;
    }
}

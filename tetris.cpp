#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()

using namespace std;

// Grid dimensions
const int GRID_WIDTH = 14;
const int GRID_HEIGHT = 18;
int score = 0;
int initialTime = 500;

// Define the grid
vector<vector<int>> grid(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));

// Define different Tetris blocks
vector<vector<vector<int>>> blocks = {
    {{1, 1, 1, 1}},         // I-shape
    {{1, 1}, {1, 1}},       // O-shape
    {{0, 1, 0}, {1, 1, 1}}, // T-shape
    {{1, 0, 0}, {1, 1, 1}}, // L-shape
    {{0, 0, 1}, {1, 1, 1}}, // J-shape
    {{1, 1, 0}, {0, 1, 1}}, // S-shape
    {{0, 1, 1}, {1, 1, 0}}  // Z-shape
};

// Current block
vector<vector<int>> block;
int blockX = 4, blockY = 0;

// Function to randomly select a block
void selectRandomBlock()
{
    block = blocks[rand() % blocks.size()];
    blockX = (GRID_WIDTH - block[0].size()) / 2; // Center the block horizontally
    blockY = 0;                                  // Start at the top
}

// Function to print the grid
void printGrid()
{
    system("cls"); // Clear the console (Windows)
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            bool isBlockCell = false;
            for (int by = 0; by < block.size(); ++by)
            {
                for (int bx = 0; bx < block[by].size(); ++bx)
                {
                    if (block[by][bx] && x == blockX + bx && y == blockY + by)
                    {
                        isBlockCell = true;
                        break;
                    }
                }
            }
            if (isBlockCell)
            {
                cout << "[]"; // Print block
            }
            else if (grid[y][x] != 0)
            {
                cout << "[]"; // Print filled cell
            }
            else
            {
                cout << " ."; // Print empty cell
            }
        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
}

// Check if the block can move down
bool canMoveDown()
{
    for (int y = 0; y < block.size(); ++y)
    {
        for (int x = 0; x < block[y].size(); ++x)
        {
            if (block[y][x])
            {
                int newY = blockY + y + 1;
                int newX = blockX + x;
                if (newY >= GRID_HEIGHT || grid[newY][newX] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// Lock the block into the grid
void lockBlock()
{
    for (int y = 0; y < block.size(); ++y)
    {
        for (int x = 0; x < block[y].size(); ++x)
        {
            if (block[y][x])
            {
                grid[blockY + y][blockX + x] = 1;
            }
        }
    }
}

void checkScoreChange()
{
    vector<int> newRow(GRID_WIDTH, 0); // insert new empty row if needed

    for (int i = GRID_HEIGHT - 1; i >= 0; i--)
    {
        int emptySpace = 0;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid[i][j] == 0)
            {
                emptySpace++;
            }
        }

        if (emptySpace == GRID_WIDTH)
        {
            break;
        }

        else if (emptySpace == 0)
        {
            score++;
            grid.erase(grid.begin() + i);
            grid.insert(grid.begin(), newRow);
            i++; // Check the same row again after shifting
        }
    }
}

bool checkGameOver()
{
    for (int j = 0; j < GRID_WIDTH; j++)
    {
        if (grid[0][j] != 0) // Check if the 0th row has any filled cell
        {
            return true; // Game over
        }
    }
    return false; // Game continues
}

// Main game loop
int main()
{
    srand(time(0));      // Seed random number generator
    selectRandomBlock(); // Choose the first block

    cout << "\n Use the 'a' and 'd' keys to move left and right respectively.\nUse the 's' key to increase speed of downfall.\nNOTE: Please tap the buttons and do not hold them down.\n";

    while (true)
    {
        printGrid();

        // Check if block can move down
        if (canMoveDown())
        {
            ++blockY;
        }
        else
        {

            lockBlock();
            checkScoreChange();
            selectRandomBlock(); // Select a new block
        }

        // Handle user input
        if (_kbhit())
        {
            char key = _getch();
            if (key == 'a' && blockX > 0)
            {
                --blockX;
            }
            else if (key == 'd' && blockX < GRID_WIDTH - block[0].size())
            {
                ++blockX;
            }
            else if (key == 's')
            {
                // Move down faster
                if (canMoveDown())
                    ++blockY;
            }
        }

        // Delay to simulate falling
        this_thread::sleep_for(chrono::milliseconds(initialTime));
        if (checkGameOver())
        {
            break;
        }
    }

    cout << "\nGAME OVER\n\nFINAL SCORE: " << score << endl;
    getch();

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <time.h>
#include <sys/ioctl.h>
#include "tpmacros.h"

class Io
{
    private:
        struct screenSizeStruct
        {
            int x;
            int y;
        };

        screenSizeStruct mScreenSize;
        char mCancelChar; // Character used to reset input prompt
        const int INPUT_BUFF_SIZE = 16;

    public:
        struct promptSequence
        {
            int numPrompts;
            int** values;
            char** messages;
        }; 
        struct prompt
        {
            int* value;
            char* message;
        };

        void clearScreen(int offset = 0)
        {
            struct winsize screenSize;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &screenSize);
            int numLines = screenSize.ws_row;
            if(!(offset < -1 && std::abs(offset) > numLines))
            {
                numLines += offset;
            }
            for(int i = 0; i < numLines; i++)
            {
                printf("\n");
            }
        }

        void runPromptSequence(promptSequence sequence)
        {
            char inputBuff[INPUT_BUFF_SIZE] = {'\0'};
            for(int i = 0; i < sequence.numPrompts; i++)
            { 
                std::cout << sequence.messages[i];
                fgets(inputBuff, ARRAY_LENGTH(inputBuff), stdin);
                while(!sscanf(inputBuff, "%d", sequence.values[i]))
                { 
                    std::cout << sequence.messages[i];
                    fgets(inputBuff, ARRAY_LENGTH(inputBuff), stdin);
                    clearScreen();
                }
            
            }
        }

        int getInt(const char* message, int min, int max, void (*callback)(void))
        { 
            const int invalidInput = min - 1;
            int input = invalidInput;
            char reset = '\0'; // This character will be checked to see if the user chose to reset the prompt
            char inputBuff[INPUT_BUFF_SIZE] = {'\0'};
            while(!sscanf(inputBuff ,"%d", &input) || input < min || input > max)
            {
                std::cout << message; 
                fgets(inputBuff, ARRAY_LENGTH(inputBuff), stdin);
                clearScreen();
                if(inputBuff[0] == mCancelChar)
                {
                    clearScreen();
                    callback();
                }
            }
            clearScreen();
            return input;
        }

        Io(char cancelChar = 0)
        {
            mCancelChar = cancelChar;
            struct winsize size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
            mScreenSize.x = size.ws_col; 
            mScreenSize.y = size.ws_row;
        }

};

class gameBoard
{
    private:
        int sizeX, sizeY;

        int terminalSizeX, terminalSizeY;

        int numMines;
        const int minePercentage = 15;

        const char coveredTile = '?';
        const char uncoveredEmptyTile = '-';
        const char bombTile = 'B';
        const char flagTile = '!';
        
        Io mIo = Io('r');

        bool gameOver = false;

        char* board = NULL;
        int* mines = NULL; 

        int tilesLeft;
    
    private:
        void generateMineCount()
        {
            numMines = sizeX * sizeY * minePercentage / 100;
            if(numMines <= 0)
            {
                numMines = 1;
            }
        }
        void generateMines()
        {
            srand((uint)time(NULL));
            int randNum;
            
            for(int i = 0; i < numMines;)
            {
                bool candidateFound = true;
                randNum = rand() % (sizeX * sizeY);
                for(int j = 0; j < i; j++)
                {
                    if(mines[j] == randNum)
                    {
                        candidateFound = false;
                        break;
                    } // Invalidate candidate if it is a duplicate
                }
                if(candidateFound)
               {
                    mines[i] = randNum;
                    i++;
                } // If candidate is valid, assign it and move to next one
            }
        }

       void drawBoard()
        {
            mIo.clearScreen();
            printf("   ");
            for(int colNum = 0; colNum < sizeX; colNum++)
            {
                printf("|%0*d", 2, colNum);
            }
            printf("|\n");
            for(int i = 0; i < sizeY; i++)
            {   
                printf(" %0*d", 2, i);
                for(int j = 0; j < sizeX; j++)
                {
                    std::cout << "  " << board[i * sizeX + j]; 
                }
                std::cout << std::endl;
            }
            mIo.clearScreen(-(sizeY + 2));
        }

        bool isMine(int tileNum)
        {
            for(int i = 0; i < numMines; i++)
            {
                if(mines[i] == tileNum)
                {
                    return true;
                }
            }
            return false;
        }

        bool isUncovered(int tileNum)
        {
            char gridSymbol = board[tileNum];
            if(gridSymbol == coveredTile)
            {
                return false;
            }
            return true;
        }

        inline void tileToXY(int tileNum, int* const outX, int* const outY)
        {
            *outX = tileNum % sizeX; 
            *outY = tileNum / sizeX;   
        }

        inline int xyToTile(int x, int y)
        {
            return (y * sizeX) + x;
        }

        int getAdjacentMines(int tileNum)
        {
            int closeMines = 0;

            int tileYCord, tileXCord;
            tileToXY(tileNum, &tileXCord, &tileYCord); 
            
            for(int i = 0; i < numMines; i++)
            {
                int mine = mines[i];
                 
                int mineYCord, mineXCord;
                tileToXY(mine, &mineXCord, &mineYCord);

                int yDistance = std::abs(mineYCord - tileYCord); 
                int xDistance = std::abs(mineXCord - tileXCord);
                int totalDistance = yDistance + xDistance;
                
                if(totalDistance == 1 || (yDistance == 1 && xDistance == 1))
                {    
                    closeMines ++;
                }
            }
            return closeMines;
        }

        void uncoverMines()
        {
            for(int i = 0; i < numMines; i++)
            {
                board[mines[i]] = bombTile;
            }
        }
        
        bool isInBounds(int x, int y)
        {
            if(x > sizeX - 1 || y > sizeY - 1 || x < 0 || y < 0)
            {
                return false;
            }
            return true;
        }

        bool isInBounds(int tileNum)
        {
            if(tileNum < 0 || tileNum > (sizeX * sizeY) - 1)
            {
                return false;
            }
            return true;
        }

        int drawTile(int tileNum)
        {
            if(isUncovered(tileNum))
            {
                return 0;
            }
            tilesLeft --;
            char tileSymbol[2] = {uncoveredEmptyTile, '\0'};
            int closeMines = getAdjacentMines(tileNum);
            if(closeMines > 0)
            {
                sprintf(tileSymbol, "%d", closeMines);
            }
            board[tileNum] = tileSymbol[0];
            return closeMines;
        }

        void uncoverRadius(int origin)
        {
            drawTile(origin);
            if(board[origin] != uncoveredEmptyTile)
            {
                return;
            }
            int originX, originY;
            tileToXY(origin, &originX, &originY);
            
            RANGE(x, 3)
            {
                int newOriginX = x + originX - 1;
                RANGE(y, 3)
                {
                    int newOriginY = y + originY - 1;
                    if(isInBounds(newOriginX, newOriginY))
                    {
                        drawTile(xyToTile(newOriginX, newOriginY));
                    }
                }
            }
        }

        void uncoverBoard()
        {
            for(int i = 0; i < sizeX * sizeY; i++)
             {
                if(isMine(i))
                {
                    board[i] = bombTile;
                    continue;
                }
                drawTile(i);
            }
        } 
        
        void setFlag(int tileNum)
        {
            if(board[tileNum] == flagTile)
            {
                board[tileNum] = coveredTile;
            }
            else if(!isUncovered(tileNum))
            {
                board[tileNum] = flagTile;
            }
        }
        
        void endGame()
        {
            uncoverMines();
            drawBoard();
            printf("KABOOOOOM!!!!!!!! DDDD:");
            gameOver = true;
        }

        void winGame()
        {
            uncoverBoard();
            drawBoard();
            printf("Ayyyyy you won!!!! :DDDD");
            gameOver = true;
        }

        #define ARGS (10, int, MAX_SIGNED(int))
        int min(VA_ARGS_SAFE(ARGS))
        {
            VA_LIST_INIT_SAFE(ARGS);
            int min = args[0];
            for(int i = 1; i < arg_count; i++)
            {
                if(args[i] < min)
                {
                    min = args[i];
                }
            }
            return min;
        }
        #undef ARGS

        void startTurn()
        {
            //printf("%d\n", min(11, -444, 42, 515, -1124));
            printf("%lu\n", MAX_UNSIGNED(ulong));
            for(;;){}
            MEMBER_FUNCTION_POINTER(callback, gameBoard, startTurn, this, void);
            // MEMBER_FUNCTION_POINTER_WITH_ARGS(foo, gameBoard, test, this, int, int, int, float);            
            drawBoard();
            
            char* messages[] = {"Do: [1] Uncover mine, [2] Set flag: ", "Choose column: ", "Choose row: "};
            int input, colNum, rowNum;
            int* inputs[] = {&input, &colNum, &rowNum};
            Io::promptSequence sequence = {3, inputs, messages};
            mIo.runPromptSequence(sequence);
            //int input = mIo.getInt("Do: [1] Uncover mine, [2] Set flag: ", 1, 2, callback);
            //int colNum = mIo.getInt("Choose column: ", 0, sizeX - 1, callback);
            //int rowNum = mIo.getInt("Choose row: ", 0, sizeY - 1, callback);    
            
            int tileNum = xyToTile(colNum, rowNum);
            if(input == 2)
            {
                setFlag(tileNum);
                drawBoard();
                startTurn();
            }
            if(isUncovered(tileNum))
            {
                startTurn();
            }
            else
            {
                uncoverRadius(tileNum);
            }

            if(isMine(input))
            {        
                endGame();   
            }
            else if(tilesLeft <= 0)
            {
                winGame();
            }
        }

        void getBoardSize()
        {
            mIo.clearScreen();
            MEMBER_FUNCTION_POINTER(callback, gameBoard, getBoardSize, this, void);
            sizeX = mIo.getInt("Choose number of columns: ", 2, 99, callback);
            sizeY = mIo.getInt("Choose number of rows: " , 2, 99, callback);
        }

    public: 
        void Run()
        {
            startTurn();
            getBoardSize();
            generateMineCount();
            mines = (int*)alloca(numMines * sizeof(int));
            generateMines();
            tilesLeft = sizeX * sizeY - numMines;
            board = (char*)alloca(sizeX * sizeY * sizeof(char)); // Allocate board memory   
            memset(board, coveredTile, sizeX * sizeY); // Initialize tiles to uncovered ones
            while(!gameOver)
            {
                startTurn();
            }
        } 
};

int main()
{
    gameBoard game;
    game.Run();
    return 0;
}

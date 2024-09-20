#include "snake.h"

int fx[26] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125};

// Get the game speed
int Snake_getGameSpeed() {
    int speed = 1;

    while (speed < 1 || speed > 9) {
        return(speed);
    }
    return 1;
}

//check collision to itself
int Snake_collisionSnake(int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect) {
    int i;
    for (i = detect; i < snakeLength; i++) {
        if (x == snakeXY[0][i] && y == snakeXY[1][i])
            return 1;
    }
    return 0;
}

//Display food
void Snake_generateFood(int fd, int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {

    do {
        srand(time(NULL));
        int temp = rand() % 27;  //Random number column 
        foodXY[0] = fx[temp];
        srand(time(NULL));
        foodXY[1] = rand() % 7;  //Random number Line
    } while (Snake_collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0));

    OLED_SetCursor(fd, foodXY[0], foodXY[1]);
    OLED_Display(fd, "o");

}

//preparing the array that represents the snake's position in the game 
void Snake_prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
    int i;
    int snakeX = snakeXY[0][0];
    int snakeY = snakeXY[1][0];

    for (i = 1; i <= snakeLength; i++) {
        snakeXY[0][i] = snakeX - 5 * i;
        snakeXY[1][i] = snakeY;
    }

}

//Move snake array through direction
void Snake_moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) {
    int i;
    for (i = snakeLength - 1; i >= 1; i--) {
        snakeXY[0][i] = snakeXY[0][i - 1];
        snakeXY[1][i] = snakeXY[1][i - 1];
    }

//checking direction to move
    switch (direction) {
        case DOWN_ARROW:
            snakeXY[1][0]++;
            break;
        case RIGHT_ARROW:
            snakeXY[0][0] += 5;
            break;
        case UP_ARROW:
            snakeXY[1][0]--;
            break;
        case LEFT_ARROW:
            snakeXY[0][0] -= 5;
            break;
    }

}

void Snake_move(int fd, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) {
    int x;
    int y;

    // Remove the tail (HAS TO BE DONE BEFORE THE ARRAY IS MOVED!!!!!)
    x = snakeXY[0][snakeLength - 1];
    y = snakeXY[1][snakeLength - 1];
    OLED_SetCursor(fd, x, y);
    OLED_Display(fd, " ");

    // Changes the head of the snake to a body part
    OLED_SetCursor(fd, snakeXY[0][0], snakeXY[1][0]);
    OLED_Display(fd, "*");
    Snake_moveSnakeArray(snakeXY, snakeLength, direction);

    OLED_SetCursor(fd, snakeXY[0][0], snakeXY[1][0]);
    OLED_Display(fd, "O");

//Gets rid of the darn flashing underscore.
    OLED_SetCursor(fd, 1, 1);

}

//This function checks if the snakes head his on top of the food, if it is then it'll generate some more food...
int Snake_eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[])
{
    if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
    {
        foodXY[0] = 0;
        foodXY[1] = 0; 

        return 1;
    }
    
    return 0;
}

//Check snake collsion with wall or itself
int Snake_collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int ScreenWidth, int ScreenHeight, int snakeLength)
{
    // Check collision with wall
    if (snakeXY[0][0] < 0 || snakeXY[1][0] < 0 || snakeXY[0][0] >= ScreenWidth || snakeXY[1][0] >= ScreenHeight)
    {
        return 1; // Colision with wall
    }
    
    // Check collision with itself
    for (int i = 1; i < snakeLength; i++) 
    {
        if (snakeXY[0][0] == snakeXY[0][i] && snakeXY[1][0] == snakeXY[1][i])
        {
            return 1; //Colision with itself
        }
    }
    
    return 0; // No collision
}

//Display snake to OLED
void Snake_loadSnake(int fd, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
    int i;
    for (i = 0; i < snakeLength; i++)
    {
        OLED_SetCursor(fd, snakeXY[0][i], snakeXY[1][i]);
        OLED_Display(fd, "*");
    }

}


//UPdate and Display Score, speed in botton 
void Snake_refreshInfoBar(int fd, int score, int speed) {
    char str[10];
    OLED_SetCursor(fd, 0, 7);
    sprintf(str, "score:%d", score);
    OLED_Display(fd, str);

    OLED_SetCursor(fd, 70, 7);
    sprintf(str, "speed:%d", speed);
    OLED_Display(fd, str);

}

//Display game over
void Snake_gameOverScreen(int fds, int fdb, char *buff, size_t size) {
    OLED_SetCursor(fds, 25, 3);
    OLED_Display(fds, "GAME OVER!");
    OLED_SetCursor(fds, 10, 4);
    OLED_Display(fds, "PRESS TO CONTINUE");
    Button_waitForAnyKey(fdb, buff, size);

}

//Display you win
void Snake_gameWin(int fds, int fdb, char *buff, size_t size) {
    OLED_SetCursor(fds, 25, 3);
    OLED_Display(fds, "YOU WIN!");
    OLED_SetCursor(fds, 10, 4);
    OLED_Display(fds, "PRESS TO CONTINUE");
    Button_waitForAnyKey(fdb, buff, size);

}


void Snake_startGame(int fds, int fdb, char *buff, size_t size, int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int ScreenWidth, int ScreenHeight, int snakeLength, int direction, int score, int speed) {
    int gameOver = 0;
    clock_t endWait;

    //CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
    int waitMili = CLOCKS_PER_SEC - (speed) * (CLOCKS_PER_SEC / 10);  //Sets the correct wait time according to the selected speed
    int tempScore = 10 * speed;
    int oldDirection;
    int canChangeDirection = 1;
    //int seconds = 1;

    endWait = clock() + waitMili;

    do {
        if (canChangeDirection) {
            oldDirection = direction;
            direction = Button_KeysPressed_to_Direction(fdb, buff, size, direction);
        }

        if (oldDirection != direction) //Temp fix to prevent the snake from colliding with itself
            canChangeDirection = 0;

        if (clock() >= endWait) //it moves according to how fast the computer running it is...
        {
            Snake_move(fds, snakeXY, snakeLength, direction);
            canChangeDirection = 1;

            if (Snake_eatFood(snakeXY, foodXY)) {
                Snake_generateFood(fds, foodXY, ScreenWidth, ScreenHeight, snakeXY, snakeLength); //Generate More Food
                snakeLength++;
                score += 10;

                if (score >= 10 * speed + tempScore) {
                    speed++;
                    tempScore = score;

                    if (speed <= 9) //this needs to be fixed
                        waitMili = waitMili - (CLOCKS_PER_SEC / 10);
                    else {
                        if (waitMili >= 40) //Maximum Speed (the game has to be beatable)
                            waitMili = waitMili - (CLOCKS_PER_SEC / 200);
                    }
                }
                Snake_refreshInfoBar(fds, score, speed);
            }
            endWait = clock() + waitMili; //TEMP FIX, NEED TO FIND A WAY TO RESET CLOCK().. Na, seems to work fine this way...
        }

        gameOver = Snake_collisionDetection(snakeXY, ScreenWidth, ScreenHeight, snakeLength);

        if (snakeLength >= SNAKE_ARRAY_SIZE - 5) //Just to make sure it doesn't get longer then the array size & crash
        {
            gameOver = 2; //You Win! <- doesn't seem to work - NEED TO FIX/TEST THIS
            score += 1500; //When you win you get an extra 1500 points!!!
        }

    } while (!gameOver);

    switch (gameOver) {
        case 1:
            Snake_gameOverScreen(fds, fdb, buff, size);
            break;
    
        case 2:
            Snake_gameWin(fds, fdb, buff, size);
            break;
    }

}


//Display game in to ssd
void Snake_loadGame(int fds, int fdb, char *buff, size_t size) {
    int snakeXY[2][SNAKE_ARRAY_SIZE]; //Two Dimensional Array, the first array is for the X coordinates and the second array for the Y coordinates

    int snakeLength = 2; //Starting Length

    int direction = LEFT_ARROW; //DO NOT CHANGE THIS TO RIGHT ARROW, THE GAME WILL INSTANTLY BE OVER IF YOU DO!!! <- Unless the prepareSnakeArray function is changed to take into account the direction....

    int foodXY[] = {20, 5}; // Stores the location of the food

    int score = 0;
    //int level = 1;

    //Window Width * Height - at some point find a way to get the actual dimensions of the console... <- Also somethings that display don't take this dimensions into account.. need to fix this...
    int ScreenWidth = 128;
    int ScreenHeight = 7;

    int speed = Snake_getGameSpeed();

    //The starting location of the snake
    snakeXY[0][0] = 20;
    snakeXY[1][0] = 1;

    Snake_prepairSnakeArray(snakeXY, snakeLength);
    Snake_loadSnake(fds, snakeXY, snakeLength);
    Snake_generateFood(fds, foodXY, ScreenWidth, ScreenHeight, snakeXY, snakeLength);
    Snake_refreshInfoBar(fds, score, speed); //Bottom info bar. Score, Level etc
    Snake_startGame(fds, fdb, buff, size, snakeXY, foodXY, ScreenWidth, ScreenHeight, snakeLength, direction, score, speed);

}
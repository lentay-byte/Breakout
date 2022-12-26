/* --------------------------------------------------------
 *    File: paddle.cpp
 *  Author: Lenworth Taylor
 *   Class: COP 2001 - CRN 10410 - TR - 202201
 * Purpose: To add collision detection between walls and moving block and to add bricks
 *          with collision detection.
 * ---- ---------------------------------------------------- */

#include "breakout_defs.h"


// Function declarations (prototypes)
// --------------------------------------------------------

void setup(Ball &ball, Borders &walls, MovingBlock &thePaddle, Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]);
Direction processInput();
bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &thePaddle, bool &started,
            Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]);
void render(sf::RenderWindow &window, Ball ball, float delta, Borders walls,
            MovingBlock thePaddle, Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]);
int getgetcollisionPoint(Ball* pBall, Block* pBlock);
bool collisionCheck(Ball* pBall, Block* pBlock);
bool checkBlockCollision(Block moving, Block stationary);
bool doCollisionChecks(Ball &ball, MovingBlock &thePaddle, Borders walls,
                       Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]) ;

// ----------------------------------------------------------

/**
 * The main application
 * @return OS status message (0=Success)
 */

int main() {

    // function calls
    Borders theWalls;
    Ball ball;
    MovingBlock thePaddle;
    Brick theBricks [BRICK_ROWS][BRICK_COLUMNS];
    setup(ball,theWalls,thePaddle, theBricks);


    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello SFML");
    window.clear(WINDOW_COLOR);

    // bool variable that determines of the game has started
    bool started = false;

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;
    bool gameOver = false;
    while (!gameOver)
    {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }

        // Process user input
        // ------------------------------------------------
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time
            gameOver = update(userInput, ball, delta, theWalls, thePaddle, started, theBricks);
            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        // ------------------------------------------------
        render(window, ball, delta, theWalls, thePaddle, theBricks);

    } // end main game loop
    // game ending, close the graphics window
    window.close();

    return 0;   // return success to the OS
} // end main

/**
 * one time game object initialization
 * @param ball - the ball
 * @param walls - the outer walls
 * @param thePaddle -- the paddle
 */

void setup(Ball &ball, Borders &walls, MovingBlock &thePaddle, Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]){

    //paddle properties
    thePaddle.block.left = (WINDOW_WIDTH - PADDLE_WIDTH)/2.0;
    thePaddle.block.top = WINDOW_HEIGHT - (2 * PADDLE_THICKNESS);
    thePaddle.block.width = PADDLE_WIDTH;
    thePaddle.block.height = PADDLE_THICKNESS;
    thePaddle.rectangle.setSize(sf::Vector2f(thePaddle.block.width, thePaddle.block.height));
    thePaddle.rectangle.setFillColor(PADDLE_COLOR);
    thePaddle.velocityX = 0;
    thePaddle.velocityY = 0;

    //ball properties
    ball.radius = BALL_RADIUS;
    ball.coordinateX = thePaddle.block.left + (PADDLE_WIDTH / 2);
    ball.coordinateY = thePaddle.block.top - BALL_RADIUS - 1;
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;

    walls.leftWall.left = 0.0;
    walls.leftWall.top = 0.0;
    walls.leftWall.width = WALL_THICKNESS;
    walls.leftWall.height = WINDOW_HEIGHT;
    walls.leftWall.rectangle.setSize(sf::Vector2(walls.leftWall.width, walls.leftWall.height));
    walls.leftWall.rectangle.setPosition(walls.leftWall.left, walls.leftWall.top);
    walls.leftWall.rectangle.setFillColor(WALL_COLOR);

    walls.topWall.left = 0.0;
    walls.topWall.top = 0.0;
    walls.topWall.width = WINDOW_WIDTH;
    walls.topWall.height = WALL_THICKNESS;
    walls.topWall.rectangle.setSize(sf::Vector2(walls.topWall.width, walls.topWall.height));
    walls.topWall.rectangle.setPosition(walls.topWall.left, walls.topWall.top);
    walls.topWall.rectangle.setFillColor(WALL_COLOR);

    walls.rightWall.left = WINDOW_WIDTH-WALL_THICKNESS;
    walls.rightWall.top = 0.0;
    walls.rightWall.width = WALL_THICKNESS;
    walls.rightWall.height = WINDOW_HEIGHT;
    walls.rightWall.rectangle.setSize(sf::Vector2(walls.rightWall.width, walls.rightWall.height));
    walls.rightWall.rectangle.setPosition(walls.rightWall.left, walls.rightWall.top);
    walls.rightWall.rectangle.setFillColor(WALL_COLOR);

    walls.bottomWall.left = 0.0;
    walls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    walls.bottomWall.width = WINDOW_WIDTH;
    walls.bottomWall.height = WALL_THICKNESS;
    walls.bottomWall.rectangle.setSize(sf::Vector2(walls.bottomWall.width, walls.bottomWall.height));
    walls.bottomWall.rectangle.setPosition(walls.bottomWall.left, walls.bottomWall.top);
    walls.bottomWall.rectangle.setFillColor(WALL_COLOR);


    float bricksTop = FIRST_BRICK; //start at lowest brick row

    Brick* pNext = &theBricks[0][0];
    for (int row = 0; row < BRICK_ROWS; row++){

        float bricksLeft = BRICKS_LEFT; // start back at far left

        for (int column = 0; column < BRICK_COLUMNS; column++){

            pNext -> block.left - bricksLeft + 1; // pixel right to leave blank pixel on left

            //offset left/top by 1 pixel to give each brick a border
            pNext -> block.left = bricksLeft + 1;
            pNext -> block.top = bricksTop + 1;

            //subtract 2 from width/height to make room for the 1 pixel
            //border on each side
            pNext -> block.width = BRICK_WIDTH - 2;
            pNext -> block.height = BRICK_HEIGHT - 2;

            //set row specific properties
            if (row < 2) {
                pNext->block.color = sf::Color::Red;
                pNext->points = 1;
                pNext->speedAdjust = 0;
            }
            else if (row < 4)
            {
                pNext -> block.color = sf::Color::Green;
                pNext->points = 3;
                pNext -> speedAdjust =0;
            }
            else if (row < 6)
            {
                pNext->block.color = sf::Color(255, 165, 0); // Orange
                pNext->points = 5;
                pNext->speedAdjust = 1;
            }
            else {
                pNext->block.color = sf::Color::Magenta;
                pNext->points = 7;
                pNext->speedAdjust = 2;
            }

            //set the drawing rectangle
            pNext -> block.rectangle.setSize(sf::Vector2f (pNext -> block.width, pNext -> block.height));
            pNext -> block.rectangle.setFillColor(pNext -> block.color);
            pNext -> block.rectangle.setPosition(pNext -> block.left, pNext -> block.top);

            //initialize hit flag off so brick is displayed;
            pNext -> hit = false;

            pNext++; // get next brick

            bricksLeft += BRICK_WIDTH; // move next bright to right

        } // brick columns

        bricksTop -= BRICK_HEIGHT; // move up to next row

    } // brick rows

}

/**
 * convert user keyboard input into recognized integer values
 * for left,up,right,down
 * @return Direction - user input (default no-input=None, quit=Exit)
 */
Direction processInput() {
    Direction input = None;  // no input

    //is statements for keyboard inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input =Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        input = Start;
    }
    return input;
} // end getUserInput

/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 * @param walls - the walls
 * @param thePaddle - the paddle
 * @param started - tells if the game has started
 */

bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &thePaddle, bool &started,
            Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]) {

    //initialized game over variable
    bool gameOver = false;

    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            case Left:
                thePaddle.velocityX -= PADDLE_SPEED;
                break;
            case Right:
                thePaddle.velocityX += PADDLE_SPEED;
                break;
            case Start:
                if (!started) {
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y * -1;

                    started = true;
                    break;
                }

        }
        //controls what the does when game starts
    }
    if (started){
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;
    }
    else{
        ball.coordinateX = thePaddle.block.left + (PADDLE_WIDTH / 2.0);
        ball.coordinateY = thePaddle.block.top - BALL_RADIUS - 1;
    }

    // adjust the location of the ball for speed * time
    thePaddle.block.left += thePaddle.velocityX * delta;

    ball.coordinateX += ball.velocityX * delta;
    ball.coordinateY += ball.velocityY * delta;

    gameOver = doCollisionChecks(ball, thePaddle, walls, theBricks);

    return gameOver;
} // end update
/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 * @param theWalls - the walls
 * @param thePaddle - the paddle
 */
void render(sf::RenderWindow &window,
            Ball ball, float delta,
            Borders theWalls,
            MovingBlock thePaddle,
            Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]){
    // Render drawing objects
    // ------------------------------------------------
    // clear the window with the background color
    window.clear(WINDOW_COLOR);
    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;
    circle.setFillColor(ball.color);
    circle.setRadius(ball.radius);
    // set screen coordinates relative to the center of the circle
    circle.setOrigin( BALL_RADIUS,BALL_RADIUS);
    // calculate current drawing location relative to speed and frame-time
    float xPosition = ball.coordinateX + ball.velocityX * delta;
    float yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);
    window.draw(theWalls.leftWall.rectangle);
    window.draw(theWalls.topWall.rectangle);
    window.draw(theWalls.rightWall.rectangle);
    window.draw(theWalls.bottomWall.rectangle);


    thePaddle.block.left  += thePaddle.velocityX * delta;
    thePaddle.block.top += thePaddle.velocityY * delta;

    thePaddle.rectangle.setPosition(thePaddle.block.left,thePaddle.block.top );
    window.draw(thePaddle.rectangle);

    // -- rendering bricks --
    Brick *pBrick = &theBricks[0][0];
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int column = 8; column < BRICK_COLUMNS; column++) {
            if (!pBrick -> hit) {
                window.draw(pBrick -> block.rectangle);
            }
            pBrick++;
        } // columns
    }  // rows

    window.display();

} // end render
/**
 * determines if there is a collision
 * @param pBall -- the ball
 * @param pBlock -- the blocks
 * @return getcollisionPoint - returns an integer
 */


int getcollisionPoint(Ball* pBall, Block* pBlock){
    int cardinalHeading = 0;
    float checkX;
    float checkY;

    // Find horizontal coordinate
    // Ball left of block

    if(pBall->coordinateX < pBlock->left)
        checkX = pBlock->left;

        //ball right of block

    else if(pBall->coordinateX > (pBlock->left + pBlock->width))
        checkX = pBlock->left + pBlock->width;

        // ball between block
    else
        checkX = pBall -> coordinateX;

    //Find vertical coordinate
    //Ball above block
    if(pBall -> coordinateY < pBlock -> top)
        checkY = pBlock -> top;

        //ball below block
    else if(pBall -> coordinateY > (pBlock -> top + pBlock -> height))
        checkY = pBlock -> top + pBlock -> height;

        //ball between block
    else
        checkY= pBall -> coordinateY;

    //Find the difference
    float diffX = checkX - pBall->coordinateX;
    float diffY = (WINDOW_HEIGHT - checkY) - (WINDOW_HEIGHT - pBall->coordinateY);


    double distance = std::sqrt(pow(diffX, 2.0) + pow(diffY, 2.0));

    if (distance <= pBall->radius) {
        double theta = atan2(diffY, diffX);
        double degrees = 90.0 - theta * 180 / M_PI;


        if (degrees <= 0)
            degrees += 360;

        cardinalHeading = int(degrees);
    }

    return cardinalHeading;

} //end getcollisionPoint

/**
 * checks if collision has occured
 * @param pBall -- the ball
 * @param pBlock -- the block
 * @return collisionCheck - returns a bool
 */

bool collisionCheck(Ball* pBall, Block* pBlock) {
    bool Collision = false;
    int value = getcollisionPoint(pBall, pBlock);

    // controls ball direction if collision has occured
    if (value) {
        Collision = true;
        if (value > 225 && value < 315) {
            pBall->velocityX = pBall->velocityX * -1;
            pBall->coordinateX = pBlock->left + pBlock->width + pBall->radius + 1;

        } else if (value > 45 && value < 135) {
            pBall->velocityX = pBall->velocityX * -1;
            pBall->coordinateX = pBlock->left - pBall->radius - 1;
        }

        if (value >= 315 || value <= 45) {
            pBall->velocityY = pBall->velocityY * -1;
            pBall->coordinateY = pBlock->top + pBlock->height + pBall->radius + 1;

        } else if (value >= 135 && value <= 225) {
            pBall->velocityY = pBall->velocityY * -1;
            pBall->coordinateY = pBlock->top - pBall->radius - 1;
        }

    }

    return Collision;
}// end collision check

/**
 * Checks for collision between moving block and wall
 * @param moving - the moving block
 * @param stationary - the stationary blocks
 * @return - returns a collision bool
 */

bool checkBlockCollision(Block moving, Block stationary){

    bool collision = false;

    if (moving.left < stationary.left + stationary.width &&
        moving.left + moving.width > stationary.left &&
        moving.top < stationary.top + stationary.height &&
        moving.top + moving.height > stationary.top) {
        collision = true;
    }

    return collision;
}//

/**
 * summarizes all the collision checks into one
 * @param ball - the ball
 * @param thePaddle -- the paddle
 * @param walls - the walls
 * @param theBricks - the bricks
 * @return -- returns a game over bool
 */

bool doCollisionChecks(Ball &ball, MovingBlock &thePaddle, Borders walls, Brick theBricks [BRICK_ROWS][BRICK_COLUMNS]){

    bool gameOver = false;

    // --- checks vertical collision ---
    if (!collisionCheck(&ball, &thePaddle.block)) {
        if (!collisionCheck(&ball, &walls.topWall)) {
            gameOver = collisionCheck(&ball, &walls.bottomWall);
        }
    }
    // --- check horizontal collisions ---
    if (!collisionCheck(&ball, &walls.leftWall)) {
        collisionCheck(&ball, &walls.rightWall);
    }

    //Checks collision of the paddle and left wall.
    if (checkBlockCollision(thePaddle.block, walls.leftWall)){

        thePaddle.velocityX = 0.0;
        thePaddle.block.left = walls.leftWall.left + walls.leftWall.width + 1;

    }

    //Checks collision of the paddle and right wall.
    if (checkBlockCollision(thePaddle.block, walls.rightWall)){

        thePaddle.velocityX = 0.0;
        thePaddle.block.left = walls.rightWall.left - thePaddle.block.width - 1;

    }

    Brick *pBrick = &theBricks[0][0];
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int column = 8; column < BRICK_COLUMNS; column++) {
            if (!pBrick -> hit) {
                pBrick -> hit = collisionCheck(&ball, &pBrick -> block);
            }
            pBrick++;
        } // columns
    }  // rows

    return gameOver;

}

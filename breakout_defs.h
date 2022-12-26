/* --------------------------------------------------------
 *    File: breakout_defs
 *  Author: Lenworth Taylor
 *   Class: COP 2001 - CRN 10410 - TR - 202201
 * Purpose: To hold all definitions, enumerations, user-identified data-types etc.
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>
#include <cmath>
#ifndef BREAKOUTPADDLE_BREAKOUT_DEFS_H
#define BREAKOUTPADDLE_BREAKOUT_DEFS_H

// window properties
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 900;
const sf::Color WINDOW_COLOR = sf::Color::White;

// drawing properties
const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::Blue;

// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float BALL_RADIUS = 10.0;
const float BALL_SPEED_X = BALL_RADIUS * 20.0 / 1000.0;    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS * 20.5 / 1000.0;   // span  vertically

// wall properties
const float WALL_THICKNESS = 2.0;
const sf::Color WALL_COLOR = sf::Color::Red;

// paddle constants
const float PADDLE_WIDTH = 600.0;
const float PADDLE_THICKNESS = 15.0;
sf::Color PADDLE_COLOR = sf::Color :: Blue;
const float PADDLE_SPEED = PADDLE_WIDTH / 5.0/ 1000.0;

// -- Brick constants --
const int BRICK_ROWS = 8;
const int BRICK_COLUMNS = 14;
const float BRICK_WIDTH = WINDOW_WIDTH / BRICK_COLUMNS;
const float BRICK_HEIGHT = PADDLE_THICKNESS * 2;
const float BRICKS_HEIGHT = BRICK_ROWS * BRICK_HEIGHT;
const float BRICKS_TOP = WINDOW_HEIGHT / 2.0 - BRICKS_HEIGHT * 0.75;
const float BRICKS_LEFT = WALL_THICKNESS;
const float FIRST_BRICK = BRICKS_TOP + (BRICK_ROWS - 1) * BRICK_HEIGHT;

        // Type definitions
// --------------------------------------------------------
enum Direction {
    Exit=-1,
    None,
    Left,
    Up,
    Right,
    Down,
    Start
};

// ball properties
struct Ball {
    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf:: Color color;
};

// wall properties
struct Block {
    float left;
    float top;
    float width;
    float height;
    sf:: Color color;
    sf::RectangleShape rectangle;
};

//moving block properties
struct MovingBlock {
    Block block;
    float velocityX;
    float velocityY;
    sf::RectangleShape rectangle;
};

//Border structures
struct Borders {
    Block leftWall;
    Block topWall;
    Block rightWall;
    Block bottomWall;
};

//brick structures
struct Brick {
    Block block;
    bool hit;
    int points;
    float speedAdjust;
};


#endif //BREAKOUTPADDLE_BREAKOUT_DEFS_H
#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Constants.hpp"
#include <vector>

namespace BertSweeper {

/** \brief Struct for one pixel of one char with it's start coordinates
 */
struct Pixel {
    int iX = 0; /**< X coordinate of pixel */
    int iY = 0; /**< Y coordinate of pixel */
};

class BertSweeper {
private:
    /** \brief Initializes new game
     *
     * \return void
     *
     */
    static void initGame();

    /** \brief Creates new game
     *
     * \return void
     *
     */
    static void newGame();


    /** \brief Converts input data to character
     *
     * \param iPosHor* int: Horizontal position of char in bitmap file
     * \param iPosVert* int: Vertical position of char in bitmap file
     * \return void
     *
     */
    static void convertChar(int* iPosHor, int* iPosVert);


    /** \brief Marks a field
     *
     * \param iPosHor int*: Horizontal position of mouse cursor
     * \param iPosVert int*: Vertical position of mouse cursor
     * \return void
     *
     */
    void static markField(Pixel* Pixel);


    /** \brief Displays GUI
     *
     * \return void
     *
     */
    static void display();


    /** \brief Event handling when mouse click event happened
     *
     * \param iButton int: Pressed mouse button
     * \param iState int: State of pressed mouse button
     * \param iX int: X position of mouse cursor
     * \param iY int: Y position of mouse cursor
     * \return void
     *
     */
    static void mouseClick(int iButton, int iState, int iX, int iY);


    /** \brief Event handling when keyboard event happened
     *
     * \param cKey unsigned char: Pressed key
     * \param iX int: X position of mouse cursor
     * \param iY int: Y position of mouse cursor
     * \return void
     *
     */
    static void keyboard(unsigned char cKey, int iX, int iY);


    /** \brief Loads bitmap with character set
     *
     * \return bool: true if success
     *
     */
    static bool loadBitmap();


    /** \brief Clicks a field
     *
     * \param iPosHor int*: Horizontal position of mouse cursor
     * \param iPosVert int*: Vertical position of mouse cursor
     * \return void
     *
     */
    static void clickField(Pixel* Pixel);


protected:
public:
    /** \brief Default constructor
     *
     * \param iArgC int: Number of arguments from main
     * \param iArgV[] char*: Pointer to argument list from main
     *
     */
    BertSweeper(int iArgCInput, char *iArgVInput[]);
     ~BertSweeper();
};
}

#include <iostream>

namespace BertSweeper {

// Change this for testing purposes
const int SCALING               = 2;                                        /**< Scaling factor of GUI */


// Better not change this
const int MIN_BLOCKS_HOR        = 8;                                        /**< Minimum horizontal number of blocks */
const int MIN_BLOCKS_VERT       = 8;                                        /**< Minimum vertical number of blocks */
const int MAX_BLOCKS_HOR        = 50;                                       /**< Maximum horizontal number of blocks */
const int MAX_BLOCKS_VERT       = 40;                                       /**< Maximum vertical number of blocks */
const int MIN_BOMBS             = 10;                                       /**< Minimum amount of bombs */

const int PIXEL_PER_BLOCK       = 8;                                        /**< Pixels per site of one block in GUI */

const int PRE_SCALING           = 2;                                        /**< Prescaling factor of GUI */

const int PIXEL_SIZE            = PRE_SCALING * SCALING;                    /**< Size of one pixel */
const int BLOCK_RES             = PIXEL_PER_BLOCK * PRE_SCALING * SCALING;  /**< Resolution of one block in GUI */
const int GRID_SIZE             = SCALING;                                  /**< Pixels of each grid line in GUI */


// Displayed texts
const char TITLE[]              = "BertSweeper";                            /**< Window title of GUI */


// Colors, can be changed
const double BACKGROUND[]       = { 1.0, 1.0, 1.0, 1.0 };                   /**< Background color set */

const double WHITE[]            = { 1.0, 1.0, 1.0, 1.0 };                   /**< White color set */
const double LIGHTGRAY[]        = { 0.5, 0.5, 0.5, 1.0 };                   /**< Light gray color set */
const double DARKGRAY[]         = { 0.3, 0.3, 0.3, 1.0 };                   /**< Dark gray color set */
const double BLACK[]            = { 0.0, 0.0, 0.0, 1.0 };                   /**< Black color set */
const double RED[]              = { 1.0, 0.0, 0.0, 1.0 };                   /**< Red color set */
const double GREEN[]            = { 0.0, 1.0, 0.0, 1.0 };                   /**< Green color set */
const double BLUE[]             = { 0.0, 0.0, 1.0, 1.0 };                   /**< Blue color set */
// TODO (Hasenfresser#3): More colors

// Bitmap configuration, better not change this
const int BITMAP_HEAD_SIZE      = 14;                                       /**< Byte size of bitmap head */
const int BITMAP_HEAD_OFFSET    = 10;                                       /**< Position of offset in bitmap head */

const int BITMAP_INFO_SIZE      = 40;                                       /**< Byte size of bitmap information */
const int BITMAP_INFO_WIDTH     = 18;                                       /**< Position of width in bitmap information */
const int BITMAP_INFO_HEIGHT    = 22;                                       /**< Position of heigth in bitmap information */

const int BITMAP_PIXEL_SIZE     = 3;                                        /**< Byte size of one pixel depending on RGB */

const int BITMAP_DATA_HEIGTH    = 64;                                       /**< Height of bitmap data in pixels */
const int BITMAP_DATA_WIDTH     = 64;                                       /**< Width of bitmap data in pixels */

const int CHAR_SET_MAX          = 64;                                       /**< Number of characters in bitmap file */
const int CHAR_SIZE             = 8;                                        /**< Pixel size per character */

const int CHAR_SET_AE           = 57;                                       /**< Number of char 'Ä' in bitmap file */
const int CHAR_SET_OE           = 58;                                       /**< Number of char 'Ö' in bitmap file */
const int CHAR_SET_UE           = 59;                                       /**< Number of char 'Ü' in bitmap file */

const char CHAR_START           = '!';                                      /**< Start number of char '!' in ASCII */
const char CHAR_END             = 'Z';                                      /**< End number of char 'Z' in ASCII */

const char CHAR_AE              = 'Ä';                                      /**< Number of char 'Ä' */
const char CHAR_OE              = 'Ö';                                      /**< Number of char 'Ö' */
const char CHAR_UE              = 'Ü';                                      /**< Number of char 'Ü' */

const std::string CHAR_SET_PATH = "data\\data.csbs";                        /**< Path of char set */


// Field configuration
const int FIELD_NOT_CLICKED     = 9;                                        /**< Field value, if field is not clicked */
const int FIELD_MARKED          = 10;                                       /**< Field value, if field is marked */
const int FIELD_UNKNOWN         = 11;                                       /**< Field value, if field is unknown */
const int FIELD_BOMB            = 12;                                       /**< Field value, if field is a bomb */

const int FIELD_CHAR_BOMB       = 63;                                       /**< Index of bomb in char set */
const int FIELD_CHAR_MARKED     = 62;                                       /**< Index of mark in char set */
const int FIELD_CHAR_UNKNOWN    = 29;                                       /**< Index of question mark in char set */
const int FIELD_CHAR_SPACE      = 5;                                        /**< Index of empty space in char set */
const int FIELD_CHAR_NUM_START  = 14;                                       /**< Index of first number in char set */


// Keyboard configuration
const char KEY_RESTART          = 'R';                                      /**< Key for restarting game */

}

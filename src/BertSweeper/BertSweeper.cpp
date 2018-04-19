#include "BertSweeper.hpp"
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <cstdio>

namespace BertSweeper {

bool bLose                  = false;
bool bVictory               = false;

int iResolutionHorizontal   = 0;
int iResolutionVertical     = 0;
int iGridSize               = 0;

int iBlocksHorizontal       = 0;
int iBlocksVertical         = 0;
int iBombs                  = 0;

int iRemainingFields        = 0;
//int iRemainingBombs         = 0;

int iArgC = 0;          /**< Number of arguments */
char** pArgV = NULL;    /**< Pointer to argument list */

std::vector<std::vector<bool> > Bombs;  /**< All bombs of the game */
std::vector<std::vector<int> > Fields;  /**< All fields of the game */
std::vector<Pixel> CharSet[CHAR_MAX];   /**< Character set in bits */


// TODO (Hasenfresser#1): Löschen!
std::vector<char> veccData;

BertSweeper::BertSweeper(int iArgCInput, char *pArgVInput[]) {
    srand(time(NULL));

    iArgC = iArgCInput;
    pArgV = pArgVInput;

    initGame();
}

BertSweeper::~BertSweeper() { }

void BertSweeper::initGame() {
    if (!loadBitmap())
        return;

    if(iArgC < 4)
        return;

    iBlocksHorizontal       = atoi(pArgV[1]);
    iBlocksVertical         = atoi(pArgV[2]);
    iBombs                  = atoi(pArgV[3]);

    if(iBlocksHorizontal    < MIN_BLOCKS_HOR                        or
       iBlocksVertical      < MIN_BLOCKS_VERT                       or
       iBombs               < MIN_BOMBS                             or
       iBlocksHorizontal    > MAX_BLOCKS_HOR                        or
       iBlocksVertical      > MAX_BLOCKS_VERT                       or
       iBombs               > iBlocksHorizontal * iBlocksVertical)
        return;

    for(int iCounterHor = 0; iCounterHor < iBlocksHorizontal; ++iCounterHor) {
        std::vector<int> Column;
        std::vector<bool> ColumnBombs;

        for(int iCounterVert = 0; iCounterVert < iBlocksVertical; ++iCounterVert) {
            Column.push_back(FIELD_NOT_CLICKED);
            ColumnBombs.push_back(false);
        }

        Fields.push_back(Column);
        Bombs.push_back(ColumnBombs);
    }

    newGame();

    iResolutionHorizontal   = iBlocksHorizontal * BLOCK_RES;
    iResolutionVertical     = iBlocksVertical * BLOCK_RES;

    glutInit(&iArgC, pArgV);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(iResolutionHorizontal, iResolutionVertical);
    glutCreateWindow(TITLE);

    glClearColor(BACKGROUND[0], BACKGROUND[1], BACKGROUND[2], BACKGROUND[3]);

    gluOrtho2D(0, iResolutionHorizontal, 0, iResolutionVertical);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutMainLoop();
}

void BertSweeper::newGame() {
    bLose               = false;
    bVictory            = false;

    iRemainingFields    = iBlocksHorizontal *  iBlocksVertical - iBombs;

    for(int iCounterHor = 0; iCounterHor < iBlocksHorizontal; ++iCounterHor) {
        for(int iCounterVert = 0; iCounterVert < iBlocksVertical; ++iCounterVert) {
            Fields[iCounterHor][iCounterVert] = FIELD_NOT_CLICKED;
            Bombs[iCounterHor][iCounterVert] = false;
        }
    }

    int iCounter = 0;

    do {
        int iHor = rand() % iBlocksHorizontal;
        int iVert = rand() % iBlocksVertical;

        if(!Bombs[iHor][iVert]) {
            Bombs[iHor][iVert] = true;
            ++iCounter;
        }
    }
    while(iCounter < iBombs);
}

void BertSweeper::markField(Pixel* Pixel) {
    if(Fields[Pixel->iX][Pixel->iY] == FIELD_NOT_CLICKED or Fields[Pixel->iX][Pixel->iY] == FIELD_BOMB)
        Fields[Pixel->iX][Pixel->iY] = FIELD_MARKED;
    else if(Fields[Pixel->iX][Pixel->iY] == FIELD_MARKED)
        Fields[Pixel->iX][Pixel->iY] = FIELD_UNKNOWN;
    else if(Fields[Pixel->iX][Pixel->iY] == FIELD_UNKNOWN)
        Fields[Pixel->iX][Pixel->iY] = FIELD_NOT_CLICKED;
}

void BertSweeper::display() {
    iResolutionHorizontal = glutGet(GLUT_WINDOW_WIDTH);
    iResolutionVertical = glutGet(GLUT_WINDOW_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT);

    for(int iCounterHor = 0; iCounterHor < iBlocksHorizontal; ++iCounterHor) {
        for(int iCounterVert = 0; iCounterVert < iBlocksVertical; ++iCounterVert) {
            glBegin(GL_QUADS);
                if(Fields[iCounterHor][iCounterVert] < FIELD_NOT_CLICKED)
                    glColor4dv(&WHITE[0]);
                else
                    glColor4dv(&LIGHTGRAY[0]);

                glVertex2i(iCounterHor * BLOCK_RES,             iCounterVert * BLOCK_RES);
                glVertex2i(iCounterHor * BLOCK_RES + BLOCK_RES, iCounterVert * BLOCK_RES);
                glVertex2i(iCounterHor * BLOCK_RES + BLOCK_RES, iCounterVert * BLOCK_RES + BLOCK_RES);
                glVertex2i(iCounterHor * BLOCK_RES,             iCounterVert * BLOCK_RES + BLOCK_RES);
            glEnd();

            if( (Bombs[iCounterHor][iCounterVert] and (bLose or bVictory))  or
               Fields[iCounterHor][iCounterVert] == FIELD_MARKED            or
               Fields[iCounterHor][iCounterVert] == FIELD_UNKNOWN           or
               Fields[iCounterHor][iCounterVert] < FIELD_NOT_CLICKED) {
                int iIndex = 0;
                double dColor[4] = { 0.0 };

                for(unsigned int iCounterColor = 0; iCounterColor < sizeof(dColor) / sizeof(double); ++iCounterColor)
                    dColor[iCounterColor] = BLUE[iCounterColor];

                if(Bombs[iCounterHor][iCounterVert] and (bLose  or  bVictory)) {
                    iIndex = FIELD_CHAR_BOMB;

                    if (bLose)
                        for(unsigned int iCounterColor = 0; iCounterColor < sizeof(dColor) / sizeof(double); ++iCounterColor)
                            dColor[iCounterColor] = RED[iCounterColor];

                    if (bVictory)
                        for(unsigned int iCounterColor = 0; iCounterColor < sizeof(dColor) / sizeof(double); ++iCounterColor)
                            dColor[iCounterColor] = GREEN[iCounterColor];
                }
                else if(Fields[iCounterHor][iCounterVert] == FIELD_MARKED)
                    iIndex = FIELD_CHAR_MARKED;
                else if(Fields[iCounterHor][iCounterVert] == FIELD_UNKNOWN)
                    iIndex = FIELD_CHAR_UNKNOWN;
                else if(Fields[iCounterHor][iCounterVert] == 0)
                    iIndex = FIELD_CHAR_SPACE;
                else if(Fields[iCounterHor][iCounterVert] < FIELD_NOT_CLICKED) {
                    iIndex = Fields[iCounterHor][iCounterVert] + FIELD_CHAR_NUM_START;

                    for(unsigned int iCounterColor = 0; iCounterColor < sizeof(dColor) / sizeof(double); ++iCounterColor)
                        dColor[iCounterColor] = DARKGRAY[iCounterColor];
                }

                for(unsigned int iCounter3 = 0; iCounter3 < CharSet[iIndex].size(); ++iCounter3) {
                    glBegin(GL_QUADS);
                        glColor4dv(&dColor[0]);

                        glVertex2i(iCounterHor * BLOCK_RES + CharSet[iIndex][iCounter3].iX * PIXEL_SIZE,                iCounterVert * BLOCK_RES + CharSet[iIndex][iCounter3].iY * PIXEL_SIZE);
                        glVertex2i(iCounterHor * BLOCK_RES + CharSet[iIndex][iCounter3].iX * PIXEL_SIZE + PIXEL_SIZE,   iCounterVert * BLOCK_RES + CharSet[iIndex][iCounter3].iY * PIXEL_SIZE);
                        glVertex2i(iCounterHor * BLOCK_RES + CharSet[iIndex][iCounter3].iX * PIXEL_SIZE + PIXEL_SIZE,   iCounterVert * BLOCK_RES + CharSet[iIndex][iCounter3].iY * PIXEL_SIZE + PIXEL_SIZE);
                        glVertex2i(iCounterHor * BLOCK_RES + CharSet[iIndex][iCounter3].iX * PIXEL_SIZE,                iCounterVert * BLOCK_RES + CharSet[iIndex][iCounter3].iY * PIXEL_SIZE + PIXEL_SIZE);
                    glEnd();
                }
            }

            if(iCounterVert != 0) {
                glBegin(GL_QUADS);
                    glColor4dv(&BLACK[0]);

                    glVertex2i(0,                       iCounterVert * BLOCK_RES - GRID_SIZE);
                    glVertex2i(iResolutionHorizontal,   iCounterVert * BLOCK_RES - GRID_SIZE);
                    glVertex2i(iResolutionHorizontal,   iCounterVert * BLOCK_RES + GRID_SIZE);
                    glVertex2i(0,                       iCounterVert * BLOCK_RES + GRID_SIZE);
                glEnd();
            }
        }

        if(iCounterHor != 0) {
            glBegin(GL_QUADS);
                glColor4dv(&BLACK[0]);

                glVertex2i(iCounterHor * BLOCK_RES - GRID_SIZE, 0);
                glVertex2i(iCounterHor * BLOCK_RES - GRID_SIZE, iResolutionVertical);
                glVertex2i(iCounterHor * BLOCK_RES + GRID_SIZE, iResolutionVertical);
                glVertex2i(iCounterHor * BLOCK_RES + GRID_SIZE, 0);
            glEnd();
        }
    }

    glBegin(GL_QUADS);
        glColor4dv(&BLACK[0]);

        glVertex2i(0,                       0);
        glVertex2i(iResolutionHorizontal,   0);
        glVertex2i(iResolutionHorizontal,   GRID_SIZE);
        glVertex2i(0,                       GRID_SIZE);
    glEnd();

    glBegin(GL_QUADS);
        glColor4dv(&BLACK[0]);

        glVertex2i(iResolutionHorizontal,   iResolutionVertical);
        glVertex2i(0,                       iResolutionVertical);
        glVertex2i(0,                       iResolutionVertical - GRID_SIZE);
        glVertex2i(iResolutionHorizontal,   iResolutionVertical - GRID_SIZE);
    glEnd();

    glBegin(GL_QUADS);
        glColor4dv(&BLACK[0]);

        glVertex2i(0,         0);
        glVertex2i(GRID_SIZE, 0);
        glVertex2i(GRID_SIZE, iResolutionVertical);
        glVertex2i(0,         iResolutionVertical);
    glEnd();

    glBegin(GL_QUADS);
        glColor4dv(&BLACK[0]);

        glVertex2i(iResolutionHorizontal,               0);
        glVertex2i(iResolutionHorizontal - GRID_SIZE,   0);
        glVertex2i(iResolutionHorizontal - GRID_SIZE,   iResolutionVertical);
        glVertex2i(iResolutionHorizontal,               iResolutionVertical);
    glEnd();

    glFlush();
}

void BertSweeper::mouseClick(int iButton, int iState, int iX, int iY) {
    if (iX >=   0                       and
        iY >=   0                       and
        iX <    iResolutionHorizontal   and
        iY <    iResolutionVertical) {
        if(iState == GLUT_DOWN  and
           !bLose               and
           !bVictory                ) {
            Pixel Pixel;

            Pixel.iX = int((double)iX / iResolutionHorizontal * iBlocksHorizontal);
            Pixel.iY = iBlocksVertical - 1 - int((double)iY / iResolutionVertical * iBlocksVertical);

            if(iButton == GLUT_LEFT_BUTTON) {
                clickField(&Pixel);

                if(iRemainingFields == 0)
                    bVictory = true;
            }
            else if(iButton == GLUT_RIGHT_BUTTON)
                markField(&Pixel);
        }
    }
}

void BertSweeper::keyboard(unsigned char ucKey, int iX, int iY) {
    if(ucKey == KEY_RESTART or ucKey - 'a' + 'A' == KEY_RESTART) {
        newGame();

        display();
    }
}

void BertSweeper::convertChar(int* iPosHor, int* iPosVert) {
    int iCounterX = 0;
    int iCounterY = 0;

    for(int iCounter1 = *iPosHor * CHAR_SIZE * BITMAP_DATA_WIDTH + *iPosVert * CHAR_SIZE;
        iCounter1 < *iPosHor * CHAR_SIZE * BITMAP_DATA_WIDTH + *iPosVert * CHAR_SIZE + BITMAP_DATA_HEIGTH * CHAR_SIZE;
        iCounter1 += BITMAP_DATA_WIDTH) {

        for(int iCounter2 = iCounter1; iCounter2 < iCounter1 + CHAR_SIZE; ++iCounter2) {
            if(veccData[iCounter2 * BITMAP_PIXEL_SIZE]      == 0 and
               veccData[iCounter2 * BITMAP_PIXEL_SIZE + 1]  == 0 and
               veccData[iCounter2 * BITMAP_PIXEL_SIZE + 2]  == 0) {
                Pixel Cache;

                Cache.iX = iCounterX;
                Cache.iY = iCounterY;

                CharSet[*iPosHor * CHAR_SIZE + *iPosVert].push_back(Cache);
            }

            iCounterX = (iCounterX + 1) % CHAR_SIZE;

            if(iCounterX == 0)
                ++iCounterY;
        }
    }
}

bool BertSweeper::loadBitmap() {
    std::fstream fsData;

    fsData.open(CHAR_SET_PATH, std::ios::in | std::ios::binary);

    if(!fsData)
        return false;

    veccData.clear();

    veccData.assign(std::istreambuf_iterator<char>(fsData), std::istreambuf_iterator<char>());

    fsData.close();

    if(veccData.size() < BITMAP_HEAD_SIZE + BITMAP_INFO_SIZE + BITMAP_PIXEL_SIZE * CHAR_SET_MAX or
       veccData[BITMAP_INFO_HEIGHT] != BITMAP_DATA_HEIGTH                                       or
       veccData[BITMAP_INFO_WIDTH] != BITMAP_DATA_WIDTH)
        return false;

    veccData.erase(veccData.begin(), veccData.begin() + veccData[BITMAP_HEAD_OFFSET]);

    for(int iCounter1 = 0; iCounter1 < BITMAP_DATA_HEIGTH / CHAR_SIZE; ++iCounter1)
        for(int iCounter2 = 0; iCounter2 < BITMAP_DATA_WIDTH / CHAR_SIZE; ++iCounter2)
            convertChar(&iCounter1, &iCounter2);

    for(int iCounter1 = 0; iCounter1 < CHAR_SET_MAX / CHAR_SIZE / 2; ++iCounter1) {
        int iDiff = CHAR_SET_MAX - (2 * iCounter1 + 1) * BITMAP_DATA_HEIGTH / CHAR_SIZE;

        for(int iCounter2 = 0; iCounter2 < BITMAP_DATA_HEIGTH / CHAR_SIZE; ++iCounter2) {
            int iIndex = iCounter1 * BITMAP_DATA_HEIGTH / CHAR_SIZE + iCounter2;

            std::vector<Pixel> Cache(CharSet[iIndex]);

            CharSet[iIndex] = CharSet[iIndex + iDiff];
            CharSet[iIndex + iDiff] = Cache;
        }
    }

    veccData.clear();

    return true;
}

void BertSweeper::clickField(Pixel* Pixel) {
    if(Bombs[Pixel->iX][Pixel->iY]) {
        bLose = true;

        return;
    }

    if(Fields[Pixel->iX][Pixel->iY] == FIELD_NOT_CLICKED    or
       Fields[Pixel->iX][Pixel->iY] == FIELD_MARKED         or
       Fields[Pixel->iX][Pixel->iY] == FIELD_UNKNOWN) {
        int iBombCounter = 0;

        for(int iCounter1 = Pixel->iX - 1; iCounter1 < Pixel->iX + 2; ++iCounter1) {
            for(int iCounter2 = Pixel->iY - 1; iCounter2 < Pixel->iY + 2; ++iCounter2) {
                if(iCounter1 >= 0                   and
                   iCounter1 <  iBlocksHorizontal   and
                   iCounter2 >= 0                   and
                   iCounter2 <  iBlocksVertical     and
                   (    iCounter1 != Pixel->iX   or
                        iCounter2 != Pixel->iY))
                    if(Bombs[iCounter1][iCounter2])
                        ++iBombCounter;
            }
        }

        Fields[Pixel->iX][Pixel->iY] = iBombCounter;
        --iRemainingFields;

        std::cout <<iRemainingFields << std::endl;

        if(iBombCounter == 0) {
            for(int iCounter1 = Pixel->iX - 1; iCounter1 < Pixel->iX + 2; ++iCounter1) {
                for(int iCounter2 = Pixel->iY - 1; iCounter2 < Pixel->iY + 2; ++iCounter2) {
                    if(iCounter1 >= 0                   and
                       iCounter1 <  iBlocksHorizontal   and
                       iCounter2 >= 0                   and
                       iCounter2 <  iBlocksVertical     and
                       (    iCounter1 != Pixel->iX   or
                            iCounter2 != Pixel->iY)) {
                        if(!Bombs[iCounter1][iCounter2]) {
                            struct Pixel PixelNew;

                            PixelNew.iX = iCounter1;
                            PixelNew.iY = iCounter2;

                            clickField(&PixelNew);
                        }
                    }
                }
            }
        }
    }
}

}



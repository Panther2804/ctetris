
#include "main.h"
#include "treiber.h"
#include <iostream>

const bool debug = false;

const bool SerialActive = true;

const int sizeM = 12; //size of the matrix
const int sizeplayfieldx = 10; //size of plafield (own layer in matrix)
const int sizeplayfieldy = 10;

const int playfieldposx = 0; //offset of playfield
const int playfieldposy = 0;

const int maxpiecesizex = 2;
const int maxpiecesizey = 4;


const bool lp[maxpiecesizex][maxpiecesizey] = {  //the l piece
        {1, 0, 0, 0},
        {1, 1, 1, 0},};


bool lpm[maxpiecesizex][maxpiecesizey] = {{0, 0, 1, 0}, //the mirrored l piece
                                          {1, 1, 1, 0},};

const bool cube[maxpiecesizex][maxpiecesizey] = {{0, 1, 1, 0}, //the "cube"
                                                 {0, 1, 1, 0},};

const bool line[maxpiecesizex][maxpiecesizey] = {{0, 0, 0, 0}, //the line
                                                 {1, 1, 1, 1},};

const bool halfh[maxpiecesizex][maxpiecesizey] = {{0, 1, 0, 0}, //the half h
                                                  {1, 1, 1, 0},};

bool piece[maxpiecesizex][maxpiecesizey] = {{0, 1, 0, 0},
                                            {1, 1, 1, 0},};

int playfield[sizeplayfieldx][sizeplayfieldy];


int a[sizeM][sizeM];

int posx = 0;  //position and rotation of the piece
int posy = 0;
int rotation = 0;

int posxold = 0;  //position and rotation of the "old" piece
int posyold = 0;
int rotationold = 0;

bool playerturn = false; //if true checks input if false does physicsgu
const int timeout = 10; //timeout before new piece
int timerem = 10;
int score = 0;

int main() {
    setup();
    while (true) {
        loop();
    }
}


void setup() {

    // put your setup code here, to run once:

    srand(1);
    playfieldinit();
    sbegin(9600);
    sprintln('1');
    posx = 3;
    posy = 0;
    //playfield[3][7] = 5;
    transfer();
    minit();
    mprint();

}

void loop() {
    // put your main code here, to run repeatedly:
    /*
    delay(1000);
    sprintln(rotation + 48);
    pieceselect(1);

    draw(piece, 1, true); //draw new piece

    transfer();
    mprint();
    draw(piece, 0, false); //undraw old piece
    //transfer(); //undo later (debug code)
    //mprint(); //undo later


    delay(1000);

    */

// playerturn =  !playerturn;
    if (playerturn) {

    } else {
        posy += 1;
        if (draw(piece, 1, true) == false) {
            draw(piece, 1, true);
            if (timerem == 0) {
                timerem = timeout;
            } else if (timerem == 1) {
                posy = 3;
                posx = sizeplayfieldx / 2;
                pieceselect(1);
                score++;
                linecheck();
            } else {
                timerem--;
            }
        }
    }

    transfer();
    mprint();
    draw(piece, 0, false);
    transfer();

    delay(1000);
}

void minit() {  //blanks the matrix (initializer)
    for (int i = 0; i < sizeM; i++) {
        for (int o = 0; o < sizeM; o++) {
            a[i][o] = 0;
        }
    }
}


void playfieldinit() {  //blanks the playfield (initializer)
    for (int i = 1; i <= sizeplayfieldx; i++) { //sets everything to 0
        for (int o = 1; o <= sizeplayfieldy; o++) {
            playfield[i][o] = 0;
        }
    }


    for (int i = 0; i < sizeplayfieldy; i++) { //left
        playfield[0][i] = 2;
    }

    for (int i = 0; i < sizeplayfieldy; i++) { //right
        playfield[sizeplayfieldx-1][i] = 2;
    }

    for (int i = 0; i < sizeplayfieldy; i++) {//top
        playfield[i][0] = 2;
    }

    for (int i = 0; i < sizeplayfieldy; i++) { //bottom
        playfield[i][sizeplayfieldy - 1] = 2;
    }
   playfieldprint();



}


void mprint() {  // does the actual drawing
    for (int i = 0; i < sizeM; i++) {
        for (int o = 0; o < sizeM; o++) {
            sprint(48 + a[o][i]);
            sprint(' ');
        }
        sprintln(' ');
    }
    sprintln('-');
}

void playfieldprint() {  // does the actual drawing (debug)
    for (int i = 0; i < sizeplayfieldy; i++) {
        for (int o = 0; o < sizeplayfieldx; o++) {
            sprint(48 + playfield[o][i]);
            sprint(' ');
        }
        sprintln(' ');
    }
    sprintln('p');
}



void transfer() { //transfers playfield onto canvas, updates pos & rot
    posxold = posx;
    posyold = posy;
    rotationold = rotation;

    for (int i = 0; i < sizeplayfieldx; i++) {
        for (int o = 0; o < sizeplayfieldy; o++) {
            a[playfieldposx + i][playfieldposy + o] = playfield[i][o];
        }
    }
}


bool mput(int x, int y, int color, bool clearcheck) { //'collision' check //only for playfield
    if (clearcheck) {
        if (a[x][y] != 0) {
            return false;
        }
        else return true;

    }
    else {
        playfield[x][y] = color;
        return true;
    }

}


bool draw(const bool b[][4], int color, bool ccheck) {  //handels piece drawing
    switch (rotation) {

        case 0: //no rot
            if(ccheck) {
                for (int i = 0; i < maxpiecesizex; i++) {
                    for (int o = 0; o < maxpiecesizey; o++) {
                        if (b[i][o]) {
                            if (mput(posx + i, posy + o, color, true) == false) {
                                undoplayfield();
                                return false;
                            }
                        }

                    }
                }
            }

            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        mput(posx + i, posy + o, color, false);

                        if (debug) {
                            transfer();
                            mprint();
                            sprint('i');
                            sprint(i + 48);
                            sprint('o');
                            sprintln(o + 48);
                        }
                    }

                }
            }
            return true;

            break;

        case 1:  //one clockwise

            if(ccheck) {
                for (int i = 0; i < maxpiecesizex; i++) {
                    for (int o = 0; o < maxpiecesizey; o++) {
                        if (b[i][o]) {
                            if (mput(posx + i, posy - o, color, true) == false) {
                                undoplayfield();
                                return false;
                            }
                        }

                    }
                }
            }

            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        mput(posx + i, posy - o, color, false);

                        if (debug) {
                            transfer();
                            mprint();
                            sprint('i');
                            sprint(i + 48);
                            sprint('o');
                            sprintln(o + 48);
                        }
                    }

                }
            }
            return true;

            break;

        case 2: //upside down

            if(ccheck) {
                for (int i = 0; i < maxpiecesizex; i++) {
                    for (int o = 0; o < maxpiecesizey; o++) {
                        if (b[i][o]) {
                            if (mput(posx - i, posy - o, color, true) == false) {
                                undoplayfield();
                                return false;
                            }
                        }

                    }
                }
            }

            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        mput(posx - i, posy - o, color, false);

                        if (debug) {
                            transfer();
                            mprint();
                            sprint('i');
                            sprint(i + 48);
                            sprint('o');
                            sprintln(o + 48);
                        }
                    }

                }
            }
            return true;

            break;


        case 3: //one counterclockwise

            if(ccheck) {
                for (int i = 0; i < maxpiecesizex; i++) {
                    for (int o = 0; o < maxpiecesizey; o++) {
                        if (b[i][o]) {
                            if (mput(posx - i, posy + o, color, true) == false) {
                                undoplayfield();
                                return false;
                            }
                        }

                    }
                }
            }

            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        mput(posx - i, posy + o, color, false);

                        if (debug) {
                            transfer();
                            mprint();
                            sprint('i');
                            sprint(i + 48);
                            sprint('o');
                            sprintln(o + 48);
                        }
                    }

                }
            }
            return true;

            break;

        default:
            sprintln('r');
            rotation = 0;
            return false;

    }
}

void undoplayfield() {
    posx = posxold;
    posy = posyold;
    rotation = rotationold;
}

void pieceselect(int a) {
    switch (a) {
        case 0:
            cnstcpy(piece, lp); // piece = lp;
        case 1:
            cnstcpy(piece, lpm);// piece =lpm;
        case 2:
            cnstcpy(piece, cube);//piece =cube;
        case 3:
            cnstcpy(piece, line);//piece =line;
        case 4:
            cnstcpy(piece, halfh);//piece =halfh;
    }
}

void cnstcpy(bool a[2][4], const bool b[2][4]) {
    for (int i = 0; i < maxpiecesizex; i++) {
        for (int o = 0; o < maxpiecesizey; o++) {
            a[i][o] = b[i][o];
        }

    }
}

void linecheck() {
    int c = 0;
    int l = 0;
    for (int i = 0; i < sizeplayfieldy - 1; i++) {
        for (int o = 0; o < sizeplayfieldx - 1; o++) {
            if (playfield[o][i] != 0) c++;
        }
    }
}






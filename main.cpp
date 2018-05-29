
#include "main.h"
#include "treiber.h"
#include <iostream>

const bool debug = false;

const bool SerialActive = true;

const int sizeM = 12; //size of the matrix
const int sizeplayfieldx = 7;
const int sizeplayfieldy = 12;

const int playfieldposx = 0;
const int playfieldposy = 0;

const int maxpiecesizex = 2;
const int maxpiecesizey = 4;


const bool lp[maxpiecesizex][maxpiecesizey] = {  //the l piece
        {1, 0, 0, 0},
        {1, 1, 1, 0},};


const bool lpm[maxpiecesizex][maxpiecesizey] = {{0, 0, 1, 0}, //the mirrored l piece
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
int playfieldold[sizeplayfieldx][sizeplayfieldy];

int a[sizeM][sizeM];

bool dets = false;

int posx = 0;  //position and rotation of the piece
int posy = 0;
int rotation = 3;

int posxold = 0;  //position and rotation of the piece
int posyold = 0;
int rotationold = 0;


int main() {
    setup();
    while (true) {
        loop();
    }
}


void setup() {

    // put your setup code here, to run once:

    srand(1);

    sbegin(9600);
    sprintln('1');
    posx = 3;
    posy = 3;
    //playfield[1][1] = 5;
    transfer();
    minit();
    mprint();

}

void loop() {
    // put your main code here, to run repeatedly:

    delay(1000);
    sprintln(rotation+48);


    draw(piece, 1,true); //draw new piece

    transfer();
    mprint();
    draw(piece, 0, false); //undraw old piece
    transfer(); //undo later (debug code)
    mprint(); //undo later

    rotation++;
    delay(1000);


}

void minit() {  //blanks the matrix (initializer)
    for (int i = 0; i < sizeM; i++) {
        for (int o = 0; o < sizeM; o++) {
            a[i][o] = 0;
        }
    }
}

void playfieldinit() {  //blanks the matrix (initializer)
    for (int i = 0; i <= sizeplayfieldx; i++) {
        for (int o = 0; o <= sizeplayfieldy; o++) {
            playfield[i][o] = 0;
        }
    }
    memcpy(playfield, playfieldold, sizeof(playfield));
}

void mprint() {  // does the actual drawing
    for (int i = 0; i < sizeM; i++) {
        for (int o = 0; o < sizeM; o++) {
            sprint(48 + a[i][o]);

        }
        sprintln(' ');
    }
    sprintln('-');
}

void transfer() { //transfers playfield onto canvas, updates pos & rot
    posxold = posx;
    posyold = posy;
    rotationold = rotation;
    memcpy(playfieldold, playfield, sizeof(playfield));
    for (int i = 0; i < sizeplayfieldx; i++) {
        for (int o = 0; o < sizeplayfieldy; o++) {
            a[playfieldposx + i][playfieldposy + o] = playfield[i][o];
        }
    }
}


bool mput(int x, int y, int color, bool clearcheck) { //'collision' check //only for playfield
    if (clearcheck) {
        if (a[x][y] != 0 && x == 0 && y == 0) {
            return false;
        }
        playfield[x][y] = color;
        return true;
    }
    playfield[x][y] = color;
    return true;
}


bool draw(const bool b[2][4], int color,bool ccheck) {  //handels piece drawing
    switch (rotation) {

        case 0: //no rot
            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        if (mput(posx + i, posy + o, color, ccheck) == false) {
                            undoplayfield();
                            return false;
                        }
                        if(debug) {transfer();mprint();sprint('i');sprint(i+48);sprint('o');sprintln(o+48);}
                    }

                }
            }
            return true;

            break;

        case 1:  //one clockwise
            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        if (mput(posx + o, posy - i, color, ccheck) == false) {
                            undoplayfield();
                            return false;
                        }
                        if(debug) {transfer();mprint();sprint('i');sprint(i+48);sprint('o');sprintln(o+48);}
                    }
                }
            }
            return true;

            break;

        case 2: //upside down
            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        if (mput(posx - i, posy - o, color, ccheck) == false) {
                            undoplayfield();
                            return false;
                        }
                        if(debug) {transfer();mprint();sprint('i');sprint(i+48);sprint('o');sprintln(o+48);}
                    }
                }
            }
            return true;

            break;

        case 3: //one counterclockwise
            for (int i = 0; i < maxpiecesizex; i++) {
                for (int o = 0; o < maxpiecesizey; o++) {
                    if (b[i][o]) {
                        if (mput(posx - o, posy + i, color, ccheck) == false) {
                            undoplayfield();
                            return false;
                        }
                        if(debug) {transfer();mprint();sprint('i');sprint(i+48);sprint('o');sprintln(o+48);}
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
    memcpy(playfield, playfieldold, sizeof(playfield));
    sprintln('u');
}

void pieceselect(int a) {
    switch(a) {
        case 0:
            piece = lp;
        case 1:
            piece =lpm;
        case 2:
            piece =cube;
        case 3:
            piece =line;
        case 4:
            piece =halfh;
    }
}






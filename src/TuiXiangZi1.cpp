#include <iostream>
using namespace std;

/*
map:
########
# .. p #
# oo   #
#      #
########

*/

enum OBJ_TYPE {
    OBJ_WALL,            // #
    OBJ_PLAYER,          // p
    OBJ_PLAYER_ON_DEST,  // P
    OBJ_BOX_NOT_ON_DEST, // o
    OBJ_BOX_ON_DEST,     // O
    OBJ_DEST_OF_BOX,     // .
    OBJ_EMPTY
};

enum MOVE {
    MOVE_UP,            // w
    MOVE_DOWN,          // z
    MOVE_LEFT,          // a
    MOVE_RIGHT          // s

};

const int lines = 5;
const int cols = 8;

int map[lines][cols] = { {OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL},
                  {OBJ_WALL, OBJ_EMPTY, OBJ_DEST_OF_BOX, OBJ_DEST_OF_BOX, OBJ_EMPTY, OBJ_PLAYER, OBJ_EMPTY, OBJ_WALL},
                  {OBJ_WALL, OBJ_EMPTY, OBJ_BOX_NOT_ON_DEST, OBJ_BOX_NOT_ON_DEST, OBJ_EMPTY, OBJ_EMPTY, OBJ_EMPTY, OBJ_WALL},
                  {OBJ_WALL, OBJ_EMPTY, OBJ_EMPTY, OBJ_EMPTY, OBJ_EMPTY, OBJ_EMPTY, OBJ_EMPTY, OBJ_WALL},
                  {OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL, OBJ_WALL}};

int curr_posx = 1;
int curr_posy = 5;

void draw() {
    for(int i = 0; i < lines; ++i){
        for(int j = 0; j < cols; ++j) {
            switch(map[i][j]) {
                case OBJ_WALL:
                    cout << '#'; break;
                case OBJ_PLAYER:
                    cout << 'p'; break;
                case OBJ_PLAYER_ON_DEST:
                    cout << 'P'; break;
                case OBJ_BOX_NOT_ON_DEST:
                    cout << 'o'; break;
                case OBJ_BOX_ON_DEST:
                    cout << 'O'; break;
                case OBJ_DEST_OF_BOX:
                    cout << '.'; break;
                case OBJ_EMPTY:
                    cout << ' '; break;
            }
        }
        cout << endl;
    }
}

int next_move;

void getInput(){
    char c;
    while (true){
        cout << "Please input one of w,W,z,Z,a,A,s,S" << endl;
        cin >> c;
        switch(c) {
            case 'w':
            case 'W':
                next_move = MOVE_UP;
                return;
            case 'z':
            case 'Z':
                next_move = MOVE_DOWN;
                return;
            case 'a':
            case 'A':
                next_move = MOVE_LEFT;
                return;
            case 's':
            case 'S':
                next_move = MOVE_RIGHT;
                return;
            default:
                continue;
        }
    }
}

void updateGame(){
    int next_posx, next_posy, nnext_posx, nnext_posy;
    switch (next_move) {
        case MOVE_UP:
            next_posx = curr_posx - 1;
            nnext_posx = curr_posx - 2;
            next_posy = nnext_posy = curr_posy;
            break;
        case MOVE_DOWN:
            next_posx = curr_posx + 1;
            nnext_posx = curr_posx + 2;
            next_posy = nnext_posy = curr_posy;
            break;
        case MOVE_LEFT:
            next_posy = curr_posy - 1;
            nnext_posy = curr_posy - 2;
            next_posx = nnext_posx = curr_posx;
            break;
        case MOVE_RIGHT:
            next_posy = curr_posy + 1;
            nnext_posy = curr_posy + 2;
            next_posx = nnext_posx = curr_posx;
            break;
    }

    // p' '
    // P' '
    if(map[next_posx][next_posy] == OBJ_EMPTY) {            // can move one step to next_pos, no effect for nnext_pos
        map[curr_posx][curr_posy] = (map[curr_posx][curr_posy] == OBJ_PLAYER) ?  OBJ_EMPTY : OBJ_DEST_OF_BOX;
        map[next_posx][next_posy] = OBJ_PLAYER;
        curr_posx = next_posx;
        curr_posy = next_posy;
        return;
    }
    // p.
    // P.
    if(map[next_posx][next_posy] == OBJ_DEST_OF_BOX) {      // can move one step to next_pos, no effect for nnext_pos
        map[curr_posx][curr_posy] = (map[curr_posx][curr_posy] == OBJ_PLAYER) ?  OBJ_EMPTY : OBJ_DEST_OF_BOX;
        map[next_posx][next_posy] = OBJ_PLAYER_ON_DEST;
        curr_posx = next_posx;
        curr_posy = next_posy;
        return;
    }
    // po' '
    // Po' '
    if(map[next_posx][next_posy] == OBJ_BOX_NOT_ON_DEST && map[nnext_posx][nnext_posy] == OBJ_EMPTY) {
        map[nnext_posx][nnext_posy] = OBJ_BOX_NOT_ON_DEST;
        map[next_posx][next_posy] = OBJ_PLAYER;
        map[curr_posx][curr_posy] = (map[curr_posx][curr_posy] == OBJ_PLAYER) ?  OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_posx = next_posx;
        curr_posy = next_posy;
        return;
    }
    // po.
    // Po.
    if(map[next_posx][next_posy] == OBJ_BOX_NOT_ON_DEST && map[nnext_posx][nnext_posy] == OBJ_DEST_OF_BOX) {
        map[nnext_posx][nnext_posy] = OBJ_BOX_ON_DEST;
        map[next_posx][next_posy] = OBJ_PLAYER;
        map[curr_posx][curr_posy] = (map[curr_posx][curr_posy] == OBJ_PLAYER) ?  OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_posx = next_posx;
        curr_posy = next_posy;
        return;
    }
    // pO' '
    // PO' '
    if(map[next_posx][next_posy] == OBJ_BOX_ON_DEST && map[nnext_posx][nnext_posy] == OBJ_EMPTY) {
        map[nnext_posx][nnext_posy] = OBJ_BOX_NOT_ON_DEST;
        map[next_posx][next_posy] = OBJ_PLAYER_ON_DEST;

        map[curr_posx][curr_posy] = (map[curr_posx][curr_posy] == OBJ_PLAYER) ?  OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_posx = next_posx;
        curr_posy = next_posy;
        return;
    }
    // pO.
    // PO.
    if(map[next_posx][next_posy] == OBJ_BOX_ON_DEST && map[nnext_posx][nnext_posy] == OBJ_DEST_OF_BOX) {
        map[nnext_posx][nnext_posy] = OBJ_BOX_ON_DEST;
        map[next_posx][next_posy] = OBJ_PLAYER_ON_DEST;

        map[curr_posx][curr_posy] = (map[curr_posx][curr_posy] == OBJ_PLAYER) ?  OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_posx = next_posx;
        curr_posy = next_posy;
        return;
    }

    // all other cases, can not move!
    return;
}

bool checkIfSuccess() {
    for(int i = 0; i < lines; ++i){
        for(int j = 0; j < cols; ++j) {
            if(map[i][j] == OBJ_DEST_OF_BOX || map[i][j] == OBJ_PLAYER_ON_DEST) return false;
        }
    }
    return true;
}

int main() {

    while(true){
        draw();
        if(checkIfSuccess()) break;
        getInput();
        updateGame();
    }
    cout << "WIN!" << endl;
    
    return 0;
}

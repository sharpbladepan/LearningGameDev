#include <iostream>
using namespace std;

class Array2D {
private:
    int m,n;  // m rows, n cols
    int *array;
public:
    Array2D(int _m, int _n):m(_m), n(_n) {
        array = new int[m*n];
    }
    Array2D(const Array2D &a) {
        int j,k;
        m = a.m;
        n = a.n;
        array = new int[m*n];
        for(int i = 0; i < m*n; ++i) {
            k = i % a.n;
            j = (i - k) /n;
            array[i] = a(j, k);
        }
    }
    Array2D(Array2D &&a) noexcept {
        m = a.m;
        n = a.n;
        array = a.array;
        a.array = nullptr;
    }
    ~Array2D() {
        delete[] array;
    }
    int& operator()(int x, int y) {
        return array[x*n+y];
    }
    const int& operator()(int x, int y) const {
        return array[x*n+y];
    }
    Array2D& operator=(const Array2D &a) {
        if (this->array != a.array) {
            int j,k;
            this->m = a.m;
            this->n = a.n;
            delete[] this->array;
            this->array = new int[m*n];
            for(int i = 0; i < m*n; ++i) {
                k = i % a.n;
                j = (i - k) /n;
                this->array[i] = a(j, k);  //calling: const int& operator()(int x, int y) const
            }
        }
        return *this;
    }
    Array2D& operator=(Array2D &&a) {
        m = a.m;
        n = a.n;
        array = a.array;
        a.array = nullptr;
        return *this;
    }
};

const char map[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int rows = 5;
const int cols = 8;

// starting coordinate of the player('p')
int curr_x = 1;
int curr_y = 5;

//get from key board input
int next_move;

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

void makeArray(Array2D &a2d) {
    int i = 0;
    int x = 0;  //row
    int y = 0;  //col
    while (map[i] != '\0') {
        switch (map[i]) {
            case '\n':
                y = 0; // return to first col of a new row
                ++x;   // switch to next row
                break;
            case '#':
                a2d(x,y) = OBJ_WALL;
                ++y;
                break;
            case 'p':
                a2d(x,y) = OBJ_PLAYER;
                ++y;
                break;
            case 'P':
                a2d(x,y) = OBJ_PLAYER_ON_DEST;
                ++y;
                break;
            case 'o':
                a2d(x,y) = OBJ_BOX_NOT_ON_DEST;
                ++y;
                break;
            case 'O':
                a2d(x,y) = OBJ_BOX_ON_DEST;
                ++y;
                break;
            case '.':
                a2d(x,y) = OBJ_DEST_OF_BOX;
                ++y;
                break;
            case ' ':
                a2d(x,y) = OBJ_EMPTY;
                ++y;
                break;
        }
        ++i;
    }
}

void draw(Array2D &a2d) {
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j) {
            switch(a2d(i,j)) {
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

void updateGame(Array2D &a2d){
    int next_x, next_y, nnext_x, nnext_y;
    switch (next_move) {
        case MOVE_UP:
            next_x = curr_x - 1;
            nnext_x = curr_x - 2;
            next_y = nnext_y = curr_y;
            break;
        case MOVE_DOWN:
            next_x = curr_x + 1;
            nnext_x = curr_x + 2;
            next_y = nnext_y = curr_y;
            break;
        case MOVE_LEFT:
            next_y = curr_y - 1;
            nnext_y = curr_y - 2;
            next_x = nnext_x = curr_x;
            break;
        case MOVE_RIGHT:
            next_y = curr_y + 1;
            nnext_y = curr_y + 2;
            next_x = nnext_x = curr_x;
            break;
        default:
            break;
    }

    // p' '
    // P' '
    if(a2d(next_x, next_y) == OBJ_EMPTY) {            // can move one step to next_pos, no effect for nnext_pos
        a2d(curr_x, curr_y) = (a2d(curr_x, curr_y) == OBJ_PLAYER) ? OBJ_EMPTY : OBJ_DEST_OF_BOX;
        a2d(next_x, next_y) = OBJ_PLAYER;
        curr_x = next_x;
        curr_y = next_y;
        return;
    }
    // p.
    // P.
    if(a2d(next_x, next_y) == OBJ_DEST_OF_BOX) {      // can move one step to next_pos, no effect for nnext_pos
        a2d(curr_x, curr_y) = (a2d(curr_x, curr_y) == OBJ_PLAYER) ? OBJ_EMPTY : OBJ_DEST_OF_BOX;
        a2d(next_x, next_y) = OBJ_PLAYER_ON_DEST;
        curr_x = next_x;
        curr_y = next_y;
        return;
    }
    // po' '
    // Po' '
    if(a2d(next_x, next_y) == OBJ_BOX_NOT_ON_DEST && a2d(nnext_x, nnext_y) == OBJ_EMPTY) {
        a2d(nnext_x, nnext_y) = OBJ_BOX_NOT_ON_DEST;
        a2d(next_x, next_y) = OBJ_PLAYER;
        a2d(curr_x, curr_y) = (a2d(curr_x, curr_y) == OBJ_PLAYER) ? OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_x = next_x;
        curr_y = next_y;
        return;
    }
    // po.
    // Po.
    if(a2d(next_x, next_y) == OBJ_BOX_NOT_ON_DEST && a2d(nnext_x, nnext_y) == OBJ_DEST_OF_BOX) {
        a2d(nnext_x, nnext_y) = OBJ_BOX_ON_DEST;
        a2d(next_x, next_y) = OBJ_PLAYER;
        a2d(curr_x, curr_y) = (a2d(curr_x, curr_y) == OBJ_PLAYER) ? OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_x = next_x;
        curr_y = next_y;
        return;
    }
    // pO' '
    // PO' '
    if(a2d(next_x, next_y) == OBJ_BOX_ON_DEST && a2d(nnext_x, nnext_y) == OBJ_EMPTY) {
        a2d(nnext_x, nnext_y) = OBJ_BOX_NOT_ON_DEST;
        a2d(next_x, next_y) = OBJ_PLAYER_ON_DEST;
        a2d(curr_x, curr_y) = (a2d(curr_x, curr_y) == OBJ_PLAYER) ? OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_x = next_x;
        curr_y = next_y;
        return;
    }
    // pO.
    // PO.
    if(a2d(next_x, next_y) == OBJ_BOX_ON_DEST && a2d(nnext_x, nnext_y) == OBJ_DEST_OF_BOX) {
        a2d(nnext_x, nnext_y) = OBJ_BOX_ON_DEST;
        a2d(next_x, next_y) = OBJ_PLAYER_ON_DEST;
        a2d(curr_x, curr_y) = (a2d(curr_x, curr_y) == OBJ_PLAYER) ? OBJ_EMPTY : OBJ_DEST_OF_BOX;
        curr_x = next_x;
        curr_y = next_y;
        return;
    }
    // all other cases, can not move!
}

bool checkIfSuccess(Array2D &a2d) {
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j) {
            if(a2d(i,j) == OBJ_DEST_OF_BOX || a2d(i,j) == OBJ_PLAYER_ON_DEST) return false;
        }
    }
    return true;
}

int main() {

    Array2D mapArray(rows, cols);
    makeArray(mapArray);

    while(true){
        draw(mapArray);
        if(checkIfSuccess(mapArray)) break;
        getInput();
        updateGame(mapArray);
    }
    cout << "WIN!" << endl;

    return 0;
}

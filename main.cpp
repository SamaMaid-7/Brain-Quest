#include <graphics.h>
#include <winbgim.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
using namespace std;

// BRAIN QUEST - seven small games using C++ and WinBGIm graphics.h.
// Controls: click buttons with the mouse or use the keyboard shortcuts shown.

const int BG = BLACK;
const int PANEL = 8;       // DARKGRAY
const int ACCENT = CYAN;
const int TEXTC = WHITE;
const int MUTED = LIGHTGRAY;
const int GREEN_C = GREEN;
const int RED_C = LIGHTRED;

int unlockedLevel = 1;
int totalScore = 0;
bool quitGame = false;

struct Button {
    int x, y, w, h;
    string label;
    int fill, border, text;
};

void textAt(int x, int y, const string &s, int size = 2, int color = WHITE) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    outtextxy(x, y, const_cast<char*>(s.c_str()));
}

void centeredText(int x, int y, const string &s, int size = 2, int color = WHITE) {
    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    outtextxy(x - textwidth(const_cast<char*>(s.c_str())) / 2, y, const_cast<char*>(s.c_str()));
}

void drawButton(const Button &b) {
    setfillstyle(SOLID_FILL, b.fill);
    bar(b.x, b.y, b.x + b.w, b.y + b.h);
    setcolor(b.border);
    setlinestyle(SOLID_LINE, 0, 2);
    rectangle(b.x, b.y, b.x + b.w, b.y + b.h);
    setlinestyle(SOLID_LINE, 0, 1);
    setcolor(b.text);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(b.x + (b.w - textwidth(const_cast<char*>(b.label.c_str()))) / 2,
              b.y + (b.h - textheight(const_cast<char*>(b.label.c_str()))) / 2,
              const_cast<char*>(b.label.c_str()));
}

bool inside(int x, int y, const Button &b) {
    return x >= b.x && x <= b.x + b.w && y >= b.y && y <= b.y + b.h;
}

bool clickAt(int &mx, int &my) {
    if (ismouseclick(WM_LBUTTONDOWN)) {
        getmouseclick(WM_LBUTTONDOWN, mx, my);
        return true;
    }
    return false;
}

int waitKeyOrClick(int &mx, int &my) {
    while (!kbhit()) {
        if (clickAt(mx, my)) return -1;
        delay(10);
    }
    return getch();
}

void clearScreen(const string &subtitle = "") {
    cleardevice();
    setbkcolor(BG);
    setfillstyle(SOLID_FILL, BG);
    bar(0, 0, getmaxx(), getmaxy());
    textAt(35, 22, "BRAIN QUEST", 3, ACCENT);
    textAt(38, 66, subtitle, 2, MUTED);
    setcolor(DARKGRAY);
    line(35, 98, getmaxx() - 35, 98);
}

void messageScreen(const string &heading, const string &message, bool won) {
    clearScreen("Level result");
    centeredText(getmaxx() / 2, getmaxy() / 2 - 55, heading, 3, won ? GREEN_C : YELLOW);
    centeredText(getmaxx() / 2, getmaxy() / 2, message, 2, WHITE);
    centeredText(getmaxx() / 2, getmaxy() / 2 + 50, "Press any key or click to continue", 1, MUTED);
    int mx, my;
    waitKeyOrClick(mx, my);
}

bool pauseForContinue(const string &message) {
    clearScreen("Instructions");
    centeredText(getmaxx() / 2, getmaxy() / 2 - 35, message, 2, WHITE);
    Button go = {getmaxx()/2 - 105, getmaxy()/2 + 25, 210, 48, "START  [ENTER]", BLUE, CYAN, WHITE};
    drawButton(go);
    int mx, my;
    while (true) {
        int k = waitKeyOrClick(mx, my);
        if (k == 13 || k == 32) return true;
        if (k == 27) return false;
        if (k == -1 && inside(mx, my, go)) return true;
    }
}

// Level 1: Number Guessing
bool playNumberGuess() {
    if (!pauseForContinue("Guess the hidden number from 1 to 100. You have 7 tries.")) return false;
    int target = rand() % 100 + 1, guess, tries = 7;
    string status = "Type a number and press ENTER.";
    while (tries > 0) {
        clearScreen("LEVEL 1  |  NUMBER GUESSING");
        centeredText(getmaxx()/2, 145, "Guess a number between 1 and 100", 2, WHITE);
        centeredText(getmaxx()/2, 195, "Attempts left: " + to_string(tries), 2, YELLOW);
        centeredText(getmaxx()/2, 245, status, 2, ACCENT);
        centeredText(getmaxx()/2, 300, "Enter your guess using the keyboard:", 2, MUTED);
        centeredText(getmaxx()/2, 350, "Press 1-9, then ENTER  |  ESC returns to menu", 1, MUTED);
        string input = "";
        while (true) {
            textAt(getmaxx()/2 - 50, 410, "Guess: " + input + "   ", 3, WHITE);
            int mx, my; int k = waitKeyOrClick(mx, my);
            if (k == 27) return false;
            if (k == 13) break;
            if (k == 8 && !input.empty()) input.erase(input.size()-1);
            else if (k >= '0' && k <= '9' && input.size() < 3) input += char(k);
        }
        if (input.empty()) { status = "Please enter a number."; continue; }
        guess = atoi(input.c_str());
        if (guess < 1 || guess > 100) { status = "Choose a number from 1 to 100."; continue; }
        if (guess == target) { totalScore += tries * 10; return true; }
        --tries;
        status = guess < target ? "Higher! Try a bigger number." : "Lower! Try a smaller number.";
    }
    messageScreen("Not quite!", "The hidden number was " + to_string(target) + ".", false);
    return false;
}

// Level 2: Memory Match
bool playMemoryMatch() {
    if (!pauseForContinue("Click two cards to find matching pairs. Find all 4 pairs in 12 moves.")) return false;
    int values[8] = {1,1,2,2,3,3,4,4};
    for (int i=7; i>0; --i) { int j=rand()%(i+1); int t=values[i]; values[i]=values[j]; values[j]=t; }
    bool shown[8] = {false,false,false,false,false,false,false,false};
    bool matched[8] = {false,false,false,false,false,false,false,false};
    int moves=0, pairs=0;
    while (moves < 12 && pairs < 4) {
        clearScreen("LEVEL 2  |  MEMORY MATCH");
        textAt(45, 125, "Pairs: " + to_string(pairs) + "/4     Moves: " + to_string(moves) + "/12", 2, YELLOW);
        int cw=110, ch=105, gap=22, startX=getmaxx()/2-(4*cw+3*gap)/2, startY=190;
        Button cards[8];
        for(int i=0;i<8;i++) {
            int x=startX+(i%4)*(cw+gap), y=startY+(i/4)*(ch+gap);
            cards[i] = {x,y,cw,ch, (matched[i] || shown[i]) ? to_string(values[i]) : "?", (matched[i]?GREEN:(shown[i]?BLUE:8)), (matched[i]?GREEN:CYAN), WHITE};
            drawButton(cards[i]);
        }
        textAt(45, getmaxy()-55, "Click cards or press 1-8. ESC returns to menu.", 1, MUTED);
        int first=-1, mx,my;
        while(first<0) {
            int k=waitKeyOrClick(mx,my);
            if(k==27) return false;
            if(k>='1'&&k<='8') first=k-'1';
            else if(k==-1) for(int i=0;i<8;i++) if(inside(mx,my,cards[i])) first=i;
            if(first>=0 && (matched[first] || shown[first])) first=-1;
        }
        shown[first]=true;
        clearScreen("LEVEL 2  |  MEMORY MATCH");
        textAt(45,125,"Pairs: "+to_string(pairs)+"/4     Moves: "+to_string(moves)+"/12",2,YELLOW);
        for(int i=0;i<8;i++) { int x=startX+(i%4)*(cw+gap), y=startY+(i/4)*(ch+gap); cards[i].label=(matched[i]||shown[i])?to_string(values[i]):"?"; cards[i].fill=matched[i]?GREEN:(shown[i]?BLUE:8); drawButton(cards[i]); }
        int second=-1;
        while(second<0) {
            int k=waitKeyOrClick(mx,my);
            if(k==27) return false;
            if(k>='1'&&k<='8') second=k-'1';
            else if(k==-1) for(int i=0;i<8;i++) if(inside(mx,my,cards[i])) second=i;
            if(second>=0 && (second==first || matched[second] || shown[second])) second=-1;
        }
        shown[second]=true; ++moves;
        clearScreen("LEVEL 2  |  MEMORY MATCH");
        textAt(45,125,"Pairs: "+to_string(pairs)+"/4     Moves: "+to_string(moves)+"/12",2,YELLOW);
        for(int i=0;i<8;i++) { int x=startX+(i%4)*(cw+gap), y=startY+(i/4)*(ch+gap); cards[i].label=(matched[i]||shown[i])?to_string(values[i]):"?"; cards[i].fill=matched[i]?GREEN:(shown[i]?BLUE:8); drawButton(cards[i]); }
        delay(650);
        if(values[first]==values[second]) { matched[first]=matched[second]=true; shown[first]=shown[second]=false; ++pairs; }
        else shown[first]=shown[second]=false;
    }
    if(pairs==4) { totalScore+=80; return true; }
    messageScreen("Moves finished", "You found " + to_string(pairs) + " of 4 pairs.", false); return false;
}

// Level 3: Word Scramble
bool playWordScramble() {
    if (!pauseForContinue("Unscramble five words. You get two attempts for each word.")) return false;
    const string words[5] = {"BRAIN", "LOGIC", "PUZZLE", "MEMORY", "CODING"};
    const string mixes[5] = {"NIRAB", "GICOL", "ZELZUP", "YROMEM", "GODCNI"};
    for(int i=0;i<5;i++) {
        int attempts=2; bool solved=false;
        while(attempts>0 && !solved) {
            clearScreen("LEVEL 3  |  WORD SCRAMBLE");
            centeredText(getmaxx()/2,155,"Word "+to_string(i+1)+" of 5",2,YELLOW);
            centeredText(getmaxx()/2,220,mixes[i],4,ACCENT);
            centeredText(getmaxx()/2,285,"Type the correct word and press ENTER",2,WHITE);
            centeredText(getmaxx()/2,325,"Attempts left: "+to_string(attempts),1,MUTED);
            string input=""; int mx,my;
            while(true) {
                textAt(getmaxx()/2-100,390,"Answer: "+input+"   ",3,WHITE);
                int k=waitKeyOrClick(mx,my);
                if(k==27) return false;
                if(k==13) break;
                if(k==8&&!input.empty()) input.erase(input.size()-1);
                else if((k>='a'&&k<='z')||(k>='A'&&k<='Z')) input+=char(toupper(k));
            }
            if(input==words[i]) solved=true; else { --attempts; messageScreen("Try again", "That word was not correct.", false); }
        }
        if(!solved) { messageScreen("Level not completed", "The word was " + words[i] + ".", false); return false; }
    }
    totalScore+=100; return true;
}

// Level 4: Tic-Tac-Toe against a simple computer opponent
bool playTicTacToe() {
    if (!pauseForContinue("You are X. Click a square or press 1-9. Get three Xs in a row.")) return false;
    char board[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
    const int wins[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int mx,my; bool done=false; string status="Your turn: choose a square.";
    while(!done) {
        clearScreen("LEVEL 4  |  TIC-TAC-TOE");
        centeredText(getmaxx()/2,125,status,2,YELLOW);
        int cell=105, sx=getmaxx()/2-cell*3/2, sy=175; Button cells[9];
        for(int i=0;i<9;i++) { int x=sx+(i%3)*cell, y=sy+(i/3)*cell; string label=board[i]==' '?to_string(i+1):string(1,board[i]); cells[i]={x,y,cell-5,cell-5,label,8,CYAN,board[i]=='X'?GREEN:(board[i]=='O'?RED_C:WHITE)}; drawButton(cells[i]); }
        textAt(45,getmaxy()-55,"ESC returns to menu.",1,MUTED);
        int move=-1;
        while(move<0) { int k=waitKeyOrClick(mx,my); if(k==27)return false; if(k>='1'&&k<='9')move=k-'1'; else if(k==-1)for(int i=0;i<9;i++)if(inside(mx,my,cells[i]))move=i; if(move>=0&&(move>8||board[move]!=' '))move=-1; }
        board[move]='X';
        bool playerWin=false, compWin=false;
        for(int i=0;i<8;i++){ if(board[wins[i][0]]=='X'&&board[wins[i][1]]=='X'&&board[wins[i][2]]=='X')playerWin=true; }
        if(playerWin){totalScore+=60; return true;}
        bool full=true; for(int i=0;i<9;i++)if(board[i]==' ')full=false;
        if(full){messageScreen("Draw!","The board is full.",false);return false;}
        int comp=-1;
        // First take a winning move, then block the player, otherwise choose an empty square.
        for(int mark=0;mark<2&&comp<0;mark++) for(int i=0;i<9&&comp<0;i++) if(board[i]==' '){board[i]=mark==0?'O':'X'; bool w=false; for(int j=0;j<8;j++)if(board[wins[j][0]]==board[wins[j][1]]&&board[wins[j][1]]==board[wins[j][2]]&&board[wins[j][0]]==(mark==0?'O':'X'))w=true; board[i]=' '; if(w)comp=i;}
        if(comp<0){int empties[9],n=0;for(int i=0;i<9;i++)if(board[i]==' ')empties[n++]=i;comp=empties[rand()%n];}
        board[comp]='O';
        for(int i=0;i<8;i++)if(board[wins[i][0]]=='O'&&board[wins[i][1]]=='O'&&board[wins[i][2]]=='O')compWin=true;
        if(compWin){messageScreen("Computer wins","Try another strategy next time.",false);return false;}
        full=true;for(int i=0;i<9;i++)if(board[i]==' ')full=false;if(full){messageScreen("Draw!","The board is full.",false);return false;}
        status="Your turn: choose a square.";
    }
    return false;
}

// Level 5: Code Breaker (4-digit sequence using colours 1-6)
bool playCodeBreaker() {
    if (!pauseForContinue("Crack the 4-colour code. Colours are numbered 1-6. You have 8 guesses.")) return false;
    int secret[4]; for(int i=0;i<4;i++)secret[i]=rand()%6+1;
    int tries=8; const int cols[6]={RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN};
    while(tries>0) {
        clearScreen("LEVEL 5  |  CODE BREAKER");
        centeredText(getmaxx()/2,130,"Choose four colours (1-6) using the keyboard",2,WHITE);
        centeredText(getmaxx()/2,170,"1 Red   2 Green   3 Blue   4 Yellow   5 Magenta   6 Cyan",1,MUTED);
        centeredText(getmaxx()/2,205,"Guesses left: "+to_string(tries),2,YELLOW);
        int guess[4]={0,0,0,0};
        for(int i=0;i<4;i++) {
            bool got=false; while(!got) { centeredText(getmaxx()/2,260,"Enter colour "+to_string(i+1)+" (1-6):",2,ACCENT); int mx,my,k=waitKeyOrClick(mx,my); if(k==27)return false; if(k>='1'&&k<='6'){guess[i]=k-'0';got=true;} }
            setfillstyle(SOLID_FILL,cols[guess[i]-1]); fillellipse(getmaxx()/2-120+i*80,320,22,22);
        }
        int exact=0, correct=0; bool usedS[4]={false,false,false,false}, usedG[4]={false,false,false,false};
        for(int i=0;i<4;i++)if(guess[i]==secret[i]){exact++;usedS[i]=usedG[i]=true;}
        for(int i=0;i<4;i++)if(!usedG[i])for(int j=0;j<4;j++)if(!usedS[j]&&guess[i]==secret[j]){correct++;usedS[j]=true;break;}
        if(exact==4){totalScore+=80;return true;}
        messageScreen("Code feedback", "Correct place: "+to_string(exact)+"   Correct colour, wrong place: "+to_string(correct), false);
        --tries;
    }
    messageScreen("Code not cracked", "The sequence was " + to_string(secret[0])+to_string(secret[1])+to_string(secret[2])+to_string(secret[3]), false);
    return false;
}

// Level 6: Lights Out - click a light to toggle it and its neighbours
bool playLightsOut() {
    if (!pauseForContinue("Turn all lights off. Clicking a tile toggles it and its neighbours. Limit: 15 moves.")) return false;
    int lights[25]={1,0,1,1,0, 0,1,1,0,1, 1,1,0,1,0, 0,1,0,1,1, 1,0,1,0,1};
    int moves=0; int mx,my;
    while(moves<15) {
        bool allOff=true;for(int i=0;i<25;i++)if(lights[i])allOff=false;if(allOff){totalScore+=90;return true;}
        clearScreen("LEVEL 6  |  LIGHTS OUT");
        centeredText(getmaxx()/2,125,"Moves: "+to_string(moves)+" / 15",2,YELLOW);
        int cell=68, gap=8, sx=getmaxx()/2-(5*cell+4*gap)/2, sy=165; Button tiles[25];
        for(int i=0;i<25;i++){int x=sx+(i%5)*(cell+gap),y=sy+(i/5)*(cell+gap);tiles[i]={x,y,cell,cell,lights[i]?"ON":"OFF",lights[i]?YELLOW:8,lights[i]?LIGHTCYAN:DARKGRAY,lights[i]?BLACK:WHITE};drawButton(tiles[i]);}
        textAt(45,getmaxy()-55,"Click a tile or press 1-9 for first nine tiles. ESC returns to menu.",1,MUTED);
        int pick=-1;while(pick<0){int k=waitKeyOrClick(mx,my);if(k==27)return false;if(k>='1'&&k<='9')pick=k-'1';else if(k==-1)for(int i=0;i<25;i++)if(inside(mx,my,tiles[i]))pick=i;if(pick>=25)pick=-1;}
        int r=pick/5,c=pick%5;lights[pick]=!lights[pick];if(r>0)lights[pick-5]=!lights[pick-5];if(r<4)lights[pick+5]=!lights[pick+5];if(c>0)lights[pick-1]=!lights[pick-1];if(c<4)lights[pick+1]=!lights[pick+1];moves++;
    }
    messageScreen("Move limit reached","Try to switch off every light in fewer moves.",false);return false;
}

// Level 7: Sudoku Challenge - standard 9x9 Sudoku with a predefined puzzle
bool playSudoku() {
    if (!pauseForContinue("Complete the 9x9 Sudoku. Each row, column and 3x3 box needs 1-9 without repeats.")) return false;
    const int solution[81]={
        5,3,4,6,7,8,9,1,2,
        6,7,2,1,9,5,3,4,8,
        1,9,8,3,4,2,5,6,7,
        8,5,9,7,6,1,4,2,3,
        4,2,6,8,5,3,7,9,1,
        7,1,3,9,2,4,8,5,6,
        9,6,1,5,3,7,2,8,4,
        2,8,7,4,1,9,6,3,5,
        3,4,5,2,8,6,1,7,9
    };
    int board[81]={
        5,3,0,0,7,0,0,0,0,
        6,0,0,1,9,5,0,0,0,
        0,9,8,0,0,0,0,6,0,
        8,0,0,0,6,0,0,0,3,
        4,0,0,8,0,3,0,0,1,
        7,0,0,0,2,0,0,0,6,
        0,6,0,0,0,0,2,8,0,
        0,0,0,4,1,9,0,0,5,
        0,0,0,0,8,0,0,7,9
    };
    bool fixedCell[81]; for(int i=0;i<81;i++) fixedCell[i]=board[i]!=0;
    int selected=2, mx, my;
    while(true) {
        bool complete=true, correct=true;
        for(int i=0;i<81;i++){if(board[i]==0)complete=false;if(board[i]!=solution[i])correct=false;}
        if(complete&&correct){totalScore+=120;return true;}
        clearScreen("LEVEL 7  |  SUDOKU CHALLENGE");
        centeredText(getmaxx()/2,115,"Select a cell, then press 1-9 to fill it",1,WHITE);
        int cell= min(52, (getmaxy()-220)/9);
        if(cell<35) cell=35;
        int sx=getmaxx()/2-(cell*9)/2, sy=145;
        Button cells[81];
        for(int i=0;i<81;i++){
            int x=sx+(i%9)*cell, y=sy+(i/9)*cell;
            string lab=board[i]?to_string(board[i]):" ";
            int edge=((i%9)%3==2 || (i/9)%3==2)?CYAN:DARKGRAY;
            cells[i]={x,y,cell-1,cell-1,lab,(i==selected?BLUE:8),edge,fixedCell[i]?YELLOW:WHITE};
            drawButton(cells[i]);
        }
        textAt(35,getmaxy()-45,"Mouse: click a cell | Keyboard: arrows + 1-9 | Backspace clears | ESC menu",1,MUTED);
        int k=waitKeyOrClick(mx,my);
        if(k==27)return false;
        if(k==0||k==224){int k2=getch();if(k2==75&&selected%9>0)selected--;if(k2==77&&selected%9<8)selected++;if(k2==72&&selected/9>0)selected-=9;if(k2==80&&selected/9<8)selected+=9;}
        else if(k>='1'&&k<='9'&&!fixedCell[selected])board[selected]=k-'0';
        else if(k==8&&!fixedCell[selected])board[selected]=0;
        else if(k==-1)for(int i=0;i<81;i++)if(inside(mx,my,cells[i]))selected=i;
    }
}

bool (*games[7])() = {playNumberGuess, playMemoryMatch, playWordScramble, playTicTacToe, playCodeBreaker, playLightsOut, playSudoku};
const char *gameNames[7] = {"Number Guessing", "Memory Match", "Word Scramble", "Tic-Tac-Toe", "Code Breaker", "Lights Out Puzzle", "Sudoku Challenge"};

void mainMenu() {
    while(!quitGame) {
        clearScreen("Seven levels of logic, memory and problem-solving");
        int w=getmaxx(), h=getmaxy();
        textAt(40,118,"Choose a level. Complete it to unlock the next one.",2,WHITE);
        textAt(40,153,"Unlocked: "+to_string(unlockedLevel)+" / 7",2,YELLOW);
        textAt(w-220,153,"Score: "+to_string(totalScore),2,ACCENT);
        int margin=40, gap=18, bw=(w-2*margin-gap)/2, bh=65, top=205;
        Button levelButtons[7];
        for(int i=0;i<7;i++) {
            int col=i%2,row=i/2;int x=margin+col*(bw+gap),y=top+row*(bh+15);
            bool open=(i+1)<=unlockedLevel;
            string label=to_string(i+1)+". "+gameNames[i]+(open?"":"  [LOCKED]");
            levelButtons[i]={x,y,bw,bh,label,open?8:BLACK,open?CYAN:DARKGRAY,open?WHITE:DARKGRAY};
            drawButton(levelButtons[i]);
        }
        Button exitB={w-185,h-75,145,45,"EXIT  [0]",RED,RED,WHITE};drawButton(exitB);
        textAt(40,h-58,"Mouse: click a level     Keyboard: press 1-7     Esc: exit",1,MUTED);
        int mx,my,k=waitKeyOrClick(mx,my);
        if(k==27||k=='0'){quitGame=true;break;}
        int chosen=-1;
        if(k>='1'&&k<='7')chosen=k-'1';
        else if(k==-1){if(inside(mx,my,exitB)){quitGame=true;break;}for(int i=0;i<7;i++)if(inside(mx,my,levelButtons[i]))chosen=i;}
        if(chosen>=0) {
            if(chosen+1>unlockedLevel){messageScreen("Level locked","Complete the previous level to unlock this one.",false);continue;}
            bool won=games[chosen]();
            if(won){if(chosen+2>unlockedLevel&&unlockedLevel<7)unlockedLevel=chosen+2;messageScreen("Level complete!","Great work. Your next level is now available.",true);}
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    int screenW=GetSystemMetrics(SM_CXSCREEN), screenH=GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenW, screenH, "Brain Quest - Seven Brain Games");
    // WinBGIm creates a normal Windows window; request maximization explicitly.
    delay(150);
    HWND hwnd=FindWindowA(NULL,"Brain Quest - Seven Brain Games");
    if(hwnd!=NULL) ShowWindow(hwnd,SW_MAXIMIZE);
    setbkcolor(BG);
    mainMenu();
    closegraph();
    return 0;
}

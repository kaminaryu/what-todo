// if anyone asks: im too lazy to use toLower() or smth
#include <ncurses.h>
#include <string.h>
#include <filesystem>
#include <vector>

void change_screen(int screen);
void mainmenu();
void writeChoice(int color, int y, int x, const char* str);
void lists();
void newFile();
void settings();
void drawBox();

int height, width;
int centeredX  = 0, centeredY = 0;
const int BG_X = 12 + (10*2), BG_Y = 15;

WINDOW* win;

// for screens
const int MAIN         = 0;
const int LISTS        = 1;
const int SETTINGS     = 2;
const int ADD_NEW_FILE = 10;


int main() {
    win = initscr();
    keypad(win, true);
    noecho();
    cbreak(); // get input immedietly
    curs_set(0); // set the cursor visibility

    start_color();
    if (can_change_color()) {
        init_color(COLOR_GREEN, 500, 500, 500);
    }
    init_pair(1, COLOR_BLACK, COLOR_GREEN);

    change_screen(MAIN);

    endwin();
    return 0;
}


void change_screen(int screen) {
    clear();
    refresh();
 
    if (screen == MAIN) {
        mainmenu();
    }
    else if (screen == LISTS) {
        lists();
    }
    else if (screen == SETTINGS) {
        settings();
    }
    else if (screen == ADD_NEW_FILE) {
        newFile();
    }
}


void mainmenu() {
    const char* MENUS[] = {
        "To-Do Lists",
        "Settings",
        "Quit"
    };

    const char title[] = "Please select an option";
    int ch = 0;
    int choice = 0;
 
    int old_height = 0;
    int old_width  = 0;

    // init the main main menu
    do {
        getmaxyx(win, height, width);
 
        // update the positioning if the screen resizes
        if (width != old_width || height != old_height) {
            clear();
            refresh();
            // drawBox();

            centeredX = (width - strlen(title)) / 2;
            centeredY = (height - 5) / 2;

            // attron(COLOR_PAIR(1));
            mvprintw(centeredY, centeredX, title);
            for (int i = 0; i < 3; i++) {
                if (i == choice) {
                    attron(COLOR_PAIR(1));
                }
                mvprintw(centeredY + 2 + i, centeredX + 2, MENUS[i]);
                attroff(COLOR_PAIR(1));
            }
            // attroff(COLOR_PAIR(1));

            old_height = height;
            old_width = width;
        }

        // choosing
        writeChoice(0, centeredY+2+choice, centeredX, MENUS[choice]);
        mvprintw(centeredY + 2 + choice, centeredX, " "); // removing the past arrow

        // get user input
        if (ch == 'q' || ch == 'Q') {
            // quit
            endwin();
        }
        else if ( ((ch == KEY_UP   || ch == (int)('w')) || ch == (int)('W')) && choice > 0) {
            // presses up
            choice--;
        }
        else if ( ((ch == KEY_DOWN || ch == (int)('s')) || ch == (int)('S')) && choice < 2) {
            // presses down
            choice++;
        }
        else if (ch == 10) {
            // pick the selected choice
            change_screen(choice+1);
            break;
        }

        writeChoice(1, centeredY+2+choice, centeredX, MENUS[choice]);

    } while ( (ch = getch()) );
}


void writeChoice(int color, int y, int x, const char* str) {
    attron(COLOR_PAIR(color));

    mvprintw(y, x, ">");
    mvprintw(y, x+1, " ");
    mvprintw(y, x+2, str);
    mvhline(y, x+strlen(str)+2, ' ', 22-strlen(str));

    attroff(COLOR_PAIR(color));
}


void lists() {
    int ch = 0;

    // get to-do lists file names
    std::vector<std::string> todoLists;

    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator("lists/")) {
        todoLists.push_back(file.path().stem().string());
    }

    int choice = 0;
    int titleCenter = (width - 11) / 2;
    border(' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
    mvprintw(0, titleCenter, "To-Do Lists");

    // loop over the files lists
    do {
        if (ch == 'q' || ch == 'Q') {
            change_screen(MAIN);
        }
        else if ( ((ch == KEY_UP   || ch == (int)('w')) || ch == (int)('W')) && choice > 0) {
            // presses up
            choice--;
        }
        else if ( ((ch == KEY_DOWN || ch == (int)('s')) || ch == (int)('S')) && choice < todoLists.size()-1) {
            // presses down
            choice++;
        }
        else if (ch == 'a' || ch == 'A') {
            change_screen(ADD_NEW_FILE);
        }
        else if (ch == 10) {
            // pick the selected choice
            change_screen(choice+1);
            break;
        }

        for (int i=0; i < todoLists.size(); i++) {
            if (i == choice) {
                attron(COLOR_PAIR(1));
            }
            mvprintw(i+2, 2, "%i.", i+1);
            mvprintw(i+2, 5, "%s", todoLists[i].c_str());

            attroff(COLOR_PAIR(1));
        }

    } while( (ch = getch()) );
}


void newFile() {
    clear();
    char str[11];

    mvprintw(0, 0, "File Name: ");
    echo();

    getnstr(str, 10);
    mvprintw(1, 0, str);

    getch();
    change_screen(LISTS);
}


void settings() {

}


void drawBox() {
    getmaxyx(win, height, width);
    int startX = (width - BG_X)/2;
    int startY = (height - BG_Y)/2;


    // attron(COLOR_PAIR(1));
    for (int i=0; i < BG_Y; i++) {
        mvhline(i + startY, startX, ' ', BG_X);
    }
    // attroff(COLOR_PAIR(1));
}

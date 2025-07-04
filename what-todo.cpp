#include <ncurses.h>
#include <string.h>

void change_screen(int screen);
void mainmenu();
void lists();
void settings();

int height, width;
int centeredX  = 0, centeredY = 0;

WINDOW* win;


int main() {
    win = initscr();
    keypad(win, true);
    noecho();
    cbreak(); // get input immedietly
    curs_set(0); // set the cursor visibility

    change_screen(-1);

    endwin();
    return 0;
}


void change_screen(int screen) {
    clear();
    refresh();
 
    if (screen == -1) {
        mainmenu();
    }
    else if (screen == 0) {
        lists();
        change_screen(-1);
    }
    else if (screen == 1) {
        settings();
        change_screen(-1);
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
            centeredX = (width - strlen(title)) / 2;
            centeredY = (height - 5) / 2;

            mvprintw(centeredY, centeredX, title);
            for (int i = 0; i < 3; i++) {
                mvprintw(centeredY + 2 + i, centeredX + 2, MENUS[i]);
            }

            old_height = height;
            old_width = width;
        }

        // choosing
        mvprintw(centeredY + 2 + choice, centeredX, " ");

        if (ch == 'q' || ch == 'Q') {
            return;
        }
        else if ( ((ch == KEY_UP   || ch == (int)('w')) || ch == (int)('W')) && choice > 0) {
            choice--;
        }
        else if ( ((ch == KEY_DOWN || ch == (int)('s')) || ch == (int)('S')) && choice < 2) {
            choice++;
        }
        else if (ch == 10) {
            change_screen(choice);
            break;
        }

        mvprintw(centeredY + 2 + choice, centeredX, ">");
    } while ( (ch = getch()) );
}


void lists() {
    int ch = 0;

    const char* todoLists[] {
        "Hi",
        "Lmao what this",
        "I love idk"
    };

    do {
        int titleCenter = (width - 11) / 2;
        border(' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
        mvprintw(0, titleCenter, "To-Do Lists");

        for (int i=0; i < 3; i++) {
            mvprintw(i+2, 2, "%i.", i+1);
            mvprintw(i+2, 5, todoLists[i]);
        }

        if (ch == 'q' || ch == 'Q') {
            return;
        }
    } while( (ch = getch()) );
}


void settings() {

}

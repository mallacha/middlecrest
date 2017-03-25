#include "../includes/screens.hpp"
#include "../includes/service.hpp"

using namespace std;

void fancyTitle()
{
    //attron(COLOR_PAIR(1));
    short center = (COLS-117)/2;
    mvwprintw(stdscr, 1, center, "     #####   ##    ##              ##        ##  ###                                                                 ");
    mvwprintw(stdscr, 2, center, "  ######  /#### #####    #          ##        ##  ###                                                                ");
    mvwprintw(stdscr, 3, center, " /#   /  /  ##### ##### ###         ##        ##   ##                                                           #    ");
    mvwprintw(stdscr, 4, center, "/    /  /   # ##  # ##   #          ##        ##   ##                                                          ##    ");
    mvwprintw(stdscr, 5, center, "    /  /    #     #                 ##        ##   ##                                                          ##    ");
    mvwprintw(stdscr, 6, center, "   ## ##    #     #    ###      ### ##    ### ##   ##      /##       /###   ###  /###     /##       /###     ########");
    mvwprintw(stdscr, 7, center, "   ## ##    #     #     ###    ######### ######### ##     / ###     / ###  / ###/ #### / / ###     / #### / ######## ");
    mvwprintw(stdscr, 8, center, "   ## ##    #     #      ##   ##   #### ##   ####  ##    /   ###   /   ###/   ##   ###/ /   ###   ##  ###/     ##    ");
    mvwprintw(stdscr, 9, center, "   ## ##    #     #      ##   ##    ##  ##    ##   ##   ##    ### ##          ##       ##    ### ####          ##    ");
    mvwprintw(stdscr,10, center, "   ## ##    #     ##     ##   ##    ##  ##    ##   ##   ########  ##          ##       ########    ###         ##    ");
    mvwprintw(stdscr,11, center, "   #  ##    #     ##     ##   ##    ##  ##    ##   ##   #######   ##          ##       #######       ###       ##    ");
    mvwprintw(stdscr,12, center, "      /     #      ##    ##   ##    ##  ##    ##   ##   ##        ##          ##       ##              ###     ##    ");
    mvwprintw(stdscr,13, center, "  /##/      #      ##    ##   ##    /#  ##    /#   ##   ####    / ###     /   ##       ####    /  /###  ##     ##    ");
    mvwprintw(stdscr,14, center, " /  #####           ##   ### / ####/     ####/     ### / ######/   ######/    ###       ######/  / #### /      ##    ");
    mvwprintw(stdscr,15, center, "/     ##                  ##/   ###       ###       ##/   #####     #####      ###       #####      ###/        ##   ");
    mvwprintw(stdscr,16, center, "#                                                                                                                    ");
    mvwprintw(stdscr,17, center, " ##*/                                                                                                                ");
	//attroff(COLOR_PAIR(1));
}

void loadSavedGame()
{
    IDatabase * database = locator::getDatabase();
    sqlite3_stmt* res;
    char * query="SELECT id, name FROM characters WHERE player_character_bool=1;";
    int query_length= strlen(query)+1;
    int saved_character=0;

    database->saveQuery(res, query, query_length);

    if(sqlite3_column_type(res, 0) != 5) {
    //If result row is null (i.e. no table, column type is 5)
        int i=1;
        int row_mod=3;
        short rows = LINES/2;
        short cols=COLS/2;
        char buf[4];
        char confirm = 'n';
        //int index=0;
        //short left = 26; //Length of longest line, i.e. "1 Keltian     1 Nordlundian" (5 spaces in middle)
        //short right = 4; //left minus (length of longest line minus four)

        echo();

        do {
            i=1;
            row_mod=3;
            rows = LINES/2;
            cols=COLS/2;

            clear();
            fancyTitle();

            do {
            //Print saved games

/*                if(i%2) {
                    cols = (COLS-left)/2;
                    --row_mod;
                }else{
                    cols = (COLS+right)/2;
                }*/
                mvwprintw(stdscr, rows-row_mod, cols, "%d %s", i, sqlite3_column_text(res, 1));
                ++i;
                --row_mod;
            }while(sqlite3_step(res) == SQLITE_ROW);

            //Add prompt message
            row_mod -=2;
            mvwprintw(stdscr, rows-row_mod, (COLS-20)/2, "Choose a saved game: ");
            refresh();

            //Prompt
            getstr(buf);

            //Check if valid entry
            //Make sure the entry was chosen
            sqlite3_finalize(res);
            database->saveQuery(res, query, query_length);
            saved_character = atoi(buf);
            --row_mod;
            mvwprintw(stdscr, rows-row_mod, cols, "Did you choose %s? Y/n: ", sqlite3_column_text(res, saved_character));

            refresh();
            confirm = getch();

            sqlite3_finalize(res);
            database->saveQuery(res, query, query_length);
        }while(confirm != 'Y');

        noecho();
    }

    //Clean up
    sqlite3_finalize(res);

    //Save id should be saved to
    if(saved_character) {
        IGame * game = locator::getGame();
        game->setPlayerInstance((unsigned int)saved_character);
    }
}

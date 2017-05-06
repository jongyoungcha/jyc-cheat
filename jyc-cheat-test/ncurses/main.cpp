#include <ncurses.h>

int main(){
  initscr();
  int height, width, start_y, start_x;
  height = 6;
  width = 42;
  start_y = start_x = 4;

  WINDOW* win = newwin(height, width, start_y, start_x);
  refresh();

  box(win, 0, 0);
  mvwprintw(win, 1, 2, ":::::::::::::::::::::::::::::::::::::");
  mvwprintw(win, 2, 2, ":: JYC Cheat Client                ::");
  mvwprintw(win, 3, 2, ":: First writer : jongyoungcha     ::");
  mvwprintw(win, 4, 2, ":::::::::::::::::::::::::::::::::::::");
  wrefresh(win);

  int c = getch();
  
  endwin();

  return 0;
}


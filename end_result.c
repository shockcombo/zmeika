#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//Основное меню
const char items[4][10] = {
    "Start",
    "Options", 
    "Color",
    "Exit"
};

//Подменю "Settings", выбор размерности окна
const char window[4][10] = { 
     "25x25",
     "40x40",
     "45x45",
     "Exit"
};
const char colorss[2][15] = {
       "defaut",
       "Blue+Yellow"
};
//Подменю color

//===Время игры===
time_t start,end;
int game_time = 0;

int ch = 0;
int score = 0;
unsigned choice = 0; //Выбор пользователя
unsigned cchoice = 0; //выбор цвета 
int tailX[100], tailY[100]; // Координаты всего после головы, 100 элементов.
int ntail = 1; // Количество элементов после головы.

// Для перемещения тушки змеи
int prevX;
int prevY;
int prev2X, prev2Y;

int color_pair = 3;

unsigned choise_window = 0;
int window_max = 10;
bool food_eat = false;
int napr;
// Еда и Голова
int hx = 6;
int hy = 6;
int fx = 4;
int fy = 4;
int row; // Строки
int col; // Cтолбцы
bool key_down,key_up,key_left,key_right; // для контроля клавиш

int draw_window()// Отрисовка  границ поля
{
   
   for(int i = 1;i<window_max+1;i++)
    {

       for(int j =1;j<window_max+1;j++)
        {

           mvaddch(1,j,'*');
           mvaddch(i,1,'*');
           mvaddch(window_max,j,'*');
           mvaddch(i,window_max,'*');          
        }
     }


}
int game_clear()// Очистка при завершении игры
{
hx = 6;
hy = 6;
fx = 4;
fy = 4;
score = 0;
game_time = 0; 
ntail = 1;
for(int i = 0; i<=ntail;i++){ tailX[i] = 0; tailY[i] = 0; }
}

int game_over() // Экран смерти
{

halfdelay(50); //Ожидание нажатия клавиши в 50 секунд



clear();
refresh();

mvwprintw(stdscr, row / 2, col / 2, "GAME OVER   [%d, %d]", row, col );
row=row+2;
mvwprintw(stdscr,  row / 2, col / 2, "Score: %d [%d, %d]",score, row, col, score); 
row=row+2;
mvwprintw(stdscr,  row / 2, col / 2, "TIme: %d [%d, %d]",game_time, row, col, score); 
getch();
}

int add_elem() // Вызываем, если змея что-то съела.
{
 //сохраняем координату нового элемента.
 tailX[ntail] = hx;	
 tailY[ntail] = hy;
 ntail++; 	
}

// Двигаем "тело"
int move_elem()
{
for (int i = 1; i<ntail; i++)
        {
          prev2X = tailX[i];
          prev2Y = tailY[i];
          tailX[i] = prevX;
          tailY[i] = prevY;
          prevX = prev2X;
          prevY = prev2Y;
        }
}

int food_generate()
{

// Генерация координат еды 
         while (food_eat == true) 
         {
            fx = rand()% (window_max-3)+2;

            fy = rand()% (window_max-3)+2;

            if(fx == hx && fy == hy) { food_eat = true; fx = rand()% (window_max-3)+2; fy = rand()% (window_max-3)+2; food_generate(); }// Если координаты совпали то
            else 
            {
              for(int i = 0; i<=ntail; i++)
              {
                 if (fx == tailX[i] && fy == tailY[i]) {  food_eat = true; fx = rand()% (window_max-3)+2; fy = rand()% (window_max-3)+2; food_generate();} // Если координаты совпали то
                 else food_eat = false;
              }
           }
         }

mvaddch(fy,fx,'F');
}

int snake()
{

napr = 1;
// Всякое
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    clear();
    refresh();
    halfdelay(4);//Ожидание нажатия в 4 секунд
    start = time(NULL);
    key_up = true; 
  
      for(;;){    
//Отрисовка
      
       mvprintw(1,window_max+1,"Сords [%d : %d] ",hy,hx);
       mvprintw(2,window_max+1,"Score: [%d] ",score );
       mvprintw(3,window_max+1,"FoodCord: [%d : %d] ",fy,fx);
       mvprintw(4,window_max+1,"TIme: %d ",game_time);

      
      
       
       
       attrset(A_BOLD);
       mvaddch(hy, hx, '@' | COLOR_PAIR(color_pair)| A_BOLD);//Голова
       for (int i = 1; i<ntail; i++)
       { 
          if (i<(ntail-1)) { mvaddch(tailY[i],tailX[i], '*' | COLOR_PAIR(color_pair)| A_BOLD); }//Тельце
            else {  mvaddch(tailY[i],tailX[i], '+' | COLOR_PAIR(color_pair)| A_BOLD); }//Хвостик
       }
   
         refresh();
         draw_window(); //
         food_generate();
        
          
        int ch = getch(); 
       
// Тянем тело      
        refresh();
        tailX[0] = hx;
        tailY[0] = hy;
        prevX = tailX[0];
        prevY = tailY[0];
        
        if ((ch == KEY_DOWN)&&(hy<window_max)&&(key_down == true))  { hy++; key_up = false; key_down = false;  key_left = true; key_right = true; move_elem(); napr = 1; }
        if ((ch == KEY_UP) &&(hy<window_max)&&(key_up == true))   { hy--; key_down = false; key_up = false;   key_left = true; key_right = true; move_elem(); napr = 2; }
        if ((ch == KEY_LEFT)&&(hx<window_max)&&(key_left == true))  { hx--; key_right = false; key_left = false;  key_up = true; key_down = true; move_elem(); napr = 3; }
        if ((ch == KEY_RIGHT)&&(hx<window_max)&&(key_right == true)) { hx++;  key_left = false; key_right = false; key_up = true; key_down = true; move_elem(); napr = 4; }
        
        
        for(int i = 2; i<ntail-1; i++) 
        {
        
        if (hx == tailX[i] && hy == tailY[i]){ game_over(); game_clear(); return 0;}
        }
        
        if (ch == KEY_BACKSPACE) { game_over(); game_clear(); return 0; }
        if (ch == ERR)
           { 
             
            if (napr == 1) { hy++; key_up = false;  key_left = true; key_right = true; move_elem(); napr = 1; refresh();}
            if (napr == 2) { hy--; key_down = false;  key_left = true; key_right = true; move_elem(); napr = 2; refresh(); }
            if (napr == 3)  { hx--; key_right = false;  key_up = true; key_down = true; move_elem(); napr = 3;  refresh(); }
            if (napr == 4)  { hx++;  key_left = false; key_up = true; key_down = true; move_elem(); napr = 4;  refresh(); }
           
           }
        if ((hy>=window_max)||(hx>=window_max)||(hy<=1)||(hx<=1)) { game_over(); game_clear(); return 0;}
        if (( hx == fx)&&(hy == fy)) {mvaddch(hx,hy,'@');refresh(); food_eat = true;  score++;  add_elem(); } // если змея что-то съела;
        end = time(NULL);
        game_time = end - start;
        clear();
        refresh();
}    

}

int choose_window()// Выбор размерности поля
{
initscr();
choise_window = 0;
curs_set(0);
keypad(stdscr, true);

while (true)
  {
     clear();
	
       mvprintw(0, 1,"You choosed: [ %d x %d ] \n",window_max,window_max); //?
        for ( unsigned i = 0; i < 4; i++ ) //Проходим по всем элементам меню
        {
            if ( i == choise_window ){ //Если текущий элемент совпадает с выбором пользователя
                addch('>'); //Выводим указатель

            }
            else{          
                addch(' '); //Иначе выводим " ", для равновесия
                 }
            printw("%s\n", window[i], "%s\n");
            
            
        }



      switch ( getch() )
        {
            
            case KEY_UP:
                if ( choise_window ) //Если возможно, переводим указатель вверх
                     choise_window--; 
                break;
            case KEY_DOWN:
                if (  choise_window != 3) //Если возможно, переводим указатель вниз
                     choise_window++;
                break;

             case KEY_BACKSPACE: // Функциональная клавиша
                   if ( choise_window == 0) { window_max = 25;  return 0; }
                   if (choise_window == 1){ window_max = 40;  return 0; }
                   if (choise_window == 2){ window_max = 45;  return 0; }
                   if (choise_window == 3){ return 0;                  }
                    break;             
        }
   }
}
// После нажатия клавшиы exit
int exitss()
{
clear();

while(row>0)
{
  clear();
  mvprintw(row / 2, ((col-27) / 2),"Developers: Sergey Osipov, Nikolay Baranov, NIkita Chikhalov \n");
 
  row=row-2;
  sleep(1);
  
  refresh();
  
}
}
int color_choice()
{

initscr();
choise_window = 0;
curs_set(0);
keypad(stdscr, true);

   while ( true )
    {
      clear();
      mvprintw(0, 1,"You choosed: \n");
        for ( unsigned i = 0; i < 2; i++ ) //Проходим по всем элементам меню
        {
            if ( i == cchoice ){ //Если текущий элемент совпадает с выбором пользователя
                addch('>'); //Выводим указатель
               
            }
            else
                {          
                addch(' '); //Иначе выводим " ", для равновесия
                 }
            printw("%s\n", colorss[i], "%s\n");
            
        }

       switch ( getch() )
        { 
            case KEY_UP:
                if ( cchoice ) //Если возможно, переводим указатель вверх
                    cchoice--; 
                    break;
            case KEY_DOWN:
                if ( cchoice != 1) //Если возможно, переводим указатель вниз
                    cchoice++;
                break;
             case KEY_BACKSPACE: // Функциональная клавиша
                      if (cchoice == 0) { color_pair = 3; choice = 0; return 0;}
                      if (cchoice == 1) { color_pair = 6; choice = 0; return 0;}                          
                      break;           
        }
   }
     
clear();
refresh();


}
int main()
{
	
	
    initscr();

    

    curs_set(0); //"Убиваем" курсор
    keypad(stdscr, true);  //Включаем режим удобной работы с функциональными клавишами, другими словами KEY_UP и KEY_DOWN и пр. без этого не работали бы
    
    start_color();//Инициализация цветов 
    // Инициализируем цветовые пары
    init_pair(1,  COLOR_RED,     COLOR_GREEN); //Голова змеи
    init_pair(2,  COLOR_RED,     COLOR_YELLOW); //Тело змеи
    init_pair(3,  COLOR_GREEN,     COLOR_GREEN); //Лужайка - поле
    init_pair(4,  COLOR_BLACK,     COLOR_WHITE); //Границы поля
    init_pair(5,  COLOR_BLUE,    COLOR_BLACK); // Элементы меню
    init_pair(6, COLOR_BLUE, COLOR_YELLOW);// 2 цвет питона

    
   
    getmaxyx(stdscr, row, col);
   mvprintw(row/2, col/2,"The Piton Game", row | A_UNDERLINE | COLOR_PAIR(5) );
   getch();
    while ( true )
    {
        clear();
        for ( unsigned i = 0; i < 4; i++ ) //Проходим по всем элементам меню
        {
            if ( i == choice ){ //Если текущий элемент совпадает с выбором пользователя
                addch('>'); //Выводим указатель
               
            }
            else
                {          
                addch(' '); //Иначе выводим " ", для равновесия
                 }
            printw("%s\n", items[i], "%s\n");
            
        }
        //Получаем нажатие пользователя
        switch ( getch() )
        { 
            case KEY_UP:
                if ( choice ) //Если возможно, переводим указатель вверх
                    choice--; 
                break;
            case KEY_DOWN:
                if ( choice != 3) //Если возможно, переводим указатель вниз
                    choice++;
                break;
             case KEY_BACKSPACE: // Функциональная клавиша
                       if (choice == 3) // элемент exit
                        {
                           
                            exitss();
                            endwin(); 
                            return 0;
                        }
                        else
                        {
                          if(choice == 2) color_choice();//Выбор цвета змеи.
                           if (choice == 1) choose_window(); // элемент Settings     
                             if (choice == 0)  snake();  // элемент Start
                        }                                       
                      break;           
        }
    }
    endwin();
    return 0;
}

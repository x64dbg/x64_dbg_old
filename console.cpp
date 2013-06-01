#include "console.h"

static int lasty=0;

static void move_rect(int x, int y, int new_x, int new_y, int width, int height)
{
    HANDLE hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
    PCHAR_INFO buffer=new CHAR_INFO[width*height];
    PCHAR_INFO buffer2=new CHAR_INFO[width*height];
    for(int i=0; i<width*height; i++)
    {
        buffer2[i].Char.AsciiChar=' ';
        buffer2[i].Attributes=0;
    }
    COORD buffer_size= {width, height};
    COORD buffer_index= {0, 0};
    SMALL_RECT read_rect= {x, y, x+width-1, y+height-1};
    SMALL_RECT write_rect= {new_x, new_y, new_x+width-1, new_y+height-1};
    ReadConsoleOutput(hStdOut, buffer, buffer_size, buffer_index, &read_rect);
    WriteConsoleOutput(hStdOut, buffer2, buffer_size, buffer_index, &read_rect);
    WriteConsoleOutput(hStdOut, buffer, buffer_size, buffer_index, &write_rect);
    delete [] buffer2;
    delete [] buffer;
}

void cinsert(const char* text)
{
    //get height (in lines) of the text
    char text_[deflen]="";
    strcpy(text_, text);
    int line_height=1;
    int len=strlen(text_);
    for(int i=0; i<len; i++)
        if(text_[i]=='\n')
        {
            line_height++;
            text_[i]=0;
        }
    int len2=strlen(text_);
    line_height+=len2/81;
    for(int i=0; i<len; i++)
        if(!text_[i])
        {
            len2=strlen(text_+i+1);
            if(len2)
                i++;
            line_height+=len2/81;
            i+=len2;
        }

    //get a console handle
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

    //get console cursor position
    CONSOLE_SCREEN_BUFFER_INFO screeninfo;
    GetConsoleScreenBufferInfo(hConsole, &screeninfo);

    //move a the rectangle of user text line_height lines down
    int lasty=consolegetlasty();
    int height=1+screeninfo.dwCursorPosition.Y-lasty;
    move_rect(0, lasty, 0, lasty+line_height, 80, height);

    //change the cursor position to the last known position (last console output)
    COORD insert_cur= {0, lasty};
    SetConsoleCursorPosition(hConsole, insert_cur);

    //write text to the console
    DWORD written=0;
    WriteConsoleA(hConsole, text, len, &written, 0);

    //set the cursor to the correct place
    screeninfo.dwCursorPosition.Y+=line_height;
    SetConsoleCursorPosition(hConsole, screeninfo.dwCursorPosition);

    //update lasty for the next (possible) insert
    consolesetlasty();
}

int consolegetlasty()
{
    return lasty;
}

int consolesetlasty()
{
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screeninfo;
    GetConsoleScreenBufferInfo(hConsole, &screeninfo);
    lasty=screeninfo.dwCursorPosition.Y;
    return lasty;
}

int cputs(const char* text)
{
    int ret=puts(text);
    consolesetlasty();
    return ret;
}

int cprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char msg[deflen]="";
    vsprintf(msg, format, args);
    int ret=printf(msg);
    consolesetlasty();
    return ret;
}

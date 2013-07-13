#include "math.h"
#include "value.h"

/*
operator precedence
1 ( )
2 ~     (NOT)
3 * / % (MUL DIV)
4 + -   (ADD SUB)
5 < >   (SHL SHR)
6 &     (AND)
7 ^     (XOR)
8 |     (OR)
*/

int mathisoperator(char ch)
{
    if(ch=='(' or ch==')')
        return 1;
    else if(ch=='~')
        return 2;
    else if(ch=='*' or ch=='#' or ch=='/' or ch=='%')
        return 3;
    else if(ch=='+' or ch=='-')
        return 4;
    else if(ch=='<' or ch=='>')
        return 5;
    else if(ch=='&')
        return 6;
    else if(ch=='^')
        return 7;
    else if(ch=='|')
        return 8;
    return 0;
}

/*
mathformat:
- remove doubles
*/
void mathformat(char* text)
{
    int len=strlen(text);
    char* temp=(char*)malloc(len+1);
    memset(temp, 0, len+1);
    for(int i=0,j=0; i<len; i++)
        if(mathisoperator(text[i])<3 or text[i]!=text[i+1])
            j+=sprintf(temp+j, "%c", text[i]);
    strcpy(text, temp);
    free(temp);
}

/*
- check for math operators
*/
bool mathcontains(const char* text)
{
    int len=strlen(text);
    for(int i=0; i<len; i++)
        if(mathisoperator(text[i])>1)
            return true;
    return false;
}

#ifdef _WIN64
static inline unsigned long long umulhi(unsigned long long x, unsigned long long y)
{
    return (unsigned long long)(((__uint128_t)x*y)>>64);
}

static inline long long mulhi(long long x, long long y)
{
    return (long long)(((__int128_t)x*y)>>64);
}
#else
static inline unsigned int umulhi(unsigned int x, unsigned int y)
{
    return (unsigned int)(((unsigned long long)x*y)>>32);
}

static inline int mulhi(int x, int y)
{
    return (int)(((long long)x*y)>>32);
}
#endif

bool mathdounsignedoperation(char op, uint left, uint right, uint* result)
{
    switch(op)
    {
    case '*':
        *result=left*right;
        return true;
    case '#':
        *result=umulhi(left, right);
        return true;
    case '/':
        if(right)
        {
            *result=left/right;
            return true;
        }
        return false;
    case '%':
        if(right)
        {
            *result=left%right;
            return true;
        }
        return false;
    case '+':
        *result=left+right;
        return true;
    case '-':
        *result=left-right;
        return true;
    case '<':
        *result=left<<right;
        return true;
    case '>':
        *result=left>>right;
        return true;
    case '&':
        *result=left&right;
        return true;
    case '^':
        *result=left^right;
        return true;
    case '|':
        *result=left|right;
        return true;
    }
    return false;
}

bool mathdosignedoperation(char op, sint left, sint right, sint* result)
{
    switch(op)
    {
    case '*':
        *result=left*right;
        return true;
    case '#':
        *result=mulhi(left, right);
        return true;
    case '/':
        if(right)
        {
            *result=left/right;
            return true;
        }
        return false;
    case '%':
        if(right)
        {
            *result=left%right;
            return true;
        }
        return false;
    case '+':
        *result=left+right;
        return true;
    case '-':
        *result=left-right;
        return true;
    case '<':
        *result=left<<right;
        return true;
    case '>':
        *result=left>>right;
        return true;
    case '&':
        *result=left&right;
        return true;
    case '^':
        *result=left^right;
        return true;
    case '|':
        *result=left|right;
        return true;
    }
    return false;
}

bool mathhandlebrackets(char* string, const char* base)
{
    dbg("brackets");
    int len=strlen(string);
    char* a=string;
    char* end=a+len;
    while(*a and *a!='(')
        a++;
    if(a!=end)
        if(!mathhandlebrackets(a+1, string))
            return false;
    if(base and string!=base) //don't do the base calculations
    {
        len=strlen(string);

        int i;
        for(i=0; i<len; i++)
            if(string[i]==')')
            {
                string[i]=0;
                i++;
                break;
            }
        i--;
        uint value=0;
        if(!mathfromstring(string, &value))
            return false;
        char* bak=0;
        int len2=strlen(string+i+1);
        bool has_bak=false;
        if(len2)
        {
            has_bak=true;
            bak=(char*)malloc(len2+1);
            strcpy(bak, string+i+1);
        }
        int len3=sprintf(string-1, "%x"fext, value);
        if(has_bak)
            strcpy(string-1+len3, bak);
        if(bak)
            free(bak);
    }
    return true;
}

/*
- handle math
*/
bool mathfromstring(const char* string, uint* value)
{
    //do math
    int highestop=0;
    int highestop_pos=0;
    int len=strlen(string);
    for(int i=0; i<len; i++)
    {
        int curop=mathisoperator(string[i]);
        if(curop>1 and curop>highestop)
        {
            highestop=curop;
            highestop_pos=i;
        }
    }
    char* strleft=(char*)malloc(len+1);
    char* strright=(char*)malloc(len+1);
    memset(strleft, 0, len+1);
    memset(strright, 0, len+1);
    strncpy(strleft, string, highestop_pos);
    strcpy(strright, string+highestop_pos+1);
    //printf("left: %s, right: %s, op: %c\n", strleft, strright, string[highestop_pos]);
    if(!*strright)
    {
        free(strleft);
        free(strright);
        return false;
    }
    uint right=0;
    if(!valfromstring(strright, &right, 0, 0))
    {
        free(strleft);
        free(strright);
        return false;
    }
    if(string[highestop_pos]=='~')
    {
        right=~right;
        if(!strlen(strleft))
        {
            *value=right;
            free(strleft);
            free(strright);
            return true;
        }
    }
    uint left=0;
    if(!valfromstring(strleft, &left, 0, 0))
    {
        free(strleft);
        free(strright);
        return false;
    }
    bool math_ok;
    if(valuesignedcalc())
        math_ok=mathdosignedoperation(string[highestop_pos], left, right, (sint*)value);
    else
        math_ok=mathdounsignedoperation(string[highestop_pos], left, right, value);
    free(strleft);
    free(strright);
    return math_ok;
}


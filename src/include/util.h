#ifndef UTIL_H_
#define UTIL_H_

#define sleep _sleep
#define CLEAR "\033[H\033[J"
#define RESET "\033[0m"
#define LLRESET "\033[A\33[2K" // '\033[A' move cursor to up one line, '\33[2K' erase all the content of the line 
#define CLRESET "\33[2K"

#define RED "\033[31m" // regular
#define GRN "\033[32m"
#define BLUE "\033[34m" 
#define PRPL "\033[35m"

#define BHRED "\033[91m" // Bold High
#define BHGRN "\033[92m"
#define BHBLUE "\033[94m"
#define BHWHT "\033[97m"

#endif
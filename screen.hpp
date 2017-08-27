#ifndef SCREEN_H
#define SCREEN_H

extern "C" {
	#include "syscall.h"
	#include "fxlib.h"
	#include "stdlib.h"
	#include "string.h"
	#include "MonochromeLib.h"
}
#define SCR_DSPLINES 10
#define SCR_NBRLINES 100
#define SCR_CHARSPERLINE 21

class Screen{
	public :
		Screen();
		int init();
		void rewritescreen();
		void write(char data);
		void clear();
		void newline();
		void setString(char* str, unsigned char line);
		void writeLine(char* str);
		void setActualLine(unsigned char line);
		int getScrindex();
		void setScrindex(int newscr);
	private :
		//char screenContent[SCR_NBRLINES][SCR_CHARSPERLINE+1];
		char** screenContent;
		int actualLine;
		int scrindex;
};


#endif // SCREEN_H
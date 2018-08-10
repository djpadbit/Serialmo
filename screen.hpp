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
		void cleanup();
		void rewritescreen();
		void write(char data);
		void clear();
		void backspace();
		void newline();
		void bell();
		void setString(char* str, unsigned char line);
		void writeLine(char* str);
		void setActualLine(unsigned char line);
		void setScrindex(int newscr);
		int actualLine;
		int scrindex;
		int chrindex;
		//int rockback;
	private :
		//char screenContent[SCR_NBRLINES][SCR_CHARSPERLINE+1];
		char** screenContent;
};


#endif // SCREEN_H
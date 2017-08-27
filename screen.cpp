#include "screen.hpp"
Screen::Screen()
{
	//Switch init to a dedicated function so i can have a return value
}

int Screen::init()
{
	//init vars
	//Stack overflow gud:
	//	https://stackoverflow.com/questions/1970698/using-malloc-for-allocation-of-multi-dimensional-arrays-with-different-row-lengt
	int i;
	if (( screenContent = (char**)malloc( SCR_NBRLINES*sizeof( char* ))) == NULL ) return 0;
	for (i=0; i < SCR_NBRLINES; i++ ) { if (( screenContent[i] = (char*)malloc( SCR_CHARSPERLINE+1 )) == NULL ) return 0; }

	for(i=0;i<SCR_NBRLINES;i++)
	{
		screenContent[i][0]=0;
	}
	actualLine = 0;
	scrindex = 0;
	//init screen
	ML_clear_vram();
	return 1;
}

void Screen::writeLine(char* str)
{
	if(screenContent[actualLine][0]!=0)
		newline();
	setString(str,actualLine);
	newline();
	//rewritescreen();
}
//unsigned char tempvar;
void Screen::setString(char* str, unsigned char line)
{
	int j,disp;
	//tempvar = line;
	for(j=0;j<SCR_CHARSPERLINE;j++)
	{
		if(str[j] == 0)
		{
			break;
		}
		char cur = str[j];
		screenContent[line][j]=cur;
		char str[] = {cur,0};
		if (int(line/float(10)) <= scrindex) PrintMini(j*6,6*(line%SCR_DSPLINES),(unsigned char*)str,MINI_OVER); disp = 1;
	}
	screenContent[line][j] = 0;
	if (disp) {
		ML_display_vram_lines(6*(line%SCR_DSPLINES),6*((line%SCR_DSPLINES)+1));
		if (scrindex != int(float(actualLine)/float(SCR_DSPLINES))) setScrindex(int(float(actualLine)/float(SCR_DSPLINES)));
	}
}

void Screen::setActualLine(unsigned char line)
{
	if((int)line >= 0 && (int)line < SCR_NBRLINES)
	{
		actualLine = line;
	}
}

int Screen::getScrindex()
{
	return scrindex;
}

void Screen::setScrindex(int newscr)
{
	scrindex = newscr;
	rewritescreen();
}

void Screen::write(char data)
{
	//verify if the char is a carrage return or a line feed
	if(data == 0xD) return;
	if(data == 0xA)
	{
		newline();
	}
	else
	{
		short unsigned int longueur = strlen(screenContent[actualLine]);
		if(longueur >= SCR_CHARSPERLINE)
		{
			newline();
			longueur = 0;
		}
		//Add the new char
		screenContent[actualLine][longueur] = data;
		screenContent[actualLine][longueur+1] = 0;
		//Writing
		if (scrindex != int(float(actualLine)/float(SCR_DSPLINES))) setScrindex(int(float(actualLine)/float(SCR_DSPLINES)));
		else {
			char str[] = {data,0};
			/*if (int(actualLine/float(10)) <= scrindex) */PrintMini(longueur*6,6*(actualLine%SCR_DSPLINES),(unsigned char*)str,MINI_OVER);
			ML_display_vram_lines(6*(actualLine%SCR_DSPLINES),6*((actualLine%SCR_DSPLINES)+1));
		}
		//int k = actualLine % SCR_DSPLINES;
		//char str[] = {data,0};
		//PrintMini(longueur*6,6*k,(unsigned char*)str,MINI_OVER);
		//ML_display_vram();
	}
}

void Screen::clear()
{
	ML_clear_vram();
	for(int i=0;i<SCR_NBRLINES-1;i++)
	{
		screenContent[i][0] = 0;
	}
	actualLine=0;
	setScrindex(0);
}

void Screen::newline()
{
	if(actualLine>=SCR_NBRLINES-1)
	{
		//shifts the lines to free the last
		for(int i=0;i<SCR_NBRLINES-1;i++)
		{
			for(int j=0;j<SCR_CHARSPERLINE;j++)
			{
				if(screenContent[i+1][j] == 0)
				{
					screenContent[i][j] = 0;
					break;
				}
				screenContent[i][j]=screenContent[i+1][j];
			}
		}
		screenContent[actualLine][0] = 0;
		rewritescreen();
	}
	else
	{
		actualLine++;
		screenContent[actualLine][0] = 0;
		//rewritescreen();
	}
}


void Screen::rewritescreen()
{
	ML_clear_vram();
	for(int i=0;i<SCR_DSPLINES;i++)
	{
		for(int j=0;j<SCR_CHARSPERLINE;j++)
		{
			int k = i+(SCR_DSPLINES*scrindex);//(SCR_DSPLINES*int(float(actualLine)/float(SCR_DSPLINES)));
			//char cur = screenContent[][j]
			if(screenContent[k][j] == 0)
				break;
			char str[] = {screenContent[k][j],0};
			PrintMini(j*6,6*i,(unsigned char*)str,MINI_OVER);
		}
		// PrintMini(0,6*i,(unsigned char*)screenContent[i],MINI_OVER);
	}
	ML_display_vram();
}

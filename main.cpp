extern "C" {
#define __KEYBIOS_H__
#include "fxlib.h"
}
#include <iostream>
#include "screen.hpp"
#include "compatibilityscripts.cpp" 

#define keypressed(code,min,maj,nbr) \
	if(KeyDown(code)){ \
	(writemode==0)?sendbyte=(min):((writemode==1)?sendbyte=(maj):sendbyte=(nbr)); \
	if(sendbyte==0x0A && !unix){Serial_WriteByte(0x0D);Serial_WriteByte(sendbyte);} \
	else Serial_WriteByte(sendbyte); \
	if(showSended)screen.write(sendbyte); \
	while(KeyDown(code) && !repetition){Sleep(1);}}

unsigned char speed = 9;
/* Speeds
0	300
1	600
2	1200
3	2400
4	4800
5	9600
6	19200
7	38400
8	57600
9	115200
*/
bool opened = false;

void setupSerial()
{
	if(opened)Serial_Close(1);
	unsigned char conf[]={0,speed,0,0,0,0};
	Serial_Open(conf);
	opened = true;
}

void printspeed(Screen *s)
{
	switch(speed) {
		case 0: s->writeLine("300 Bps");break;
		case 1: s->writeLine("600 Bps");break;
		case 2: s->writeLine("1200 Bps");break;
		case 3: s->writeLine("2400 Bps");break;
		case 4: s->writeLine("4800 Bps");break;
		case 5: s->writeLine("9600 Bps");break;
		case 6: s->writeLine("19200 Bps");break;
		case 7: s->writeLine("38400 Bps");break;
		case 8: s->writeLine("57600 Bps");break;
		case 9: s->writeLine("115200 Bps");break;
	}
}

void info(Screen *s)
{
	s->writeLine("SerMo 2.0 by DJPadbit");
	s->writeLine("Originally by Ziqumu");
	s->writeLine("F1: min, MAJ, Numbers");
	s->writeLine("F2: read hexa");
	s->writeLine("F3: clear");
	s->writeLine("F4: key repeat");
	s->writeLine("F5: show sended char");
	s->writeLine("F6: quit");
	s->writeLine("UP: Go up");
	s->writeLine("DOWN: Go down");
	s->writeLine("SHIFT: Show info");
	s->writeLine("OPTN: CRLF/LF");
	s->writeLine("VARS: Serial speed");
	//s->writeLine("ALPHA:Toggle Rockback");
	//s->setScrindex(0);
}

/* cba to do this shit
unsigned char* pad(unsigned char* topad)
{
	unsigned char toret[21];
	int size = float(sizeof(topad))/sizeof(unsigned char);
	int sid = float(21-(size))/2;
	for (int i=0;i<sid;i++) {
		if (!i) toret[i] = (unsigned char)"|";
		else toret[i] = (unsigned char)" ";
	}
	for (int i=sid;i<size;i++) toret[i] = topad[i];
	for (int i=sid+size;i<21;i++) {
		if (i == 20) toret[i] = (unsigned char)"|";
		else toret[i] = (unsigned char)" ";
	}
	return &toret;
}*/

void error(unsigned char* err) // that is really jenky and bad but it works
{							   // And it's not pretty at all
	unsigned int key;
	locate(1,2);		//21
	Print((unsigned char*)"--------ERROR--------");
	locate(1,3);
	Print((unsigned char*)"|                   |");
	locate(1,4);
	Print(err);
	locate(1,5);
	Print((unsigned char*)"|                   |");
	locate(1,6);
	Print((unsigned char*)"|   Press any key   |");
	locate(1,7);
	Print((unsigned char*)"---------------------");
	GetKey(&key);
}

int main()
{
	//defaults settings
	bool hex = false;
	bool repetition = false;
	bool showSended = true;
	bool unix = true;
	unsigned char writemode = 0;//0:min , 1:maj, 2:nombres
	//init
	unsigned char readbyte = 0;
	unsigned char sendbyte = 0;
	//start and prepare screen
	Screen screen;
	if (!screen.init()) {error((unsigned char*)"| Not enough memory |");return 0;}
	info(&screen);
	//open serial port	
	setupSerial();
	//Keys
	while(1)
	{
		//F1:Send in Min, Maj, Nbr
		if(KeyDown(KEY_CTRL_F1)){
			writemode = (writemode+1)%3;
			switch(writemode)
			{
				case 0 : screen.writeLine("Send lowercase"); break;
				case 1 : screen.writeLine("Send uppercase"); break;
				case 2 : screen.writeLine("Send numbers"); break;
			}
			while(KeyDown(KEY_CTRL_F1)){Sleep(1);}}
		//F2:Receive in hex or not
		if(KeyDown(KEY_CTRL_F2)){
			hex=!hex;
			if(hex)screen.writeLine("Receive in hexa");
			else screen.writeLine("Receive in ASCII");
			while(KeyDown(KEY_CTRL_F2)){Sleep(1);}}
		//F3:Clear screen
		if(KeyDown(KEY_CTRL_F3)){
			screen.clear();
			while(KeyDown(KEY_CTRL_F3)){Sleep(1);}}
		//F4:authorize or not key repetition
		if(KeyDown(KEY_CTRL_F4)){
			repetition=!repetition;
			if(repetition)screen.writeLine("Key repetition ON");
			else screen.writeLine("Key repetition OFF");
			while(KeyDown(KEY_CTRL_F4)){Sleep(1);}}
		//F5:Show sended chars
		if(KeyDown(KEY_CTRL_F5)){
			showSended=!showSended;
			if(showSended)screen.writeLine("Show sended chars");
			else screen.writeLine("Hide sended chars");
			while(KeyDown(KEY_CTRL_F5)){Sleep(1);}}
		//F6&Menu:Quit
		if(KeyDown(KEY_CTRL_F6)||KeyDown(KEY_CTRL_MENU)){
			break;
			//while(KeyDown(KEY_CTRL_F6)||KeyDown(KEY_CTRL_MENU)){Sleep(1);}
		}
		//OPTN:Unix/Win(shit) line ending (LF/CRLF)
		if(KeyDown(KEY_CTRL_OPTN)){
			unix=!unix;
			if(unix)screen.writeLine("Using LF");
			else screen.writeLine("Using CRLF");
			while(KeyDown(KEY_CTRL_OPTN)){Sleep(1);}}
		//VARS:Serial Speed
		if(KeyDown(KEY_CTRL_VARS)){
			if (speed==9) speed=0;
			else speed++;
			printspeed(&screen);
			setupSerial(); // Reopen serial to change baud rate
			while(KeyDown(KEY_CTRL_VARS)){Sleep(1);}}
		//SHIFT:Print info
		if(KeyDown(KEY_CTRL_SHIFT)){
			info(&screen);
			while(KeyDown(KEY_CTRL_SHIFT)){Sleep(1);}}
		//UP:Scroll up (duh)
		if(KeyDown(KEY_CTRL_UP)){
			int line = screen.getScrindex();
			if (line > 0) screen.setScrindex(line-1);
			while(KeyDown(KEY_CTRL_UP)){Sleep(1);}}
		//DOWN:Scroll down (duh)
		if(KeyDown(KEY_CTRL_DOWN)){
			int line = screen.getScrindex();
			  //line < int(float(SCR_NBRLINES)/float(SCR_DSPLINES))-1
			if (line < SCR_NBRLINES-SCR_DSPLINES) screen.setScrindex(line+1);
			while(KeyDown(KEY_CTRL_DOWN)){Sleep(1);}}
		//ALPHA:Toggle Rockback
		//still in alpha HA! got the joke ?
		//sorry
		/*if(KeyDown(KEY_CTRL_ALPHA)){
			screen.rockback = !screen.rockback;
			if(screen.rockback)screen.writeLine("Rockback on");
			else screen.writeLine("Rockback off");
			while(KeyDown(KEY_CTRL_ALPHA)){Sleep(1);}}*/
		//EXE:NewLine 
		keypressed(KEY_CTRL_EXE,0x0A,0x0A,0x0A);
		//Letters
		keypressed(KEY_CHAR_0,'z','Z','0');
		keypressed(KEY_CHAR_1,'u','U','1');
		keypressed(KEY_CHAR_2,'v','V','2');
		keypressed(KEY_CHAR_3,'w','W','3');
		keypressed(KEY_CHAR_4,'p','P','4');
		keypressed(KEY_CHAR_5,'q','Q','5');
		keypressed(KEY_CHAR_6,'r','R','6');
		keypressed(KEY_CHAR_7,'m','M','7');
		keypressed(KEY_CHAR_8,'n','N','8');
		keypressed(KEY_CHAR_9,'o','O','9');
		keypressed(KEY_CTRL_XTT,'a','A','A');
		keypressed(KEY_CHAR_LOG,'b','B','B');
		keypressed(KEY_CHAR_LN,'c','C','C');
		keypressed(KEY_CHAR_SIN,'d','D','D');
		keypressed(KEY_CHAR_COS,'e','E','E');
		keypressed(KEY_CHAR_TAN,'f','F','F');
		keypressed(KEY_CHAR_FRAC,'g','G','G');
		keypressed(KEY_CTRL_FD,'h','H','H');
		keypressed(KEY_CHAR_LPAR,'i','I','(');
		keypressed(KEY_CHAR_RPAR,'j','J',')');
		keypressed(KEY_CHAR_COMMA,'k','K',',');
		keypressed(KEY_CHAR_STORE,'l','L','L');
		keypressed(KEY_CHAR_DP,' ','=','.');
		keypressed(KEY_CHAR_EXP,'"','"','"');
		keypressed(KEY_CHAR_PLUS,'x','X','+');
		keypressed(KEY_CHAR_MINUS,'y','y','-');
		keypressed(KEY_CHAR_MULT,'s','S','*');
		keypressed(KEY_CHAR_DIV,'t','T','/')
		//Read serial
		if(Serial_ReadByte(&readbyte)==0) {
			if(!hex) {
				screen.write(readbyte);
			} else {
				//hex (string) conversion
				screen.write(' ');//delete this if you don't want space between hex bytes
				//First 4 bits
				if((readbyte>>4) <10)
					screen.write((readbyte>>4)+48);
				else
					screen.write((readbyte>>4)+87);
				//Last 4 bits
				if((readbyte&15) <10)
					screen.write((readbyte&15)+48);
				else
					screen.write((readbyte&15)+87);
			}
		}
	}
	Serial_Close(1);
	screen.cleanup();
}

extern "C" 
{
	int AddIn_main(int isAppli, unsigned short OptionNum)
	{
		return main();
	}

	#pragma section _BR_Size
	unsigned long BR_Size;
	#pragma section
	#pragma section _TOP
	int InitializeSystem(int isAppli, unsigned short OptionNum){
		return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
	}
	#pragma section
}
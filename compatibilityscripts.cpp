#define KEY_CHAR_0 OS2(0x30,71)
#define KEY_CHAR_1 OS2(0x31,72)
#define KEY_CHAR_2 OS2(0x32,62)
#define KEY_CHAR_3 OS2(0x33,52)
#define KEY_CHAR_4 OS2(0x34,73)
#define KEY_CHAR_5 OS2(0x35,63)
#define KEY_CHAR_6 OS2(0x36,53)
#define KEY_CHAR_7 OS2(0x37,74)
#define KEY_CHAR_8 OS2(0x38,64)
#define KEY_CHAR_9 OS2(0x39,54)
#define KEY_CHAR_DP OS2(0x2e,61)
#define KEY_CHAR_EXP OS2(0x0f,51)
#define KEY_CHAR_PMINUS OS2(0x87,41)
#define KEY_CHAR_PLUS OS2(0x89,42)
#define KEY_CHAR_MINUS OS2(0x99,32)
#define KEY_CHAR_MULT OS2(0xa9,43)
#define KEY_CHAR_DIV OS2(0xb9,33)
#define KEY_CHAR_FRAC OS2(0xbb,75)
#define KEY_CHAR_LPAR OS2(0x28,55)
#define KEY_CHAR_RPAR OS2(0x29,45)
#define KEY_CHAR_COMMA OS2(0x2c,35)
#define KEY_CHAR_STORE OS2(0x0e,25)
#define KEY_CHAR_LOG OS2(0x95,66)
#define KEY_CHAR_LN OS2(0x85,56)
#define KEY_CHAR_SIN OS2(0x81,46)
#define KEY_CHAR_COS OS2(0x82,36)
#define KEY_CHAR_TAN OS2(0x83,26)
#define KEY_CHAR_SQUARE OS2(0x8b,67)
#define KEY_CHAR_POW OS2(0xa8,57)
#define KEY_CTRL_EXE OS2(30004,31)
#define KEY_CTRL_DEL OS2(30025,44)
#define KEY_CTRL_AC OS2(30015,32)
#define KEY_CTRL_FD OS2(30046,65)
#define KEY_CTRL_EXIT OS2(30002,47)
#define KEY_CTRL_SHIFT OS2(30006,78)
#define KEY_CTRL_ALPHA OS2(30007,77)
#define KEY_CTRL_OPTN OS2(30008,68)
#define KEY_CTRL_VARS OS2(30016,58)
#define KEY_CTRL_UP OS2(30018,28)
#define KEY_CTRL_DOWN OS2(30023,37)
#define KEY_CTRL_LEFT OS2(30020,38)
#define KEY_CTRL_RIGHT OS2(30021,27)
#define KEY_CTRL_F1 OS2(30009,79)
#define KEY_CTRL_F2 OS2(30010,69)
#define KEY_CTRL_F3 OS2(30011,59)
#define KEY_CTRL_F4 OS2(30012,49)
#define KEY_CTRL_F5 OS2(30013,39)
#define KEY_CTRL_F6 OS2(30014,29)
#define KEY_CTRL_MENU OS2(30003,48)
#define KEY_CTRL_XTT OS2(30001,76)
 
#ifndef isOS2// This is new
 
typedef int(*sc_i2cp2sip)(char*, char*, short int*, short int*);
const unsigned int sc0015[] = { 0xD201D002, 0x422B0009, 0x80010070, 0x0015 };
#define GlibGetOSVersionInfo (*(sc_i2cp2sip)sc0015)
 
int OSVersionAsInt(void)
{
	char mainversion;
	char minorversion;
	short release;
	short build;
	GlibGetOSVersionInfo( &mainversion, &minorversion, &release, &build );
	return ( ( mainversion << 24 ) & 0xFF000000 ) | ( ( minorversion << 16 ) & 0x00FF0000 ) | ( release & 0x0000FFFF );
}
 
#define isOS2 (OSVersionAsInt() >= 0x02020000)
#define OS2(x,y) ((OSVersionAsInt() >= 0x02020000)?y:x)
 
#endif
 
const unsigned short* keyboardregister = (unsigned short*)0xA44B0000;
 
unsigned short key[8] = {0};
 
int KeyDown(int keycode)
{
	if(isOS2)
	{
		int row, col, word, bit;
		row = keycode%10;
		col = keycode/10-1;
		word = row>>1;
		bit = col + 8*(row&1);
		memcpy(key, keyboardregister, sizeof(unsigned short) << 3);
		//Sleep(1); //slow down, this function is faster than IsKeyDown() :-P
		return (0 != (key[word] & 1<<bit));
	}
	else
	{
		return IsKeyDown(keycode);
	}
}
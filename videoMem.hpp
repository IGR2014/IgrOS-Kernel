#ifndef	VIDEO_MEMORY_H
#define	VIDEO_MEMORY_H


// Video memory colors enumeration
enum videoMemColor {

        black   	= 0x00,
        blue		= 0x01,
	green		= 0x02,
	cyan		= 0x03,
	red		= 0x04,
	magenta		= 0x05,
	brown		= 0x06,
	lightGray	= 0x07,
	darkGray	= 0x08,
	lightBlue	= 0x09,
	lightGreen	= 0x0A,
	lightCyan	= 0x0B,
	lightRed	= 0x0C,
	lightMagenta	= 0x0D,
	yellow		= 0x0E,
	white		= 0x0F

};

// Video memory symbol
struct videoMemSymbol {

	char		symbol;
	unsigned char	color;

};

// Video memory cursor position
struct videoMemCursor {

        unsigned short	X;
        unsigned short	Y;

};

static const unsigned short	videoMemWidth	= 80;
static const unsigned short	videoMemHeight	= 25;
static const unsigned short	videoMemSize	= 2000;

static unsigned short		videoMemX	= 0;
static unsigned short		videoMemY	= 0;

// Video memory start pointer
static videoMemSymbol*		videoMemPtr	= (videoMemSymbol*)0xb8000;

// Clear wideo memory
void videoMemClear();

// Set cursor position to X, Y
void videoMemSetCursor(const unsigned int&, const unsigned int&);

// Write symbol to video memory
void videoMemWriteSymbol(const char&);

// Write message to video memory
void videoMemWriteMessage(const char*, const unsigned int&);

// Write message to video memory (overloaded)
void videoMemWriteMessage(const char*);

// Write line to video memory
void videoMemWriteLine(const char*, const unsigned int&);

// Write line to video memory (overloaded)
void videoMemWriteLine(const char*);

#endif	// VIDEO_MEMORY_H

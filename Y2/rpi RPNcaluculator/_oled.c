#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <Python.h>

extern unsigned char ucFont[], ucSmallFont[];
static int iScreenOffset; 
static unsigned char ucScreen[1024]; 
static int file_i2c = 0;
static int oled_type, oled_flip;

static int SDA=2;

static int SCL=3;


enum {
  OLED_128x32 = 1,
  OLED_128x64,
  OLED_132x64,
  OLED_64x32
};

typedef enum
{
   FONT_NORMAL=0,	
   FONT_BIG,		
   FONT_SMALL		
} FONTSIZE;

static void oledWriteCommand(unsigned char);


static PyObject * _oledInit(PyObject * self, PyObject * args)
{
 int iChannel, iAddr, iType, bFlip, bInvert;
 int sdaL, sclL;

 PyArg_ParseTuple(args,"iiiiibb",&sdaL,&sclL,&iChannel,&iAddr,&iType,&bFlip,&bInvert);
 SDA=sdaL;
 SCL=sclL;

const unsigned char oled64_initbuf[]={0x00,0xae,0xa8,0x3f,0xd3,0x00,0x40,0xa1,0xc8,
      0xda,0x12,0x81,0xff,0xa4,0xa6,0xd5,0x80,0x8d,0x14,
      0xaf,0x20,0x02};
const unsigned char oled32_initbuf[] = {
0x00,0xae,0xd5,0x80,0xa8,0x1f,0xd3,0x00,0x40,0x8d,0x14,0xa1,0xc8,0xda,0x02,
0x81,0x7f,0xd9,0xf1,0xdb,0x40,0xa4,0xa6,0xaf};

char filename[32];
unsigned char uc[4];

	oled_type = iType;
	oled_flip = bFlip;
	sprintf(filename, "/dev/i2c-%d", iChannel);
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		fprintf(stderr, "Failed to open i2c bus %d\n", iChannel);
		file_i2c = 0;
		return 1;
	}

	if (ioctl(file_i2c, I2C_SLAVE, iAddr) < 0)
	{
		fprintf(stderr, "Failed to acquire bus access or talk to slave\n");
		file_i2c = 0;
		return 1;
	}

	if (iType == OLED_128x32)
	{
		write(file_i2c, oled32_initbuf, sizeof(oled32_initbuf));
	}
	else
	{
		write(file_i2c, oled64_initbuf, sizeof(oled64_initbuf));
	}
	if (bInvert)
	{
		uc[0] = 0; // command
		uc[1] = 0xa7; // invert command
		write(file_i2c, uc, SDA);
	}
	if (bFlip) // rotate display 180
	{
		uc[0] = 0; // command
		uc[1] = 0xa0;
		write(file_i2c, uc, SDA);
		uc[1] = 0xc0;
		write(file_i2c, uc, SDA);
	}
	return Py_BuildValue("i",0);
}

static PyObject * _oledShutdown(PyObject * self)
{
	if (file_i2c != 0)
	{
		oledWriteCommand(0xaE); // turn off OLED
		close(file_i2c);
		file_i2c = 0;
	}

	return Py_BuildValue( "i", 0 );
}

static void oledWriteCommand(unsigned char c)
{
unsigned char buf[2];
int rc;

	buf[0] = 0x00; // command introducer
	buf[1] = c;
	rc = write(file_i2c, buf, SDA);
	if (rc) {} // suppress warning
}

static void oledWriteCommand2(unsigned char c, unsigned char d)
{
unsigned char buf[3];
int rc;

	buf[0] = 0x00;
	buf[1] = c;
	buf[2] = d;
	rc = write(file_i2c, buf, SCL);
	if (rc) {} // suppress warning
}

int oledSetContrast(unsigned char ucContrast)
{
        if (file_i2c == 0)
                return -1;

	oledWriteCommand2(0x81, ucContrast);
	return Py_BuildValue("i",0);
}

static void oledSetPosition(int x, int y)
{
	iScreenOffset = (y*128)+x;
	if (oled_type == OLED_64x32) // visible display starts at column 32, row 4
	{
		x += 32; // display is centered in VRAM, so this is always true
		if (oled_flip == 0) // non-flipped display starts from line 4
		y += 4;
	}
	else if (oled_type == OLED_132x64) // SH1106 has 128 pixels centered in 132
	{
		x += 2;
	}

	oledWriteCommand(0xb0 | y); // go to page Y
	oledWriteCommand(0x00 | (x & 0xf)); // // lower col addr
	oledWriteCommand(0x10 | ((x >> 4) & 0xf)); // upper col addr
}

static void oledWriteDataBlock(unsigned char *ucBuf, int iLen)
{
unsigned char ucTemp[129];
int rc;

	ucTemp[0] = 0x40; // data command
	memcpy(&ucTemp[1], ucBuf, iLen);
	rc = write(file_i2c, ucTemp, iLen+1);
	if (rc) {} // suppress warning
	// Keep a copy in local buffer
	memcpy(&ucScreen[iScreenOffset], ucBuf, iLen);
	iScreenOffset += iLen;
}

int oledSetPixel(int x, int y, unsigned char ucColor)
{
int i;
unsigned char uc, ucOld;

	if (file_i2c == 0)
		return -1;

	i = ((y >> 3) * 128) + x;
	if (i < 0 || i > 1023) // off the screen
		return -1;
	uc = ucOld = ucScreen[i];
	uc &= ~(0x1 << (y & 7));
	if (ucColor)
	{
		uc |= (0x1 << (y & 7));
	}
	if (uc != ucOld) // pixel changed
	{
		oledSetPosition(x, y>>3);
		oledWriteDataBlock(&uc, 1);
	}
	return Py_BuildValue("i",0);
}

static PyObject * _oledWriteString(PyObject * self, PyObject * args)
{
    int x, y, iSize;
	char *szMsg;
    PyArg_ParseTuple(args,"iis|i",&x,&y,&szMsg,&iSize);
int i, iLen;
unsigned char *s;

	if (file_i2c == 0) return -1; 
	if (iSize < FONT_NORMAL || iSize > FONT_SMALL)
		return -1;

	iLen = strlen(szMsg);
	if (iSize == FONT_BIG) 
	{
		if (iLen+x > 8) iLen = 8-x;
		if (iLen < 0) return -1;
		x *= 16;
		for (i=0; i<iLen; i++)
		{
			s = &ucFont[9728 + (unsigned char)szMsg[i]*64];
			oledSetPosition(x+(i*16), y);
			oledWriteDataBlock(s, 16);
			oledSetPosition(x+(i*16), y+1);
			oledWriteDataBlock(s+16, 16);	
			oledSetPosition(x+(i*16), y+2);
			oledWriteDataBlock(s+32, 16);		
		}
	}
	else if (iSize == FONT_NORMAL)
	{
		oledSetPosition(x*8, y);
		if (iLen + x > 16) iLen = 16 - x;
		if (iLen < 0)return -1;

		for (i=0; i<iLen; i++)
		{
			s = &ucFont[(unsigned char)szMsg[i] * 8];
			oledWriteDataBlock(s, 8);
		}	
	}
	else 
	{
		oledSetPosition(x*6, y);
		if (iLen + x > 21) iLen = 21 - x;
		if (iLen < 0) return -1;
		for (i=0; i<iLen; i++)
		{
			s = &ucSmallFont[(unsigned char)szMsg[i]*6];
			oledWriteDataBlock(s, 6);
		}
	}
	return Py_BuildValue("i",0);
} 

static PyObject * _oledFill(PyObject * self, PyObject * args)
{
    unsigned char ucData;
    PyArg_ParseTuple(args,"B",&ucData);
int y;
unsigned char temp[128];
int iLines, iCols;

	if (file_i2c == 0) return -1; // not initialized

	iLines = (oled_type == OLED_128x32 || oled_type == OLED_64x32) ? 4:8;
	iCols = (oled_type == OLED_64x32) ? 4:8;

	memset(temp, ucData, 128);
	for (y=0; y<iLines; y++)
	{
		oledSetPosition(0,y); // set to (0,Y)
		oledWriteDataBlock(temp, iCols*16); // fill with data byte
	} // for y
	return Py_BuildValue("i",0);
}

static PyObject * _oledDrawLine(PyObject * self, PyObject * args){
int x1, y1,x2, y2;
unsigned char ucPixel;
PyArg_ParseTuple(args,"iiiiB",&x1,&y1,&x2,&y2,&ucPixel);

	if (file_i2c == 0) return -1;

      int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        oledSetPixel( x1, y1,ucPixel);
        int err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
	return Py_BuildValue("i",0);
}

static PyObject * _oledDrawCircle(PyObject * self, PyObject * args) {
    int x1,y1, r; 
	unsigned char ucPixel;
    PyArg_ParseTuple(args,"iiiB",&x1,&y1,&r,&ucPixel);

	if (file_i2c == 0) return -1;

   int x = 0, y = r, p = 1 - r;

   cliplot(x1, y1, x, y, ucPixel);

   while (x < y) {

      x++;

      if (p < 0)

         p += 2 * x + 1;

      else {

         y--;

         p += 2 * (x - y) + 1;

      }

      cliplot(x1, y1, x, y, ucPixel);

   }

   return Py_BuildValue("i",0);

}

void cliplot(int xctr, int yctr, int x, int y, unsigned char ucPixel) {

   oledSetPixel(xctr + x, yctr + y, ucPixel);

   oledSetPixel(xctr - x, yctr + y, ucPixel);

   oledSetPixel(xctr + x, yctr - y, ucPixel);

   oledSetPixel(xctr - x, yctr - y, ucPixel);

   oledSetPixel(xctr + y, yctr + x, ucPixel);

   oledSetPixel(xctr - y, yctr + x, ucPixel);

   oledSetPixel(xctr + y, yctr - x, ucPixel);

   oledSetPixel(xctr - y, yctr - x, ucPixel);

} 

static struct PyMethodDef OMeths [] = {
    {"oledInit", (PyCFunction)_oledInit, METH_VARARGS, "oledINITfunction"},
    {"oledShutdown",(PyCFunction)_oledShutdown,METH_NOARGS,"oledShutdown"},
    {"oledWriteString",(PyCFunction)_oledWriteString,METH_VARARGS,"oledWriteString"},
    {"oledFill",(PyCFunction)_oledFill,METH_VARARGS,"oledFill"},
    {"oledDrawLine",(PyCFunction)_oledDrawLine,METH_VARARGS,"oledDrawLine"},
    {"oledDrawCircle",(PyCFunction)_oledDrawCircle,METH_VARARGS,"oledDrawCircle"},
    
    {NULL,NULL,0,NULL}
};

static struct PyModuleDef oledM = {
    PyModuleDef_HEAD_INIT,
    "oled",
    "OM",
    -1,
    OMeths
};

PyMODINIT_FUNC PyInit_oled(void){
    PyObject *module = PyModule_Create(&oledM);

    return module;
}
#include "zxccp.h"
#include "zxbdos.h"
#include "zxdbdos.h"

/* This file used to deal with all disc-based BDOS calls. 
  Now the calls have been moved into libcpmredir, it's a bit empty round
  here. 

   ZXCCP does a few odd things when searching, to make Hi-Tech C behave
   properly.
*/


/* If a file could not be found on the default drive, try again on a "search"
  drive (A: for .COM files, B: for .LIB and .OBJ files) */
  
int fcbforce(byte *fcb, byte *odrv)
{
	byte drive;
	char typ[4];	
	char nam[9];	
	int n;

	for (n = 0; n < 8; n++) 
        nam[n] = fcb[n+1] & 0x7F;
	nam[8] = 0;
	for (n = 0; n < 3; n++) 
        typ[n] = fcb[n+9] & 0x7F;
	typ[3] = 0;

	drive = 0;
	if (*fcb) return 0;	/* not using default drive */
	if (!strcasecmp(nam,"BCLOAD  ") && !strcasecmp(typ, "   ")) drive = 2;
	if (!strcasecmp(typ, "COM")) drive = 1;
	if (!strcasecmp(typ, "OVR")) drive = 1;
	if (!strcasecmp(typ, "LIB")) drive = 2; 
	if (!strcasecmp(typ, "OBJ")) drive = 2; 
	if (!strcasecmp(typ, "REL")) drive = 2; 

	if (!drive) return 0;
	
	*odrv = *fcb;
	*fcb = drive;
	return 1;
}

/* zxccp has a trick with some filenames: If it can't find them where they
       should be, and a drive wasn't specified, it searches BINDIR80, 
       LIBDIR80 or INCDIR80 (depending on the type of the file).
 */

word x_fcb_open(byte *fcb, byte *dma)
{
	word rv = fcb_open(fcb, dma);
	byte odrv;

	if (rv == 0xFF)
	{
		if (fcbforce(fcb, &odrv))
		{
			rv = fcb_open(fcb, dma);
			*fcb = odrv;
		}
	}
	return rv;
}



word x_fcb_stat(byte *fcb)
{
        word rv = fcb_stat(fcb);
        byte odrv;

        if (rv == 0xFF)
        {
                if (fcbforce(fcb, &odrv))
                {
                        rv = fcb_stat(fcb);
                        *fcb = odrv;
                }
        }
        return rv;
}




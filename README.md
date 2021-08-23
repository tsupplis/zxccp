
*ZXCCP* Adaptation of ZXCC https://www.seasip.info/Unix/Zxcc

   zxccp is a wrapper for the Hi-Tech C CP/M compiler, allowing it to be
   used as a cross-compiler under UNIX. Version 0.5.0 also works with the
   build tools necessary to assemble CP/M 3 (MAC, RMAC, LINK, GENCOM).

   New in this version:
     - Compiles on boxes where sync() does not return int (reported by
       multiple users).
     - BDOS function 10 takes a pointer to unsigned char, thus allowing
       input buffers longer than 128 bytes (Andy Parkins)

**Setting up**

   Firstly, build the compiler. It should not be necessary to rebuild
   bios.bin; but if you have to, it assembles using the z80asm assembler
   (part of the z80pack emulation package which can be found on the Walnut
   Creek CP/M CDROM) or the [1]ZMAC assembler (downloadable from
   <http://www.nenie.org/cpcip/index.html#zmac>).

   Previous versions of ZXCCP could be compiled under DOS using DJGPP; this
   may still be possible, but has not been tested since the build system
   was changed to use the GNU autotools.

   This version of zxccp contains copies of the CPMIO and CPMREDIR
   libraries, so you won't need to obtain them separately.

   You will need the tools you want to use; either the Hi-Tech C compiler
   for CP/M <[2]http://www.hitech.com.au> or Digital Research's tools at
   <[3]http://www.cpm.z80.de> Once you have obtained the tools,
   documentation and possibly the library source, you need to decide where
   to put the files. zxccp uses three directories:
     * BINDIR80 (by default, /usr/local/lib/cpm/bin80) holds the compiler
       itself. You should copy the compiler .com files (or MAC, RMAC etc.)
       and bios.bin to this directory.
     * LIBDIR80 (by default, /usr/local/lib/cpm/lib80) holds the C
       libraries libc.lib, libf.lib, crtcpm.obj and rrtcpm.obj.
     * INCDIR80 (by default, /usr/local/lib/cpm/include80) holds the
       compiler .h files.

   The locations of these directories are normally set by the configure
   script; you can override them by editing zxccp.h and uncommenting the
   lines that redefine them.

   Once you have installed zxccp and the build tools, try building Hello
   World:

     #include <stdio.h>
     void main()
     {
     printf("Hello World\n");
     }

   or for RMAC:

     CSEG
     LXI D,HELLO
     MVI C,9
     CALL 5
     RST 0
     HELLO: DB 'Hello World',13,10,'$'

   To compile the first example, type

     zxc hello.c

   ; if all goes well, you should end up with a file called hello.com. You
   can test the resulting file by typing

     zxccp hello.com

   .

   To assemble the second example, type

     zxccp rmac.com hello
     zxccp link.com hello

   and run it as above. NOTE: RMAC requires that lines be terminated with
   CR/LF. You may need to put a unix2dos command in your makefile before
   you invoke RMAC.

**Using zxccp**

   For detailed instructions, see the documentation for Hi-Tech C or the
   CP/M tools. zxccp behaves in the same way, but note the following
   points:

  Program names

   The names of the programs have been changed between CP/M and UNIX; for
   example, you would type

     zxc hello.c

   instead of

     c hello.c

   . The programs to use are:

   zxc
          The equivalent of C.COM.

   zxas
          The equivalent of ZAS.COM.

   zxlink
          The equivalent of LINK.COM.

   zxlibr
          The equivalent of LIBR.COM.

   All these programs work by converting their arguments to a form
   suitable for zxccp, and then invoking zxccp.

   There are no front-end programs for the CP/M build tools; you will have
   to enter arguments to these in the zxccp format given below.

  Filenames

   Where the documentation allows you to enter a CP/M filename, you should
   instead enter a UNIX one. The filename itself (as opposed to any
   directories in its path) must obey CP/M 8.3 naming conventions and be
   all lowercase.

   Where the documentation requires a CP/M driveletter / user number

     -I2:C:

   you should enter a path complete with trailing slash:

     -I/usr/src/linux-80/include/

**Technical**

   zxccp emulates a subset of CP/M 3; hopefully enough to run the Hi-Tech C
   compiler. It can be used as a limited general-purpose CP/M 3 emulator
   provided the emulated program only uses a restricted subset of system
   calls.

   zxccp behaves like the emulator com, allowing CP/M programs to be used
   transparently from a UNIX prompt. However com:
     * Emulates all of CP/M 2, rather than a subset of CP/M 3;
     * Is designed for general use, not tailored to Hi-Tech C;
     * Is written partly in assembly language and will only run on
       68000-based computers;
     * Cannot map UNIX directories to CP/M drives;
     * Contains some bugs connected with command parsing and file I/O.

   Syntax for zxccp is:

     zxccp comfile.com arg1 arg2 ...

   The comfile is the program to run; zxccp searches the current directory
   and BINDIR80 for it.

   The arguments are parsed in this way:
     * Any argument starting with a - sign is passed to the CP/M program
       as-is, minus the leading - sign.
     * Any argument starting with a + sign is parsed as a filename (see
       below) and then concatenated to the previous argument.
     * Any argument starting "+-" is concatenated without being parsed.
     * All other arguments are parsed as filenames. The UNIX pathname is
       converted to a CP/M driveletter.

   For example:

     zxccp foo.com --Q -A /dev/null --I +/dev/zero +-, +/foo/bar

   would pass these arguments to foo.com:

     -Q A d:null -Id:zero,e:bar

   The other programs are merely wrappers that convert their command lines
   into the form required by zxccp.

Errors

   Any errors raised by the zxccp runtime system will be prefixed with
   zxccp:. Some errors you may encounter are:

   Unsupported BDOS call
          Part of CP/M 3 that the program uses has not been emulated. Add
          the required functionality to zxbdos.c and recompile.

   Z80 encountered invalid trap
          The CP/M program being run attempted to call the zxccp runtime
          system with an unknown call number. This will happen if the
          program was written for my emulator "Joyce".

**Acknowledgements**

     * Hi-Tech C was written by Hi-Tech Software.
     * The Z80 emulation engine was written by Ian Collier.
     * Thanks to Jacob Nevins, Andy Parkins and others for bug fix
       suggestions.
     __________________________________________________________________


    John Elliott, 28 March 2003

References

   1. http://www.nenie.org/cpcip/index.html#zmac
   2. http://www.hitech.com.au/
   3. http://www.cpm.z80.de/

## Updates

* An apparent error in the source of the CP/M BDOS function 10 (read
console buffer) emulation - courtesy of @tsupplis from his repository
at https://github.com/tsupplis/zxccp

* The C compiler (zxc) front-end was not accepting HI-TECH C-style
command options that require filenames.  It now accepts the following

```
  -efile.com     CP/M executable file name
  -ffile.sym     Symbol-table for debugger or overlay creation
  -idirectory    Include directory (in native format)
  -mfile.map     Linker map file
  -crfile.crf    Cross-reference listing
```

* The linker (zxlink) should use the renamed HI-TECH linker
(now linq.com).

* Fix compilation warnings

* Emulation of CP/M BDOS function 60 (call resident system extension)
should be disabled and return 0xFF in both the A and L registers.

* Change cpm_bdos_10() to return an unsigned result to avoid buffer
size being interpreted as negative.

* Fix the emulation of Z80 opcodes for IN (HL),(C) and OUT (C),(HL) -
opcodes 0xED,0x70 and 0xED,0x71 respectively.  This
is noted in Fred Weigel's AM9511 arithmetic processing unit
emulation from https://github.com/ratboy666/am9511 in the howto.txt
description.  NB: I have not included Fred's am9511 support at
this time into ZXCC.

* Russell Marks contributed a fix to the emulation of the Z80 DAA
(decimal adjust) instruction - based on code from the yaze 1.10
emulator.  The original code in ZXCC was based on incorrect documentation
of the DAA instruction in the Zilog and Mostek programming manuals (and
even in Rodney Zaks "Programming the Z80" book).  The most recent
documentation from Zilog has the correct description(1).  With this
fix in-place ZXCC is able to run the Z80 instruction emulator
test suite ZEXDOC from yaze-ag(2).  You can download a copy of the
CP/M binary for ZEXDOC from
https://raw.githubusercontent.com/agn453/ZEXALL/main/zexdoc.com

--

(1) The most recent Zilog Z80 Family CPU User Manual (UM008001-1000)
can be found at http://www.zilog.com/docs/z80/z80cpu_um.pdf

(2) You'll find the final version of yaze-ag 2.51.1 (as curated by
Andreas Gerlich) at http://www.mathematik.uni-ulm.de/users/ag/yaze-ag

--

Tony Nicholson 23-Aug-2021


/*
 *  SMAPI; Modified Squish MSGAPI
 *
 *  Squish MSGAPI0 is copyright 1991 by Scott J. Dudley.  All rights reserved.
 *  Modifications released to the public domain.
 *
 *  Use of this file is subject to the restrictions contain in the Squish
 *  MSGAPI0 licence agreement.  Please refer to licence.txt for complete
 *  details of the licencing restrictions.  If you do not find the text
 *  of this agreement in licence.txt, or if you do not have this file,
 *  you should contact Scott Dudley at FidoNet node 1:249/106 or Internet
 *  e-mail Scott.Dudley@f106.n249.z1.fidonet.org.
 *
 *  In no event should you proceed to use any of the source files in this
 *  archive without having accepted the terms of the MSGAPI0 licensing
 *  agreement, or such other agreement as you are able to reach with the
 *  author.
 */

#include "prog.h"
#include "unused.h"


#if !defined(UNIX) && !defined(SASC)
#include <io.h>
#endif

#ifdef UNIX
#include <unistd.h>
#endif

#if !defined(__IBMC__) && !defined(MSDOS) && !defined(UNIX) && !defined(__MINGW32__)
#include <dos.h>
#endif

#ifdef OS2

#ifdef EXPENTRY
#undef EXPENTRY
#endif

#define INCL_NOPM
#include <os2.h>

#if defined(__386__) || defined(__FLAT__) || defined(__EMX__)
#undef DosBufReset
#define DosBufReset DosResetBuffer
#endif

#endif

#if defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(UNIX) || defined(SASC) || defined(__DJGPP__)

void pascal far flush_handle2(int fh)
{
    unused(fh);
}

#elif defined(OS2)

void pascal far flush_handle2(int fh)
{
    DosBufReset((HFILE) fh);
}

#elif defined(__NT__)

void pascal far flush_handle2(int fh)
{
    FlushFileBuffers((HANDLE) fh);
}

#else

/* refer to MS-DOS flush_handle2 code in FLUSHASM.ASM */

#endif

/*
 *  This makes sure a file gets flushed to disk.  Thanks to Ray Duncan
 *  for this tip in his _Advanced MS-DOS_ book.
 */

void _fast flush_handle(FILE * fp)
{
    fflush(fp);

#if defined(OS2)
    DosBufReset(fileno(fp));
#elif defined(MSDOS) || defined(__NT__) || defined(__TURBOC__) || defined(SASC)
    flush_handle2(fileno(fp));
#else
    {
        int nfd;

        nfd = dup(fileno(fp));
        if (nfd != -1)
        {
            close(nfd);
        }
    }
#endif
}

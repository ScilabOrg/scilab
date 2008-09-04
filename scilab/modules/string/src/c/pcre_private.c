
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-------------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <errno.h>
#include <pcre.h>
#include "MALLOC.h"
#include "BOOL.h"
#include "pcre_private.h"


/* A number of things vary for Windows builds. Originally, pcretest opened its
input and output without "b"; then I was told that "b" was needed in some
environments, so it was added for release 5.0 to both the input and output. (It
makes no difference on Unix-like systems.) Later I was told that it is wrong
for the input on Windows. I've now abstracted the modes into two macros that
are set here, to make it easier to fiddle with them, and removed "b" from the
input mode under Windows. */

#if _MSC_VER
#include <io.h>                /* For _setmode() */
#include <fcntl.h>             /* For _O_BINARY */
#define INPUT_MODE   "r"
#define OUTPUT_MODE  "wb"

#else
#include <sys/time.h>          /* These two includes are needed */
#include <sys/resource.h>      /* for setrlimit(). */
#define INPUT_MODE   "rb"
#define OUTPUT_MODE  "wb"
#endif

#define LINK_SIZE		2

/* We have to include pcre_internal.h because we need the internal info for
displaying the results of pcre_study() and we also need to know about the
internal macros, structures, and other internal data values; pcretest has
"inside information" compared to a program that strictly follows the PCRE API.

Although pcre_internal.h does itself include pcre.h, we explicitly include it
here before pcre_internal.h so that the PCRE_EXP_xxx macros get set
appropriately for an application, not for building PCRE. */

#include <pcre.h>
#include "pcre_internal.h"

/* We need access to the data tables that PCRE uses. So as not to have to keep
two copies, we include the source file here, changing the names of the external
symbols to prevent clashes. */

#define _pcre_utf8_table1      utf8_table1
#define _pcre_utf8_table1_size utf8_table1_size
#define _pcre_utf8_table2      utf8_table2
#define _pcre_utf8_table3      utf8_table3
#define _pcre_utf8_table4      utf8_table4
#define _pcre_utt              utt
#define _pcre_utt_size         utt_size
#define _pcre_utt_names        utt_names
#define _pcre_OP_lengths       OP_lengths

#include "pcre_tables.c"

/* It is possible to compile this test program without including support for
testing the POSIX interface, though this is not available via the standard
Makefile. */

#if !defined NOPOSIX
#include "pcreposix.h"
#endif

/* It is also possible, for the benefit of the version currently imported into
Exim, to build pcretest without support for UTF8 (define NOUTF8), without the
interface to the DFA matcher (NODFA), and without the doublecheck of the old
"info" function (define NOINFOCHECK). In fact, we automatically cut out the
UTF8 support if PCRE is built without it. */

#ifndef SUPPORT_UTF8
#ifndef NOUTF8
#define NOUTF8
#endif
#endif



/* Static variables */

static FILE *outfile=NULL;
static int callout_count=0;
static int callout_fail_count=0;
static int callout_fail_id=0;
static size_t gotten_store=0;

/* The buffers grow automatically if very long input lines are encountered. */

char *buffer = NULL;


static int check_match_limit(pcre *re, pcre_extra *extra, char *bptr, int len,
				  int start_offset, int options, int *use_offsets, int use_size_offsets,
				  int flag, unsigned long int *limit, int errnumber);

/*************************************************
*              Callout function                  *
*************************************************/

/* Called from PCRE as a result of the (?C) item. We print out where we are in
the match. Yield zero unless more callouts than the fail count, or the callout
data is not zero. */

static int callout(pcre_callout_block *cb)
{
/* Always print appropriate indicators, with callout number if not already
shown. For automatic callouts, show the pattern offset. */

if (cb->callout_data != NULL)
  {
  int callout_data = *((int *)(cb->callout_data));
  if (callout_data != 0)
    {
    return callout_data;
    }
  }

return (cb->callout_number != callout_fail_id)? 0 :
       (++callout_count >= callout_fail_count)? 1 : 0;
}


/*************************************************
*            Local malloc functions              *
*************************************************/

/* Alternative malloc function, to test functionality and show the size of the
compiled re. */

static void *new_malloc(size_t size)
{
void *block = MALLOC(size);
gotten_store = size;
return block;
}


/*************************************************
*          Call pcre_fullinfo()                  *
*************************************************/

/* Get one piece of information from the pcre_fullinfo() function */

static void new_info(pcre *re, pcre_extra *study, int option, void *ptr)
{
	int rc = 0;
	rc = pcre_fullinfo(re, study, option, ptr);
}

/*************************************************
*        Check match or recursion limit          *
*************************************************/

static int check_match_limit(pcre *re, pcre_extra *extra, char *bptr, int len,
  int start_offset, int options, int *use_offsets, int use_size_offsets,
  int flag, unsigned long int *limit, int errnumber)
{
int count;
int min = 0;
int mid = 64;
int max = -1;

extra->flags |= flag;

for (;;)
  {
  *limit = mid;

  count = pcre_exec(re, extra, (char *)bptr, len, start_offset, options,
    use_offsets, use_size_offsets);

  if (count == errnumber)
    {
    min = mid;
    mid = (mid == max - 1)? max : (max > 0)? (min + max)/2 : mid*2;
    }

  else if (count >= 0 || count == PCRE_ERROR_NOMATCH ||
                         count == PCRE_ERROR_PARTIAL)
    {
    if (mid == min + 1)
      {
      break;
      }
    max = mid;
    mid = (min + mid)/2;
    }
  else break;    /* Some other error */
  }

extra->flags &= ~flag;
return count;
}



/*************************************************
*         Case-independent strncmp() function    *
*************************************************/

/*
Arguments:
  s         first string
  t         second string
  n         number of characters to compare

Returns:    < 0, = 0, or > 0, according to the comparison
*/

static int strncmpic(char *s, char *t, int n)
{
	while (n--)
	{
		int c = tolower(*s++) - tolower(*t++);
		if (c) return c;
	}
	return 0;
}


/*************************************************
*               Algorithm                      *
*************************************************/

/* Read lines from named file or stdin and write to named file or stdout; lines
consist of a regular expression, in delimiters and optionally followed by
options, followed by a set of test data, terminated by an empty line. */

pcre_error_code pcre_private(char *INPUT_LINE,char *INPUT_PAT,int *Output_Start,int *Output_End)
{
	int options = 0;
	int timeit = 0;
	int size_offsets = 45;
	int size_offsets_max;
	int *offsets = NULL;
	int all_use_dfa = 0;
	BOOL LOOP_PCRE_TST = FALSE;

	/* These vectors store, end-to-end, a list of captured substring names. Assume
	that 1024 is plenty long enough for the few names we'll be testing. */

	char copynames[1024];
	char getnames[1024];

	char *copynamesptr=NULL;
	char *getnamesptr=NULL;


	buffer = (char *)MALLOC(strlen(INPUT_LINE)+1);


	size_offsets_max = size_offsets;
	offsets = (int *)MALLOC(size_offsets_max * sizeof(int));
	if (offsets == NULL)
	{
		return NOT_ENOUGH_MEMORY_FOR_VECTOR;
	}
	/* Main loop */
	LOOP_PCRE_TST = FALSE;
	while (!LOOP_PCRE_TST)
	{
		pcre *re = NULL;
		pcre_extra *extra = NULL;
		const char *error=NULL;
		char *p=NULL; 
		char *pp=NULL;
		char *ppp=NULL;
		const unsigned char *tables = NULL;
		unsigned long int true_size, true_study_size = 0;
		int do_G = 0;
		int do_g = 0;
		int erroroffset, len, delimiter;
		
		LOOP_PCRE_TST = TRUE;
		p = INPUT_PAT;
		while (isspace(*p)) p++;
		if (*p == 0) continue;
		/* See if the pattern is to be loaded pre-compiled from a file. */
		if (*p == '<' && strchr((char *)(p+1), '<') == NULL)
		{
			unsigned long int get_options;
			char sbuf[8];
			FILE *f;
			p++;
			pp = p + (int)strlen((char *)p);
			while (isspace(pp[-1])) pp--;
			*pp = 0;
			f = fopen((char *)p, "rb");
			if (f == NULL)
			{
				continue;
			}
			if (fread(sbuf, 1, 8, f) != 8) goto FAIL_READ;
			true_size = (sbuf[0] << 24) | (sbuf[1] << 16) | (sbuf[2] << 8) | sbuf[3];
			true_study_size = (sbuf[4] << 24) | (sbuf[5] << 16) | (sbuf[6] << 8) | sbuf[7];
			re = (real_pcre *)new_malloc(true_size);
			if (fread(re, 1, true_size, f) != true_size) goto FAIL_READ;
			
		
			/* Need to know if UTF-8 for printing data strings */
			new_info(re, NULL, PCRE_INFO_OPTIONS, &get_options);
			
			/* Now see if there is any following study data */
			if (true_study_size != 0)
			{
				pcre_study_data *psd;

				extra = (pcre_extra *)new_malloc(sizeof(pcre_extra) + true_study_size);
				extra->flags = PCRE_EXTRA_STUDY_DATA;

				psd = (pcre_study_data *)(((char *)extra) + sizeof(pcre_extra));
				extra->study_data = psd;

				if (fread(psd, 1, true_study_size, f) != true_study_size)
				{
					FAIL_READ:
					if (extra != NULL) FREE(extra);
					if (re != NULL) FREE(re);
					fclose(f);
					continue;
				}
				
			}
			goto SHOW_INFO;
		}

	/* In-line pattern (the usual case). Get the delimiter and seek the end of
	the pattern; if is isn't complete, read more. */

	delimiter = *p++;

	if (isalnum(delimiter) || delimiter == '\\')
    {
		return DELIMITER_NOT_ALPHANUMERIC;
    }

	pp = p;

	while (*pp != 0)
	{
		if (*pp == '\\' && pp[1] != 0) pp++;
		else if (*pp == delimiter) break;
		pp++;
	}

	/* If the delimiter can't be found, it's a syntax error */
	if(*pp == 0) return CAN_NOT_COMPILE_PATTERN;

	/* If the first character after the delimiter is backslash, make
	the pattern end with backslash. This is purely to provide a way
	of testing for the error message when a pattern ends with backslash. */

	if (pp[1] == '\\') *pp++ = '\\';

	/* Terminate the pattern at the delimiter, and save a copy of the pattern
	for callouts. */

	*pp++ = 0;

	/* Look for options after final delimiter */

	options = 0;
		
	while (*pp != 0)
    {
		switch (*pp++)
		{
			case 'f': options |= PCRE_FIRSTLINE; break;
			case 'g': do_g = 1; break;
			case 'i': options |= PCRE_CASELESS; break;
			case 'm': options |= PCRE_MULTILINE; break;
			case 's': options |= PCRE_DOTALL; break;
			case 'x': options |= PCRE_EXTENDED; break;
			case '+': break;
			case 'A': options |= PCRE_ANCHORED; break;
			case 'B': break;
			case 'C': options |= PCRE_AUTO_CALLOUT; break;
			case 'D': break;
			case 'E': options |= PCRE_DOLLAR_ENDONLY; break;
			case 'F': break;
			case 'G': do_G = 1; break;
			case 'I': break;
			case 'J': options |= PCRE_DUPNAMES; break;
			case 'M': break;
			case 'N': options |= PCRE_NO_AUTO_CAPTURE; break;
			case 'S': break;
			case 'U': options |= PCRE_UNGREEDY; break;
			case 'X': options |= PCRE_EXTRA; break;
			case 'Z': break;
			case '8': options |= PCRE_UTF8; break;
			case '?': options |= PCRE_NO_UTF8_CHECK; break;
			case 'L':
				ppp = pp;
				/* The '\r' test here is so that it works on Windows. */
				/* The '0' test is just in case this is an unterminated line. */
				while (*ppp != 0 && *ppp != '\n' && *ppp != '\r' && *ppp != ' ') ppp++;
				*ppp = 0;
				if (setlocale(LC_CTYPE, (const char *)pp) == NULL)
				{
					goto SKIP_DATA;
				}
				
				tables = pcre_maketables();
				pp = ppp;
			break;
			case '>':
				while (*pp != 0) pp++;
				while (isspace(pp[-1])) pp--;
				*pp = 0;
			break;
			case '<':
			{
				while (*pp++ != '>');
			}
			break;
			case '\r':                      /* So that it works in Windows */
			case '\n':
			case ' ':
			break;

			default:
			goto SKIP_DATA;
		}
	}

	/* Handle compiling via the POSIX interface, which doesn't support the
	timing, showing, or debugging options, nor the ability to pass over
	local character tables. */


    {
		if (timeit > 0)
		{
			register int i;
			for (i = 0; i < timeit; i++)
			{
				re = pcre_compile((char *)p, options, &error, &erroroffset, tables);
				/* "re" allocated by pcre_compile (better to use free function associated to pcre)*/
				if (re != NULL) (*pcre_free)(re);
			}
			
		}
		re = pcre_compile((char *)p, options, &error, &erroroffset, tables);
		/* Compilation failed; go back for another re, skipping to blank line
		if non-interactive. */
		if (re == NULL)
		{
		    SKIP_DATA:
			return CAN_NOT_COMPILE_PATTERN;
		}
		true_size = ((real_pcre *)re)->size;
		SHOW_INFO: ;

		/* If the '>' option was present, we write out the regex to a file, and
		that is all. The first 8 bytes of the file are the regex length and then
		the study length, in big-endian order. */

    }        /* End of non-POSIX compile */

  /* Read data lines and test them */
    {
		char *q=NULL;
		char *bptr=NULL;
		int *use_offsets = offsets;
		int use_size_offsets = size_offsets;
		int callout_data = 0;
		int callout_data_set = 0;
		int count=0;
		int c=0;
		int copystrings = 0;
		int find_match_limit = 0;
		int getstrings = 0;
		int gmatched = 0;
		int start_offset = 0;
		int g_notempty = 0;
		int use_dfa = 0;
	
		options = 0;
		*copynames = 0;
		*getnames = 0;

		copynamesptr = copynames;
		getnamesptr = getnames;
  		
		callout_count = 0;
		callout_fail_count = 999999;
		callout_fail_id = -1;
		
		if (extra != NULL) extra->flags &= ~(PCRE_EXTRA_MATCH_LIMIT|PCRE_EXTRA_MATCH_LIMIT_RECURSION);
		len = 0;
		p = INPUT_LINE;
		while (isspace(*p)) p++;
		bptr = q = buffer;
		while ((c = *p++) != 0)
		{
			int i = 0;
			int n = 0;

			if (c == '\\') switch ((c = *p++))
			{
				case 'a': c =    7; break;
				case 'b': c = '\b'; break;
				case 'e': c =   27; break;
				case 'f': c = '\f'; break;
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				case 'v': c = '\v'; break;
				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					c -= '0';
					while (i++ < 2 && isdigit(*p) && *p != '8' && *p != '9')
					c = c * 8 + *p++ - '0';
				break;
				case 'x':
				/* Ordinary \x */
				c = 0;
				while (i++ < 2 && isxdigit(*p))
				{
					c = c * 16 + tolower(*p) - ((isdigit(*p))? '0' : 'W');
					p++;
				}
				break;
				case 0:   /* \ followed by EOF allows for an empty line */
					p--;
				continue;
				case '>':
					while(isdigit(*p)) start_offset = start_offset * 10 + *p++ - '0';
				continue;
				case 'A':  /* Option setting */
					options |= PCRE_ANCHORED;
				continue;
				case 'B':
					options |= PCRE_NOTBOL;
				continue;
				case 'C':
					if (isdigit(*p))    /* Set copy string */
					{
						while(isdigit(*p)) n = n * 10 + *p++ - '0';
						copystrings |= 1 << n;
					}
					else if (isalnum(*p))
					{
						char *npp = copynamesptr;
						while (isalnum(*p)) *npp++ = *p++;
						*npp++ = 0;
						*npp = 0;
						n = pcre_get_stringnumber(re, (char *)copynamesptr);
						copynamesptr = npp;
					}
					else if (*p == '+')
					{
						
						p++;
					}
					else if (*p == '-')
					{
					   p++;
					}
					else if (*p == '!')
					{
						callout_fail_id = 0;
						p++;
						while(isdigit(*p))
						callout_fail_id = callout_fail_id * 10 + *p++ - '0';
						callout_fail_count = 0;
						if (*p == '!')
						{
							p++;
							while(isdigit(*p))
							callout_fail_count = callout_fail_count * 10 + *p++ - '0';
						}
					}
					else if (*p == '*')
					{
						int sign = 1;
						callout_data = 0;
						if (*(++p) == '-') { sign = -1; p++; }
						while(isdigit(*p)) callout_data = callout_data * 10 + *p++ - '0';
						callout_data *= sign;
						callout_data_set = 1;
					}
				continue;
				case 'G':
					if (isdigit(*p))
					{
						while(isdigit(*p)) n = n * 10 + *p++ - '0';
						getstrings |= 1 << n;
					}
					else if (isalnum(*p))
					{
						char *npp = getnamesptr;
						while (isalnum(*p)) *npp++ = *p++;
						*npp++ = 0;
						*npp = 0;
						n = pcre_get_stringnumber(re, (char *)getnamesptr);
						getnamesptr = npp;
					}
				continue;
				case 'L':
				continue;
				case 'M':
					find_match_limit = 1;
				continue;
				case 'N':
					options |= PCRE_NOTEMPTY;
				continue;
				case 'O':
					while(isdigit(*p)) n = n * 10 + *p++ - '0';
					if (n > size_offsets_max)
					{
						size_offsets_max = n;
						FREE(offsets);
						use_offsets = offsets = (int *)MALLOC(size_offsets_max * sizeof(int));
					}
					use_size_offsets = n;
					if (n == 0) use_offsets = NULL;   /* Ensures it can't write to it */
				continue;
				case 'P':
					options |= PCRE_PARTIAL;
				continue;
				case 'Q':
					while(isdigit(*p)) n = n * 10 + *p++ - '0';
					if (extra == NULL)
					{
						extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
						extra->flags = 0;
					}
					extra->flags |= PCRE_EXTRA_MATCH_LIMIT_RECURSION;
					extra->match_limit_recursion = n;
				continue;
				case 'q':
					while(isdigit(*p)) n = n * 10 + *p++ - '0';
					if (extra == NULL)
					{
						extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
						extra->flags = 0;
					}
					extra->flags |= PCRE_EXTRA_MATCH_LIMIT;
					extra->match_limit = n;
				continue;
			#if !defined NODFA
				case 'R':
					options |= PCRE_DFA_RESTART;
				continue;
			#endif
				case 'S':
					
				continue;
				case 'Z':
					options |= PCRE_NOTEOL;
				continue;
				case '?':
					options |= PCRE_NO_UTF8_CHECK;
				continue;
				case '<':
				{
					while (*p++ != '>');
				}
				continue;
			}
			*q++ =(char)c;
		}
		*q = 0;
		len = (int)(q - buffer);
		if ((all_use_dfa || use_dfa) && find_match_limit)
		{
			return LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING;
		}
		/* Handle matching via the POSIX interface, which does not
		support timing or playing with the match limit or callout data. */
		for (;; gmatched++)    /* Loop for /g or /G */
		{
			
			/* If find_match_limit is set, we want to do repeated matches with
			varying limits in order to find the minimum value for the match limit and
			for the recursion limit. */

			if (find_match_limit)
			{
				if (extra == NULL)
				{
					extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
					extra->flags = 0;
				}

				(void)check_match_limit(re, extra, bptr, len, start_offset,
					options|g_notempty, use_offsets, use_size_offsets,
					PCRE_EXTRA_MATCH_LIMIT, &(extra->match_limit),
					PCRE_ERROR_MATCHLIMIT);

				count = check_match_limit(re, extra, bptr, len, start_offset,
					options|g_notempty, use_offsets, use_size_offsets,
					PCRE_EXTRA_MATCH_LIMIT_RECURSION, &(extra->match_limit_recursion),
				PCRE_ERROR_RECURSIONLIMIT);
			}
			/* If callout_data is set, use the interface with additional data */
			else if (callout_data_set)
			{
				if (extra == NULL)
				{
					extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
					extra->flags = 0;
				}
				extra->flags |= PCRE_EXTRA_CALLOUT_DATA;
				extra->callout_data = &callout_data;
				count = pcre_exec(re, extra, (char *)bptr, len, start_offset,
						options | g_notempty, use_offsets, use_size_offsets);

				extra->flags &= ~PCRE_EXTRA_CALLOUT_DATA;
			}
			/* The normal case is just to do the match once, with the default
			value of match_limit. */
			else
			{
				count = pcre_exec(re, extra, (char *)bptr, len,
					start_offset, options | g_notempty, use_offsets, use_size_offsets);
				if (count == 0)
				{
					count = use_size_offsets/3;
				}
			}
			/* Matched */
			if (count >= 0)
			{
				int i, maxcount;
				maxcount = use_size_offsets/3;
				/* This is a check against a lunatic return value. */
				if (count > maxcount)
				{
					return TOO_BIG_FOR_OFFSET_SIZE;
				}

				for (i = 0; i < count * 2; i += 2)
				{
					if (use_offsets[i] >= 0)
					{
						*Output_Start=use_offsets[i];
						*Output_End=use_offsets[i+1];
						if (buffer) FREE(buffer);

						/* use_offsets = offsets no need to free use_offsets if we free offsets */
						if (offsets) FREE(offsets);

						/* "re" allocated by pcre_compile (better to use free function associated)*/
						if (re) (*pcre_free)(re);

						if (extra) FREE(extra);
						if (tables)
						{
							/* "tables" allocated by pcre_maketables (better to use free function associated to pcre)*/
							(*pcre_free)((void *)tables);
							setlocale(LC_CTYPE, "C");
						}
						return 0;
					}
				}

				for (copynamesptr = copynames; *copynamesptr != 0;copynamesptr += (int)strlen((char*)copynamesptr) + 1)
				{
					char copybuffer[256];
					pcre_copy_named_substring(re, (char *)bptr, use_offsets,count, (char *)copynamesptr, copybuffer, sizeof(copybuffer));
				}

				for (i = 0; i < 32; i++)
				{
					if ((getstrings & (1 << i)) != 0)
					{
						const char *substring;
						pcre_get_substring((char *)bptr, use_offsets, count,i, &substring);
					}
				}

				for (getnamesptr = getnames;*getnamesptr != 0;getnamesptr += (int)strlen((char*)getnamesptr) + 1)
				{
					const char *substring;
					pcre_get_named_substring(re, (char *)bptr, use_offsets,count, (char *)getnamesptr, &substring);
				}

        }
		/* Failed to match. If this is a /g or /G loop and we previously set
		g_notempty after a null match, this is not necessarily the end. We want
		to advance the start offset, and continue. We won't be at the end of the
		string - that was checked before setting g_notempty.
		Complication arises in the case when the newline option is "any" or
		"anycrlf". If the previous match was at the end of a line terminated by
		CRLF, an advance of one character just passes the \r, whereas we should
		prefer the longer newline sequence, as does the code in pcre_exec().
		Fudge the offset value to achieve this.

		Otherwise, in the case of UTF-8 matching, the advance must be one
		character, not one byte. */
		else
        {
				if (count == PCRE_ERROR_NOMATCH)
				{
					if (gmatched == 0) 
					{
						return NO_MATCH;
					}
				}

				if (count == PCRE_ERROR_MATCHLIMIT )
				{
					return MATCH_LIMIT;
				}
				break;  /* Out of loop */
		}

		/* If not /g or /G we are done */
		if (!do_g && !do_G) break;

		/* If we have matched an empty string, first check to see if we are at
		the end of the subject. If so, the /g loop is over. Otherwise, mimic
		what Perl's /g options does. This turns out to be rather cunning. First
		we set PCRE_NOTEMPTY and PCRE_ANCHORED and try the match again at the
		same point. If this fails (picked up above) we advance to the next
		character. */

		g_notempty = 0;

		if (use_offsets[0] == use_offsets[1])
        {
			if (use_offsets[0] == len) break;
			g_notempty = PCRE_NOTEMPTY | PCRE_ANCHORED;
		}

		/* For /g, update the start offset, leaving the rest alone */

		if (do_g) start_offset = use_offsets[1];
		/* For /G, update the pointer and length */
		else
        {
			bptr += use_offsets[1];
			len -= use_offsets[1];
        }
	}  /* End of loop for /g and /G */

    continue;
    }    /* End of loop for data lines */

  }
	
	if (buffer) FREE(buffer);
	if (offsets) FREE(offsets);
	return PCRE_EXIT;
}


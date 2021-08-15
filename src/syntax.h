#ifndef QSHELL_SYNTAX
#define QSHELL_SYNTAX

#define CWORD     0x0000 //! an ordinary character
#define CMETA     0x0001 //! shell meta character
#define CBREAK    0x0002 //! shell break character
#define CBACKQ    0x0004 //! back quote
#define CQUOTE    0x0008 //! quote character
#define CSPEC     0x0010 //! special character that needs quoting
#define CEXPA     0x0020 //! shell expansion character
#define CBSDQUOTE 0x0040 //! characters escaped by backslash in double quotes
#define CBSHDOC   0x0080 //! characters escaped by backslash in here doc
#define CGLOB     0x0100 //! globbing characters
#define CXGLOB    0x0200 //! extended globbing characters
#define CXQUOTE   0x0400 //! quote + backslash
#define CSPECVAR  0x0800 //! single-character shell variable name
#define CSUBSTOP  0x1000 //! values of OP for ${word[:]OPstuff}
#define CBLANK    0x2000 //! whitespace character

extern int qshtb_syntax[256]; //! from syntax.c

//! "()<>;&|"
#define qshp_ismeta(c)	 (qshtb_syntax[(unsigned char)(c)] & CMETA)

//! "()<>;&| \t\n"
#define qshp_break(c)	 (qshtb_syntax[(unsigned char)(c)] & CBREAK)

//! "\"'`"
#define qshp_isquote(c)	 (qshtb_syntax[(unsigned char)(c)] & CQUOTE)

//! "\"'\\`"
#define qshp_isxquote(c) (qshtb_syntax[(unsigned char)(c)] & CXQUOTE)

//! "\t "
#define qshp_isblank(c)	 (qshtb_syntax[(unsigned char)(c)] & CBLANK)

//! "$<>"
#define qshp_isexpa(c)   (qshtb_syntax[(unsigned char)(c)] & CEXPA)

//! "*?[]^"
#define qshp_isglob(c)   (qshtb_syntax[(unsigned char)(c)] & CGLOB)

//! "@*+?!"
#define qshp_isxglob(c)  (qshtb_syntax[(unsigned char)(c)] & CXGLOB) //! PATTERN_CHAR

#define qshp_issyntype(c, t) ((qshtb_syntax[(unsigned char)(c)] & (t)) != 0)
#define qshp_notsyntype(c, t) ((qshtb_syntax[(unsigned char)(c)] & (t)) == 0)

#define CTLESC '\001'
#define CTLNUL '\177'

#endif
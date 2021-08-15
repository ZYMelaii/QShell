#ifndef QSHELL_CMDFLAGS
#define QSHELL_CMDFLAGS

//! flags for word_t (this is from gnu/bash-5.1.8/command.h)
#define WORD_HASDOLLAR     (1 <<  0)    //! dollar sign present.
#define WORD_QUOTED        (1 <<  1)    //! some form of quote character is present.
#define WORD_ASSIGNMENT    (1 <<  2)    //! this word is a variable assignment.
#define WORD_SPLITSPACE    (1 <<  3)    //! split this word on " " regardless of ifs
#define WORD_NOSPLIT       (1 <<  4)    //! do not perform word splitting on this word because ifs is empty string.
#define WORD_NOGLOB        (1 <<  5)    //! do not perform globbing on this word.
#define WORD_NOSPLIT2      (1 <<  6)    //! don't split word except for $@ expansion (using spaces) because context does not allow it.
#define WORD_TILDEEXP      (1 <<  7)    //! tilde expand this assignment word
#define WORD_DOLLARAT      (1 <<  8)    //! $@ and its special handling -- unused
#define WORD_DOLLARSTAR    (1 <<  9)    //! $* and its special handling -- unused
#define WORD_NOCOMSUB      (1 << 10)    //! don't perform command substitution on this word.
#define WORD_ASSIGNRHS     (1 << 11)    //! word is rhs of an assignment statement
#define WORD_NOTILDE       (1 << 12)    //! don't perform tilde expansion on this word
#define WORD_ITILDE        (1 << 13)    //! internal flag for word expansion
#define WORD_EXPANDRHS     (1 << 14)    //! expanding word in ${paramopword}
#define WORD_COMPASSIGN    (1 << 15)    //! compound assignment
#define WORD_ASSNBLTIN     (1 << 16)    //! word is a builtin command that takes assignments
#define WORD_ASSIGNARG     (1 << 17)    //! word is assignment argument to command
#define WORD_HASQUOTEDNULL (1 << 18)    //! word contains a quoted null character
#define WORD_DQUOTE        (1 << 19)    //! word should be treated as if double-quoted
#define WORD_NOPROCSUB     (1 << 20)    //! don't perform process substitution
#define WORD_SAWQUOTEDNULL (1 << 21)    //! word contained a quoted null that was removed
#define WORD_ASSIGNASSOC   (1 << 22)    //! word looks like associative array assignment
#define WORD_ASSIGNARRAY   (1 << 23)    //! word looks like a compound indexed array assignment
#define WORD_ARRAYIND      (1 << 24)    //! word is an array index being expanded
#define WORD_ASSNGLOBAL    (1 << 25)    //! word is a global assignment to declare (declare/typeset -g)
#define WORD_NOBRACE       (1 << 26)    //! don't perform brace expansion
#define WORD_COMPLETE      (1 << 27)    //! word is being expanded for completion
#define WORD_CHKLOCAL      (1 << 28)    //! check for local vars on assignment
#define WORD_NOASSNTILDE   (1 << 29)    //! don't do tilde expansion like an assignment statement
#define WORD_FORCELOCAL    (1 << 30)    //! force assignments to be to local variables, non-fatal on assignment errors

//! flags for command_t (this is from gnu/bash-5.1.8/command.h)
#define CMD_TRY_SUBSHELL      0x0001 //! want a subshell.
#define CMD_FORCE_SUBSHELL    0x0002 //! force a subshell.
#define CMD_INVRETURN         0x0004 //! invert the exit value.
#define CMD_NORETURN          0x0008 //! ignore the exit value.  for set -e.
#define CMD_NOFUNC            0x0010 //! ignore functions during command lookup.
#define CMD_NOEXPA            0x0020 //! do not expand the command words.
#define CMD_NOFORK            0x0040 //! don't fork; just call execve
#define CMD_TIME_PIPELINE     0x0080 //! time a pipeline
#define CMD_TIME_POSIX        0x0100 //! time -p; use posix.2 time output spec.
#define CMD_AMPERSAND         0x0200 //! command &
#define CMD_STDIN_REDIR       0x0400 //! async command needs implicit </dev/null
#define CMD_COMMAND_BUILTIN   0x0800 //! command executed by `command' builtin
#define CMD_COPROC_SUBSHELL   0x1000 //! command coproc
#define CMD_LASTPIPE          0x2000 //! 
#define CMD_STDPATH           0x4000 //! use standard path for command lookup
#define CMD_TRYOPT            0x8000 //! try to optimize this simple command

//! flags for cmd_subshell_t (this is from gnu/bash-5.1.8/command.h)
#define SUBSHELL_ASYNC     0x01    //! subshell caused by `command &'
#define SUBSHELL_PAREN     0x02    //! subshell caused by ( ... )
#define SUBSHELL_COMSUB    0x04    //! subshell caused by `command` or $(command)
#define SUBSHELL_FORK      0x08    //! subshell caused by executing a disk command
#define SUBSHELL_PIPE      0x10    //! subshell from a pipeline element
#define SUBSHELL_PROCSUB   0x20    //! subshell caused by <(command) or >(command)
#define SUBSHELL_COPROC    0x40    //! subshell from a coproc pipeline
#define SUBSHELL_RESETTRAP 0x80    //! subshell needs to reset trap strings on first call to trap

#endif
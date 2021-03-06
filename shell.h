#ifndef SHELL_HEADER
#define SHELL_HEADER
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#define MAXARGS 256
#define MAXCMDS 50
#define MAXLINELEN 1024
//#define DEBUG

/* A process is a single process.  */
typedef struct process
{
    struct process *next; /* next process in pipeline */
    int nargs;
    char *argv[MAXARGS]; /* for exec */
    pid_t pid;           /* process ID */
    char completed;      /* 1 if process has completed */
    char stopped;        /* 1 if process has stopped */
    int status;          /* reported status value */
    int stderrno;
} process;

/* A job is a pipeline of processes.  */
typedef struct job
{
    struct job *next;                 /* next active job */
    char *command;                    /* command line, used for messages */
    process *first_process;           /* list of processes in this job */
    pid_t pgid;                       /* process group ID */
    char notified;                    /* true if user told about stopped job */
    int stdinno, stdoutno, stderrno;  /* standard i/o channels */
    struct termios tmodes;            /* saved terminal modes */
    char foreground;
    char launched;
    char builtin;
} job;

/* ������ ������ ������� */
extern job *first_job;

extern pid_t shell_pgid;

typedef enum tokenType
{
    WORD,
    AMPERSAND,        /* '&' */
    SEMICOLON,        /* ';' */
    LEFTARROW,        /* '<' */
    RIGHTARROW,       /* '>' */
    REALLYRIGHTARROW, /* '>>' */
    PIPE,             /* '|' */
    ERRORRIGHTARROW   /* 2> */
} tokenType;

typedef struct token
{
    char *place;
    tokenType type;
} token;


int parseline(char *);
int promptline(char *, char *, int);

void launch_job(job *j);

extern struct termios shell_tmodes;

#endif

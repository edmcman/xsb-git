/* File:      init.c
** Author(s): Warren, Swift, Xu, Sagonas, Johnson, Prasad
** Contact:   xsb-contact@cs.sunysb.edu
** 
** Copyright (C) The Research Foundation of SUNY, 1986, 1993-1998
** Copyright (C) ECRC, Germany, 1990
** 
** XSB is free software; you can redistribute it and/or modify it under the
** terms of the GNU Library General Public License as published by the Free
** Software Foundation; either version 2 of the License, or (at your option)
** any later version.
** 
** XSB is distributed in the hope that it will be useful, but WITHOUT ANY
** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
** FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License for
** more details.
** 
** You should have received a copy of the GNU Library General Public License
** along with XSB; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** $Id: init.c,v 1.1.1.1 1998-11-05 16:55:17 sbprolog Exp $
** 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configs/config.h"
#include "debugs/debug.h"

#include "auxlry.h"
#include "cell.h"
#include "xsberror.h"
#include "inst.h"
#include "psc.h"
#include "hash.h"
#include "memory.h"
#include "register.h"
#include "tries.h"
#include "choice.h"
#include "flags.h"
#include "loader.h"
#include "load_seg.h"
#include "xmacro.h"
#include "tr_utils.h"


/*-----------------------------------------------------------------------*/

/* Sizes of the Data Regions in K-byte blocks
   ------------------------------------------ */
#ifdef BITS64
#define PDL_DEFAULT_SIZE         (64*2)
#define GLSTACK_DEFAULT_SIZE    (768*2)
#define TCPSTACK_DEFAULT_SIZE   (768*2)
#define COMPLSTACK_DEFAULT_SIZE  (64*2)
#else
#define PDL_DEFAULT_SIZE         64
#define GLSTACK_DEFAULT_SIZE    768
#define TCPSTACK_DEFAULT_SIZE   768
#define COMPLSTACK_DEFAULT_SIZE  64
#endif

long pspacesize = 0;	/* actual space dynamically allocated by loader.c */

/* The SLG-WAM data regions
   ------------------------ */
System_Stack
pdl = {NULL, NULL, 0,
       PDL_DEFAULT_SIZE},             /* PDL                   */
  glstack = {NULL, NULL, 0,
	     GLSTACK_DEFAULT_SIZE},     /* Global + Local Stacks */
    tcpstack = {NULL, NULL, 0,
		TCPSTACK_DEFAULT_SIZE},   /* Trail + CP Stack      */
      complstack = {NULL, NULL, 0,
		    COMPLSTACK_DEFAULT_SIZE};   /* Completion Stack  */

extern char executable[];

Exec_Mode xsb_mode;     /* How XSB is run: interp, disassem, user spec, etc. */

extern char* index(char *, int);

/* real_alloc uses malloc only to keep pspacesize straight. */
#define real_alloc(X) malloc(X) 

/*
byte retry_active_inst = retry_active;
byte completion_suspension_inst = completion_suspension;
byte check_complete_inst = check_complete;
byte hash_handle_inst = hash_handle;
byte fail_inst = fail;
byte halt_inst = halt;
byte proceed_inst = proceed; */         /* returned by load_obj */

Cell retry_active_inst;
Cell completion_suspension_inst;
Cell check_complete_inst;
Cell hash_handle_inst;
Cell fail_inst;
Cell halt_inst;
Cell proceed_inst;

extern double realtime_count;
extern pw reloc_table[];

/* jf: to init stat. structs */
extern void perproc_reset_stat(), reset_stat_total(); 

extern char *install_dir; /* from xmain.c */
extern char *xsb_config_file; /* from xmain.c */
extern char *user_home; /* the user HOME dir or install dir, if HOME is null */

Cell *term_stack;

/* Version message */

char *word_mode[3] = {
  "single word",
  "one & half word",
  "double word"
};

char *par_mode[3] = {
  "sequential",
  "parallel"
};

char *months[12] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};



/*==========================================================================*/

void static display_file(char *infile_name)
{
  FILE *infile;
  char buffer[MAXBUFSIZE];

  if ((infile = fopen(infile_name, "r")) == NULL) {
    fprintf(stderr,
	    "\nCan't open `%s'; XSB installation might be corrupted\n\n",
	    infile_name);
  }

  while (fgets(buffer, MAXBUFSIZE-1, infile) != NULL)
    printf("%s", buffer);

  fclose(infile);
}


void static version_message(void) {

  char licensefile[MAXPATHLEN], configfile[MAXPATHLEN];


  sprintf(licensefile, "%s%cetc%ccopying.msg", install_dir, SLASH, SLASH);
  sprintf(configfile, "%s%cetc%cconfig.msg", install_dir, SLASH, SLASH);

  display_file(configfile);
  puts("");
  display_file(licensefile);

  exit(0);
}

void static help_message(void) {
  char helpfile[MAXPATHLEN];
  sprintf(helpfile, "%s%cetc%chelp.msg", install_dir, SLASH, SLASH);
  puts("");
  display_file(helpfile);

  exit(0);
}


/*==========================================================================*/

/* Initialize System Parameters
   ---------------------------- */
char *init_para(int argc, char *argv[])
{
  int i;
  char c, warning[80];
  /* Boot module is usually the loader that loads the Prolog code of XSB.
  ** Or it can be a code to disassemble.
  ** Cmd loop driver is usually the XSB interpreter (x_interp.P).
  ** However, it can be any program that communicates with XSB and drives its
  ** comand loop.
  */
  char *boot_module, *cmd_loop_driver;
  char *cmd_line_goal="true.";
  int strlen_instdir, strlen_initfile, strlen_2ndfile;

  {
    /* This needs cleaning up : should go to a .h file in the
       next integration -Prasad */
    extern struct HASHhdr HASHroot;
    extern struct HASHhdr *HASHrootptr;

    HASHrootptr = &HASHroot;
  }
  init_newtrie();
  alloc_arr(Cell,term_stack,term_stacksize);
  alloc_arr(CPtr,var_addr,var_addr_arraysz);
  alloc_arr(CPtr,Addr_Stack,addr_stack_size );
  alloc_arr(Cell,reg_array,reg_array_size);
  reg_arrayptr = reg_array -1;

  /* jf: init stat. structures */
  perproc_reset_stat();
  reset_stat_total();

  /*
    term_stack = (Cell *)malloc(sizeof(Cell) * term_stacksize);
    exit_if_null(term_stack);

    var_addr = (CPtr *)malloc(sizeof(CPtr)* var_addr_arraysz);
    exit_if_null(var_addr);

    Addr_Stack = (CPtr *)malloc(sizeof(CPtr)*addr_stack_size );
    exit_if_null(Addr_Stack);
  */ 


  flags[STACK_REALLOC] = TRUE;


  /* Set default Prolog files.
     ------------------------- */
#ifdef WIN_NT
  boot_module = "\\syslib\\loader.O";
#elif DJGPP
  boot_module = "/syslib/loader.OX";
#else
  boot_module = "/syslib/loader.O";
#endif

  /* File extensions are automatically added for Loader-loaded files. */
#ifdef WIN_NT
  cmd_loop_driver = "\\syslib\\x_interp";
#else
  cmd_loop_driver = "/syslib/x_interp";
#endif


  xsb_mode = DEFAULT;


  /* Modify Parameters Using Command Line Options
     -------------------------------------------- */
  for (i=1; i<argc; i++) {
    if (*argv[i] != '-') {        /* command-line module specified */
      if (xsb_mode != DEFAULT)
	help_message();
      xsb_mode = CUSTOM_CMD_LOOP_DRIVER;
      cmd_loop_driver = argv[i];
      continue;
    }

    /* Otherwise, get command-line switch (and arg).  Will dump core
       if the accompanying argument is omitted. */
    switch((c = argv[i][1])) {
    case 'r':
      flags[STACK_REALLOC] = FALSE;
      break;
    case 'u':
      if (argv[i][2] != '\0')
	sscanf(argv[i]+2, "%ld", &pdl.init_size);
      else {
	i++;
	if (i < argc)
	   sscanf(argv[i], "%ld", &pdl.init_size); /* JF: long */
	 else
	   xsb_warn("Missing size value");
      }
      break;
    case 'm':
      if (argv[i][2] != '\0')
	sscanf(argv[i]+2, "%ld", &glstack.init_size);
      else {
	i++;
	if (i < argc)
	   sscanf(argv[i], "%ld", &glstack.init_size);
	 else
	   xsb_warn("Missing size value");
      }
      break;
    case 'c':
      if (argv[i][2] != '\0')
	sscanf(argv[i]+2, "%ld", &tcpstack.init_size);
      else {
	i++;
	if (i < argc)
	   sscanf(argv[i], "%ld", &tcpstack.init_size);
	 else
	   xsb_warn("Missing size value");
      }
      break;
    case 'o':
      if (argv[i][2] != '\0')
	sscanf(argv[i]+2, "%ld", &complstack.init_size);
      else {
	i++;
	if (i < argc)
	  sscanf(argv[i], "%ld", &complstack.init_size);
	else
	  xsb_warn("Missing size value");
      }
      break;
    case 's':
      flags[TRACE_STA] = call_intercept = 1;
      break;
    case 'd':
      if ( (xsb_mode != DEFAULT) && (xsb_mode != CUSTOM_BOOT_MODULE) )
	help_message();
      xsb_mode = DISASSEMBLE;
      break;
    case 'T': 
      flags[HITRACE] = call_intercept = 1; 
      break;
    case 't': 
#ifdef DEBUG
      flags[PIL_TRACE] = flags[HITRACE] = call_intercept = 1;
#else
      xsb_exit("-t option unavailable for this executable (non-debug mode)");
#endif
      break;
    case 'i':
      if (xsb_mode != DEFAULT)
	help_message();
      xsb_mode = INTERPRETER;
      break;
    case 'l':
      flags[LETTER_VARS] = 1;
      break;
    case 'n':
      if (xsb_mode != DEFAULT)
	help_message();
      xsb_mode = C_CALLING_XSB;
#ifdef WIN_NT
      cmd_loop_driver = "\\syslib\\xcallxsb";
#else
      cmd_loop_driver = "/syslib/xcallxsb";
#endif
      break;
    case 'B':
      if (xsb_mode == DEFAULT)
	xsb_mode = CUSTOM_BOOT_MODULE;
      else if (xsb_mode != DISASSEMBLE)   /* retain disassemble command for */
	help_message();                /* -d -f <file> AWA -f <file> -d */
      if (argv[i][2] != '\0')
	boot_module = argv[i]+2;
      else {
	i++;
	if (i < argc)
	   boot_module = argv[i];
	 else
	   xsb_warn("Missing boot module's file name");
      }
      break;
    case 'D':
      if (xsb_mode == DEFAULT)
	xsb_mode = CUSTOM_CMD_LOOP_DRIVER;
      else if (xsb_mode != CUSTOM_BOOT_MODULE)
	help_message();
      if (argv[i][2] != '\0')
	cmd_loop_driver = argv[i]+2;
      else {
	i++;
	if (i < argc)
	   cmd_loop_driver = argv[i];
	 else
	   xsb_warn("Missing top-level command loop driver's file name");

      }
      break;
    case 'e':
      if (argv[i][2] != '\0')
	cmd_line_goal = argv[i]+2;
      else {
	i++;
	if (i < argc)
	   cmd_line_goal = argv[i];
	 else
	   xsb_warn("Missing command line goal");
      }

      if (index(cmd_line_goal, '.') == NULL) {
	char tmpbuf[100];
	sprintf(tmpbuf, "Syntax error in command line goal:\n\t`%s'",
		cmd_line_goal);
	xsb_abort(tmpbuf);
      }
      break;
    case 'h':
      help_message();
      break;
    case 'v':
      version_message();
      break;
    default:
      sprintf(warning, "Unknown command line option %s", argv[i]);
      xsb_warn(warning);
    } /* switch */
  } /* for */
  /* Done with command line arguments */

  /* This is where we will be looking for the .xsb directory */
  flags[USER_HOME] = (Cell) malloc(strlen(user_home) + 1);
  strcpy( (char *)flags[USER_HOME], user_home );

  /* install_dir is computed dynamically at system startup (in xmain.c).
     Therefore, the entire directory tree can be moved --- only the relative
     positions count.
  */ 
  flags[INSTALL_DIR] = (Cell) malloc(strlen(install_dir) + 1);   
  strcpy( (char *)flags[INSTALL_DIR], install_dir );

  /* loader uses CONFIG_NAME flag before xsb_configuration is loaded */
  flags[CONFIG_NAME] = (Cell) malloc(strlen(CONFIGURATION) + 1);
  strcpy( (char *)flags[CONFIG_NAME], CONFIGURATION );

  flags[CONFIG_FILE] = (Cell) malloc(strlen(xsb_config_file) + 1);
  strcpy( (char *)flags[CONFIG_FILE], xsb_config_file );

  /* the default for cmd_line_goal goal is "true." */
  flags[CMD_LINE_GOAL] = (Cell) malloc(strlen(cmd_line_goal) + 1);
  strcpy( (char *)flags[CMD_LINE_GOAL], cmd_line_goal );
  

  /* Set the Prolog startup files.
     ----------------------------- */
  /* Default execution mode is to load and run the interpreter. */
  if (xsb_mode == DEFAULT)
    xsb_mode = INTERPRETER;

  strlen_instdir = strlen(install_dir);
  strlen_initfile = strlen(boot_module);
  strlen_2ndfile = strlen(cmd_loop_driver);

  switch(xsb_mode) {
  case INTERPRETER:
  case C_CALLING_XSB:
    /*
     *  A "short-cut" option in which the loader is the loader file and
     *  an XSB-supplied "server" program is the interpreter file.  Since
     *  it is known where these files exist, the full paths are built.
     */
    flags[BOOT_MODULE] = (Cell) malloc(strlen_instdir + strlen_initfile + 1);
    flags[CMD_LOOP_DRIVER ] = (Cell)malloc(strlen_instdir + strlen_2ndfile + 1);
    sprintf( (char *)flags[BOOT_MODULE], "%s%s", install_dir,
	     boot_module );
    sprintf( (char *)flags[CMD_LOOP_DRIVER ], "%s%s", install_dir,
	     cmd_loop_driver );
    break;
  case CUSTOM_BOOT_MODULE:
    /*
     *  The user has specified a private loader to be used instead of the
     *  standard one and possibly a top-level command loop driver as well.  In
     *  either case, we can 
     *  make no assumptions as to where these files exist, and so the 
     *  user must supply an adequate filename in each case.
     */
    flags[BOOT_MODULE] = (Cell) malloc(strlen_initfile + 1);
    flags[CMD_LOOP_DRIVER ] = (Cell) malloc(strlen_2ndfile + 1);
    strcpy( (char *)flags[BOOT_MODULE], boot_module );
    strcpy( (char *)flags[CMD_LOOP_DRIVER ], cmd_loop_driver );
    break;
  case CUSTOM_CMD_LOOP_DRIVER:
    /*
     *  The user has specified a private top-level command loop.
     *  The filename can be absolute; however if not, it will
     *  be looked for in XSB's library path.
     */
    flags[BOOT_MODULE] = (Cell) malloc(strlen_instdir + strlen_initfile + 1);
    flags[CMD_LOOP_DRIVER ] = (Cell) malloc(strlen_2ndfile + 1);
    sprintf( (char *)flags[BOOT_MODULE], "%s%s", install_dir,
	     boot_module );
    strcpy( (char *)flags[CMD_LOOP_DRIVER ], cmd_loop_driver );
    break;
  case DISASSEMBLE:
    /*
     *  A loader file should have been specified for disassembling.
     */
    flags[BOOT_MODULE] = (Cell) malloc(strlen_initfile + 1);
    strcpy( (char *)flags[BOOT_MODULE], boot_module );
    break;
  default:
    xsb_exit("Setting startup files: Bad XSB mode!");
    break;
  }

  return ( (char *) flags[BOOT_MODULE] );
}

/*==========================================================================*/

/* Initialize Memory Regions and Related Variables
   ----------------------------------------------- */
void init_machine(void) {
  int i;

  /* set special (X)WAM instruction addresses */
  cell_opcode(&retry_active_inst) = retry_active;
  cell_opcode(&completion_suspension_inst) = completion_suspension;
  cell_opcode(&check_complete_inst) = check_complete;
  cell_opcode(&hash_handle_inst) = hash_handle;
  cell_opcode(&fail_inst) = fail;
  cell_opcode(&halt_inst) = halt;
  cell_opcode(&proceed_inst) = proceed;         /* returned by load_obj */

  /* Allocate Stack Spaces and set Boundary Parameters
     ------------------------------------------------- */
  pdl.low = (byte *)real_alloc(pdl.init_size * K);
  if (!pdl.low)
    xsb_exit("Not enough core for the PDL!");
  pdl.high = pdl.low + pdl.init_size * K;
  pdl.size = pdl.init_size;

  glstack.low = (byte *)real_alloc(glstack.init_size * K);
  if (!glstack.low)
    xsb_exit("Not enough core for the Global and Local Stacks!");
  glstack.high = glstack.low + glstack.init_size * K;
  glstack.size = glstack.init_size;

  tcpstack.low = (byte *)real_alloc(tcpstack.init_size * K);
  if (!tcpstack.low)
    xsb_exit("Not enough core for the Trail and Choice Point Stack!");
  tcpstack.high = tcpstack.low + tcpstack.init_size * K;
  tcpstack.size = tcpstack.init_size;

  complstack.low = (byte *)real_alloc(complstack.init_size * K);
  if (!complstack.low)
    xsb_exit("Not enough core for the Completion Stack!");
  complstack.high = complstack.low + complstack.init_size * K;
  complstack.size = complstack.init_size;

  /* -------------------------------------------------------------------
     So, the layout of the memory looks as follows:

     pdl.low
     /\
     pdlreg   |
     pdl.high
     ===================
     glstack.low
     hreg   |
     \/
     /\
     ereg   |
     glstack.high
     ===================
     tcpstack.low
     trreg  |
     \/
     /\
     breg   |
     tcpstack.high
     ===================
     complstack.low

     /\
     openreg  |
     complstack.high
     ---------------------------------------------------------------------	*/

  /* Initialize Registers
     -------------------- */
  cpreg = (pb) &halt_inst;		/* halt on final success */

  pdlreg = (CPtr)(pdl.high) - 1;

  hbreg = hreg = (CPtr)(glstack.low);

  ebreg = ereg = (CPtr)(glstack.high) - 1;

  *(ereg-1) = (Cell) cpreg;

  trreg	 = (CPtr *)(tcpstack.low);
  *(trreg) = (CPtr) trreg;

  /* Place a base choice point frame on the CP Stack.
     ------------------------------------------------ */
  breg	 = (CPtr)(tcpstack.high) - CP_SIZE;
  cp_pcreg(breg) = (pb) &halt_inst; 	  /* halt on last failure */
  cp_ebreg(breg) = ebreg;		  /* need for cut.  */
  cp_hreg(breg) = hreg;		 	  /* need for cut.  */
  cp_trreg(breg) = trreg;            	  /* need for cut.  */
  cp_prevbreg(breg) = breg;      	  /* need for cut.  */

  reset_freeze_registers;
  openreg = ((CPtr) complstack.high);
  delayreg = NULL;


  /* Other basic initializations
     --------------------------- */
  realtime_count = real_time();
  inst_begin = 0;

  symbol_table.table = calloc(symbol_table.size, sizeof(Pair));
  string_table.table = calloc(string_table.size, sizeof(char *));

  for (i = 0; i < NUM_TRIEVARS; i++)
    VarEnumerator[i] = (Cell) & (VarEnumerator[i]);

  open_files[0] = stdin;
  open_files[1] = stdout;
  open_files[2] = stderr;
  for (i=3; i < MAX_OPEN_FILES; i++) open_files[i] = NULL;
}

/*==========================================================================*/

/* Initialize System Flags
   ----------------------- */
void init_flags(void)
{
  int i;

  for (i=0; i<64; i++) flags[i] = 0;
  flags[RELOC_TABLE] = (Cell)reloc_table;
  /*    flags[CURRENT_OUTPUT] = 1; */
}

/*==========================================================================*/

/* Initialize Standard PSC Records
   ------------------------------- */
void init_symbols(void)
{
  Psc tables_psc;
  Pair temp, tp;
  int new_indicator;

  /* insert mod name global */
  tp = (Pair)insert_module(T_MODU, "global");	/* loaded */
  set_ep(pair_psc(tp), (byte *)1);	/* 1 stands for global mod */
  global_mod = pair_psc(tp);

  /* insert "[]"/0 into String Table */
  nil_sym = string_find("[]", 1);

  /* insert "."/2 into global list */
  temp = (Pair)insert(".", 2, global_mod, &new_indicator);
  list_str = temp;
  list_psc = pair_psc(temp);
  list_dot = get_name(list_psc);

  /* insert symbol ","/2 */
  temp = (Pair)insert(",", 2, global_mod, &new_indicator);
  comma_psc = pair_psc(temp);

  /* insert symbol tnot/1 into module tables */
  tp = (Pair)insert_module(0, "tables");		/* unloaded */
  tables_psc = pair_psc(tp);
  temp = (Pair)insert("tnot", 1, tables_psc, &new_indicator);
  tnot_psc = pair_psc(temp);
  set_ep(tnot_psc, (byte *)tables_psc);
  set_env(tnot_psc, T_UNLOADED);
  set_type(tnot_psc, T_ORDI);
  temp = (Pair)insert("DL", 3, global_mod, &new_indicator);
  delay_psc = pair_psc(temp);

  /* make another reference to global module -- "usermod" */
  tp = (Pair)insert_module(T_MODU, "usermod");	/* loaded */
  set_ep(pair_psc(tp), get_ep(global_mod));
}

/*==========================================================================*/

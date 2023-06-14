/********************************************************************************
*                                                                               *
*                             Regular Expression Test                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"


/*


*/

#define NCAP 10    // Must be less that or equal to 10


/*******************************************************************************/


// Print command line help
static void printusage(){
  fxmessage("Usage: rex [options] pat [string]\n");
  fxmessage("  options:\n");
  fxmessage("  -?, -h, --help                      Print help.\n");
  fxmessage("  -tracetopics <topics>               Enable trace topics.\n");
  fxmessage("  -u, --unicode                       Unicode mode.\n");
  fxmessage("  -s, --syntax                        Syntax check only.\n");
  fxmessage("  -v, --verbatim                      Verbatim mode.\n");
  fxmessage("  -c, --capture                       Capturing parentheses.\n");
  fxmessage("  -i, --ignore-case                   Case-insensitive.\n");
  fxmessage("  -nl, --newline                      Newline matches \"any\" character classes.\n");
  fxmessage("  -x, --exact                         Match should consume all of string.\n");
  fxmessage("  -n, --not-empty                     Match should be non-empty.\n");
  fxmessage("  -r, --reverse                       Reverse match.\n");
  fxmessage("  -w, --words                         Match whole words.\n");
  fxmessage("  -nb, --not-bol                      Start of string is not begin of line.\n");
  fxmessage("  -ne, --not-eol                      End of string is not end of line.\n");
  fxmessage("  -l <num>, --levels <num>            Capure levels.\n");
  }


// Start the whole thing
int main(int argc,char** argv){
  FXint beg[NCAP]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  FXint end[NCAP]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  FXint mode=FXRex::Normal;
  FXint ncap=1;
  FXint where;
  FXint arg=1;
  FXRex rex;
  FXRex::Error err;

  // Parse options
  while(arg<argc && argv[arg][0]=='-'){
    if(FXString::compare(argv[arg],"-s")==0 || FXString::compare(argv[arg],"--syntax")==0){
      mode|=FXRex::Syntax;
      }
    else if(FXString::compare(argv[arg],"-v")==0 || FXString::compare(argv[arg],"--verbatim")==0){
      mode|=FXRex::Verbatim;
      }
    else if(FXString::compare(argv[arg],"-c")==0 || FXString::compare(argv[arg],"--capture")==0){
      mode|=FXRex::Capture;
      }
    else if(FXString::compare(argv[arg],"-i")==0 || FXString::compare(argv[arg],"--ignore-case")==0){
      mode|=FXRex::IgnoreCase;
      }
    else if(FXString::compare(argv[arg],"-nl")==0 || FXString::compare(argv[arg],"--newline")==0){
      mode|=FXRex::Newline;
      }
    else if(FXString::compare(argv[arg],"-x")==0 || FXString::compare(argv[arg],"--exact")==0){
      mode|=FXRex::Exact;
      }
    else if(FXString::compare(argv[arg],"-n")==0 || FXString::compare(argv[arg],"--not-empty")==0){
      mode|=FXRex::NotEmpty;
      }
    else if(FXString::compare(argv[arg],"-r")==0 || FXString::compare(argv[arg],"--reverse")==0){
      mode|=FXRex::Reverse;
      }
    else if(FXString::compare(argv[arg],"-w")==0 || FXString::compare(argv[arg],"--words")==0){
      mode|=FXRex::Words;
      }
    else if(FXString::compare(argv[arg],"-nb")==0 || FXString::compare(argv[arg],"--not-bol")==0){
      mode|=FXRex::NotBol;
      }
    else if(FXString::compare(argv[arg],"-ne")==0 || FXString::compare(argv[arg],"--not-eol")==0){
      mode|=FXRex::NotEol;
      }
    else if(FXString::compare(argv[arg],"-u")==0 || FXString::compare(argv[arg],"--unicode")==0){
      mode|=FXRex::Unicode;
      }
    else if(FXString::compare(argv[arg],"-l")==0 || FXString::compare(argv[arg],"--levels")==0){
      if(++arg>=argc){ fxwarning("rex: missing capture levels.\n"); return 1; }
      sscanf(argv[arg],"%d",&ncap);
      }
    else if(FXString::compare(argv[arg],"-?")==0 || FXString::compare(argv[arg],"-h")==0 || FXString::compare(argv[arg],"--help")==0){
      printusage();
      return 0;
      }
    else if(FXString::compare(argv[arg],"-tracetopics")==0){
      if(++arg>=argc){ fxwarning("rex: missing argument for -tracetopics.\n"); return 0; }
      setTraceTopics(argv[arg++]);
      continue;
      }
    arg++;
    }

  // Pattern
  if(arg<argc){

    // Parse expression
    err=rex.parse(argv[arg],mode);

    // Show result of parse
    fxmessage("parse(\"%s\") = %s\n",argv[arg],FXRex::getError(err));

    // Next
    arg++;

    // Input
    if(arg<argc){

      // Search string with pattern
      where=rex.search(argv[arg],strlen(argv[arg]),0,strlen(argv[arg]),mode,beg,end,ncap);

      // If found, show where
      if(0<=where){
        fxmessage("found at %d\n",where);
        for(FXint i=0; i<ncap; i++){
          fxmessage("capture at %d:%d\n",beg[i],end[i]);
          }
        for(FXint i=beg[0]; i<end[0]; i++){
          fxmessage("%c",argv[arg][i]);
          }
        fxmessage("\n");
        }
      else{
        fxmessage("no match\n");
        }

      // Next
      arg++;
      }
    }
  return 1;
  }


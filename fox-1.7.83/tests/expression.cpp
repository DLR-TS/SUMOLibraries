/********************************************************************************
*                                                                               *
*                           Expression Evaluator Test                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 2006,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include"fx.h"

/*
  Notes:

  - Call as follows:

      expression <expression> [ <variables> [ <values> ]]

    where:

       <expression>     math expression to be evaluated
       <variables>      comma-separated list of variable names
       <values>         numbers, one for each variable

  - Example:

      expression a+b   a,b   5 6

    Will print out:

      parse("a+b","a,b") : 11

*/


/*******************************************************************************/

// Print command line help
static void printusage(){
  fxmessage("Usage: rex [options] pat [string]\n");
  fxmessage("  options:\n");
  fxmessage("  -?, -h, --help                      Print help.\n");
  fxmessage("  -tracetopics <topics>               Enable trace topics.\n");
  }



/* Evaluate expression with arguments */
int main(int argc,char **argv){
  const FXchar *expr="";
  const FXchar *vars="";
  FXdouble parameter[256];
  FXExpression::Error err;
  FXExpression expression;
  FXdouble result;
  FXint arg=1;
  FXint i=0;

  // Zero parameters
  clearElms(parameter,256);

  // Parse options
  while(arg<argc && argv[arg][0]=='-'){
    if(FXString::compare(argv[arg],"-?")==0 || FXString::compare(argv[arg],"-h")==0 || FXString::compare(argv[arg],"--help")==0){
      printusage();
      return 0;
      }
    else if(FXString::compare(argv[arg],"-tracetopics")==0){
      if(++arg>=argc){ fxwarning("%s: missing argument for -tracetopics.\n",argv[0]); return 0; }
      setTraceTopics(argv[arg++]);
      continue;
      }
    arg++;
    }

  // Expression
  if(arg<argc){
    expr=argv[arg++];
    if(arg<argc){
      vars=argv[arg++];
      }
    fxmessage("parse(\"%s\",\"%s\") ",expr,vars);
    err=expression.parse(expr,vars);
    if(err!=FXExpression::ErrOK){
      fxwarning("Error: %s\n",FXExpression::getError(err));
      return 0;
      }
    while(arg<argc){
      parameter[i++]=strtod(argv[arg++],nullptr);
      }
    result=expression.evaluate(parameter);
    fxmessage(" : %.17lg\n",result);
    }
  return 0;
  }

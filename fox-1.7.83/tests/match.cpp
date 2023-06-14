/********************************************************************************
*                                                                               *
*                             File Pattern Match Test                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 2007,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"

/*
  Notes:
  - Test battery for FXPath::match().
*/


/*******************************************************************************/

static FXbool matchtest(const FXchar* pattern,const FXchar* string,FXuint flags,FXbool expected){
  const FXchar *const passorfail[]={"false","true "};
  const FXchar *const okorbad[]={"BAD","   "};
  FXbool result=FXPath::match(string,pattern,flags);
  FXbool ok=result==expected;
  fxwarning("%s pattern: %-25s match: %-25s  result: %s expected: %s\n",okorbad[ok],pattern,string,passorfail[result],passorfail[expected]);
  return ok;
  }


// Start
int main(int,char*[]){
  setTraceLevel(1);
  matchtest("ABCD", "ABCD", 0, true);
  matchtest("ABCD", "abcd", 0, false);
  matchtest("ABCD", "abcd", FXPath::CaseFold, true);
  matchtest("ABCD", "ABCD", FXPath::CaseFold, true);
  matchtest("ABC?", "abcd", FXPath::CaseFold, true);
  matchtest("A*", "a", FXPath::CaseFold, true);
  matchtest("A*", "ab", FXPath::CaseFold, true);
  matchtest("A*", "abc", FXPath::CaseFold, true);
  matchtest("A*", "a/b", FXPath::CaseFold, true);
  matchtest("A*", "a/b", FXPath::CaseFold|FXPath::PathName, false);
  matchtest("*", ".bashrc", FXPath::CaseFold, true);
  matchtest("*", ".bashrc", FXPath::CaseFold|FXPath::DotFile, false);
  matchtest("*.bashrc", ".bashrc", FXPath::CaseFold|FXPath::DotFile, true);
  matchtest("*", ".bashrc", FXPath::CaseFold|FXPath::DotFile, false);
  matchtest("A\\*", "a*", FXPath::CaseFold, true);
  matchtest("A\\*", "a\\bc", FXPath::CaseFold|FXPath::NoEscape, true);
  matchtest("[ABCD", "aefgh", FXPath::CaseFold, false);
  matchtest("[]]", "]", FXPath::CaseFold, true);
  matchtest("[ABCD]efgh", "aefgh", FXPath::CaseFold, true);
  matchtest("[^ABCD]efgh", "aefgh", FXPath::CaseFold, false);
  matchtest("[A-D]efgh", "aefgh", FXPath::CaseFold, true);
  matchtest("[A-D]efgh", "defgh", FXPath::CaseFold, true);
  matchtest("[A-D]efgh", "xefgh", FXPath::CaseFold, false);
  matchtest("[]A-D]efgh", "]efgh", FXPath::CaseFold, true);
  matchtest("[A-]efgh", "-efgh", FXPath::CaseFold, true);
  matchtest("*J*", "ABCDJEFGH", FXPath::CaseFold, true);
  matchtest("a,b,c", "B", FXPath::CaseFold, true);
  matchtest("(C|D)E", "CE", 0, true);
  matchtest("(A|B)(C|D)E", "BCE", 0, true);
  matchtest("a,b,\\,", ",", FXPath::CaseFold, true);
  matchtest("(A|B).o", "B.o", FXPath::CaseFold, true);
  matchtest("*x", "x", FXPath::CaseFold, true);
  matchtest("*x", "yyyx", FXPath::CaseFold, true);
  matchtest("ABC\\", "ABC", FXPath::CaseFold, false);
  matchtest("B)", "B", 0, false);
  matchtest("(B", "B", 0, false);
  matchtest("c.(e|f*)", "c.fx", 0, true);
  matchtest("(a|([bc)d])|e)F", ")F", FXPath::CaseFold, true);
  matchtest("(a|([bc)d])|e)F", ")F", FXPath::CaseFold, true);
  matchtest("(a.(b|c)|d.(e|f))", "a.c", 0, true);
  matchtest("(a.(b|c)|d.(e|f*))", "d.fx", 0, true);
  matchtest("(a|([bc()d])|e)F", "EF", FXPath::CaseFold, true);
  matchtest("A*", "a/b", FXPath::CaseFold|FXPath::PathName|FXPath::LeadDir, true);
  matchtest("((A|B)C|D)E", "DE", 0, true);
  matchtest("(AB|EF)", "EF", 0, true);
  matchtest("(AB|EF))", "EF", 0, false);
/*
*/
  return 1;
  }


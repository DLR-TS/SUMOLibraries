/********************************************************************************
*                                                                               *
*                         S y n t a x   P a r s e r                             *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This program is free software: you can redistribute it and/or modify          *
* it under the terms of the GNU General Public License as published by          *
* the Free Software Foundation, either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This program is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU General Public License for more details.                                  *
*                                                                               *
* You should have received a copy of the GNU General Public License             *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.         *
********************************************************************************/
#ifndef SYNTAXPARSER_H
#define SYNTAXPARSER_H


// Parser for syntax file
class SyntaxParser {
private:
  const FXchar *from;           // Source of text being parsed
  const FXchar *head;           // Start of token
  const FXchar *tail;           // End of token+1
  FXuint        token;          // Token type
  FXint         line;           // Line number
private:
  FXuint gettok();
  FXbool parseString(FXString& value);
  FXbool parseRule(Syntax *syntax,FXint parent);
  FXbool parseLanguage(SyntaxList& syntaxes);
  FXbool parse(SyntaxList& syntaxes);
protected:
  enum {
    TK_ERROR=0U,
    TK_EOF=1U,
    TK_INTEGER=2U,
    TK_STRING=3U,
    TK_RULE=4120910U,
    TK_LANGUAGE=1350891346U,
    TK_FILESMATCH=1228350630U,
    TK_CONTENTSMATCH=586315177U,
    TK_DELIMITERS=2185620912U,
    TK_CONTEXTLINES=2311868482U,
    TK_CONTEXTCHARS=2319640212U,
    TK_PATTERN=3508332936U,
    TK_CLOSE=116773014U,
    TK_OPEN=3974612U,
    TK_STOP=4087960U,
    TK_SPAN=4083660U,
    TK_GROUP=121469983U,
    TK_STYLE=134930039U,
    TK_AUTOINDENT=3363853907U,
    TK_WRAPWIDTH=2171940434U,
    TK_TABWIDTH=3437041073U,
    TK_WORDWRAP=2976955066U,
    TK_EXPANDTABS=2659402850U,
    TK_NO=3649U,
    TK_YES=134927U,
    TK_END=113327U
    };
public:

  // Construct parser
  SyntaxParser(const FXchar* pat,const FXchar* frm="input");

  // Parse string and return syntaxes found in it; return false if problem.
  static FXbool parse(SyntaxList& syntaxes,const FXchar* patterns);

  // Parse string and return syntaxes found in it; return false if problem.
  static FXbool parse(SyntaxList& syntaxes,const FXString& patterns);

  // Parse file and return syntaxes found in it; return false if problem.
  static FXbool parseFile(SyntaxList& syntaxes,const FXString& filename);

  // Destroy parser
 ~SyntaxParser();
  };


#endif

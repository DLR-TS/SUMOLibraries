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
#include "fx.h"
#include "Syntax.h"
#include "SyntaxParser.h"


/*
  Notes:

  - SyntaxParser is a simple recursive descent parser.  The style language has
    very few lexical elements: comments, spaces, numbers, strings, and identifiers.

    o  Comments.  Comments are lines preceeded by a '#'.  All text after the
       '#' is skipped until the end of the line.

    o  Spaces.  Spaces are not significant, except inside strings.  Spaces are
       simply skipped.

    o  Numbers.  Only decimal integer numbers are supported.

    o  Strings.  Strings are any text between double quotes ('"').  There are no
       escape sequences, except to enter a double quote ('\"').

       For instance "\"quoted\"" means "quoted" but "line\n" means line\n, not
       line<NL>.  This is to keep patterns moderately sane and avoid multiple
       levels of escaping, since the regular expression engine already has its
       own escape mechanisms.

    o  Keywords.  As with most programming languages, keywords start with
       a letter, followed by letters or digits.

  - Syntax patterns syntax:

      Syntax         =     { Language }*

      Language       =     "language" String
                           { LangDefinition }*
                           "end"

      LangDefinition =     "filesmatch"    String
                     |     "contentsmatch" String
                     |     "delimiters"    String
                     |     "contextlines"  Number
                     |     "contextchars"  Number
                     |     "autoindent"    YesNo
                     |     "wrapwidth"     Number
                     |     "tabwidth"      Number
                     |     "wordwrap"      YesNo
                     |     "expandtabs"    YesNo
                     |     "group"         String
                     |     { Rule }*

      Rule           =     "rule" String
                           { RuleDefinition }*
                           "end"

      RuleDefinition =     "style"         String
                     |     "pattern"       String
                     |     "span"          String
                     |     "open"          String
                     |     "close"         String
                     |     "stop"          String
                     |     { Rule }*

      YesNo          =     "yes" | "no"

      String         =     '"' Characters '"'

      Characters     =     ([^"]|\\")*

      Number         =     [0-9]+

*/

/*******************************************************************************/

// Rule types
enum {
  RULETYPE_SIMPLE,
  RULETYPE_DEFAULT,
  RULETYPE_BRACKET,
  RULETYPE_SAFE_BRACKET,
  RULETYPE_SPAN
  };


// Initialize parser
SyntaxParser::SyntaxParser(const FXchar* pat,const FXchar* frm):from(frm),head(pat),tail(pat),token(TK_END),line(1){
  FXTRACE((10,"SyntaxParser::SyntaxParser(%p,%p)\n",pat,frm));
  }


// Obtain next token from input
FXuint SyntaxParser::gettok(){
  FXuint tok;
  while(*tail){
    head=tail;
    if(*tail=='\r'){                    // Carriage return
      tail++;
      if(*tail=='\n') tail++;
      line++;
      continue;
      }
    if(*tail=='\n'){                    // New line
      tail++;
      line++;
      continue;
      }
    if(Ascii::isSpace(*tail)){          // Space
      tail++;
      continue;
      }
    if(*tail=='#'){                     // Comment
      tail++;
      while(*tail && *tail!='\n' && *tail!='\r') tail++;
      continue;
      }
    if(*tail=='"'){                     // String
      tail++;
      while(*tail && *tail!='\n' && *tail!='\r' && *tail!='"'){
        if(*tail=='\\' && *(tail+1)=='"') tail++;
        tail++;
        }
      if(*tail=='"'){
        tail++;
        return TK_STRING;
        }
      return TK_ERROR;                  // Error
      }
    if(Ascii::isDigit(*tail)){          // Number
      tail++;
      while(Ascii::isDigit(*tail)) tail++;
      return TK_INTEGER;
      }
    if(Ascii::isLetter(*tail)){         // Keyword
      tok=(FXuchar)*tail++;
      while(Ascii::isWord(*tail)){
        tok=((tok<<5)+tok) ^ (FXuchar)*tail++;
        }
      FXTRACE((20,"hash(%s) = %u\n",FXString(head,tail-head).text(),tok));
      return tok;
      }
    return TK_ERROR;                    // Error
    }
  head=tail;
  return TK_EOF;                        // End of input
  }


// Check syntax
static FXRex::Error checkRexSyntax(const FXString& text){
  FXRex rex;
  return rex.parse(text,FXRex::Syntax);
  }



// Parse rule and sub rules
FXbool SyntaxParser::parseRule(Syntax *syntax,FXint parent){
  FXString name,style,pat,openpat,clospat,stoppat;
  FXint ruletype=RULETYPE_DEFAULT;
  FXRex::Error error;
  FXint index=-1;
  if(token==TK_RULE){
    token=gettok();

    // Rule name
    if(token!=TK_STRING){
      fxwarning("%s:%d: error: expected 'rule' <name>.\n",from,line);
      return false;
      }

    // Snarf name from parse buffer
    name.assign(head+1,tail-head-2);
    token=gettok();

    // Parse various features
    while(1){
      switch(token){
      case TK_PATTERN:                  // Simple pattern
        token=gettok();
        if(token!=TK_STRING){
          fxwarning("%s:%d: error: expected 'pattern' <regex>.\n",from,line);
          return false;
          }
        pat.assign(head+1,tail-head-2);
        pat.substitute("\\\"","\"",true);
        if((error=checkRexSyntax(pat))!=FXRex::ErrOK){
          fxwarning("%s:%d: bad pattern: %s.\n",from,line,FXRex::getError(error));
          return false;
          }
        token=gettok();
        ruletype=RULETYPE_SIMPLE;
        continue;
      case TK_OPEN:                     // Open pattern
        token=gettok();
        if(token!=TK_STRING){
          fxwarning("%s:%d: error: expected 'open' <regex>.\n",from,line);
          return false;
          }
        openpat.assign(head+1,tail-head-2);
        openpat.substitute("\\\"","\"",true);
        if((error=checkRexSyntax(openpat))!=FXRex::ErrOK){
          fxwarning("%s:%d: bad 'open' pattern: %s.\n",from,line,FXRex::getError(error));
          return false;
          }
        token=gettok();
        ruletype=RULETYPE_BRACKET;
        continue;
      case TK_CLOSE:                    // Close pattern
        token=gettok();
        if(token!=TK_STRING){
          fxwarning("%s:%d: error: expected 'close' <regex>.\n",from,line);
          return false;
          }
        clospat.assign(head+1,tail-head-2);
        clospat.substitute("\\\"","\"",true);
        if((error=checkRexSyntax(clospat))!=FXRex::ErrOK){
          fxwarning("%s:%d: bad 'close' pattern: %s.\n",from,line,FXRex::getError(error));
          return false;
          }
        token=gettok();
        ruletype=RULETYPE_BRACKET;
        continue;
      case TK_STOP:                     // Stop pattern
        token=gettok();
        if(token!=TK_STRING){
          fxwarning("%s:%d: error: expected 'stop' <regex>.\n",from,line);
          return false;
          }
        stoppat.assign(head+1,tail-head-2);
        stoppat.substitute("\\\"","\"",true);
        if((error=checkRexSyntax(stoppat))!=FXRex::ErrOK){
          fxwarning("%s:%d: bad 'stop' pattern: %s.\n",from,line,FXRex::getError(error));
          return false;
          }
        token=gettok();
        ruletype=RULETYPE_SAFE_BRACKET;
        continue;
      case TK_SPAN:                     // Span pattern
        token=gettok();
        if(token!=TK_STRING){
          fxwarning("%s:%d: error: expected 'span' <regex>.\n",from,line);
          return false;
          }
        pat.assign(head+1,tail-head-2);
        pat.substitute("\\\"","\"",true);
        if((error=checkRexSyntax(pat))!=FXRex::ErrOK){
          fxwarning("%s:%d: bad span pattern: %s.\n",from,line,FXRex::getError(error));
          return false;
          }
        token=gettok();
        ruletype=RULETYPE_SPAN;
        continue;
      case TK_STYLE:                    // Default style
        token=gettok();
        if(token!=TK_STRING){
          fxwarning("%s:%d: error: expected 'style' colors.\n",from,line);
          return false;
          }
        style.assign(head+1,tail-head-2);
        style.substitute("\\\"","\"",true);
        token=gettok();
        continue;
        }
      break;
      }

    // Create rules
    switch(ruletype){
    case RULETYPE_SIMPLE:
      index=syntax->appendSimple(name,style,pat,parent);
      break;
    case RULETYPE_BRACKET:
      if(openpat.empty() || clospat.empty()){
        fxwarning("%s:%d: error: both 'openpattern' and 'closepattern' must be specified.\n",from,line);
        return false;
        }
      index=syntax->appendBracket(name,style,openpat,clospat,parent);
      break;
    case RULETYPE_SAFE_BRACKET:
      if(openpat.empty() || clospat.empty()){
        fxwarning("%s:%d: error: both 'openpattern', 'closepattern', and 'stoppattern' must be specified.\n",from,line);
        return false;
        }
      index=syntax->appendSafeBracket(name,style,openpat,clospat,stoppat,parent);
      break;
    case RULETYPE_SPAN:
      index=syntax->appendSpan(name,style,pat,parent);
      break;
    default:
      index=syntax->appendDefault(name,style,parent);
      break;
      }

    // Parse subrules
    if(RULETYPE_SIMPLE<ruletype){
      while(token==TK_RULE){
        if(!parseRule(syntax,index)) return false;
        }
      }

    // Check end
    if(token!=TK_END){
      fxwarning("%s:%d: error: expected 'end'.\n",from,line);
      return false;
      }
    token=gettok();
    return true;
    }
  return false;
  }


// Parse language
FXbool SyntaxParser::parseLanguage(SyntaxList& syntaxes){
  FXString delimiters=FXText::textDelimiters;
  FXString contentsmatch;
  FXString filesmatch;
  FXString name;
  FXString group;
  FXint contextlines=0;
  FXint contextchars=0;
  FXint autoindent=-1;
  FXint wrapwidth=-1;
  FXint tabwidth=-1;
  FXint wrapmode=-1;
  FXint tabmode=-1;
  FXRex::Error error;
  Syntax *syntax;

  // Expect language
  if(token==TK_LANGUAGE){
    token=gettok();

    // Language name
    if(token!=TK_STRING){
      fxwarning("%s:%d: error: expected 'language' <name>.\n",from,line);
      return false;
      }

    // Snarf language name from parse buffer
    name.assign(head+1,tail-head-2);
    token=gettok();

    // Default group name is same as language name
    group=name;

    // Parse various features
    while(1){
      switch(token){
        case TK_FILESMATCH:             // File extensions
          token=gettok();
          if(token!=TK_STRING){
            fxwarning("%s:%d: error: expected 'filesmatch' <wildcard>.\n",from,line);
            return false;
            }
          filesmatch.assign(head+1,tail-head-2);
          filesmatch.substitute("\\\"","\"",true);
          token=gettok();
          continue;
        case TK_CONTENTSMATCH:          // File contents
          token=gettok();
          if(token!=TK_STRING){
            fxwarning("%s:%d: error: expected 'contentsmatch' <regex>.\n",from,line);
            return false;
            }
          contentsmatch.assign(head+1,tail-head-2);
          contentsmatch.substitute("\\\"","\"",true);
          if((error=checkRexSyntax(contentsmatch))!=FXRex::ErrOK){
            fxwarning("%s:%d: bad pattern in contentsmatch: %s.\n",from,line,FXRex::getError(error));
            return false;
            }
          token=gettok();
          continue;
        case TK_DELIMITERS:             // Word delimiters
          token=gettok();
          if(token!=TK_STRING){
            fxwarning("%s:%d: error: expected 'delimiters' <characters>.\n",from,line);
            return false;
            }
          delimiters.assign(head+1,tail-head-2);
          delimiters.substitute("\\\"","\"",true);
          token=gettok();
          continue;
        case TK_CONTEXTLINES:           // Context lines
          token=gettok();
          if(token!=TK_INTEGER){
            fxwarning("%s:%d: error: expected 'contextlines' <number>.\n",from,line);
            return false;
            }
          contextlines=strtol(head,nullptr,0);
          token=gettok();
          continue;
        case TK_CONTEXTCHARS:           // Context chars
          token=gettok();
          if(token!=TK_INTEGER){
            fxwarning("%s:%d: error: expected 'contextchars' <number>.\n",from,line);
            return false;
            }
          contextchars=strtol(head,nullptr,0);
          token=gettok();
          continue;
        case TK_AUTOINDENT:             // Set autoindent mode (0,1)
          token=gettok();
          if(token!=TK_YES && token!=TK_NO){
            fxwarning("%s:%d: error: expected 'autoindent' to be followed by 'yes' or 'no'.\n",from,line);
            return false;
            }
          autoindent=(token==TK_YES);
          token=gettok();
          continue;
        case TK_WRAPWIDTH:              // Set wrap width
          token=gettok();
          if(token!=TK_INTEGER){
            fxwarning("%s:%d: error: expected 'wrapwidth' <number>.\n",from,line);
            return false;
            }
          wrapwidth=strtol(head,nullptr,0);
          token=gettok();
          continue;
        case TK_TABWIDTH:               // Set tab width for language
          token=gettok();
          if(token!=TK_INTEGER){
            fxwarning("%s:%d: error: expected 'tabwidth' <number>.\n",from,line);
            return false;
            }
          tabwidth=strtol(head,nullptr,0);
          token=gettok();
          continue;
        case TK_WORDWRAP:               // Word wrap mode
          token=gettok();
          if(token!=TK_YES && token!=TK_NO){
            fxwarning("%s:%d: error: expected 'wordwrap' to be followed by 'yes' or 'no'.\n",from,line);
            return false;
            }
          wrapmode=(token==TK_YES);
          token=gettok();
          continue;
        case TK_EXPANDTABS:             // Tab expands to characters
          token=gettok();
          if(token!=TK_YES && token!=TK_NO){
            fxwarning("%s:%d: error: expected 'expandtabs' to be followed by 'yes' or 'no'.\n",from,line);
            return false;
            }
          tabmode=(token==TK_YES);
          token=gettok();
          continue;
        case TK_GROUP:                  // Style group
          token=gettok();
          if(token!=TK_STRING){
            fxwarning("%s:%d: error: expected 'group' <wildcard>.\n",from,line);
            return false;
            }
          group.assign(head+1,tail-head-2);
          group.substitute("\\\"","\"",true);
          token=gettok();
          continue;
        }
      break;
      }

    // Create language
    syntax=new Syntax(name,group);
    syntax->setExtensions(filesmatch);
    syntax->setContents(contentsmatch);
    syntax->setDelimiters(delimiters);
    syntax->setContextLines(contextlines);
    syntax->setContextChars(contextchars);
    syntax->setAutoIndent(autoindent);
    syntax->setWrapWidth(wrapwidth);
    syntax->setTabWidth(tabwidth);
    syntax->setWrapMode(wrapmode);
    syntax->setTabMode(tabmode);

    FXTRACE((11,"name=%s\n",name.text()));
    FXTRACE((11,"group=%s\n",group.text()));
    FXTRACE((11,"filesmatch=%s\n",filesmatch.text()));
    FXTRACE((11,"contentsmatch=%s\n",contentsmatch.text()));
    FXTRACE((11,"delimiters=%s\n",delimiters.text()));
    FXTRACE((11,"contextlines=%d\n",contextlines));
    FXTRACE((11,"contextchars=%d\n",contextchars));
    FXTRACE((11,"autoindent=%d\n",autoindent));
    FXTRACE((11,"wrapwidth=%d\n",wrapwidth));
    FXTRACE((11,"tabwidth=%d\n",tabwidth));
    FXTRACE((11,"wrapmode=%d\n",wrapmode));
    FXTRACE((11,"tabmode=%d\n",tabmode));

    // Add new syntax to list
    syntaxes.append(syntax);

    // Parse rules
    while(token==TK_RULE){
      if(!parseRule(syntax,0)) return false;
      }

    // Check end
    if(token!=TK_END){
      fxwarning("%s:%d: error: expected 'end'.\n",from,line);
      return false;
      }
    token=gettok();
    FXTRACE((11,"\n\n"));
    return true;
    }
  return false;
  }


// Parse file
FXbool SyntaxParser::parse(SyntaxList& syntaxes){
  token=gettok();
  while(token==TK_LANGUAGE){
    if(!parseLanguage(syntaxes)) return false;
    }
  return true;
  }


// Parse string and return syntaxes found in it; return false if problem.
FXbool SyntaxParser::parse(SyntaxList& syntaxes,const FXchar* patterns){
  SyntaxParser parser(patterns);
  return parser.parse(syntaxes);
  }


// Parse string and return syntaxes found in it; return false if problem.
FXbool SyntaxParser::parse(SyntaxList& syntaxes,const FXString& patterns){
  return parse(syntaxes,patterns.text());
  }


// Parse file and return syntaxes found in it; return false if problem.
FXbool SyntaxParser::parseFile(SyntaxList& syntaxes,const FXString& filename){
  FXFile file(filename,FXIO::Reading);
  FXTRACE((11,"SyntaxParser::parseFile(%s)\n",filename.text()));
  if(file.isOpen()){
    FXString patterns;
    patterns.length(file.size());
    if(file.readBlock(patterns.text(),patterns.length())==patterns.length()){
      SyntaxParser parser(patterns.text(),filename.text());
      return parser.parse(syntaxes);
      }
    }
  return false;
  }


// Clean up
SyntaxParser::~SyntaxParser(){
  FXTRACE((10,"SyntaxParser::~SyntaxParser()\n"));
  }

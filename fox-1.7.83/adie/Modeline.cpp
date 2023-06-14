/********************************************************************************
*                                                                               *
*                          M o d e l i n e   P a r s e r                        *
*                                                                               *
*********************************************************************************
* Copyright (C) 2017,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#include "Modeline.h"

/*
  Notes:

  - Parse commonly used editor modeline from vim, emacs, or adie editor
    syntax embedded in comments.
  - Vim modelines there are two forms of modelines.  The first form:

        [text]{white}{vi:|vim:|ex:}[white]{options1}

    The second form compatible with some versions of vi:

        [text]{white}{vi:|vim:|ex:}[white]se[t] {options2}:[text]

    Where:

        [text]          any text or empty
        {white}         at least one blank character (<Space> or <Tab>)
        {vi:|vim:|ex:}  the string "vi:", "vim:" or "ex:"
        [white]         optional white space
        {options1}      a list of option settings, separated with white space or ':',
                        where each part between ':' is the argument for a ":set"
                        command (can be empty)
        {options2}      a list of options, separated with white space, which is the
                        argument for a ":set" command
        se[t]           the string "set " or "se " (note the space)

    Example of the 1st:

        vi:noai:sw=3 ts=6

    Example of the 2nd:

        vim: set ai tw=75:

    More details can be found here:

        http://vimdoc.sourceforge.net/htmldoc/options.html#modeline

  - Emacs modeline; Emacs modelines looks like this:

        "-*- key1: value1; key2: value2; -*-"

    See:

        http://www.delorie.com/gnu/docs/emacs/emacs_486.html

  - Adie editor modelines. Adie modelines are of the form:

        "adie: key1=value1 key2=value2 key3=value3 :"

    Adie understands the following keys:

        la      Language. Value is string identifying programming language.
        ts      Tab stops. Must be numeric value greater than 1.
        ww      Wrap width.  Must be numeric value greater than 1.
        et      Expand tabs to spaces.  Possible values 0,1.
        wr      Wrap mode. Possible values 0,1
        ai      Autoindent.  Possible values: 0,1.

  - Modelines, when present, will override user-settings and language-dependent
    settings if Modeline support is enabled and modelines are detected in designated
    areas (first few lines, or last few lines) of the text buffer.

  - FIXME coordinate some of this new stuff with setting same variable from
    the syntax file.  For example, if file is called "Makefile" we want the
    hard-tabs to be set ON and the tabstops set to 8.
*/

/*******************************************************************************/


// Initialize modeline
Modeline::Modeline():autoindent(-1),wrapwidth(-1),tabwidth(-1),wrapmode(-1),tabmode(-1){
  }


// Parse VIM modeline
FXbool Modeline::parseVimModeline(const FXchar* s){
  FXbool set=false;
  FXbool yes=true;
  FXString key;
  FXString val;
  while(*s!='\0' && !(*s==':' && set)){
    while(*s==':' || *s=='\t' || *s==' ') s++;
    if(*s=='\0') break;
    if(*s=='s' && *(s+1)=='e' && *(s+1)==' '){ s+=3; set=true; }
    else if(*s=='s' && *(s+1)=='e' && *(s+2)=='t' && *(s+3)==' '){ s+=4; set=true; }
    yes=true;
    if(*s=='n' && *(s+1)=='o'){ s+=2; yes=false; }
    key=FXString::null;
    val=FXString::null;
    while(*s!='\0' && *s!=':' && *s!='=' && *s!='\t' && *s!=' '){
      key+=*s++;
      }
    if(*s=='='){
      s++;
      while(*s!='\0' && *s!=':' && *s!='\t' && *s!=' '){
        val+=*s++;
        }
      }
    if(key=="ft" || key=="filetype"){
      setLanguage(val);
      }
    else if(key=="et" || key=="expandtab"){
      setTabMode(yes);
      }
    else if(key=="wrap"){
      setWrapMode(yes);
      }
    else if(key=="ai"){
      setAutoIndent(yes);
      }
    else if(key=="ts" || key=="tabstop"){
      setTabWidth(val.toInt());
      }
    else if(key=="textwidth"){
      setWrapWidth(val.toInt());
      }
    }
  return true;
  }


// Parse emacs modeline
FXbool Modeline::parseEmacsModeline(const FXchar* s){
  FXString key;
  FXString val;
  while(*s!='\0'){
    while(*s==';' || *s=='\t' || *s==' ') s++;
    if(*s=='\0' || (*s=='-' && *(s+1)=='*' && *(s+2)=='-')) break;
    key=FXString::null;
    val=FXString::null;
    while(*s!='\0' && *s!=':' && *s!=';' && *s!='\t' && *s!=' '){
      key+=*s++;
      }
    while(*s=='\t' || *s==' ') s++;
    if(*s=='\0') break;
    if(*s!=':') continue;
    s++;
    while(*s=='\t' || *s==' ') s++;
    if(*s=='\0') break;
    while(*s!='\0' && *s!=';' &&  *s!='\t' && *s!=' '){
      val+=*s++;
      }
    if(FXString::comparecase(key,"mode")==0){
      setLanguage(val);
      }
    else if(key=="tab-width"){
      setTabWidth(val.toInt());
      }
    else if(key=="indent-tabs-mode"){
      setTabMode(val=="nil");
      }
    else if(key=="autowrap"){
      setWrapMode(val!="nil");
      }
    }
  return true;
  }


// Parse Adie modelines
FXbool Modeline::parseAdieModeline(const FXchar* s){
  FXString key;
  FXString val;
  while(*s!='\0' && *s!=':'){
    key=FXString::null;
    val=FXString::null;
    while(*s=='\t' || *s==' ') s++;
    if(*s=='\0') break;
    while(*s!='\0' && *s!=':' && *s!='=' && *s!='\t' && *s!=' '){
      key+=*s++;
      }
    while(*s=='\t' || *s==' ') s++;
    if(*s=='\0') break;
    if(*s!='=') continue;
    s++;
    while(*s=='\t' || *s==' ') s++;
    if(*s=='\0') break;
    while(*s!='\0' && *s!=':' && *s!='\t' && *s!=' '){
      val+=*s++;
      }
    if(key=="la"){
      setLanguage(val);
      }
    else if(key=="ts"){
      setTabWidth(val.toInt());
      }
    else if(key=="ww"){
      setWrapWidth(val.toInt());
      }
    else if(key=="et"){
      setTabMode(val!="0");
      }
    else if(key=="wr"){
      setWrapMode(val!="0");
      }
    else if(key=="ai"){
      setAutoIndent(val!="0");
      }
    }
  return true;
  }


// Parse mode line from character string
FXbool Modeline::parseModeline(const FXchar* s){
  FXchar old=' ';
  while(*s!='\0'){
    if(old==' ' || old=='\t'){

      // Adie modeline
      if(*s=='a' && *(s+1)=='d' && *(s+2)=='i' && *(s+3)=='e' && *(s+4)==':'){
        return parseAdieModeline(s+5);
        }

      // Emacs modeline
      if(*s=='-' && *(s+1)=='*' && *(s+2)=='-'){
        return parseEmacsModeline(s+3);
        }

      // Ex modeline
      if(*s=='e' && *(s+1)=='x' && *(s+2)==':'){
        return parseVimModeline(s+3);
        }

      // Vi modeline
      if(*s=='v' && *(s+1)=='i' && *(s+2)==':'){
        return parseVimModeline(s+3);
        }

      // Vim modeline
      if(*s=='v' && *(s+1)=='i' && *(s+2)=='m' && *(s+3)==':'){
        return parseVimModeline(s+4);
        }
      }
    old=*s++;
    }
  return false;
  }


// Parse mode line from string
FXbool Modeline::parseModeline(const FXString& s){
  return parseModeline(s.text());
  }


// Clear for another call
void Modeline::clear(){
  language.clear();
  autoindent=-1;
  wrapwidth=-1;
  tabwidth=-1;
  wrapmode=-1;
  tabmode=-1;
  }

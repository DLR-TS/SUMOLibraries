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
#ifndef MODELINE_H
#define MODELINE_H


// Parse modelines to set various editor attrubutes.
// Modelines are typically embedded in the first or last
// few lines of the document (inside comments so as to not
// interfere with compilers or interpreters).
class Modeline {
private:
  FXString language;    // Language
  FXint    autoindent;  // Auto indent
  FXint    wrapwidth;   // Wrap width
  FXint    tabwidth;    // Tab stop columns
  FXint    wrapmode;    // Wrap lines
  FXint    tabmode;     // Tabs expanded into spaces
private:
  FXbool parseVimModeline(const FXchar* s);
  FXbool parseEmacsModeline(const FXchar* s);
  FXbool parseAdieModeline(const FXchar* s);
public:

  // Initialize
  Modeline();

  // Parse mode line from character string
  FXbool parseModeline(const FXchar* s);

  // Parse mode line from string
  FXbool parseModeline(const FXString& s);

  // Access language name
  void setLanguage(const FXString& n){ language=n; }
  const FXString& getLanguage() const { return language; }

  // Access auto-indent
  void setAutoIndent(FXint a){ autoindent=a; }
  FXint getAutoIndent() const { return autoindent; }

  // Access wrap width
  void setWrapWidth(FXint w){ wrapwidth=w; }
  FXint getWrapWidth() const { return wrapwidth; }

  // Access tab width
  void setTabWidth(FXint w){ tabwidth=w; }
  FXint getTabWidth() const { return tabwidth; }

  // Access line wrap mode
  void setWrapMode(FXint m){ wrapmode=m; }
  FXint getWrapMode() const { return wrapmode; }

  // Access tab expand mode
  void setTabMode(FXint m){ tabmode=m; }
  FXint getTabMode() const { return tabmode; }

  // Clear for another call
  void clear();
  };


#endif

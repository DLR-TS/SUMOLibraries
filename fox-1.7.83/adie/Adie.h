/********************************************************************************
*                                                                               *
*                     T h e   A d i e   T e x t   E d i t o r                   *
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
#ifndef ADIE_H
#define ADIE_H


// Version
#define VERSION_MAJOR 3
#define VERSION_MINOR 6
#define VERSION_PATCH 0


class HelpWindow;
class Preferences;
class TextWindow;
class FindInFiles;
struct ParseInfo;


// Main Application class
class Adie : public FXApp {
  FXDECLARE(Adie)
  friend class TextWindow;
  friend class FindInFiles;
protected:
  TextWindowList      windowlist;               // Window list
  FXFileAssociations *associations;             // File association table
  SyntaxList          syntaxes;                 // List of syntax patterns
  FXString            syntaxpaths;              // Where to look for syntax file
  FXIcon             *bigicon;                  // Big application icon
  FXIcon             *smallicon;                // Small application icon
  FXIcon             *newicon;
  FXIcon             *newfileicon;
  FXIcon             *reloadicon;
  FXIcon             *openicon;
  FXIcon             *closeicon;
  FXIcon             *saveicon;
  FXIcon             *saveasicon;
  FXIcon             *savetoicon;
  FXIcon             *printicon;
  FXIcon             *importicon;
  FXIcon             *exporticon;
  FXIcon             *cuticon;
  FXIcon             *copyicon;
  FXIcon             *pasteicon;
  FXIcon             *deleteicon;
  FXIcon             *undoicon;
  FXIcon             *redoicon;
  FXIcon             *fontsicon;
  FXIcon             *helpicon;
  FXIcon             *quiticon;
  FXIcon             *searchicon;
  FXIcon             *replaceicon;
  FXIcon             *expressicon;
  FXIcon             *searchfilesicon;
  FXIcon             *searchnexticon;
  FXIcon             *searchprevicon;
  FXIcon             *searchrexicon;
  FXIcon             *searchnorexicon;
  FXIcon             *searchcaseicon;
  FXIcon             *searchnocaseicon;
  FXIcon             *searchwordicon;
  FXIcon             *searchnowordicon;
  FXIcon             *searchupicon;
  FXIcon             *searchdnicon;
  FXIcon             *bookseticon;
  FXIcon             *booknexticon;
  FXIcon             *bookprevicon;
  FXIcon             *bookdelicon;
  FXIcon             *bookclricon;
  FXIcon             *shiftlefticon;
  FXIcon             *shiftrighticon;
  FXIcon             *configicon;
  FXIcon             *browsericon;
  FXIcon             *nobrowsericon;
  FXIcon             *loggericon;
  FXIcon             *nologgericon;
  FXIcon             *uppercaseicon;
  FXIcon             *lowercaseicon;
  FXIcon             *backwardicon;
  FXIcon             *forwardicon;
  FXIcon             *shownicon;
  FXIcon             *hiddenicon;
  FXIcon             *wordwrapicon;
  FXIcon             *nowrapicon;
  FXIcon             *filtericon;
  FXIcon             *commandicon;
  FXIcon             *scripticon;
  FXIcon             *switchicon;
  FXIcon             *openselicon;
  FXIcon             *docsicon;
  FXIcon             *pointicon;
private:
  Adie(){}
  Adie(const Adie&);
  Adie& operator=(const Adie&);
public:
  enum{
    ID_CLOSEALL=FXApp::ID_LAST,
    ID_SYNTAXPATHS,
    ID_HARVEST,
    ID_LAST
    };
public:
  long onSigHarvest(FXObject*,FXSelector,void*);
  long onCmdCloseAll(FXObject*,FXSelector,void*);
  long onCmdSyntaxPaths(FXObject*,FXSelector,void*);
  long onUpdSyntaxPaths(FXObject*,FXSelector,void*);
public:

  // Construct application object
  Adie(const FXString& name);

  // Run the application
  FXint start(int argc,char** argv);

  // Get number of windows
  FXint numWindows() const { return windowlist.no(); }

  // Get text window i
  TextWindow* getWindow(FXint i) const { return windowlist[i]; }

  // Append window
  void appendWindow(TextWindow* w){ windowlist.append(w); }

  // Remove window
  void removeWindow(TextWindow* w){ windowlist.remove(w); }

  // Find an as yet untitled, unedited window
  TextWindow* findUnused() const;

  // Find window, if any, currently editing the given file
  TextWindow* findWindow(const FXString& file) const;

  // Open window on file, creating new one if not already open
  TextWindow* openFileWindow(const FXString& file,FXbool edit=true);

  // Get file associations
  FXFileAssociations* getFileAssociations() const { return associations; }

  // Get number of syntaxes
  FXint numSyntaxes() const { return syntaxes.no(); }

  // Get syntax i
  Syntax* getSyntax(FXint i) const { return syntaxes[i]; }

  // Get syntax for language name
  Syntax* getSyntaxByName(const FXString& lang);

  // Get syntax by consulting registry
  Syntax* getSyntaxByRegistry(const FXString& file);

  // Get syntax by matching pattern
  Syntax* getSyntaxByPattern(const FXString& file);

  // Get syntax by contents
  Syntax* getSyntaxByContents(const FXString& contents);

  // Generate unique name from given path
  FXString unique(const FXString& path) const;

  // Set syntax paths
  void setSyntaxPaths(const FXString& paths){ syntaxpaths=paths; }

  // Get syntax paths
  const FXString& getSyntaxPaths() const { return syntaxpaths; }

  // Delete application object
  virtual ~Adie();
  };

#endif


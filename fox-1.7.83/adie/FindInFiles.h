/********************************************************************************
*                                                                               *
*                    F i n d   P a t t e r n   I n   F i l e s                  *
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
#ifndef FINDINFILES_H
#define FINDINFILES_H


class Adie;
class FindInFiles;


/// Directory search visitor
class SearchVisitor : public FXGlobVisitor {
private:
  FindInFiles* dlg;     // Find dialog
  FXRex        rex;     // Regex parser
  FXlong       limit;   // File size limit
private:
  FXlong loadFile(const FXString& path,FXString& text) const;
  FXint searchFile(const FXString& path) const;
private:
  SearchVisitor();
  SearchVisitor(const SearchVisitor&);
  SearchVisitor& operator=(const SearchVisitor&);
public:

  // Construct search file for pattern visitor
  SearchVisitor(FindInFiles* dg):dlg(dg),limit(100000000L){ }

  // Traverse files under path and search for pattern in each
  FXuint traverse(const FXString& path,const FXString& pattern,const FXString& wild="*",FXint mode=FXRex::Normal,FXuint opts=FXDir::MatchAll,FXint depth=1000);

  // Set file size limit
  void setLimit(FXlong size);

  // Get file size limit
  FXlong getLimit() const { return limit; }

  // Visit given file and scan it
  virtual FXuint visit(const FXString& path);
  };


/// Find patterns in Files
class FindInFiles : public FXDialogBox {
  FXDECLARE(FindInFiles)
protected:
  SearchVisitor   visitor;              // Search visitor
  FXIconList     *locations;            // Search hits
  FXTextField    *findstring;           // String to search for
  FXTextField    *filefolder;           // Folder to search
  FXComboBox     *filefilter;           // File filters
  FXToggleButton *pausebutton;          // Pause button
  FXLabel        *searching;            // Show file being scanned
  FXString        filePattern;          // Search files matching pattern
  FXString        searchHistory[20];    // Search string history
  FXString        folderHistory[20];    // Search folder history
  FXuint          patternHistory[20];   // Search wildcard history
  FXuint          optionsHistory[20];   // Search option history
  FXuint          searchmode;           // Search options
  FXString        savedsearchtext;      // Saved search text
  FXString        savedsearchfolder;    // Saved search folder
  FXuint          savedsearchmode;      // Saved search mode
  FXuint          savedcurrentpattern;  // Saved search pattern
  FXint           index;                // History index
  FXuint          proceed;              // Flag
  FXbool          firsthit;             // Record only first hit in file
protected:
  FindInFiles();
private:
  FindInFiles(const FindInFiles&);
  FindInFiles& operator=(const FindInFiles&);
private:
  void readRegistry();
  void writeRegistry();
  void appendHistory(const FXString& text,const FXString& dir,FXuint patt,FXuint opts);
public:
  long onUpdStop(FXObject*,FXSelector,void*);
  long onCmdStop(FXObject*,FXSelector,void*);
  long onUpdPause(FXObject*,FXSelector,void*);
  long onCmdPause(FXObject*,FXSelector,void*);
  long onUpdSearch(FXObject*,FXSelector,void*);
  long onCmdSearch(FXObject*,FXSelector,void*);
  long onUpdDelete(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
  long onCmdFilter(FXObject*,FXSelector,void*);
  long onUpdFirstHit(FXObject*,FXSelector,void*);
  long onCmdFirstHit(FXObject*,FXSelector,void*);
  long onUpdFlags(FXObject*,FXSelector,void*);
  long onCmdFlags(FXObject*,FXSelector,void*);
  long onCmdFolder(FXObject*,FXSelector,void*);
  long onUpdHistoryUp(FXObject*,FXSelector,void*);
  long onUpdHistoryDn(FXObject*,FXSelector,void*);
  long onCmdHistoryUp(FXObject*,FXSelector,void*);
  long onCmdHistoryDn(FXObject*,FXSelector,void*);
  long onArrowKey(FXObject*,FXSelector,void*);
  long onMouseWheel(FXObject*,FXSelector,void*);
  long onCmdFileDblClicked(FXObject*,FXSelector,void*);
public:
  enum {
    SearchExact    = 0,         /// Search exact matches
    SearchCaseFold = 1,         /// Search with case folding
    SearchRegex    = 2,         /// Search regular expression
    SearchWords    = 4,         /// Search whole words
    SearchRecurse  = 8,         /// Search files recursively
    SeachHidden    = 16         /// Search hidden files also
    };
public:
  enum{
    ID_SEARCH=FXDialogBox::ID_LAST,
    ID_SEARCH_TEXT,
    ID_FOLDER_TEXT,
    ID_FILTER_TEXT,
    ID_FOLDER,
    ID_HIST_UP,
    ID_HIST_DN,
    ID_ICASE,
    ID_REGEX,
    ID_WORDS,
    ID_RECURSIVE,
    ID_HIDDEN,
    ID_FIRST_HIT,
    ID_FILELIST,
    ID_PAUSE,
    ID_STOP,
    ID_DELETE,
    ID_LAST
    };
public:

  /// Create find-in-files widget
  FindInFiles(Adie *a);

  /// Return Adie application
  Adie* getApp() const { return (Adie*)FXDialogBox::getApp(); }

  /// Create server-side resources
  virtual void create();

  /// Close the window, return true if actually closed
  virtual FXbool close(FXbool notify=false);

  /// Called by visitor to see if we should continue processing
  FXbool continueProcessing();

  /// Called by visitor to deposit new search result
  void appendSearchResult(const FXString& relpath,const FXString& text,FXint lineno,FXint column);

  /// Clear search results
  void clearSearchResults();

  /// Change directory
  void setDirectory(const FXString& path);

  /// Return directory
  FXString getDirectory() const;

  /// Set text or pattern to search for
  void setSearchText(const FXString& text);

  /// Return text or pattern the user has entered
  FXString getSearchText() const;

  /// Set search match mode
  void setSearchMode(FXuint mode){ searchmode=mode; }

  /// Return search mode the user has selected
  FXuint getSearchMode() const { return searchmode; }

  /// Set first hit only flag
  void setFirstHit(FXbool flag){ firsthit=flag; }

  /// Return first hit only flag
  FXbool getFirstHit() const { return firsthit; }

  /// Change file pattern
  void setPattern(const FXString& ptrn);

  /// Return file pattern
  const FXString& getPattern() const { return filePattern; }

  /// Set list of patterns
  void setPatternList(const FXString& patterns);

  /// Return list of patterns
  FXString getPatternList() const;

  /// Set currently selected pattern
  void setCurrentPattern(FXint patno);

  /// Return current pattern number
  FXint getCurrentPattern() const;

  /// Change pattern text for pattern number
  void setPatternText(FXint patno,const FXString& text);

  /// Get pattern text for given pattern number
  FXString getPatternText(FXint patno) const;

  /// Return number of patterns
  FXint getNumPatterns() const;

  /// Allow pattern entry
  void allowPatternEntry(FXbool flag);

  /// Return true if pattern entry is allowed
  FXbool allowPatternEntry() const;

  /// Update file being scanned
  void setSearchingText(const FXString& fn);

  /// Read it back
  FXString getSearchingText() const;

  /// Destroy it
  virtual ~FindInFiles();
  };

#endif

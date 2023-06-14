/********************************************************************************
*                                                                               *
*                        P r e f e r e n c e s   D i a l o g                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 2003,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef PREFERENCES_H
#define PREFERENCES_H



class PathFinderMain;


// Preferences for PathFinder
class Preferences : public FXDialogBox {
  FXDECLARE(Preferences)
protected:
  FXText          *pattern;
  FXTextField     *editor;
  FXTextField     *terminal;
  FXTextField     *execpaths;
  FXCheckButton   *preview;
  FXCheckButton   *blending;
  FXCheckButton   *scaling;
  FXTextField     *icondirs;
  FXList          *extensions;
  FXComboBox      *mimetypes;
  FXButton        *iconbutton[4];
  FXIcon          *iconimage[4];
  FXCheckButton   *runinterminal;
  FXCheckButton   *changedirectory;
  FXTextField     *description;
  FXTextField     *command;
  FXString         filedesc;
  FXString         filemime;
  FXString         filecommand;
  FXString         fileicons[4];
  FXuint           fileflags;
  FXCheckButton   *autosize;
  FXTextField     *itemspace;
  FXIcon          *brw;
  FXIcon          *pat;
  FXIcon          *mim;
  FXIcon          *run;
  FXIcon          *dir;
private:
  Preferences(){}
  Preferences(const Preferences&);
  Preferences& operator=(const Preferences&);
public:
  long onCmdAccept(FXObject*,FXSelector,void*);
  long onCmdBrowseIcons(FXObject*,FXSelector,void*);
  long onCmdBrowseEditor(FXObject*,FXSelector,void*);
  long onCmdBrowseTerminal(FXObject*,FXSelector,void*);
  long onCmdBrowsePaths(FXObject*,FXSelector,void*);
  long onCmdBrowseCommand(FXObject*,FXSelector,void*);
  long onCmdBrowseIcon(FXObject*,FXSelector,void*);
  long onCmdCommand(FXObject*,FXSelector,void*);
  long onUpdCommand(FXObject*,FXSelector,void*);
  long onCmdDescription(FXObject*,FXSelector,void*);
  long onUpdDescription(FXObject*,FXSelector,void*);
  long onCmdMimeType(FXObject*,FXSelector,void*);
  long onUpdMimeType(FXObject*,FXSelector,void*);
  long onCmdAppendExtension(FXObject*,FXSelector,void*);
  long onCmdChangeExtension(FXObject*,FXSelector,void*);
  long onCmdRemoveExtension(FXObject*,FXSelector,void*);
  long onCmdSelectExtension(FXObject*,FXSelector,void*);
  long onCmdDeselectExtension(FXObject*,FXSelector,void*);
  long onUpdSelectExtension(FXObject*,FXSelector,void*);
  long onUpdRunInTerminal(FXObject*,FXSelector,void*);
  long onCmdRunInTerminal(FXObject*,FXSelector,void*);
  long onUpdChangeDirectory(FXObject*,FXSelector,void*);
  long onCmdChangeDirectory(FXObject*,FXSelector,void*);
public:
  enum{
    ID_BROWSE_ICONS=FXDialogBox::ID_LAST,
    ID_BROWSE_EDITOR,
    ID_BROWSE_TERMINAL,
    ID_BROWSE_PATHS,
    ID_BROWSE_COMMAND,
    ID_BROWSE_SMALLICON,
    ID_BROWSE_BIGICON,
    ID_BROWSE_SMALLICONOPEN,
    ID_BROWSE_BIGICONOPEN,
    ID_COMMAND,
    ID_MIMETYPE,
    ID_DESCRIPTION,
    ID_SELECT_EXTENSION,
    ID_APPEND_EXTENSION,
    ID_CHANGE_EXTENSION,
    ID_REMOVE_EXTENSION,
    ID_RUN_IN_TERMINAL,
    ID_CHANGE_DIRECTORY
    };
public:

  // Create preferences dialog
  Preferences(PathFinderMain *owner);

  // Populate file bindings and mime types
  void setupFileBindings();

  // Search iconpath for given name and load the icon
  FXIcon *createIconFromName(const FXString& name) const;

  // Change icon on button
  void changeIconButton(const FXString& name,FXint index);

  // Read file extension from registry
  void readFileExtension(const FXString& ext);

  // Save file extension to registry
  void writeFileExtension(const FXString& ext);

  // Get/set filename patterns
  void setPatterns(const FXString& ptrn){ pattern->setText(ptrn); }
  FXString getPatterns() const { return pattern->getText(); }

  // Get/set text editor
  void setEditor(const FXString& edit){ editor->setText(edit); }
  FXString getEditor() const { return editor->getText(); }

  // Get/set terminal
  void setTerminal(const FXString& term){ terminal->setText(term); }
  FXString getTerminal() const { return terminal->getText(); }

  // Get/set exec paths
  void setExecPaths(const FXString& paths){ execpaths->setText(paths); }
  FXString getExecPaths() const { return execpaths->getText(); }

  // Set image preview
  void setPreview(FXbool flag){ preview->setCheck(flag); }
  FXbool getPreview() const { return preview->getCheck(); }

  // Set image preview
  void setPreviewBlend(FXbool blend){ blending->setCheck(blend); }
  FXbool getPreviewBlend() const { return blending->getCheck(); }

  // Set image scale
  void setPreviewScale(FXbool scale){ scaling->setCheck(scale); }
  FXbool getPreviewScale() const { return scaling->getCheck(); }

  // Get/set icon path
  void setIconPath(const FXString& text){ icondirs->setText(text); }
  FXString getIconPath() const { return icondirs->getText(); }

  // Set autosize
  void setAutoSize(FXbool flag){ autosize->setCheck(flag); }
  FXbool getAutoSize() const { return autosize->getCheck(); }

  // Set item space
  void setItemSpace(FXint s);
  FXint getItemSpace() const;

  // Clean up
  virtual ~Preferences();
  };

#endif


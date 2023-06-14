/********************************************************************************
*                                                                               *
*                   FOX Desktop Setup - FOX Desktop Enviroment                  *
*                                                                               *
*********************************************************************************
* Copyright (C) 2004,2023 Sander Jansen.  All Rights Reserved.                  *
*********************************************************************************
* This library is free software; you can redistribute it and/or modify          *
* it under the terms of the GNU Lesser General Public License as published by   *
* the Free Software Foundation; either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU Lesser General Public License for more details.                           *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public License      *
* along with this program.  If not, see <http://www.gnu.org/licenses/>          *
********************************************************************************/
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H


// Color theme info
struct ColorTheme {
  const FXchar* name;
  FXColor       base;
  FXColor       border;
  FXColor       back;
  FXColor       fore;
  FXColor       selback;
  FXColor       selfore;
  FXColor       tipback;
  FXColor       tipfore;
  FXColor       menuback;
  FXColor       menufore;
  };


// Up to 4 icons associated with file
enum {
  BIG_ICON,
  BIG_ICON_OPEN,
  MINI_ICON,
  MINI_ICON_OPEN
  };


// File binding
struct FXFileBinding {
  FXString key;                 // Registry key
  FXString description;         // Description
  FXString command;             // Command to invoke
  FXString iconfile[4];         // Icon file names
  FXString mime;                // Mime type
  FXuint   flags;               // Flags
  };


// Control panel widget
class FXDesktopSetup : public FXMainWindow {
  FXDECLARE(FXDesktopSetup)
private:
  FXSettings         desktopsettings;   // Desktop Settings Registry
  FXFont            *titlefont;
  FXIcon            *desktopicon;
  FXIcon            *icon_colors;
  FXIcon            *icon_settings;
  FXIcon            *icon_filebinding;
private:
  FXListBox         *list;
  FXList            *filebindinglist;
  FXComboBox        *mimetypelist;
  FXButton          *iconbutton[4];
  FXIcon            *iconimage[4];
  FXLabel           *vendorandapplication;
private:
  FXToolTip         *tooltip;
  FXTabBook         *tabbook;
  FXTabItem         *tabitem;
  FXVerticalFrame   *tabframe;
  FXVerticalFrame   *mainframe;
  FXVerticalFrame   *menuframe;
  FXHorizontalFrame *labeltextframe1;
  FXHorizontalFrame *labeltextframe2;
  FXHorizontalFrame *textframe1;
  FXHorizontalFrame *textframe2;
  FXHorizontalFrame *tabsubframe;
  FXGroupBox        *grpbox1;
  FXGroupBox        *grpbox2;
  FXLabel           *label1;
  FXLabel           *label2;
  FXLabel           *label3;
  FXLabel           *label4;
  FXLabel           *label5;
  FXLabel           *menulabels[6];
  FXTextField       *textfield1;
  FXButton          *button1;
  FXButton          *fontbutton;
  FXSeparator       *sep1;
  FXSeparator       *sep2;
  FXSeparator       *sep3;
private:
  ColorTheme         theme_current;     // Current Settings
  ColorTheme         theme_user;        // Theme User may have set, which is different from the other themes
private:
  FXFont            *font;
  FXbool             hascurrent;
  FXString           fontspec;          // Selected font
  FXString           filename;          // Filename of settings file
  FXString           applicationname;   // If editing specific application settings
  FXString           vendorname;        // If editing specific vendor settings
  FXString           iconpath;          // Path where icons are found
  FXTime             typingSpeed;
  FXTime             clickSpeed;
  FXTime             scrollSpeed;
  FXTime             scrollDelay;
  FXTime             blinkSpeed;
  FXTime             animSpeed;
  FXTime             menuPause;
  FXTime             tooltipPause;
  FXTime             tooltipTime;
  FXuint             maxcolors;
  FXint              dragDelta;
  FXint              wheelLines;
  FXint              barSize;
  FXfloat            gamma;
  FXint              subpixel;
  FXint              hintstyle;
  FXbool             hinting;
  FXbool             autohint;
  FXbool             antialias;
  FXFileBinding      filebinding;       // Current file binding
private:
  FXDataTarget       target_base;                // Color targets
  FXDataTarget       target_back;
  FXDataTarget       target_border;
  FXDataTarget       target_fore;
  FXDataTarget       target_selfore;
  FXDataTarget       target_selback;
  FXDataTarget       target_tipfore;
  FXDataTarget       target_tipback;
  FXDataTarget       target_menufore;
  FXDataTarget       target_menuback;
  FXDataTarget       target_typingspeed;        // Time targets
  FXDataTarget       target_clickspeed;
  FXDataTarget       target_scrollspeed;
  FXDataTarget       target_scrolldelay;
  FXDataTarget       target_blinkspeed;
  FXDataTarget       target_animspeed;
  FXDataTarget       target_menupause;
  FXDataTarget       target_tooltippause;
  FXDataTarget       target_tooltiptime;
  FXDataTarget       target_dragdelta;          // Move mouse delta
  FXDataTarget       target_wheellines;         // Mouse wheel lines
  FXDataTarget       target_barsize;            // Scroll bar size
  FXDataTarget       target_maxcolors;          // Maximum colors
  FXDataTarget       target_gamma;              // Display gamma value
  FXDataTarget       target_subpixel;           // Xft font subpixel mode
  FXDataTarget       target_hintstyle;          // Xft font hint style
  FXDataTarget       target_hinting;            // Xft font hinting
  FXDataTarget       target_autohint;           // Xft autohint
  FXDataTarget       target_antialias;          // Xft anti-aliasing
  FXDataTarget       target_filebinding_description;
  FXDataTarget       target_filebinding_command;
  FXDataTarget       target_iconpath;
private:
  void setupFont();
  void initColors();
  void setupColors();
  void saveFileBinding();
  void setupFileBindings();
  void setupVendorAndAppLabel();
  void setupIconButton(const FXString& name,FXint index);
  FXIcon *createIconFromName(const FXString& name) const;
private:
  FXDesktopSetup(){}
  FXDesktopSetup(const FXDesktopSetup&);
  FXDesktopSetup& operator=(const FXDesktopSetup&);
public:
  enum {
    ID_COLORS=FXMainWindow::ID_LAST,
    ID_COLOR_THEME,
    ID_CHOOSE_FONT,
    ID_SELECT_COMMAND,
    ID_CREATE_FILEBINDING,
    ID_REMOVE_FILEBINDING,
    ID_RENAME_FILEBINDING,
    ID_SELECT_FILEBINDING,
    ID_SELECT_ICON_BIG,
    ID_SELECT_ICON_BIGOPEN,
    ID_SELECT_ICON_MINI,
    ID_SELECT_ICON_MINIOPEN,
    ID_SELECT_MIMETYPE,
    ID_RUN_IN_TERMINAL,
    ID_CHANGE_DIRECTORY
    };
public:
  long onColorChanged(FXObject*,FXSelector,void*);
  long onColorTheme(FXObject*,FXSelector,void*);
  long onChooseFont(FXObject*,FXSelector,void*);
  long onCmdFileBinding(FXObject*,FXSelector,void*);
  long onCmdMimeType(FXObject*,FXSelector,void*);
  long onCmdCreateFileBinding(FXObject*,FXSelector,void*);
  long onCmdRemoveFileBinding(FXObject*,FXSelector,void*);
  long onCmdRenameFileBinding(FXObject*,FXSelector,void*);
  long onCmdSelectCommand(FXObject*,FXSelector,void*);
  long onCmdSelectIcon(FXObject*,FXSelector,void*);
  long onUpdRunInTerminal(FXObject*,FXSelector,void*);
  long onCmdRunInTerminal(FXObject*,FXSelector,void*);
  long onUpdChangeDirectory(FXObject*,FXSelector,void*);
  long onCmdChangeDirectory(FXObject*,FXSelector,void*);
public:

  // Constructor
  FXDesktopSetup(FXApp *app);

  // Set filename
  void setFilename(const FXString& file){ filename=file; }

  // Return filename
  FXString getFilename() const { return filename; }

  // Change application name
  void setApplicationName(const FXString& name){ applicationname=name; }

  // Return application name
  const FXString& getApplicationName() const { return applicationname; }

  // Change vendor name
  void setVendorName(const FXString& name){ vendorname=name; }

  // Return vendor name
  const FXString& getVendorName() const { return vendorname; }

  // Set filename from vendor and application name
  FXbool setApplicationAndVendor(const FXString& an,const FXString& vn=FXString::null);

  // Create widgets
  virtual void create();

  // Read settings file
  FXbool readSettingsFile(const FXString& file);

  // Write settings file
  FXbool writeSettingsFile(const FXString& file);

  // Close the application, return TRUE if actually closed
  virtual FXbool close(FXbool notify=false);

  // Destructor
  virtual ~FXDesktopSetup();
  };


#endif

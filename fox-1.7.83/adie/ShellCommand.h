/********************************************************************************
*                                                                               *
*                             S h e l l - C o m m a n d                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 2014,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef SHELLCOMMAND_H
#define SHELLCOMMAND_H



// Shell Command
class ShellCommand : public FXObject {
  FXDECLARE(ShellCommand)
private:
  FXProcess   process;          // Child process
  TextWindow *window;           // Window to send messages to
  FXSelector  selin;            // Message sent for input
  FXSelector  selout;           // Message sent for output
  FXSelector  selerr;           // Message sent for errors
  FXSelector  seldone;          // Message sent when done
  FXString    directory;        // Directory where to start
  FXString    input;            // Input to child process
  FXString    output;           // Output from child process
  FXival      ninput;           // Number of inputs sent to child
  FXival      noutput;          // Number of outputs received from child
  FXPipe      ipipe;            // Pipe input to child
  FXPipe      opipe;            // Pipe output from child
  FXPipe      epipe;            // Pipe errors from child
  FXuint      flags;            // Flags
private:
  ShellCommand(){}
  ShellCommand(const ShellCommand&);
  ShellCommand &operator=(const ShellCommand&);
public:
  long onCmdInput(FXObject*,FXSelector,void*);
  long onCmdOutput(FXObject*,FXSelector,void*);
  long onCmdError(FXObject*,FXSelector,void*);
public:
  enum {
    ID_INPUT=1,
    ID_OUTPUT,
    ID_ERROR
    };
  enum {
    STREAM=0,
    COLLECT=1
    };
public:

  // Construct shell command
  ShellCommand(TextWindow* win,FXSelector so=0,FXSelector se=0,FXSelector sd=0,FXuint flg=STREAM);

  // Set directory
  void setDirectory(const FXString& dir){ directory=dir; }

  // Return directory
  const FXString& getDirectory() const { return directory; }

  // Set command input
  void setInput(FXString& in);

  // Get command output
  void getOutput(FXString& out);

  // Access window
  void setWindow(TextWindow* win){ window=win; }
  TextWindow* getWindow() const { return window; }

  // Access input message
  void setInputMessage(FXSelector sel){ selin=sel; }
  FXSelector getInputMessage() const { return selin; }

  // Access output message
  void setOutputMessage(FXSelector sel){ selout=sel; }
  FXSelector getOutputMessage() const { return selout; }

  // Access error message
  void setErrorMessage(FXSelector sel){ selerr=sel; }
  FXSelector getErrorMessage() const { return selerr; }

  // Access done message
  void setDoneMessage(FXSelector sel){ seldone=sel; }
  FXSelector getDoneMessage() const { return seldone; }

  // Start command
  virtual FXbool start(const FXString& command);

  // Cancel command
  virtual FXbool cancel();

  // Stop command
  virtual FXbool stop();

  // Destroy it
  virtual ~ShellCommand();
  };

#endif

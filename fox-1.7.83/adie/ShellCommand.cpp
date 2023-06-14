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
#include "fx.h"
#include "Syntax.h"
#include "Modeline.h"
#include "TextWindow.h"
#include "Adie.h"
#include "ShellCommand.h"


/*
  Notes:

  - The ShellCommand object represents a running process inside Adie.

  - Need to add options to NOT capture output and stderr.

  - Need to add option to capture outputs into logger instead of text buffer.

  - On Windows, the pipe stuff won't work in addInput() because you can't
    block on anonymous pipes (or can you?).

  - FIXME add some code to break up commandline arguments, or alternatively,
    kick program off through shell (in this case, need to escape text to make
    magic characters safe from shell).

  - FIXME Maybe we should just connect child process's stderr to our own stderr?
*/

enum{BUFFERSIZE=2048};

/*******************************************************************************/

FXDEFMAP(ShellCommand) ShellCommandMap[]={
  FXMAPFUNC(SEL_IO_READ,ShellCommand::ID_ERROR,ShellCommand::onCmdError),
  FXMAPFUNC(SEL_IO_READ,ShellCommand::ID_OUTPUT,ShellCommand::onCmdOutput),
  FXMAPFUNC(SEL_IO_WRITE,ShellCommand::ID_INPUT,ShellCommand::onCmdInput),
  };


FXIMPLEMENT(ShellCommand,FXObject,ShellCommandMap,ARRAYNUMBER(ShellCommandMap))


// Construct shell command
ShellCommand::ShellCommand(TextWindow* win,FXSelector so,FXSelector se,FXSelector sd,FXuint flg):window(win),selout(so),selerr(se),seldone(sd),ninput(0),noutput(0),flags(flg){
  FXTRACE((1,"ShellCommand::ShellCommand\n"));
  }


// Set command input
void ShellCommand::setInput(FXString& in){
  input.adopt(in);
  }


// Get command output
void ShellCommand::getOutput(FXString& out){
  out.adopt(output);
  }


// Execute command
FXbool ShellCommand::start(const FXString& command){
  FXTRACE((1,"ShellCommand::start(%s)\n",command.text()));
  FXbool result=false;
  if(!command.empty() && !process.id()){
    FXchar** argv=nullptr;

    // Assemble command
    if(FXPath::parseArgs(argv,command)){

      // Find the command (argv[0]) in the path
      FXString exec=FXPath::search(FXSystem::getExecPath(),argv[0]);

      // Got full path to executable
      if(!exec.empty()){

        // Old directory
        FXString cwd=FXSystem::getCurrentDirectory();

        // Switch to new directory
        if(FXSystem::setCurrentDirectory(directory)){

          // Pipes at child's end
          FXPipe ichild;
          FXPipe ochild;
          FXPipe echild;

          // Open pipe for child input (the parent writes, child reads)
          if(!ipipe.open(ichild,FXIO::WriteOnly|FXIO::Inheritable)) goto x;

          // Open pipe for child outout (parent reads, child writes)
          if(!opipe.open(ochild,FXIO::ReadOnly|FXIO::Inheritable)) goto x;

          // Open pipe for child errors (parent reads, child writes)
          if(!epipe.open(echild,FXIO::ReadOnly|FXIO::Inheritable)) goto x;

          // Set handles to be used by child
          process.setInputStream(&ichild);
          process.setOutputStream(&ochild);
          process.setErrorStream(&echild);

          // Start it
          if(process.start(exec.text(),argv)){

            // Close child-side handles
            ichild.close();
            ochild.close();
            echild.close();

            // Set non-blocking on our end
            ipipe.mode(ipipe.mode()|FXIO::NonBlocking);
            opipe.mode(opipe.mode()|FXIO::NonBlocking);
            epipe.mode(epipe.mode()|FXIO::NonBlocking);

            // Set I/O callbacks
            if(ipipe.isOpen()){
              window->getApp()->addInput(this,ID_INPUT,ipipe.handle(),INPUT_WRITE);
              }
            if(opipe.isOpen()){
              window->getApp()->addInput(this,ID_OUTPUT,opipe.handle(),INPUT_READ);
              }
            if(epipe.isOpen()){
              window->getApp()->addInput(this,ID_ERROR,epipe.handle(),INPUT_READ);
              }
            result=true;
            }

          // Switch back to old directory
          FXSystem::setCurrentDirectory(cwd);
          }
        }
x:    freeElms(argv);
      }
    }
  return result;
  }


// Stop command
FXbool ShellCommand::stop(){
  FXTRACE((1,"ShellCommand::stop()\n"));
  if(process.id()){

    // Remove I/O callbacks
    if(ipipe.isOpen()){
      window->getApp()->removeInput(ipipe.handle(),INPUT_WRITE);
      ipipe.close();
      }
    if(opipe.isOpen()){
      window->getApp()->removeInput(opipe.handle(),INPUT_READ);
      opipe.close();
      }
    if(epipe.isOpen()){
      window->getApp()->removeInput(epipe.handle(),INPUT_READ);
      epipe.close();
      }

    // Wait till child is done
    return process.wait();
    }
  return false;
  }


// Cancel command
FXbool ShellCommand::cancel(){
  FXTRACE((1,"ShellCommand::cancel()\n"));
  if(process.id()){
    process.kill();
    return stop();
    }
  return false;
  }

/*******************************************************************************/

// Input to child process
long ShellCommand::onCmdInput(FXObject*,FXSelector,void*){
  FXTRACE((1,"ShellCommand::onCmdInput\n"));
  FXival count;
  count=ipipe.writeBlock(&input[ninput],input.length()-ninput);
  if(count<0){
    if(count!=FXIO::Again){
      if(seldone){ window->handle(this,seldone,nullptr); }
      stop();
      }
    return 1;
    }
  ninput+=count;
  if(count==0){
    window->getApp()->removeInput(ipipe.handle(),INPUT_WRITE);
    ipipe.close();
    return 1;
    }
  return 1;
  }


// Output from child process
long ShellCommand::onCmdOutput(FXObject*,FXSelector,void*){
  FXTRACE((1,"ShellCommand::onCmdOutput\n"));
  FXchar buffer[BUFFERSIZE+1];
  FXival count;
  count=opipe.readBlock(buffer,BUFFERSIZE);
  if(count<0){
    if(count!=FXIO::Again){
      stop();
      if(seldone){ window->handle(this,seldone,nullptr); }
      }
    return 1;
    }
  buffer[count]='\0';
  if(flags&COLLECT){
    output.append(buffer,count);
    }
  if(0<count && !(flags&COLLECT)){
    if(selout){ window->handle(this,selout,buffer); }
    return 1;
    }
  if(seldone){ window->handle(this,seldone,nullptr); }
  return 1;
  }


// Errors from child process
long ShellCommand::onCmdError(FXObject*,FXSelector,void*){
  FXTRACE((1,"ShellCommand::onCmdError\n"));
  FXchar buffer[BUFFERSIZE+1];
  FXival count;
  count=epipe.readBlock(buffer,BUFFERSIZE);
  if(count<0){
    if(count!=FXIO::Again){
      stop();
      if(seldone){ window->handle(this,seldone,nullptr); }
      }
    return 1;
    }
  buffer[count]='\0';
  if(0<count){
    if(selerr){ window->handle(this,selerr,buffer); }
    return 1;
    }
  return 1;
  }

/*******************************************************************************/

// Destroy shell command
ShellCommand::~ShellCommand(){
  FXTRACE((1,"ShellCommand::~ShellCommand\n"));
  stop();
  }


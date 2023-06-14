/********************************************************************************
*                                                                               *
*                  F O X   D e s k t o p   C a l c u l a t o r                  *
*                                                                               *
*********************************************************************************
* Copyright (C) 2001,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#include "fxkeys.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <ctype.h>
#include "Calculator.h"


// Start the whole thing
int main(int argc,char *argv[]){

  // Make sure  we're linked against the right library version
  if(fxversion[0]!=FOX_MAJOR || fxversion[1]!=FOX_MINOR || fxversion[2]!=FOX_LEVEL){
    fxerror("FOX Library mismatch; expected version: %d.%d.%d, but found version: %d.%d.%d.\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL,fxversion[0],fxversion[1],fxversion[2]);
    }

  // Make application
  FXApp application("Calculator");

  // Open display
  application.init(argc,argv);

  // Main window
  Calculator* calculator=new Calculator(&application);

  // Handle interrupt to save stuff nicely
  application.addSignal(SIGINT,calculator,Calculator::ID_CLOSE);

  // Create app
  application.create();

  // Run
  return application.run();
  }



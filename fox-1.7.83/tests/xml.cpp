/********************************************************************************
*                                                                               *
*                               X M L   T e s t                                 *
*                                                                               *
*********************************************************************************
* Copyright (C) 2016,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"


// Print options
void printusage(const char* prog){
  fxmessage("%s options:\n",prog);
  fxmessage("  --load <file>            Load xml file.\n");
  fxmessage("  --save <file>            Save xml file.\n");
  fxmessage("  --oasis <dir>            Run OASIS tests from directory <dir>.\n");
  fxmessage("  --tracelevel <level>     Set trace level.\n");
  fxmessage("  -v, --verbose            Increase verbosity level.\n");
  fxmessage("  -h, --help               Print help.\n");
  }


// Pass or fail
const FXchar *const passed[2]={"fail","pass"};

/*******************************************************************************/

// Save to this
FXXMLFile xmloutput;

// Load from this
FXXMLFile xmlinput;

/*******************************************************************************/

// Just print in the callback

// Start document
FXXML::Error startDocument(){
  fxmessage("startDocument()\n");
  return FXXML::ErrOK;
  }


// Element start
FXXML::Error startElement(const FXString& tag,const FXStringDictionary& atts){
  fxmessage("startElement(%s",tag.text());
  if(!atts.empty()){
    for(FXint i=0; i<atts.no(); ++i){
      if(!atts.empty(i)){ fxmessage(",%s=\"%s\"",atts.key(i).text(),atts.data(i).text()); }
      }
    }
  fxmessage(")\n");
  return FXXML::ErrOK;
  }


// Characters
FXXML::Error characters(const FXString& text){
  fxmessage("characters(%s)\n",text.text());
  return FXXML::ErrOK;
  }


// Comment
FXXML::Error comment(const FXString& text){
  fxmessage("comment(%s)\n",text.text());
  return FXXML::ErrOK;
  }


// Processing instruction
FXXML::Error processing(const FXString& target,const FXString& text){
  fxmessage("processing(%s,%s)\n",target.text(),text.text());
  return FXXML::ErrOK;
  }


// Element end
FXXML::Error endElement(const FXString& tag){
  fxmessage("endElement(%s)\n",tag.text());
  return FXXML::ErrOK;
  }


// Document end
FXXML::Error endDocument(){
  fxmessage("endDocument()\n");
  return FXXML::ErrOK;
  }

/*******************************************************************************/

// Test XML I/O
int main(int argc,char *argv[]){
  const FXchar* xmltests="xmltests.json";
  const FXchar* loadfile=nullptr;
  const FXchar* savefile=nullptr;
  const FXchar* oasisdir=nullptr;
  FXint verbosity=0;

  // Grab a few arguments
  for(FXint arg=1; arg<argc; ++arg){
    if(strcmp(argv[arg],"-h")==0 || strcmp(argv[arg],"--help")==0){
      printusage(argv[0]);
      exit(0);
      }
    if(strcmp(argv[arg],"-v")==0 || strcmp(argv[arg],"--verbose")==0){
      verbosity++;
      }
    else if(strcmp(argv[arg],"--save")==0){
      if(++arg>=argc){ fxmessage("Missing save filename argument.\n"); exit(1); }
      savefile=argv[arg];
      }
    else if(strcmp(argv[arg],"--load")==0){
      if(++arg>=argc){ fxmessage("Missing load filename argument.\n"); exit(1); }
      loadfile=argv[arg];
      }
    else if(strcmp(argv[arg],"--oasis")==0){
      if(++arg>=argc){ fxmessage("Missing oasis directory argument.\n"); exit(1); }
      oasisdir=argv[arg];
      }
    else if(strcmp(argv[arg],"--tracelevel")==0){
      if(++arg>=argc){ fxmessage("Missing tracelevel number argument.\n"); exit(1); }
      setTraceLevel(strtoul(argv[arg],nullptr,0));
      }
    else{
      fxmessage("Bad argument.\n");
      printusage(argv[0]);
      exit(1);
      }
    }

  // Run tests
  if(oasisdir){
    FXVariant testcases;

    {
    FXJSONFile json;
    if(json.open(xmltests,FXJSON::Load)){
      fxmessage("Loading tests from: %s\n\n",xmltests);
      FXJSON::Error jsonerr=json.load(testcases);
      json.close();
      if(jsonerr!=FXJSON::ErrOK){
        fxmessage("Error: %s:%d:%d: %s\n",xmltests,json.getLine(),json.getColumn(),FXJSON::getError(jsonerr));
        return 1;
        }
      }
    else{
      fxwarning("Unable to open %s.\n",xmltests);
      return 1;
      }
    }

    // Run test cases
    for(FXint i=0; i<testcases.no(); i++){
      FXString pass=testcases[i]["type"];
      FXString desc=testcases[i]["description"];
      FXString file=FXPath::absolute(oasisdir,testcases[i]["filename"]);
      FXXMLFile xml;
      if(xml.open(file,FXXML::Load)){
        FXXML::Error xmlerr=xml.parse();
        FXbool expect=(pass=="pass");
        FXbool actual=(xmlerr==FXXML::ErrOK);
        if(actual!=expect){
          if(2<verbosity){ fxmessage("\nDescription: %s\n",desc.text()); }
          fxmessage("Fail: %s",file.text());
          if(0<verbosity && !actual){
            fxmessage(":%d:%d: %s",xml.getLine(),xml.getColumn(),FXXML::getError(xmlerr));
            }
          if(1<verbosity){ fxmessage(" (expected %s)",passed[expect]); }
          fxmessage("\n");
          }
        else{
          if(2<verbosity){ fxmessage("\nDescription: %s\n",desc.text()); }
          fxmessage("Pass: %s",file.text());
          if(0<verbosity && !actual){
            fxmessage(":%d:%d: %s",xml.getLine(),xml.getColumn(),FXXML::getError(xmlerr));
            }
          if(1<verbosity){ fxmessage(" (expected %s)",passed[expect]); }
          fxmessage("\n");
          }
        xml.close();
        }
      else{
        fxwarning("\n\nUnable to open: \"%s\".\n",file.text());
        }
      }
    }

  // Unless output file is specified, we will just print
  // Notice we can connect plain functions as well as member
  // functions of some object
  xmlinput.startDocumentCB.connect<startDocument>();
  xmlinput.startElementCB.connect<startElement>();
  xmlinput.charactersCB.connect<characters>();
  xmlinput.commentCB.connect<comment>();
  xmlinput.processingCB.connect<processing>();
  xmlinput.endElementCB.connect<endElement>();
  xmlinput.endDocumentCB.connect<endDocument>();

  // Open save file
  if(savefile){
    if(!xmloutput.open(savefile,FXXML::Save)){
      fxwarning("Error: unable to open: \"%s\" for writing.\n",savefile);
      return 0;
      }

    // Connect source XML file parser to write back out to destination file
    // Just for kicks, we disconnect comments and processing instructions
    xmlinput.startDocumentCB.connect<FXXML,&FXXML::startDocument>(&xmloutput);
    xmlinput.startElementCB.connect<FXXML,&FXXML::startElement>(&xmloutput);
    xmlinput.charactersCB.connect<FXXML,&FXXML::characters>(&xmloutput);
    xmlinput.commentCB.disconnect();
    xmlinput.processingCB.disconnect();
    xmlinput.endElementCB.connect<FXXML,&FXXML::endElement>(&xmloutput);
    xmlinput.endDocumentCB.connect<FXXML,&FXXML::endDocument>(&xmloutput);
    }

  // Now load some XML file, and invoke the callbacks as we go
  if(loadfile){
    FXXML::Error xmlerr;
    if(!xmlinput.open(loadfile,FXXML::Load)){
      fxwarning("Error: unable to open: \"%s\" for reading.\n",loadfile);
      return 0;
      }
    xmlerr=xmlinput.parse();
    fxmessage("Loaded %lld bytes\n",xmlinput.getOffset());
    if(xmlerr!=FXXML::ErrOK){
      fxmessage("Error: %s:%d:%d: %s\n",loadfile,xmlinput.getLine(),xmlinput.getColumn(),FXXML::getError(xmlerr));
      }
    else{
      fxmessage("OK\n");
      }
    xmlinput.close();
    }

  // Close if it was open
  if(savefile){
    fxmessage("Stored %lld bytes\n",xmloutput.getOffset());
    xmloutput.close();
    fxmessage("OK\n");
    }

  return 0;
  }



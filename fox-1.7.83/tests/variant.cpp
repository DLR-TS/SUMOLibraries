/********************************************************************************
*                                                                               *
*                   V a r i a n t   a n d   J S O N   T e s t s                 *
*                                                                               *
*********************************************************************************
* Copyright (C) 2013,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"


// Print options
void printusage(const char* prog){
  fxmessage("%s options:\n",prog);
  fxmessage("  --load-json <file>       Load json file.\n");
  fxmessage("  --save-json <file>       Save json file.\n");
  fxmessage("  --load-ini <file>        Load ini file.\n");
  fxmessage("  --save-ini <file>        Save ini file.\n");
  fxmessage("  --tracelevel <level>     Set trace level.\n");
  fxmessage("  --precision <prec>       Set numeric precision for floating point.\n");
  fxmessage("  --format <format>        Set exponent format for floating point (0=Never,1=Always,2=As-Needed).\n");
  fxmessage("  --flow <flow>            Set output flow format (0=Stream,1=Compact,2=Pretty).\n");
  fxmessage("  --dent <dent>            Set indentation amount.\n");
  fxmessage("  --wrap <columns>         Set line wrap columns.\n");
  fxmessage("  --esc <mode>             Set unicode escape mode (0=OFF,1=\\xHH,2=\\uHHHH).\n");
  fxmessage("  --ver 5                  Set JSON version (currently, 1...5).\n");
  fxmessage("  --single-quotes          Set single quotes mode (JSON5 only).\n");
  fxmessage("  --double-quotes          Set double quotes mode.\n");
  fxmessage("  -h, --help               Print help.\n");
  }


// Test JSON I/O
int main(int argc,char *argv[]){
  FXString savefile="test.json";
  FXString loadfile;
  FXbool loadjson=true;
  FXbool savejson=true;
  FXint precision;
  FXint format;
  FXint flow;
  FXint dent;
  FXint wrap;
  FXint quote;
  FXint esc;
  FXint ver;
  FXbool ok=true;

  // JSON I/O
  FXJSONFile json;
  FXINIFile  ini;

  // Variant to save
  FXVariant var;

  // Assume defaults for starters
  precision=json.getNumericPrecision();
  format=json.getNumericFormat();
  flow=json.getOutputFlow();
  dent=json.getIndentation();
  wrap=json.getLineWrap();
  quote=json.getQuote();
  esc=json.getEscapeMode();
  ver=json.getVersion();

  fxmessage("sizeof(FXVariant) = %ld\n",sizeof(FXVariant));
  fxmessage("sizeof(FXVariantMap) = %ld\n",sizeof(FXVariantMap));
  fxmessage("sizeof(FXVariantArray) = %ld\n",sizeof(FXVariantArray));
  fxmessage("sizeof(FXString) = %ld\n",sizeof(FXString));
  fxmessage("sizeof(FXHash) = %ld\n",sizeof(FXHash));
  fxmessage("sizeof(FXStringDictionary) = %ld\n",sizeof(FXStringDictionary));
  fxmessage("sizeof(FXDictionary) = %ld\n",sizeof(FXDictionary));
  fxmessage("sizeof(FXReverseDictionary) = %ld\n",sizeof(FXReverseDictionary));

  // Grab a few arguments
  for(FXint arg=1; arg<argc; ++arg){
    if(strcmp(argv[arg],"-h")==0 || strcmp(argv[arg],"--help")==0){
      printusage(argv[0]);
      exit(0);
      }
    else if(strcmp(argv[arg],"--load-json")==0){
      if(++arg>=argc){ fxmessage("Missing filename argument.\n"); exit(1); }
      loadfile=argv[arg];
      loadjson=true;
      }
    else if(strcmp(argv[arg],"--save-json")==0){
      if(++arg>=argc){ fxmessage("Missing filename argument.\n"); exit(1); }
      savefile=argv[arg];
      savejson=true;
      }
    else if(strcmp(argv[arg],"--load-ini")==0){
      if(++arg>=argc){ fxmessage("Missing filename argument.\n"); return 1; }
      loadfile=argv[arg];
      loadjson=false;
      }
    else if(strcmp(argv[arg],"--save-ini")==0){
      if(++arg>=argc){ fxmessage("Missing filename argument.\n"); return 1; }
      savefile=argv[arg];
      savejson=false;
      }
    else if(strcmp(argv[arg],"--tracelevel")==0){
      if(++arg>=argc){ fxmessage("Missing tracelevel number argument.\n"); exit(1); }
      setTraceLevel(strtoul(argv[arg],nullptr,0));
      }
    else if(strcmp(argv[arg],"--tracetopics")==0){
      if(++arg>=argc){ fxmessage("Missing tracelevel number argument.\n"); exit(1); }
      setTraceTopics(argv[arg],true);
      }
    else if(strcmp(argv[arg],"--precision")==0){
      if(++arg>=argc){ fxmessage("Missing precision number argument.\n"); exit(1); }
      precision=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--format")==0){
      if(++arg>=argc){ fxmessage("Missing format number argument.\n"); exit(1); }
      format=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--flow")==0){
      if(++arg>=argc){ fxmessage("Missing flow number argument.\n"); exit(1); }
      flow=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--dent")==0){
      if(++arg>=argc){ fxmessage("Missing indentation amount number argument.\n"); exit(1); }
      dent=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--wrap")==0){
      if(++arg>=argc){ fxmessage("Missing line wrap columns argument.\n"); exit(1); }
      wrap=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--esc")==0){
      if(++arg>=argc){ fxmessage("Missing escape mode argument.\n"); exit(1); }
      esc=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--ver")==0){
      if(++arg>=argc){ fxmessage("Missing version argument.\n"); exit(1); }
      ver=strtoul(argv[arg],nullptr,0);
      }
    else if(strcmp(argv[arg],"--single-quotes")==0){
      quote='\'';
      }
    else if(strcmp(argv[arg],"--double-quotes")==0){
      quote='"';
      }
    else{
      fxmessage("Bad argument.\n");
      printusage(argv[0]);
      exit(1);
      }
    }

  // If not loading, make up some data
  if(loadfile.empty()){

    // Simple values
    var["real"]=PI;
    var["bool"]=true;
    var["int"]=87654321;
    var["string"]="variant";
    var["char"]='X';
    var["null"]=FXVariant::null;
    var["inf"]=33.0;
    var["nan"]=22.0;
    var["array"][5]=10.0;
    var["array"][3][2]=10.0;
    var["array"][3][1]='x';
    var["array"][2]="variant";
    var["array"][4]=2.0;
    var["array"][0]=var["array"][5];
    var["array"][1]='c';
    var["map"]["key"]="value";
    var["map"]["more"]["pi"]=3.141592653;
    var["map"]["more"]["e"]=2.718281828;
    var["map"]["more"]["c"]=299792458.0;
    var["map"]["more"]["answer"]=42.0;
    var["map"]["more"]["letter"]="Unicode: \xC3\xBC Hex:\377\xff\b\n\f\v\"";
    var["emptymap"].setType(FXVariant::MapType);
    var["emptyarray"].setType(FXVariant::ArrayType);
    for(FXival i=0; i<100; ++i){
      var["bigarray"][i]=Math::sin(0.005*i/PI);
      }
    }

  // Load some data
  if(!loadfile.empty()){

    // Load as json
    if(loadjson){
      if(json.open(loadfile,FXJSON::Load)){
        fxmessage("Start load from: %s\n",loadfile.text());
        FXJSON::Error loaderr=json.load(var);
        fxmessage("Loaded %lld bytes, %d lines\n",json.getOffset(),json.getLine());
        if(loaderr!=FXJSON::ErrOK){
          fxmessage("Error: %s:%d:%d: %s\n",loadfile.text(),json.getLine(),json.getColumn(),FXJSON::getError(loaderr));
          }
        else{
          fxmessage("OK\n");
          }
        json.close();
        }
      else{
        fxwarning("Error: unable to open: \"%s\" for reading.\n",loadfile.text());
        }
      }

    // Load as ini
    else{
      if(ini.open(loadfile,FXINI::Load)){
        fxmessage("Start load from: %s\n",loadfile.text());
        FXINI::Error loaderr=ini.load(var);
        fxmessage("Loaded %lld bytes\n",ini.getOffset());
        if(loaderr!=FXINI::ErrOK){
          fxwarning("Error: %s:%d:%d: %s\n",loadfile.text(),ini.getLine(),ini.getColumn(),FXINIFile::getError(loaderr));
          }
        else{
          fxwarning("OK\n");
          }
        ini.close();
        }
      else{
        fxwarning("Error: unable to open: \"%s\" for reading.\n",loadfile.text());
        }
      }
    }

  // Save some data
  if(!savefile.empty()){

    // Save as json
    if(savejson){

      // Set precision and format
      json.setNumericPrecision(precision);
      json.setNumericFormat(format);
      json.setIndentation(dent);
      json.setOutputFlow(flow);
      json.setLineWrap(wrap);
      json.setEscapeMode(esc);
      json.setQuote(quote);
      json.setVersion(ver);

      // Report float precision used to save
      fxmessage("Precision: %d format: %d flow: %d dent: %d wrap: %d\n",precision,format,flow,dent,wrap);

      // Save test
      if(json.open(savefile,FXJSON::Save)){
        fxmessage("Start save to: %s\n",savefile.text());
        FXJSON::Error saveerr=json.save(var);
        fxmessage("Stored %lld bytes, %d lines\n",json.getOffset(),json.getLine());
        if(saveerr!=FXJSON::ErrOK){
          fxmessage("Error: %s:%d:%d: %s\n",savefile.text(),json.getLine(),json.getColumn(),FXJSON::getError(saveerr));
          }
        else{
          fxmessage("OK\n");
          }
        json.close();
        }
      else{
        fxwarning("Error: unable to open: \"%s\" for writing.\n",savefile.text());
        }
      }

    // Save as ini
    else{
      if(ini.open(savefile,FXINI::Save)){
        fxmessage("Start save to: %s\n",savefile.text());
        FXINI::Error saveerr=ini.save(var);
        fxmessage("Stored %lld bytes, %d lines\n",ini.getOffset(),ini.getLine());
        if(saveerr!=FXINI::ErrOK){
          fxmessage("Error: %s:%d:%d: %s\n",savefile.text(),ini.getLine(),ini.getColumn(),FXINIFile::getError(saveerr));
          }
        else{
          fxmessage("OK\n");
          }
        json.close();
        }
      else{
        fxwarning("Error: unable to open: \"%s\" for writing.\n",savefile.text());
        }
      }
    }

  return 0;
  }



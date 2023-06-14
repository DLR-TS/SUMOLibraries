/********************************************************************************
*                                                                               *
*                             String Format I/O Test                            *
*                                                                               *
*********************************************************************************
* Copyright (C) 2007,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"

/*
  Notes:
  - Test battery for fxscanf().
*/


/*******************************************************************************/

namespace FX {

extern FXint __sscanf(const FXchar* string,const FXchar* format,...);
extern FXint __snprintf(FXchar* string,FXint length,const FXchar* format,...);

}

/*******************************************************************************/

const FXchar *intformat[]={
  "%d",
  "%5d",
  "%'d",
  "%'5d",
  "%i",
  "%u",
  "%x",
  "%o",
  "%b",
  };


const FXchar *intnumbers[]={
  "111",
  "-111",
  "0xff",
  "0b11111111",
  "0377",
  "1,234,567,890",
  "12,345,678",
  "123,456,789",
  "1,234,567,89",
  "0,123",
  "2147483647",
  "-2147483648",
  "4294967295",
  };

/*******************************************************************************/

const FXchar* floatformat[]={
  "%lf",
  "%5lf",
  "%'lf",
  "%'5lf",
  };


const FXchar *floatnumbers[]={
  "3.1415926535897932384626433833",
  "-1.23456789E-99",
  "1.7976931348623157e+308",
  "2.2250738585072014e-308",
  "1.8e+308",
  "4.94065645841246544177e-324",
  "0.0E400",
  "1,000.0",
  "0.0000000000000001234567891234567",
  "0.0000000001234567891234567",
  "123456789.1234567",
  "1234567891234567.",
  "1234567891234567000000",
  "1,234,567,891,234,567,000,000",
  "-1.",
  "-.1",
  "1.",
  ".1",
  "NaN",
  "Inf",
  "Infinity",
  "+0.5",
  "-.5",
  "0.005",
  "0x1.ac53a7df93d691111p+66",
  "0x003.fffffffffffffp+00",
  "0x0.00ffffffp+00",
  "0x1.p+1024",
  "0x1.ffffffffffffp+1024",
  "0x1.222p-1066",
  "0x0p+66",
  "1,234,567,890",
  "12,345,678",
  "123,456,789",
  "1,234,567,89",
  "0,123",
  };

/*******************************************************************************/

const FXchar *intformat3[]={
  "%i %i %i",
  "%3$i %1$i %2$i"
  };


const FXchar *intnumbers3[]={
  "111 222 333",
  "-111 +222 -333",
  "0xff 0377 123456789",
  "11111111 10101010 0b1111111111111111111111111111111",
  "4294967295 2147483647 -2147483648"
  };

/*******************************************************************************/

const FXchar* floatformat3[]={
  "%lf %lf %lf",
  "%3$lf %1$lf %2$lf",
  };


const FXchar *floatnumbers3[]={
  "0.0 1.0 3.1415926535897932384626433833",
  "-0.1 +0.11111 -1.23456789E-99",
  "1.7976931348623157e+308 2.2250738585072014e-308 1.17549435e-38",
  "1.8e+308 4.94065645841246544177e-324 0.0E400",
  "1,000.0 26,345,123 1000.0",
  "0000000000000000000000000000 0.1 1.0E-1",
  "-1. -.1 1.",
  "NaN Inf 3.14",
  "+0.5 -.5 +0x1.ac54p+66",
  };


/*******************************************************************************/

const FXchar *stringformat[]={
  "%s",
  "%4s",
  "%[0-9.Ee+-]",
  "%[^a-c]",
  "%[]]",
  "%[0-9-]",
  "%[a-a]",
  "%[a-zA-Z0-9_]"
  };

const FXchar *stringinputs[]={
  "1.0E-99",
  "123abc",
  "]]]][[[[",
  "123-1456",
  "aaaaabbbb",
  "Camel_Case_1337"
  };

/*******************************************************************************/

// Uncomment to revert to native version
//#define __sscanf sscanf


// Start
int main(int,char*[]){
  FXuint   x,y;
  FXint    res;
  FXint    ia,ib,ic;
  FXdouble da,db,dc;
  FXchar   str[1000];
  FXchar   buf[2000];
/*

//  const FXchar* string="0x1.ac53a7df93d691111p+66";
//  const FXchar* string="0x003.fffffffffffffp+00";
//  const FXchar* string="0x0.00ffffffp+00";
//  const FXchar* string="0x1.p+1024";
//  const FXchar* string="0x1.ffffffffffffp+1024";
//  const FXchar* string="0x1.222p-1074";
//  const FXchar* string="0x1.222p-1066";
//  const FXchar* string="0x0p+66";
//  const FXchar* string="2.2250738585072014e-308";
//  const FXchar* string="1,234,567,891,234,567,000,000";
//  const FXchar* string="123,456,789,123,456,789,123";
  const FXchar* string="1,000,000,000,000,000,999";
  const FXchar* format="%'lf";

  res=__sscanf(string,format,&dc);
__snprintf(buf,sizeof(buf),"format=\"%s\" input=%s  res=%d  num=%.18lg = 0x%016llx = %a  man=0x%016llx  exp=%d\n",format,string,res,dc,Math::fpBits(dc),dc,Math::fpMantissa(dc),Math::fpExponent(dc));
  fprintf(stdout,"%s",buf);
  snprintf(buf,sizeof(buf),"format=\"%s\" input=%s  res=%d  num=%.18lg = 0x%016llx = %a  man=0x%016llx  exp=%d\n",format,string,res,dc,Math::fpBits(dc),dc,Math::fpMantissa(dc),Math::fpExponent(dc));
  fprintf(stdout,"%s",buf);
  fprintf(stdout,"\n");
  return 1;

*/

  // Reading integers
  for(x=0; x<ARRAYNUMBER(intformat); x++){
    for(y=0; y<ARRAYNUMBER(intnumbers); y++){
      ia=0;
      res=__sscanf(intnumbers[y],intformat[x],&ia);
      fprintf(stdout,"format=\"%s\" input=%-40s res=%d a=%-40d\n",intformat[x],intnumbers[y],res,ia);
      }
    }
  fprintf(stdout,"\n");

 // Reading floats
  for(x=0; x<ARRAYNUMBER(floatformat); x++){
    for(y=0; y<ARRAYNUMBER(floatnumbers); y++){
      da=0;
      res=__sscanf(floatnumbers[y],floatformat[x],&da);
      snprintf(buf,sizeof(buf),"format=\"%s\" input=%-40s res=%d a=%-40.18lg",floatformat[x],floatnumbers[y],res,da);
      fprintf(stdout,"%s\n",buf);
      }
    }
  fprintf(stdout,"\n");

  // Reading integers
  for(x=0; x<ARRAYNUMBER(intformat3); x++){
    for(y=0; y<ARRAYNUMBER(intnumbers3); y++){
      ia=ib=ic=0;
      res=__sscanf(intnumbers3[y],intformat3[x],&ia,&ib,&ic);
      snprintf(buf,sizeof(buf),"format=\"%s\" input=%-40s res=%d a=%d b=%d c=%d",intformat3[x],intnumbers3[y],res,ia,ib,ic);
      fprintf(stdout,"%s\n",buf);
      }
    }
  fprintf(stdout,"\n");

  // Reading floats
  for(x=0; x<ARRAYNUMBER(floatformat3); x++){
    for(y=0; y<ARRAYNUMBER(floatnumbers3); y++){
      da=db=dc=0;
      res=__sscanf(floatnumbers3[y],floatformat3[x],&da,&db,&dc);
      snprintf(buf,sizeof(buf),"format=\"%s\" input=%-40s res=%d a=%.18lg b=%.18lg c=%.18lg",floatformat3[x],floatnumbers3[y],res,da,db,dc);
      fprintf(stdout,"%s\n",buf);
      }
    }
  fprintf(stdout,"\n");

  // Reading strings
  for(x=0; x<ARRAYNUMBER(stringformat); x++){
    for(y=0; y<ARRAYNUMBER(stringinputs); y++){
      memset(str,0,sizeof(str));
      res=__sscanf(stringinputs[y],stringformat[x],str);
      snprintf(buf,sizeof(buf),"format=\"%s\" input=%-40s res=%d str=%s",stringformat[x],stringinputs[y],res,str);
      fprintf(stdout,"%s\n",buf);
      }
    }
  fprintf(stdout,"\n");

  return 1;
  }


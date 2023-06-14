/********************************************************************************
*                                                                               *
*                             String Format I/O Test                            *
*                                                                               *
*********************************************************************************
* Copyright (C) 2007,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
//#include <locale.h>
#include <time.h>


/*
  Notes:
  - Test battery for fxprintf().
*/


/*******************************************************************************/

namespace FX {

extern FXint __snprintf(FXchar* string,FXint length,const FXchar* format,...);
extern FXint __sscanf(const FXchar* string,const FXchar* format,...);

}

const FXchar *floatformat[]={
  "%.15E",
  "%'.5e",
  "%10.5f",
  "%-10.5f",
  "%+10.5f",
  "% 10.5f",
  "%123.9f",
  "%+22.9f",
  "%+4.9f",
  "%01.3f",
  "%4f",
  "%3.1f",
  "%3.2f",
  "%.0f",
  "%.3f",
  "%'.8f",
  "%+.3g",
  "%#.3g",
  "%.g",
  "%#.g",
  "%g",
  "%#g",
  "%'.8g",
  "%12.4e",
  "%a",
  "%A",
  "%.a",
  "%#.a",
  "%+15.4a",
  "%.20a",
  };


const double floatnumbers[]={
  0.000000001,
  -1.5,
  0.8,
  1.0,                          // 0x1p+0
  10.0,
  100.0,
  1000.0,
  10000.0,
  999.0,
  1010.0,
  134.21,
  91340.2,
  341.1234,
  203.9,
  0.4,
  0.96,
  0.996,
  0.9996,
  1.996,
  4.136,
  6442452944.1234,
  1.23456789E+20,
  1.23456789E-205,
  6.4969530541989433e-17,
  0.99999999,
  0.000009995,
  0.123456789,
  2.2250738585072014e-308,      // 0x1p-1022
  -4.94065645841246544177e-324, // -0x0.0000000000001p-1022
  1.7976931348623157e+308,      // 0x1.fffffffffffffp+1023
  1.9382023e-03,
  5e-320,
  0.0,
  -0.0,
  8.6796844466580162e-315
  };


// Double Infinite
static const union{ FXulong u; FXdouble f; } doubleinf={
  FXULONG(0x7ff0000000000000)
  };

// Double NaN
static const union{ FXulong u; FXdouble f; } doublenan={
  FXULONG(0x7fffffffffffffff)
  };

// Double small
static const union{ FXulong u; FXdouble f; } doublesmall={
  FXULONG(0x0000000000002788)
  };


const FXchar *intformat[]={
  "%d",
  "%0d",
  "%'d",
  "%02x",
  "%0.2x",
  "%-8d",
  "%8d",
  "%08d",
  "%.6d",
  "%u",
  "%+i",
  "% i",
  "%x",
  "%#x",
  "%#08x",
  "%o",
  "%#o",
  "%.32b",
  };

const FXint intnumbers[]={
  0,
  1,
  -1,
  0x90,
  -34,
  2147483647,
  -2147483648
  };


const FXchar *positionalformat[]={
  "%d%d%d",
  "%3$d%2$d%1$d",
  "%2$*1$d%3$d"
  };

const FXchar *positionalformat2="%1$*2$.*3$lf";

const FXchar *positionalformat3="%3$d%1$d%2$d";


// Uncomment to revert to native version
//#define __snprintf snprintf

void specialcases(const char* fmt){
  FXchar buffer[1024];
  double num=1234567890.123456789;
  while(0.000001<=num){
    __snprintf(buffer,sizeof(buffer),fmt,num);
    fprintf(stdout,"format=\"%s\" output=\"%s\"\n",fmt,buffer);
    num*=0.1;
    }
  fprintf(stdout,"\n");
  }


// Start
int main(int argc,char* argv[]){
  FXchar buffer[1024];
  FXuval x,y;
  FXdouble trouble;

/*
*/
  fxmessage("clz32(0x12345678) = %d\n",clz32(0x12345678));      // 3
  fxmessage("clz32(0x01234567) = %d\n",clz32(0x01234567));      // 7
  fxmessage("clz32(0x00123456) = %d\n",clz32(0x00123456));      // 11
  fxmessage("clz32(0x00012345) = %d\n",clz32(0x00012345));      // 15
  fxmessage("clz32(0x00001234) = %d\n",clz32(0x00001234));      // 19
  fxmessage("clz32(0x00000123) = %d\n",clz32(0x00000123));      // 23
  fxmessage("clz32(0x00000012) = %d\n",clz32(0x00000012));      // 27
  fxmessage("clz32(0x00000001) = %d\n",clz32(0x00000001));      // 31
  fxmessage("\n");

  fxmessage("clz64(0x123456789ABCDEF0) = %lld\n",clz64(FXULONG(0x123456789ABCDEF0)));    // 3
  fxmessage("clz64(0x0123456789ABCDEF) = %lld\n",clz64(FXULONG(0x0123456789ABCDEF)));    // 7
  fxmessage("clz64(0x00123456789ABCDE) = %lld\n",clz64(FXULONG(0x00123456789ABCDE)));    // 11
  fxmessage("clz64(0x000123456789ABCD) = %lld\n",clz64(FXULONG(0x000123456789ABCD)));    // 15
  fxmessage("clz64(0x0000123456789ABC) = %lld\n",clz64(FXULONG(0x0000123456789ABC)));    // 19
  fxmessage("clz64(0x00000123456789AB) = %lld\n",clz64(FXULONG(0x00000123456789AB)));    // 23
  fxmessage("clz64(0x000000123456789A) = %lld\n",clz64(FXULONG(0x000000123456789A)));    // 27
  fxmessage("clz64(0x0000000123456789) = %lld\n",clz64(FXULONG(0x0000000123456789)));    // 31
  fxmessage("clz64(0x0000000012345678) = %lld\n",clz64(FXULONG(0x0000000012345678)));    // 35
  fxmessage("clz64(0x0000000001234567) = %lld\n",clz64(FXULONG(0x0000000001234567)));    // 39
  fxmessage("clz64(0x0000000000123456) = %lld\n",clz64(FXULONG(0x0000000000123456)));    // 43
  fxmessage("clz64(0x0000000000012345) = %lld\n",clz64(FXULONG(0x0000000000012345)));    // 47
  fxmessage("clz64(0x0000000000001234) = %lld\n",clz64(FXULONG(0x0000000000001234)));    // 51
  fxmessage("clz64(0x0000000000000123) = %lld\n",clz64(FXULONG(0x0000000000000123)));    // 55
  fxmessage("clz64(0x0000000000000012) = %lld\n",clz64(FXULONG(0x0000000000000012)));    // 59
  fxmessage("clz64(0x0000000000000001) = %lld\n",clz64(FXULONG(0x0000000000000001)));    // 63
  fxmessage("\n");

  fxmessage("ctz32(0x87654321) = %d\n",ctz32(0x87654321));      // 0
  fxmessage("ctz32(0x87654320) = %d\n",ctz32(0x87654320));      // 5
  fxmessage("ctz32(0x87654300) = %d\n",ctz32(0x87654300));      // 8
  fxmessage("ctz32(0x87654000) = %d\n",ctz32(0x87654000));      // 14
  fxmessage("ctz32(0x87650000) = %d\n",ctz32(0x87650000));      // 16
  fxmessage("ctz32(0x87600000) = %d\n",ctz32(0x87600000));      // 21
  fxmessage("ctz32(0x87000000) = %d\n",ctz32(0x87000000));      // 24
  fxmessage("ctz32(0x80000000) = %d\n",ctz32(0x80000000));      // 31
  fxmessage("\n");

  fxmessage("ctz64(0x8FEDCBA987654321) = %lld\n",ctz64(FXULONG(0x8FEDCBA987654321)));      // 0
  fxmessage("ctz64(0x8FEDCBA987654320) = %lld\n",ctz64(FXULONG(0x8FEDCBA987654320)));      // 5
  fxmessage("ctz64(0x8FEDCBA987654300) = %lld\n",ctz64(FXULONG(0x8FEDCBA987654300)));      // 8
  fxmessage("ctz64(0x8FEDCBA987654000) = %lld\n",ctz64(FXULONG(0x8FEDCBA987654000)));      // 14
  fxmessage("ctz64(0x8FEDCBA987650000) = %lld\n",ctz64(FXULONG(0x8FEDCBA987650000)));      // 16
  fxmessage("ctz64(0x8FEDCBA987600000) = %lld\n",ctz64(FXULONG(0x8FEDCBA987600000)));      // 21
  fxmessage("ctz64(0x8FEDCBA987000000) = %lld\n",ctz64(FXULONG(0x8FEDCBA987000000)));      // 24
  fxmessage("ctz64(0x8FEDCBA980000000) = %lld\n",ctz64(FXULONG(0x8FEDCBA980000000)));      // 31
  fxmessage("ctz64(0x8FEDCBA900000000) = %lld\n",ctz64(FXULONG(0x8FEDCBA900000000)));      // 32
  fxmessage("ctz64(0x8FEDCBA000000000) = %lld\n",ctz64(FXULONG(0x8FEDCBA000000000)));      // 37
  fxmessage("ctz64(0x8FEDCB0000000000) = %lld\n",ctz64(FXULONG(0x8FEDCB0000000000)));      // 40
  fxmessage("ctz64(0x8FEDC00000000000) = %lld\n",ctz64(FXULONG(0x8FEDC00000000000)));      // 46
  fxmessage("ctz64(0x8FED000000000000) = %lld\n",ctz64(FXULONG(0x8FED000000000000)));      // 48
  fxmessage("ctz64(0x8FE0000000000000) = %lld\n",ctz64(FXULONG(0x8FE0000000000000)));      // 53
  fxmessage("ctz64(0x8F00000000000000) = %lld\n",ctz64(FXULONG(0x8F00000000000000)));      // 56
  fxmessage("ctz64(0x8000000000000000) = %lld\n",ctz64(FXULONG(0x8000000000000000)));      // 63
  fxmessage("\n");

  fxmessage("pop32(0x12345678) = %d\n", pop32(0x12345678));                // 13
  fxmessage("ctz64(0x123456789ABCDEF0) = %lld\n", pop64(FXULONG(0x123456789ABCDEF0)));      // 32
  fxmessage("\n");

  //setlocale(LC_ALL,"");
  if(3<=argc){
    const FXchar *num=argv[1];
    const FXchar *fmt=argv[2];
    if(strchr(num,'.') || strchr(num,'e') || strchr(num,'E')){
      FXdouble val=strtod(num,nullptr);
      snprintf(buffer,sizeof(buffer),fmt,val);
      fprintf(stdout,"native: %s\n",buffer);
      __snprintf(buffer,sizeof(buffer),fmt,val);
      fprintf(stdout,"ours  : %s\n",buffer);
      }
    else{
      FXlong val=strtoll(num,nullptr,10);
      snprintf(buffer,sizeof(buffer),fmt,val);
      fprintf(stdout,"native: %s\n",buffer);
      __snprintf(buffer,sizeof(buffer),fmt,val);
      fprintf(stdout,"ours  : %s\n",buffer);
      }
    return 0;
    }

  // Testing int formats
  for(x=0; x<ARRAYNUMBER(intformat); x++){
    for(y=0; y<ARRAYNUMBER(intnumbers); y++){
      __snprintf(buffer,sizeof(buffer),intformat[x],intnumbers[y]);
      fprintf(stdout,"format=\"%s\" output=\"%s\"\n",intformat[x],buffer);
      }
    fprintf(stdout,"\n");
    }

  fprintf(stdout,"\n");

  // 33 is the Magic Number
  __snprintf(buffer,sizeof(buffer),"%0*d",6,33);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n","%0*d",buffer);

  // Testing double formats
  for(x=0; x<ARRAYNUMBER(floatformat); x++){
    for(y=0; y<ARRAYNUMBER(floatnumbers); y++){
      __snprintf(buffer,sizeof(buffer),floatformat[x],floatnumbers[y]);
      fprintf(stdout,"format=\"%s\" output=\"%s\"\n",floatformat[x],buffer);
      }
    fprintf(stdout,"\n");
    }

  for(y=0; y<ARRAYNUMBER(floatnumbers); y++){
    __snprintf(buffer,sizeof(buffer),"0x%016llx",Math::fpBits(floatnumbers[y]));
    fprintf(stdout,"format=\"%s\" output=\"%s\"\n","0x%016llx",buffer);
    }

  fprintf(stdout,"\n");

  // Testing Inf's
  for(x=0; x<ARRAYNUMBER(floatformat); x++){
    __snprintf(buffer,sizeof(buffer),floatformat[x],doubleinf.f);
    fprintf(stdout,"format=\"%s\" output=\"%s\"\n",floatformat[x],buffer);
    }

  fprintf(stdout,"\n");

  // Testing NaN's
  for(x=0; x<ARRAYNUMBER(floatformat); x++){
    __snprintf(buffer,sizeof(buffer),floatformat[x],doublenan.f);
    fprintf(stdout,"format=\"%s\" output=\"%s\"\n",floatformat[x],buffer);
    }

  fprintf(stdout,"\n");

  // Testing positional formats
  for(x=0; x<ARRAYNUMBER(positionalformat); x++){
    __snprintf(buffer,sizeof(buffer),positionalformat[x],10,20,30);
    fprintf(stdout,"format=\"%s\" output=\"%s\"\n",positionalformat[x],buffer);
    }

  fprintf(stdout,"\n");

  __snprintf(buffer,sizeof(buffer),positionalformat2,3.14159265358979,20,10);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n",positionalformat2,buffer);

  __snprintf(buffer,sizeof(buffer),positionalformat3,10,20,30);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n",positionalformat3,buffer);

  fprintf(stdout,"\n");

  // Special cases float formatting
  specialcases("%'.5e");
  specialcases("%'+.5e");
  specialcases("%'+ .5e");
  specialcases("%' .5e");
  specialcases("%'.5f");
  specialcases("%'.5g");

  __snprintf(buffer,sizeof(buffer),"%#.3g",0.0);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n","%#.3g",buffer);

  fprintf(stdout,"\n");

  // Small dernormalized float, regular notation
  __snprintf(buffer,sizeof(buffer),"%.18le",5e-320);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n","%.18le",buffer);
  __snprintf(buffer,sizeof(buffer),"%a",5e-320);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n","%a",buffer);
  fprintf(stdout,"\n");

  // Small dernormalized float, passed as hex (types may be flagged as wrong)
  __snprintf(buffer,sizeof(buffer),"%.18le",doublesmall.f);
  fprintf(stdout,"small format=\"%s\" output=\"%s\"\n","%.18le",buffer);
  __snprintf(buffer,sizeof(buffer),"%a",doublesmall.f);
  fprintf(stdout,"small format=\"%s\" output=\"%s\"\n","%a",buffer);
  fprintf(stdout,"\n");

  // Small dernormalized float, passed as floating point hex syntax
#if defined(__GNUC__)
  __snprintf(buffer,sizeof(buffer),"%.18le",0x0.0000000002788p-1023);
  fprintf(stdout,"hex format=\"%s\" output=\"%s\"\n","%.18le",buffer);

  __snprintf(buffer,sizeof(buffer),"%a",0x0.0000000002788p-1023);
  fprintf(stdout,"hex format=\"%s\" output=\"%s\"\n","%a",buffer);

  fprintf(stdout,"\n");

  // Scan ourselves
  __sscanf("0x0.0000000002788p-1023","%la",&trouble);

  __snprintf(buffer,sizeof(buffer),"%.18le",trouble);
  fprintf(stdout,"scanned hex format=\"%s\" output=\"%s\"\n","%.18le",buffer);

  __snprintf(buffer,sizeof(buffer),"%a",trouble);
  fprintf(stdout,"scanned hex format=\"%s\" output=\"%s\"\n","%a",buffer);

  fprintf(stdout,"\n");
#endif

  __snprintf(buffer,sizeof(buffer),"%.20le",6.4969530541989433e-17);
  fprintf(stdout,"format=\"%s\" output=\"%s\"\n","%.20le",buffer);

  return 0;
  }


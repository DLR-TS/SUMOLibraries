/********************************************************************************
*                                                                               *
*                                 Codecs Tests                                  *
*                                                                               *
*********************************************************************************
* Copyright (C) 2005,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FXKOI8RCodec.h"
#include "FX88592Codec.h"
#include "FXUTF16Codec.h"
#include "FXUTF32Codec.h"
#include "FXUTF8Codec.h"

/*


*/

/*******************************************************************************/

FXKOI8RCodec   koi8_r;
FX88592Codec   iso8859_2;

FXUTF16Codec   utf16;
FXUTF16BECodec utf16be;
FXUTF16LECodec utf16le;

FXUTF8Codec    utf8;

FXUTF32Codec   utf32;
FXUTF32BECodec utf32be;
FXUTF32LECodec utf32le;

const FXschar utf8Len[256]={
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
  };

// UTF8 string of 1, 2, 3, 4, 5, and 6 bytes
const FXchar utfteststring[]="\x7f\xdf\xbf\xef\xbf\xbf\xf7\xbf\xbf\xbf\xfb\xbf\xbf\xbf\xbf\xfd\xbf\xbf\xbf\xbf\xbf";

/*******************************************************************************/

// Turns out this is the best one (when NO branch predict)
static inline void inc3(const FXchar*& ptr){
  FXival c=(FXuchar)*ptr;
  ptr-=((191-c)>>63)+((223-c)>>63)+((239-c)>>63)-1;
  }


static inline FXival utf8len(FXival c){
  return 1-((191-c)>>63)-((223-c)>>63)-((239-c)>>63);
  }


// Turns out this is the best one (when NO branch predict)
// inc3 and inc4 are very close on Zen3
static inline void inc4(const FXchar*& ptr){
  FXuchar c=*ptr;
  ptr+=(((0xE5000000>>((c>>4)<<1))&3)+1);
  }


static inline void inc5(const FXchar*& ptr){
  FXuchar c=*ptr++;
  if(0xC0<=c){ ptr++;
  if(0xE0<=c){ ptr++;
  if(0xF0<=c){ ptr++; }}}
  }


// The best one yet (when branch predict)
static inline void inc6(const FXchar*& ptr){
  if(0xC0<=(FXuchar)*ptr){ if(0xE0<=(FXuchar)*ptr){ if(0xF0<=(FXuchar)*ptr){ ptr++; } ptr++; } ptr++; } ptr++;
  }


// Not horrible but needs table...
static inline void inc7(const FXchar*& ptr){
  ptr+=utf8Len[(FXuchar)*ptr];
  }


// Test roundtrip for codec
void roundtriptest(FXTextCodec *codec){
  FXchar dst[32],src[32];
  FXint i,j;
  FXwchar wc;
  fxmessage("Roundtrip test for: %s..\n",codec->name());
  for(i=0; i<256; i++){
    src[0]=i;
    codec->mb2wc(wc,src,1);
    codec->wc2mb(dst,32,wc);
    j=(FXuchar)dst[0];
    fxmessage("0x%02x -> 0x%04x -> 0x%02x %s\n",i,wc,j,(i!=j)?"BAD":"");
    }
  fxmessage("Roundtrip test done!\n");
  }


// Test utf8 roundtrip for codec
void utf8roundtriptest(FXTextCodec *codec){
  FXchar dst[32],src[32];
  FXint n,i,j;
  fxmessage("UTF8 Roundtrip test for: %s..\n",codec->name());
  for(i=0; i<256; i++){
    src[0]=i;
    memset(dst,0,sizeof(dst));
    n=codec->mb2utf(dst,32,src,1);
    if(n<=0) fxmessage("mb2utf(0x%02x) gave error %d\n",i,n);
    memset(src,0,sizeof(src));
    n=codec->utf2mb(src,32,dst,n);
    if(n<=0) fxmessage("utf2mb(0x%02x) gave error %d\n",i,n);
    j=(FXuchar)src[0];
    if(i!=j) fxmessage("0x%02x -> utf8 -> 0x%02x\n",i,j);
    }
  fxmessage("UTF8 Roundtrip test done!\n");
  }


// Test buffer
static FXchar  buffer[1024];


// Start the whole thing
int main(int argc,char *argv[]){
  const FXchar *ptr;
  FXnchar scratch[32];
  FXnchar hctarcs[32];
  FXwchar w1,w2;
  FXint n,m,s;
  FXint mode=0;

  // Round-trip tests
  fxmessage("**** Testing utf2wcs(wcs2utf(str))\n");

  // Probability distributions
  if(1<argc){
    mode=strtoul(argv[1],nullptr,0);
    }

  // Test utf for codes of 4 bytes
  fxmessage("4-byte conversions\n");
  for(w1=0x10000; w1<0x110000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    n=utf2wcs(&w2,buffer,1,m);
    if(w1!=w2 || m!=4 || n!=1) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  // Test utf for codes of 3 bytes
  fxmessage("3-byte conversions\n");
  for(w1=0x800; w1<0x10000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    n=utf2wcs(&w2,buffer,1,m);
    if(w1!=w2 || m!=3 || n!=1) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  // Test utf for codes of 2 bytes
  fxmessage("2-byte conversions\n");
  for(w1=0x80; w1<0x800; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    n=utf2wcs(&w2,buffer,1,m);
    if(w1!=w2 || m!=2 || n!=1) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  // Test utf for codes of 1 byte
  fxmessage("1-byte conversions\n");
  for(w1=0; w1<0x80; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    n=utf2wcs(&w2,buffer,1,m);
    if(w1!=w2 || m!=1 || n!=1) fxmessage("%06X: Problem: %06X m=%d n=%d\n",w1,w2,m,n);
    }

  // Round-trip tests
  fxmessage("**** Testing utf2ncs(ncs2utf(str))\n");

  // Test utf16 for codes of 2 words
  fxmessage("1-word and 2-word conversions\n");
  for(w1=0; w1<0x110000; w1++){
    s=wc2nc(scratch,w1);
    m=ncs2utf(buffer,scratch,4,s);
    n=utf2ncs(hctarcs,buffer,2,m);
    if(n==1 && (scratch[0]!=hctarcs[0])) fxmessage("%06X: Problem: %04X s=%d n=%d\n",w1,hctarcs[0],s,n);
    if(n==2 && (scratch[0]!=hctarcs[0] || scratch[1]!=hctarcs[1])) fxmessage("%06X: Problem: %04X %04X s=%d n=%d\n",w1,hctarcs[0],hctarcs[1],s,n);
    }

  fxmessage("**** Testing wcs2utf(wc)\n");

  // More tests
  fxmessage("4-byte conversions\n");
  for(w1=0x10000; w1<0x110000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    w2=((FXuchar)buffer[0]<<18) ^ ((FXuchar)buffer[1]<<12) ^ ((FXuchar)buffer[2]<<6) ^ (FXuchar)buffer[3] ^ 0x3C82080;
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  // Test utf for codes of 3 bytes
  fxmessage("3-byte conversions\n");
  for(w1=0x800; w1<0x10000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    w2=((FXuchar)buffer[0]<<12) ^ ((FXuchar)buffer[1]<<6) ^ (FXuchar)buffer[2] ^ 0x0E2080;
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  // Test utf for codes of 2 bytes
  fxmessage("2-byte conversions\n");
  for(w1=0x80; w1<0x800; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    w2=((FXuchar)buffer[0]<<6) ^ (FXuchar)buffer[1] ^ 0x3080;
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  // Test utf for codes of 1 byte
  fxmessage("1-byte conversions\n");
  for(w1=0; w1<0x80; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    w2=(FXuchar)buffer[0];
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    }

  fxmessage("**** Testing WCINC(ptr)\n");

  // Forward step tests
  fxmessage("4-byte step forward\n");
  for(w1=0x10000; w1<0x110000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer;
    w2=wcnxt(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer+4!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer+4,ptr);
    }
  fxmessage("3-byte step forward\n");
  for(w1=0x800; w1<0x10000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer;
    w2=wcnxt(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer+3!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer+3,ptr);
    }

  fxmessage("2-byte step forward\n");
  for(w1=0x80; w1<0x800; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer;
    w2=wcnxt(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer+2!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer+2,ptr);
    }

  fxmessage("1-byte step forward\n");
  for(w1=0; w1<0x80; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer;
    w2=wcnxt(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer+1!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer+1,ptr);
    }

  fxmessage("**** Testing WCDEC(ptr)\n");

  // Backward step tests
  fxmessage("4-byte step backward\n");
  for(w1=0x10000; w1<0x110000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer+4;
    w2=wcprv(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer,ptr);
    }

  fxmessage("3-byte step backward\n");
  for(w1=0x800; w1<0x10000; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer+3;
    w2=wcprv(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer,ptr);
    }

  fxmessage("2-byte step backward\n");
  for(w1=0x80; w1<0x800; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer+2;
    w2=wcprv(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer,ptr);
    }

  fxmessage("1-byte step backward\n");
  for(w1=0; w1<0x80; w1++){
    m=wcs2utf(buffer,&w1,4,1);
    ptr=buffer+1;
    w2=wcprv(ptr);
    if(w1!=w2) fxmessage("%06X: Problem: %06X\n",w1,w2);
    if(buffer!=ptr) fxmessage("%06X: Problem: %p!=%p\n",w1,buffer,ptr);
    }

  fxmessage("**** Performance tests\n");
  FXRandom rnd(FXThread::time());
  FXArray<FXchar> data('\0',100000000);
  FXchar* head=data.data();
  FXchar* tail=data.data()+data.no();
  FXchar* p=head;
  FXTime beg,end;
  long count,sum;

  fxmessage("ARRAY: head=%p tail=%p\n",head,tail);

  count=0;
  sum=0;
  while(p+4<=tail){
    if(mode==0){        // Uniformly 1,2,3, or 4
      w1=rnd.randLong()&3;
      switch(w1){
        case 0:
          w1=rnd.randLong()%0x80;
          break;
        case 1:
          w1=0x80+rnd.randLong()%(0x800-0x80);
          break;
        case 2:
          w1=0x800+rnd.randLong()%(0x10000-0x800);
          break;
        case 3:
          w1=0x10000+rnd.randLong()%(0x110000-0x10000);
          break;
        }
      }
    else if(mode==1){   // Up to 1
      w1=rnd.randLong()%0x80;
      }
    else if(mode==2){   // Up to 2
      w1=rnd.randLong()%0x800;
      }
    else if(mode==3){   // Up to 3
      w1=rnd.randLong()%0x10000;
      }
    else if(mode==4){   // Up to 4
      w1=rnd.randLong()%0x110000;
      }
    p+=wc2utf(p,w1);
    sum+=w1;
    count++;
    }
  fxmessage("ARRAY: count=%'ld sum=%'ld end=%p\n",count,sum,p);

  fxmessage("**** wcinc: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    sum+=wcnxt(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("wcinc: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("wcinc: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** wcdec: move backward\n");
  count=0;
  sum=0;
  beg=FXThread::ticks();
  do{
    sum+=wcprv(ptr);
    count++;
    }
  while(head<ptr);
  end=FXThread::ticks();
  fxmessage("wcdec: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("wcdec: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** wcnxt: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    wcnxt(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("wcnxt: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("wcnxt: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** wcprv: move backward\n");
  count=0;
  sum=0;
  beg=FXThread::ticks();
  do{
    wcprv(ptr);
    count++;
    }
  while(head<ptr);
  end=FXThread::ticks();
  fxmessage("wcprv: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("wcprv: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** inc3: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    inc3(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("inc3: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("inc3: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** inc4: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    inc4(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("inc4: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("inc4: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** inc5: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    inc5(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("inc5: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("inc5: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** inc6: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    inc6(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("inc6: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("inc6: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** inc7: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    inc7(ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("inc7: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("inc7: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);

  fxmessage("**** lenUTF8: move forward\n");
  count=0;
  sum=0;
  ptr=head;
  beg=FXThread::ticks();
  do{
    ptr+=lenUTF8((FXuchar)*ptr);
    count++;
    }
  while(ptr+4<=tail);
  end=FXThread::ticks();
  fxmessage("lenUTF8: count=%'ld sum=%'ld end=%p\n",count,sum,ptr);
  fxmessage("lenUTF8: %'ld ticks %'ld chars %.2lf ticks/char\n",(long)(end-beg),count,(FXdouble)(end-beg)/count);


/*
//    n=wc2utfs((FXchar*)buf,128,&wc,1);
//    utf2wcs(&ww,1,(const FXchar*)buf,n);
  const FXwchar *wcdec;
  FXwchar wc,ww,vv;
  FXint i,n,dec,num1,num2,num3,numt;
  FXString value,norm;
  FXchar buf[128];
  FXchar input[256];
//utf8codingtest();
//utf16codingtest();
  // Test codecs
//  roundtriptest(&koi8_r);
//  roundtriptest(&iso8859_2);

//  utf8roundtriptest(&koi8_r);
//  utf8roundtriptest(&iso8859_2);

  wc=0x7fffffff;
  wc=0x3ffffff;
  wc=0x1fffff;
  wc=0xffff;
  wc=0x7ff;
  wc=0x7f;
  value.assign(&wc,1);
  switch(value.length()){
    case 1: printf("%06X: \\x%02x\n",wc,(FXuchar)value[0]); break;
    case 2: printf("%06X: \\x%02x\\x%02x\n",wc,(FXuchar)value[0],(FXuchar)value[1]); break;
    case 3: printf("%06X: \\x%02x\\x%02x\\x%02x\n",wc,(FXuchar)value[0],(FXuchar)value[1],(FXuchar)value[2]); break;
    case 4: printf("%06X: \\x%02x\\x%02x\\x%02x\\x%02x\n",wc,(FXuchar)value[0],(FXuchar)value[1],(FXuchar)value[2],(FXuchar)value[3]); break;
    case 5: printf("%06X: \\x%02x\\x%02x\\x%02x\\x%02x\\x%02x\n",wc,(FXuchar)value[0],(FXuchar)value[1],(FXuchar)value[2],(FXuchar)value[3],(FXuchar)value[4]); break;
    case 6: printf("%06X: \\x%02x\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x\n",wc,(FXuchar)value[0],(FXuchar)value[1],(FXuchar)value[2],(FXuchar)value[3],(FXuchar)value[4],(FXuchar)value[5]); break;
    }
  value=utfteststring;
  i=0;
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  printf("inc(%d)=",i); printf("%d\n",value.inc(i));
  exit(0);

  while(fgets(input,sizeof(input),stdin)){
    input[strlen(input)-1]=0;
//    value=unescape(input);
    value=input;
    printf("in : \"%s\"\n",escape(value).text());
    value=fromAscii(value);
    printf("org: \"%s\"\n",escape(value).text());
    value=decompose(value,DecCanonical);
    printf("dec: \"%s\"\n",escape(value).text());
    value=toAscii(value);
    printf("out: \"%s\"\n",value.text());
    }
*/

//  exit(0);

/*
  // Test lengths
  for(wc=0; wc<0x110000; wc++){
    num1=utfslen(&wc,1);
    ww=Unicode::toUpper(wc);
    vv=Unicode::toLower(wc);
    num2=utfslen(&ww,1);
    num3=utfslen(&vv,1);
    printf("%06x: %2d %2d %2d %s\n",wc,num1,num2,num3,((num1==num2) && (num1==num3))?"":"differ!");
    }

  exit(0);
*/


/*
  // Test decompose table
  for(wc=num1=num2=numt=0; wc<=0x110000; wc++){
    dec=Unicode::decomposeType(wc);
    cc=Unicode::charCombining(wc);
    if(dec==DecCanonical && cc==0){
      wcdec=Unicode::charDecompose(wc);
      if(1<wcdec[-1]){
        if(wcdec[-1]==1) num1++;
        if(wcdec[-1]==2) num2++;
        numt++;
        printf("%04X: cc=%2d (%d): ",wc,cc,wcdec[-1]);
        for(i=0; i<wcdec[-1]; i++){
          printf("%04X ",wcdec[i]);
          }
        printf(" comp=%04X ",Unicode::charCompose(wcdec[0],wcdec[1]));
        printf("\n");
        }
      }
    }
  printf("num1=%d num2=%d numt=%d",num1,num2,numt);
*/
/*
  for(wc=0; wc<0x110000; wc++){
    value.assign(&wc,1);
    norm=decompose(value,1);
    printf("%04X: (%2d): ",wc,norm.length());
    for(i=0; i<value.length(); i++){
      printf("%02X ",(FXuchar)value[i]);
      //printf("%c",(FXuchar)value[i]);
      }
    printf(" -> ");
    for(i=0; i<norm.length(); i++){
      printf("%02X ",(FXuchar)norm[i]);
      //printf("%c",(FXuchar)norm[i]);
      }
    printf("\n");
    }
*/

  return 1;
  }

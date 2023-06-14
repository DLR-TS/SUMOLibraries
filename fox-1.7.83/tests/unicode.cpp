/********************************************************************************
*                                                                               *
*                        Unicode  Character  Database  Tests                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 2011,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



const FXchar *const category[]={
  "Cn",
  "Cc",
  "Cf",
  "Cs",
  "Co",
  "Mn",
  "Mc",
  "Me",
  "Zs",
  "Zl",
  "Zp",
  "Lu",
  "Ll",
  "Lt",
  "Lm",
  "Lo",
  "Nl",
  "Nd",
  "No",
  "Pc",
  "Pd",
  "Ps",
  "Pe",
  "Pi",
  "Pf",
  "Po",
  "Sm",
  "Sc",
  "Sk",
  "So"
  };


const FXchar *const direction[]={
  "L",
  "LRE",
  "LRO",
  "R",
  "AL",
  "RLE",
  "RLO",
  "PDF",
  "EN",
  "ES",
  "ET",
  "AN",
  "CS",
  "NSM",
  "BN",
  "B",
  "S",
  "WS",
  "ON"
  };

const FXchar *const linebreak[]={
  "XX",

  "BK",
  "CR",
  "LF",
  "CM",
  "NL",
  "SG",
  "WJ",
  "ZW",
  "GL",
  "SP",

  "B2",
  "BA",
  "BB",
  "HY",
  "CB",

  "CL",
  "CP",
  "EX",
  "IN",
  "NS",
  "OP",
  "QU",

  "IS",
  "NU",
  "PO",
  "PR",
  "SY",

  "AI",
  "AL",
  "H2",
  "H3",
  "ID",

  "JL",
  "JV",
  "JT",
  "SA"
  };


const FXchar *const scriptcode[]={
  "Zzzz",
  "Zyyy",
  "Zinh",
  "Latn",
  "Grek",
  "Cyrl",
  "Armn",
  "Hebr",
  "Arab",
  "Syrc",
  "Thaa",
  "Deva",
  "Beng",
  "Guru",
  "Gujr",
  "Orya",
  "Taml",
  "Telu",
  "Knda",
  "Mlym",
  "Sinh",
  "Thai",
  "Laoo",
  "Tibt",
  "Mymr",
  "Geor",
  "Hang",
  "Ethi",
  "Cher",
  "Cans",
  "Ogam",
  "Runr",
  "Khmr",
  "Mong",
  "Hira",
  "Kana",
  "Bopo",
  "Hani",
  "Yiii",
  "Ital",
  "Goth",
  "Dsrt",
  "Tglg",
  "Hano",
  "Buhd",
  "Tagb",
  "Limb",
  "Tale",
  "Linb",
  "Ugar",
  "Shaw",
  "Osma",
  "Cprt",
  "Brai",
  "Bugi",
  "Copt",
  "Talu",
  "Glag",
  "Tfng",
  "Sylo",
  "Xpeo",
  "Khar",
  "Bali",
  "Xsux",
  "Phnx",
  "Phag",
  "Nkoo",
  "Sund",
  "Lepc",
  "Olck",
  "Vaii",
  "Saur",
  "Kali",
  "Rjng",
  "Lyci",
  "Cari",
  "Lydi",
  "Cham",
  "Lana",
  "Tavt",
  "Avst",
  "Egyp",
  "Samr",
  "Lisu",
  "Bamu",
  "Java",
  "Mtei",
  "Armi",
  "Sarb",
  "Prti",
  "Phli",
  "Orkh",
  "Kthi",
  "Batk",
  "Brah",
  "Mand"
  };

const FXchar *const joiningtype[]={
  "NJ",
  "RJ",
  "DJ",
  "JC",
  "LJ",
  "JT"
  };


const FXchar *const decomposetype[]={
  "None",
  "Cann",
  "Comp",
  "Font",
  "NBrk",
  "Init",
  "Medi",
  "Fina",
  "Isol",
  "Circ",
  "Supr",
  "Sub",
  "Vert",
  "Wide",
  "Narr",
  "Smll",
  "Sqre",
  "Frac"
  };


// Start the whole thing
int main(int,char**){
  FXchar buffer[2048];
  FXint len;
  FXwchar uc;
  FXchar ch;

  // Dump character properties
  for(uc=0; uc<0x110000; ++uc){
    if(uc%32==0){
      printf("\nCode\tChar\tCat\tDir\tBreak\tValue\tScript\tComb\tJoin\tDecom\tNumdec\tCase\n");
      printf("============================================================================================\n");
      }
    printf("%04X\t",uc);
    if(' '<=uc){
      len=wc2utf(buffer,uc); buffer[len]=0;
      printf("%s\t",buffer);
      }
    else{
      printf(" \t");
      }
    printf("%2s\t",category[Unicode::charCategory(uc)]);
    printf("%3s\t",direction[Unicode::charDirection(uc)]);
    printf("%2s\t",linebreak[Unicode::lineBreakType(uc)]);
    if(0<=Unicode::digitValue(uc)){
      printf("%2d\t",Unicode::digitValue(uc));
      }
    else{
      printf("--\t");
      }
    printf("%4s\t",scriptcode[Unicode::scriptType(uc)]);
    printf("%4d\t",Unicode::charCombining(uc));
    printf("%3s\t",joiningtype[Unicode::joiningType(uc)]);
    printf("%4s\t",decomposetype[Unicode::decomposeType(uc)]);
    printf("%2d\t",Unicode::charNumDecompose(uc));
    ch='-';
    if(Unicode::isUpper(uc)) ch='U';
    else if(Unicode::isLower(uc)) ch='L';
    else if(Unicode::isTitle(uc)) ch='T';
    printf("%c\t",ch);
    printf("\n");
    }
  return 1;
  }

/********************************************************************************
*                                                                               *
*                             Half Float Support Test                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 2008,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"

/*
  Notes:
  - Tests for FXhalf.
*/

#define HALFMINDEN 0x0001       // Smallest denormalized
#define HALFMAXDEN 0x03FF       // Largest denormalized
#define HALFMIN    0x0400       // Minimum normalized
#define HALFMAX    0x7BFF       // Maximum normalized
#define HALFEXP    0x7C00       // Mask exponent
#define HALFMNT    0x03FF       // Mask mantissa
#define HALFSGN    0x8000       // Mask sign
#define HALFABS    0x7FFF       // Mask exponent and mantissa
#define HALFINF    0x7C00       // Infinite

/*******************************************************************************/

union CNV {
  FXuint  u;
  FXfloat f;
  };

// Initial half data
FXhalf  initial[65536];

// Converted from half
FXfloat converted[65536];

// Converted back from float
FXhalf  convertedback[65536];


// Start
int main(int argc,char* argv[]){
  FXint i;
  CNV c;

  // Fill initial half data
  for(i=0; i<65536; ++i){ *((FXushort*)&initial[i])=i; }

  // Convert to float
  for(i=0; i<65536; ++i){ converted[i]=initial[i]; }

  // Convert back to half
  for(i=0; i<65536; ++i){ convertedback[i]=converted[i]; }

  // Now check it
  for(i=0; i<65536; ++i){
    c.f=converted[i];
    printf("i=%-5d [0x%04x] FLOAT=%+22.15lf [0x%08x] %s EXP=%-+4d MAN=0x%08x",i,i,c.f,c.u,((c.u>>31)&1)?"NEG":"POS",((c.u>>23)&255)-127,c.u&0x007fffff);
    if((i&HALFABS)<HALFMIN){
      printf(" DEN");
      }
    else if((i&HALFABS)>HALFMAX){
      if((i&HALFABS)>HALFINF) printf(" NaN"); else printf(" INF");
      }
    else{
      printf(" NOR");
      }
    if(initial[i]==convertedback[i]){
      printf(" OK");
      }
    printf("\n");
    }
  return 0;
  }


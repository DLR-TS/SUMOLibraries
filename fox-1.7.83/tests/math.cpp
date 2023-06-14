/********************************************************************************
*                                                                               *
*                              M a t h   T e s t                                *
*                                                                               *
*********************************************************************************
* Copyright (C) 2007,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"


/*
  Notes:
  - Test battery for FX::Math functions.
*/

/*******************************************************************************/

// To pick apart a IEEE754 double
union DP {
  FXdouble f;
  FXulong  u;
  };


// To pick apart a IEEE754 float
union SP {
  FXfloat f;
  FXuint  u;
  };


// Functions
typedef FXdouble (*DBLFUN1)(FXdouble);
typedef FXfloat  (*FLTFUN1)(FXfloat);


// A pair of float funs
struct FLTFUNS {
  FLTFUN1 tf;
  FLTFUN1 rf;
  };


// And pair of double funs
struct DBLFUNS {
  DBLFUN1 tf;
  DBLFUN1 rf;
  };


// IEEE754 number patterns
enum {
  FORBID_NEG =  1,      // Forbid negative values
  FORBID_INF =  2,      // Forbid infinity
  FORBID_NAN =  4,      // Forbid NaN
  FORBID_DEN =  8,      // Forbid denormals
  FORCE_NEG  = 16,      // Force negative values
  FORCE_NAN  = 32,      // Force inifinity or NaN
  FORCE_DEN  = 64       // Force denormals
  };


// Keeping track of accuracy
struct DPAccuracy {
  FXdouble worstabserr;         // Worst absolute error
  FXdouble worstabsarg;         // Argument passed to function
  FXdouble worstabsres;         // Desired result
  FXdouble worstabsact;         // Actual value returned
  FXdouble worstrelerr;         // Worst relative error
  FXdouble worstrelarg;         // Argument passed to function
  FXdouble worstrelres;         // Desired result
  FXdouble worstrelact;         // Actual value returned
  FXulong  errcount;            // Total number of errors
  };


// Keeping track of accuracy
struct SPAccuracy {
  FXfloat worstabserr;          // Worst absolute error
  FXfloat worstabsarg;          // Argument passed to function
  FXfloat worstabsres;          // Desired result
  FXfloat worstabsact;          // Actual value returned
  FXfloat worstrelerr;          // Worst relative error
  FXfloat worstrelarg;          // Argument passed to function
  FXfloat worstrelres;          // Desired result
  FXfloat worstrelact;          // Actual value returned
  FXulong errcount;             // Total number of errors
  };


// Function names
static const char *const names[]={
  "fabs",
  "ceil",
  "floor",
  "round",
  "trunc",
  "nearbyint",
  "rint",
  "sinh",
  "cosh",
  "tanh"
  "asinh",
  "acosh",
  "atanh",
  "expm1",
  "exp2",
  "exp10",
  "log1p",
  "log2"
  };



// log1p()
#if defined(NO_LOG1P)
FXdouble log1p(FXdouble x){ return ::log(1.0+x); }
#endif

// log1pf()
#if defined(NO_LOG1PF)
FXfloat log1pf(FXfloat x){ return ::log(1.0f+x); }
#endif


// exp2()
#if defined(NO_EXP2)
FXdouble expm1(FXdouble x){ return ::exp(x)-1.0; }
#endif

// exp2f()
#if defined(NO_EXP2F)
FXfloat expm1f(FXfloat x){ return ::expf(x)-1.0f; }
#endif


// exp2()
#if defined(NO_EXP2)
FXdouble exp2(FXdouble x){ return ::pow(2.0,x); }
#endif

// exp2f()
#if defined(NO_EXP2F)
FXfloat exp2f(FXfloat x){ return ::powf(2.0f,x); }
#endif


// exp10()
#if defined(NO_EXP10)
FXdouble exp10(FXdouble x){ return ::pow(10.0,x); }
#endif

// exp10f()
#if defined(NO_EXP10F)
FXfloat exp10f(FXfloat x){ return ::powf(10.0f,x); }
#endif


// log2()
#if defined(NO_EXP10)
FXdouble log2(FXdouble x){ return ::log(x)*1.442695040888963407359924681001892137; }
#endif

// log2f()
#if defined(NO_EXP10F)
FXfloat log2f(FXfloat x){ return ::log(x)*1.442695040888963407359924681001892137f; }
#endif


// Double function list
static const DBLFUNS dblfuns[]={
  {Math::fabs,      fabs},
  {Math::ceil,      ceil},
  {Math::floor,     floor},
  {Math::round,     round},
  {Math::trunc,     trunc},
  {Math::nearbyint, nearbyint},
  {Math::rint,      rint},
  {Math::sinh,      sinh},
  {Math::cosh,      cosh},
  {Math::tanh,      tanh},
  {Math::asinh,     asinh},
  {Math::acosh,     acosh},
  {Math::atanh,     atanh},
  {Math::expm1,     expm1},
  {Math::exp2,      exp2},
  {Math::exp10,     exp10},
  {Math::log1p,     log1p},
  {Math::log2,      log2},
  };


// Float function list
static const FLTFUNS fltfuns[]={
  {Math::fabs,      fabsf},
  {Math::ceil,      ceilf},
  {Math::floor,     floorf},
  {Math::round,     roundf},
  {Math::trunc,     truncf},
  {Math::nearbyint, nearbyintf},
  {Math::rint,      rintf},
  {Math::sinh,      sinhf},
  {Math::cosh,      coshf},
  {Math::tanh,      tanhf},
  {Math::asinh,     asinhf},
  {Math::acosh,     acoshf},
  {Math::atanh,     atanhf},
  {Math::expm1,     expm1f},
  {Math::exp2,      exp2f},
  {Math::exp10,     exp10f},
  {Math::log1p,     log1pf},
  {Math::log2,      log2f},
  };


// Random source
FXRandom rng;

/*******************************************************************************/

// Find function with name
FXint findFunc(const char* name){
  for(FXuint i=0; i<ARRAYNUMBER(names); ++i){
    if(strcmp(name,names[i])==0) return i;
    }
  return -1;
  }


// Record result of res=f(arg), which should have been act
void record(DPAccuracy& acc,FXdouble arg,FXdouble act,FXdouble res){
  FXdouble err,mag,abserr,relerr;
  err=(act-res);
  if(err!=0.0){
    abserr=(err<0.0)?-err:err;
    if(acc.worstabserr<abserr){         // Record worst absolute error in results
      acc.worstabserr=abserr;
      acc.worstabsarg=arg;
      acc.worstabsres=res;
      acc.worstabsact=act;
      }
    mag=(res<0.0)?-res:res;
    if(0.0<mag){
      relerr=abserr/mag;
      if(acc.worstrelerr<relerr){       // Record worst relative error in results
        acc.worstrelerr=relerr;
        acc.worstrelarg=arg;
        acc.worstrelres=res;
        acc.worstrelact=act;
        }
      }
    acc.errcount++;
    }
  }


// Record result of res=f(arg), which should have been act
void record(SPAccuracy& acc,FXfloat arg,FXfloat act,FXfloat res){
  FXfloat err,mag,abserr,relerr;
  err=(act-res);
  if(err!=0.0f){
    abserr=(err<0.0f)?-err:err;
    if(acc.worstabserr<abserr){         // Record worst absolute error in results
      acc.worstabserr=abserr;
      acc.worstabsarg=arg;
      acc.worstabsres=res;
      acc.worstabsact=act;
      }
    mag=(res<0.0f)?-res:res;
    if(0.0f<mag){
      relerr=abserr/mag;
      if(acc.worstrelerr<relerr){       // Record worst relative error in results
        acc.worstrelerr=relerr;
        acc.worstrelarg=arg;
        acc.worstrelres=res;
        acc.worstrelact=act;
        }
      }
    acc.errcount++;
    }
  }

static const char sign[2]={'+','-'};

// Print a double, disassembled
void printDP(FXdouble x){
  fprintf(stderr,"0x%016lx (%c)(% 4ld)(0x%014lx) -> %+.17lg\n",Math::fpBits(x),sign[Math::fpSign(x)],Math::fpExponent(x),Math::fpMantissa(x),x);
  }


// Print a double, disassembled
void printSP(FXfloat x){
  fprintf(stderr,"0x%08x (%c)(% 3d)(0x%06x) -> %+.17lg\n",Math::fpBits(x),sign[Math::fpSign(x)],Math::fpExponent(x),Math::fpMantissa(x),(FXdouble)x);
  }


// Report errors found during tests
void report(const DPAccuracy& acc,const char* name){
  if(!acc.errcount) return;
  fprintf(stderr,"Worst Absolute: %s(%- 23.17lg): %- 23.17lg expected: %- 23.17lg err=%- 23.17lg\n",name,acc.worstabsarg,acc.worstabsact,acc.worstabsres,acc.worstabserr);
  fprintf(stderr,"arg: "); printDP(acc.worstabsarg);
  fprintf(stderr,"act: "); printDP(acc.worstabsact);
  fprintf(stderr,"res: "); printDP(acc.worstabsres);
  fprintf(stderr,"Worst Relative: %s(%- 23.17lg): %- 23.17lg expected: %- 23.17lg err=%- 23.17lg\n",name,acc.worstrelarg,acc.worstrelact,acc.worstrelres,acc.worstrelerr);
  fprintf(stderr,"arg: "); printDP(acc.worstrelarg);
  fprintf(stderr,"act: "); printDP(acc.worstrelact);
  fprintf(stderr,"res: "); printDP(acc.worstrelres);
  fprintf(stderr,"total: %ld\n",acc.errcount);
  }


// Report errors found during tests
void report(const SPAccuracy& acc,const char* name){
  if(!acc.errcount) return;
  fprintf(stderr,"Worst Absolute: %s(%- 23.17lg): %- 23.17lg expected: %- 23.17lg err=%- 23.17lg\n",name,(FXdouble)acc.worstabsarg,(FXdouble)acc.worstabsact,(FXdouble)acc.worstabsres,(FXdouble)acc.worstabserr);
  fprintf(stderr,"arg: "); printSP(acc.worstabsarg);
  fprintf(stderr,"act: "); printSP(acc.worstabsact);
  fprintf(stderr,"res: "); printSP(acc.worstabsres);
  fprintf(stderr,"Worst Relative: %s(%- 23.17lg): %- 23.17lg expected: %- 23.17lg err=%- 23.17lg\n",name,(FXdouble)acc.worstrelarg,(FXdouble)acc.worstrelact,(FXdouble)acc.worstrelres,(FXdouble)acc.worstrelerr);
  fprintf(stderr,"arg: "); printSP(acc.worstrelarg);
  fprintf(stderr,"act: "); printSP(acc.worstrelact);
  fprintf(stderr,"res: "); printSP(acc.worstrelres);
  fprintf(stderr,"total: %ld\n",acc.errcount);
  }


// FIXME instead, random draws + or- and and-mask.
// FIXME will allow for testing special patterns easily.

// FIXME this is wrong; UPDATE THIS!

// Make a double number given some constraints
FXdouble makeDouble(FXuint flags){
  DP z;
  z.u=(FXulong)rng.randLong();
  if(flags){
    FXulong sx=z.u&FXULONG(0x8000000000000000);
    FXulong mx=z.u&FXULONG(0x7fffffffffffffff);
    if(flags&FORCE_NEG){sx=FXULONG(0x8000000000000000);}                                        // Force negative
    if(flags&FORCE_NAN){mx|=FXULONG(0x7ff0000000000000);}                                       // Force NaN
    if(flags&FORCE_DEN){mx=(mx&FXULONG(0x000fffffffffffff))|FXULONG(0x0010000000000000);}       // Force denormal
    if(flags&FORBID_NEG){sx=0;}                                                                 // No negative
    if((flags&FORBID_DEN)&&(mx<FXULONG(0x0010000000000000))){mx=0;}                             // No denormal
    if((flags&FORBID_NAN)&&(mx>FXULONG(0x7ff0000000000000))){mx&=FXULONG(0x7ff0000000000000);}  // No NaN
    if((flags&FORBID_INF)&&(mx==FXULONG(0x7ff0000000000000))){mx-=1;}                           // No infinite
    z.u=sx|mx;
    }
  return z.f;
  }


// Test one: zero
void testDoubleZero(DPAccuracy& acc,DBLFUN1 tf,DBLFUN1 rf){
  FXdouble res,act;
  act=tf(0.0);
  res=rf(0.0);
  record(acc,0.0,act,res);
  act=tf(-0.0);
  res=rf(-0.0);
  record(acc,-0.0,act,res);
  }


// Test two: Infinity
void testDoubleInfinity(DPAccuracy& acc,DBLFUN1 tf,DBLFUN1 rf){
  FXdouble res,act;
  DP neginf,posinf;
  neginf.u=FXULONG(0xfff0000000000000);
  posinf.u=FXULONG(0x7ff0000000000000);
  act=tf(neginf.f);
  res=rf(neginf.f);
  record(acc,neginf.f,act,res);
  act=tf(posinf.f);
  res=rf(posinf.f);
  record(acc,posinf.f,act,res);
  }


// Test tree: NaN
void testDoubleNaN(DPAccuracy& acc,DBLFUN1 tf,DBLFUN1 rf,FXulong cov){
  FXdouble arg,res,act;
  while(cov-->0){
    arg=makeDouble(FORCE_NAN);
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    }
  }


// Test tree: denormals
void testDoubleDenorm(DPAccuracy& acc,DBLFUN1 tf,DBLFUN1 rf,FXulong cov){
  FXdouble arg,res,act;
  while(cov-->0){
    arg=makeDouble(FORCE_DEN);
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    }
  }


// Test tree: denormals
void testFloatDenorm(SPAccuracy& acc,FLTFUN1 tf,FLTFUN1 rf,FXulong){
  FXfloat arg,res,act;
  SP z;
  for(z.u=0x00000001; z.u<0x00800000; z.u++){
    arg=z.f;
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    z.u|=0x80000000;
    arg=z.f;
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    z.u&=0x7fffffff;
    }
  }


// Test four: any
void testDoubleFinite(DPAccuracy& acc,DBLFUN1 tf,DBLFUN1 rf,FXulong cov){
  FXdouble arg,res,act;
  while(cov-->0){
    arg=makeDouble(FORBID_NAN|FORBID_INF);
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    }
  }


// Test all for float
void testFloatAll(SPAccuracy& acc,FLTFUN1 tf,FLTFUN1 rf){
  FXfloat arg,res,act;
  SP z;
  for(z.u=0x00000000; z.u<=0x7fffffff; z.u++){
    arg=z.f;
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    z.u|=0x80000000;
    arg=z.f;
    act=tf(arg);
    res=rf(arg);
    record(acc,arg,act,res);
    z.u&=0x7fffffff;
    }
  }


// Test all
void testDouble(DBLFUN1 tf,DBLFUN1 rf,const char* name){
  DPAccuracy acc;
  fprintf(stderr,"Testing %s():\n",name);
  acc.worstabserr=0.0;
  acc.worstabsarg=0.0;
  acc.worstabsres=0.0;
  acc.worstabsact=0.0;
  acc.worstrelerr=0.0;
  acc.worstrelarg=0.0;
  acc.worstrelres=0.0;
  acc.worstrelact=0.0;
  acc.errcount=0;
  testDoubleZero(acc,tf,rf);
  testDoubleInfinity(acc,tf,rf);
  testDoubleNaN(acc,tf,rf,1000000000);
  testDoubleFinite(acc,tf,rf,1000000000);
  report(acc,name);
  fprintf(stderr,"\n");
  }


// Test all
void testFloat(FLTFUN1 tf,FLTFUN1 rf,const char* name){
  SPAccuracy acc;
  fprintf(stderr,"Testing %s():\n",name);
  acc.worstabserr=0.0f;
  acc.worstabsarg=0.0f;
  acc.worstabsres=0.0f;
  acc.worstabsact=0.0f;
  acc.worstrelerr=0.0f;
  acc.worstrelarg=0.0f;
  acc.worstrelres=0.0f;
  acc.worstrelact=0.0f;
  acc.errcount=0;
  testFloatAll(acc,tf,rf);
  report(acc,name);
  fprintf(stderr,"\n");
  }


// Start
int main(int argc,char *argv[]){
  rng.seed(FXThread::time());
  FXdouble dbl_arg,dbl_res,dbl_act;
  FXfloat  flt_arg,flt_res,flt_act;
  FXint    func;

  if(3<argc){
    func=findFunc(argv[1]);
    if(func<0){
      fprintf(stderr,"Unknown function: %s\n",argv[1]);
      exit(1);
      }
    if(argv[2][0]=='d'){
      dbl_arg=strtod(argv[3],nullptr);
      dbl_act=dblfuns[func].tf(dbl_arg);
      dbl_res=dblfuns[func].rf(dbl_arg);
      fprintf(stderr,"%s()\n",argv[1]);
      fprintf(stderr,"arg: ");
      printDP(dbl_arg);
      fprintf(stderr,"act: ");
      printDP(dbl_act);
      fprintf(stderr,"res: ");
      printDP(dbl_res);
      }
    else{
      flt_arg=strtof(argv[3],nullptr);
      flt_act=fltfuns[func].tf(flt_arg);
      flt_res=fltfuns[func].rf(flt_arg);
      fprintf(stderr,"%s()\n",argv[1]);
      fprintf(stderr,"arg: ");
      printSP(flt_arg);
      fprintf(stderr,"act: ");
      printSP(flt_act);
      fprintf(stderr,"res: ");
      printSP(flt_res);
      }
    return 0;
    }
/*
*/

  // Hyperbolic sine, cosine, tangent
  testDouble(Math::sinh,sinh,"sinh");
  testFloat(Math::sinh,sinhf,"sinhf");
  testDouble(Math::cosh,cosh,"cosh");
  testFloat(Math::cosh,coshf,"coshf");
  testDouble(Math::tanh,tanh,"tanh");
  testFloat(Math::tanh,tanhf,"tanhf");

  // Exp
  testDouble(Math::expm1,expm1,"expm1");
  testFloat(Math::expm1,expm1f,"expm1f");
  testDouble(Math::exp2,exp2,"exp2");
  testFloat(Math::exp2,exp2f,"exp2f");
  testDouble(Math::exp10,exp10,"exp10");
  testFloat(Math::exp10,exp10f,"exp10f");

  // Inverse hyperbolic sine, cosine, tangent
  testDouble(Math::asinh,asinh,"asinh");
  testFloat(Math::asinh,asinhf,"asinhf");
  testDouble(Math::acosh,acosh,"acosh");
  testFloat(Math::acosh,acoshf,"acoshf");
  testDouble(Math::atanh,atanh,"atanh");
  testFloat(Math::atanh,atanhf,"atanhf");

  // Ceil and floor
  testDouble(Math::ceil,ceil,"ceil");
  testFloat(Math::ceil,ceilf,"ceilf");
  testDouble(Math::floor,floor,"floor");
  testFloat(Math::floor,floorf,"floorf");

  // Round and trunc
  testDouble(Math::round,round,"round");
  testFloat(Math::round,roundf,"roundf");
  testDouble(Math::trunc,trunc,"trunc");
  testFloat(Math::trunc,truncf,"truncf");

  // Round to nearby int
  testDouble(Math::nearbyint,nearbyint,"nearbyint");
  testFloat(Math::nearbyint,nearbyintf,"nearbyintf");
  testDouble(Math::rint,rint,"rint");
  testFloat(Math::rint,rintf,"rintf");

  // Log
  testDouble(Math::log1p,log1p,"log1p");
  testFloat(Math::log1p,log1pf,"log1pf");
  testDouble(Math::log2,log2,"log2");
  testFloat(Math::log2,log2f,"log2f");
  testDouble(Math::log10,log10,"log10");
  testFloat(Math::log10,log10f,"log10f");
/*
*/
  return 0;
  }


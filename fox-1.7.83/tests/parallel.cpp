/********************************************************************************
*                                                                               *
*                P a r a l l e l   P r o g r a m m i n g   T e s t              *
*                                                                               *
*********************************************************************************
* Copyright (C) 2012,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "xincs.h"
#include "fx.h"


/*******************************************************************************/

// Stop QTest threads
volatile FXbool stopit=false;


/*******************************************************************************/

// Make-work procedure just uses lots of CPU by drawing random numbers till
// a number close to 1 is drawn.
void churn(){
  FXRandom random(FXThread::time()+FXLONG(128628761545));
  fxmessage("Churn start th %p core %d/%d\n",(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  while(random.randDouble()<0.99999999){ }
  fxmessage("Churn done  th %p core %d/%d\n",(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  }


// Make-work procedure does a nested parallel call to churn, then returns.
// This is to illustrate arbitrary nesting of parallelism.
void churn2(){
  fxmessage("Churn2 start th %p core %d/%d\n",(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  FXParallelInvoke(churn,churn,churn,churn);
  fxmessage("Churn2 done  th %p core %d/%d\n",(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  }


// Make-work procedure does a nested parallel call to churn, then returns.
// This is to illustrate arbitrary nesting of parallelism.
void churn3(){
  fxmessage("Churn3 start th %p core %d/%d\n",(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  FXParallelInvoke(churn2,churn2,churn2);
  fxmessage("Churn3 done  th %p core %d/%d\n",(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  }


// Make-work procedure like the ones above, this one depending on loop index i.
void looping(FXint i){
  FXRandom random(FXThread::time()+FXLONG(128628761545));
  fxmessage("Looping %05d begin th %p core %d/%d\n",i,(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  while(random.randDouble()<0.9999999){ }
  fxmessage("Looping %05d finis th %p core %d/%d\n",i,(void*)FXThread::current(),FXThread::processor(),FXThread::processors());
  }


// Low-level interface to thread pool
class Job : public FXRunnable {
public:
  Job(){}
  virtual FXint run();
  };


// Self-destructing task-let performing make-work procedure.
FXint Job::run(){
  churn();
  delete this;
  return 0;
  }

/*******************************************************************************/

// Print options
void printusage(const char* prog){
  fxmessage("%s options:\n",prog);
  fxmessage("  --threads <number>          Number of threads to start.\n");
  fxmessage("  --minimum <number>          Minimum number of threads to around.\n");
  fxmessage("  --maximum <number>          Maximum number of threads to be started.\n");
  fxmessage("  --jobs <number>             Number of jobs to run.\n");
  fxmessage("  --size <number>             Queue size.\n");
  fxmessage("  --pieces <number>           Split in this many pieces.\n");
  fxmessage("  -tracelevel <number>        Set trace level.\n");
  fxmessage("  -W, --wait                  Calling thread waits.\n");
  fxmessage("  -h, --help                  Print help.\n");
  fxmessage("  -N, --null                  Test create/destroy pool.\n");
  fxmessage("  -P, --pool                  Test thread pool.\n");
  fxmessage("  -L, --loop                  Test parallel loop.\n");
  fxmessage("  -I, --invoke                Test parallel invoke.\n");
  }


// Find power of two
FXuint powoftwo(FXuint n){
  --n;
  n|=n>>1;
  n|=n>>2;
  n|=n>>4;
  n|=n>>8;
  n|=n>>16;
  ++n;
  return n;
  }


// Test program
int main(int argc,char* argv[]){
  FXuint pieces=FXThread::processors();
  FXuint maximum=FXThread::processors();
  FXuint minimum=1;
  FXuint nthreads=1;
  FXuint size=512;
  FXuint njobs=10;
  FXuint test=2;
  FXuint wait=0;

  // Grab a few arguments
  for(FXint arg=1; arg<argc; ++arg){
    if(strcmp(argv[arg],"-h")==0 || strcmp(argv[arg],"--help")==0){
      printusage(argv[0]);
      exit(0);
      }
    else if(strcmp(argv[arg],"--threads")==0){
      if(++arg>=argc){ fxmessage("Missing threads number argument.\n"); exit(1); }
      nthreads=strtoul(argv[arg],nullptr,0);
      if(nthreads<1){ fxmessage("Value for threads (%d) too small.\n",nthreads); exit(1); }
      }
    else if(strcmp(argv[arg],"--pieces")==0){
      if(++arg>=argc){ fxmessage("Missing pieces number argument.\n"); exit(1); }
      pieces=strtoul(argv[arg],nullptr,0);
      if(pieces<1){ fxmessage("Value for pieces number of pieces (%d) too small.\n",pieces); exit(1); }
      if(pieces>FXParallelMax){ fxmessage("Value for pieces number of pieces (%d) too large (%d).\n",pieces,FXParallelMax); exit(1); }
      }
    else if(strcmp(argv[arg],"--minimum")==0){
      if(++arg>=argc){ fxmessage("Missing threads number argument.\n"); exit(1); }
      minimum=strtoul(argv[arg],nullptr,0);
      if(minimum<1){ fxmessage("Value for minimum number of threads (%d) too small.\n",minimum); exit(1); }
      }
    else if(strcmp(argv[arg],"--maximum")==0){
      if(++arg>=argc){ fxmessage("Missing threads number argument.\n"); exit(1); }
      maximum=strtoul(argv[arg],nullptr,0);
      if(maximum<2){ fxmessage("Value for maximum number of threads (%d) too small.\n",minimum); exit(1); }
      }
    else if(strcmp(argv[arg],"--size")==0){
      if(++arg>=argc){ fxmessage("Missing size argument.\n"); exit(1); }
      size=strtoul(argv[arg],nullptr,0);
      size=powoftwo(size);
      if(size<4){ fxmessage("Value for size (%d) too small.\n",size); }
      }
    else if(strcmp(argv[arg],"--jobs")==0){
      if(++arg>=argc){ fxmessage("Missing jobs count argument.\n"); exit(1); }
      njobs=strtoul(argv[arg],nullptr,0);
      if(njobs<1){ fxmessage("Value for njobs (%d) too small.\n",njobs); exit(1); }
      }
    else if(strcmp(argv[arg],"-W")==0 || strcmp(argv[arg],"--wait")==0){
      wait=1;
      }
    else if(strcmp(argv[arg],"-P")==0 || strcmp(argv[arg],"--pool")==0){
      test=1;
      }
    else if(strcmp(argv[arg],"-L")==0 || strcmp(argv[arg],"--loop")==0){
      test=2;
      }
    else if(strcmp(argv[arg],"-I")==0 || strcmp(argv[arg],"--invoke")==0){
      test=3;
      }
    else if(strcmp(argv[arg],"-N")==0 || strcmp(argv[arg],"--null")==0){
      test=0;
      }
    else if(strcmp(argv[arg],"-tracelevel")==0){
      if(++arg>=argc){ fxmessage("Missing tracelevel argument.\n"); exit(1); }
      setTraceLevel(strtoul(argv[arg],nullptr,0));
      }
    else{
      fxmessage("Bad argument.\n");
      printusage(argv[0]);
      exit(1);
      }
    }

  fxmessage("main thread %p\n",(void*)FXThread::current());

  // Create thread pool with queue size
  FXThreadPool pool(size);

  // Set number of threads
  pool.setMinimumThreads(minimum);
  pool.setMaximumThreads(maximum);
  pool.setExpiration(1000000);

  fxmessage("starting %d of maximum of %d threads, keeping at least %d\n",nthreads,maximum,minimum);

  // Start context
  pool.start(nthreads);

  fxmessage("running: %d!\n",pool.getRunningThreads());

  getchar();

  // Test plain thread pool usage
  if(1==test){

    fxmessage("starting execute: %d jobs...\n",njobs);

    // Start njobs-1 jobs
    for(FXuint j=wait; j<njobs; ++j){
      pool.execute(new Job);
      }

    // Main thread also runs jobs
    if(wait){
      pool.executeAndWait(new Job);
      }

    fxmessage("...done\n");
    }

  // Test task groups
  if(2==test){
    fxmessage("%d-way parallel for-loop...\n",nthreads);

    // Do somthing in parallel
    FXParallelFor(0U,njobs,1U,pieces,looping);

    fxmessage("...done!\n");
    }

  // Test parallel invoke
  if(3==test){
    fxmessage("%d-way parallel call...\n",nthreads);

    // Start parallel churn; some are recursive
    FXParallelInvoke(churn,churn2,churn,churn,churn,churn,churn2,churn2);

    fxmessage("halfway\n");

    // Start parallel churn; some are recursive
    FXParallelInvoke(churn2,churn,churn,churn3,churn,churn,churn,churn2);

    fxmessage("...done\n");
    }

  fxmessage("running: %d!\n",pool.getRunningThreads());

  // Wait for user
  getchar();

  fxmessage("stopping...\n");
  pool.stop();
  fxmessage("...done!\n");
  return 0;
  }



/********************************************************************************
*                                                                               *
*                                Thread Pool Test                               *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2023 by Jeroen van der Zijp.   All Rights Reserved.        *
********************************************************************************/
#include "fx.h"

#include <errno.h>

/*
  Notes:

  - Thread pool test.

*/

/*******************************************************************************/



// Job runner
class Runner : public FXRunnable {
protected:
  FXdouble value;
  FXint    number;
  FXint    count;
public:
  Runner(FXint n,FXint c):value(1.0),number(n),count(c){}
  virtual FXint run();
  };


// Job producer
class Producer : public FXThread {
protected:
  FXThreadPool *pool;
  FXint         count;
  FXint         groups;
public:
  Producer(FXThreadPool *p,FXint c,FXint g):pool(p),count(c),groups(g){}
  virtual FXint run();
  };


// Run jobs
FXint Runner::run(){
  fxmessage("runner %d start\n",number);
  value=1.0;
  for(FXint i=0; i<count; i++){
    value=Math::cos(value);
    }
  fxmessage("runner %d done\n",number);
  delete this;
  return 1;
  }



// Generate jobs
FXint Producer::run(){
  FXint job=0;
  fxmessage("producer start on cpus: %llb\n",affinity());
  FXRandom rng;
  for(FXint g=0; g<groups; ++g){
    for(FXint c=0; c<count; c++){
      if(!pool->execute(new Runner(job,(rng.randLong()/4000000)/1000000))) goto x;
      fxmessage("producer job %d\n",job);
      job++;
      }
    fxmessage("producer waiting\n");
    pool->wait();
    fxmessage("producer resumed\n");
    }
x:fxmessage("producer done\n");
  return 1;
  }



// Start
int main(int,char**){
  FXuint cpus=FXThread::processors();
  int started;

  // Trace
  setTraceLevel(150);

  // Make thread pool
  FXThreadPool pool(256);

  // Make producer thread
  Producer producer(&pool,100,10);

  fxmessage("Found %d processors\n",cpus);

  fxmessage("starting pool\n");
  started=pool.start(cpus);
  fxmessage("running: %d (%d)\n",pool.getRunningThreads(),started);
  getchar();
  fxmessage("started pool %d\n",started);
  getchar();

  fxmessage("running: %d\n",pool.getRunningThreads());
  fxmessage("starting jobs\n");
  producer.start();
  fxmessage("running jobs\n");
  fxmessage("running: %d\n",pool.getRunningThreads());

  getchar();
  fxmessage("stopping\n");
  pool.stop();
  fxmessage("running: %d\n",pool.getRunningThreads());
  fxmessage("stopped\n");

  getchar();
  producer.join();
  fxmessage("bye\n");
  return 1;
  }

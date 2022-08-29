// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================
//
// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pixels() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//
// Currently the function ignores the n_threads parameter. Your job is to
// parallelize the function so that it uses n_threads threads to do
// the c#include <iostream>
#include <thread>
#include <iostream>
#include "calcpi.h"
// approach adapted from lecture slides given by Dr. Pavol
struct TMem {
    pthread_t tid;
    int x1=0, x2=0, radius=0;
    uint64_t count_for_each_thread = 0;
};
// some changes in the original code provided by Dr. Pavol
void* threadFunction(void * args)
{
    uint64_t count = 0;

    struct TMem * tm = (struct TMem *) args;
    int radius = tm->radius;
    double  rsq = (double)radius * (double)radius;
    for( double x = tm ->x1 ; x <= tm->x2 ; x ++){
      for( double y = 0 ; y <= radius ; y ++){
        if( x*x + y*y <= rsq) count++;
      }  
    }    
    tm->count_for_each_thread = count ;
    pthread_exit(NULL);
}
uint64_t count_pixels(int r, int n_threads)
{
  // algorithm for equal division of work by Anshdeep Singh
  TMem tarr[n_threads];
  uint64_t count = 0;
  for(long i=0; i< n_threads; i++){
      tarr[i].radius = r;
      if(i==0){
        tarr[i].x1 = 1; 
        tarr[i].x2 =(r/n_threads); 
      } 
      else if(i==n_threads-1){
        tarr[i].x1 =tarr[i-1].x2+1;
        tarr[i].x2 = r;
      }
      else{
        tarr[i].x1 = tarr[i-1].x2+1; 
        tarr[i].x2 = tarr[i].x1 + (r/n_threads); 
      } 
      pthread_create(&tarr[i].tid, NULL,threadFunction, & tarr[i]);   
  }


  // code adapted from TA Manpreet Singh
  for(int i=0; i< n_threads; i++){
      pthread_join(tarr[i].tid, NULL);
      count+=tarr[i].count_for_each_thread;
  }
  return count * 4 + 1;
 
}


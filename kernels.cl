// vi:filetype=c
//
#include "constants.h"
#define N (GRID_N)
#define SZ (GRID_SZ)

__kernel void dp_source(__global float *d) {
  int id = get_global_id(0);
}

__kernel void dp_diffuse(__global float *d, __global const float *dp) {
  int id = get_global_id(0);

  float diff = 0.01;
  d[id] = (dp[id] + diff*(dp[id-1] + dp[id+1] + dp[id-N] + dp[id+N]))/(1+4*diff);
}

__kernel void dp_color(__global const float *a, __global unsigned char *rgb) {
  
  int id = get_global_id(0);

  rgb[3*id+0] = a[id];
  rgb[3*id+1] = a[id];
  rgb[3*id+2] = a[id];
}

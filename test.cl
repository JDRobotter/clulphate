// vi:filetype=c
//
#define N (30)
#define SZ (N*N)

__kernel void dp_add1(__global const float *a, __global float *b) {
  int id = get_global_id(0);

  float diff = 1;
  b[id] = (a[id] + diff*(a[id-1] + a[id+1] + a[id-N] + a[id+N]))/(1+4*diff);
}

// vi:filetype=c
__kernel void dp_add1(__global float *a) {
  int id = get_global_id(0);
  a[id] = 10.0;
  printf("%d", a[id]);
}

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include <CL/cl.hpp>
#include <CL/opencl.h>

int main(int argc, char **argv) {

  // __ OpenCL __
  // initializing OpenCL
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);
  if(all_platforms.size() == 0) {
    std::cout<<"No OpenCL platform found\n";
    return 255;
  }
  // use first platform
  auto platform = all_platforms[0];
  std::vector<cl::Device> all_devices;
  platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
  if(all_devices.size() == 0) {
    std::cout<<"No OpenCL devices found\n";
    return 255;
  }
  auto device = all_devices[0];

  std::cout<<"[+] Using "
    <<platform.getInfo<CL_PLATFORM_NAME>()
    <<" : "<<device.getInfo<CL_DEVICE_NAME>()
    <<"\n";

  // create an openCL context
  cl::Context context({device});
  cl::CommandQueue queue(context, device);

  std::ifstream cl_file("test.cl");
  std::string cl_string(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));
  cl::Program::Sources source(1, std::make_pair(cl_string.c_str(), cl_string.length() + 1));

  // create program
  cl::Program program(context, source);

  // compile sources
  cl_int rv = program.build({device});
  if(rv != CL_SUCCESS) {
    std::cerr<<"ERROR"<<rv<<"\n";
    std::cerr<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
  }

  // load kernel from source
  cl::Kernel kernel(program, "dp_add1");

  const int N = 30;
  const int SZ = N*N;

  float input[SZ] = {0};
  for(int i=0;i<SZ;i++) {
    if( (i > (N-3)*N) && (i < (N-2)*N) ) {
      input[i] = 10;
    }
    else{
      input[i] = 0;
    }
  }

  float output[SZ] = {0};
  cl::Buffer buffer_in(context, CL_MEM_READ_WRITE, SZ*sizeof(float));
  cl::Buffer buffer_out(context, CL_MEM_READ_WRITE, SZ*sizeof(float));

  // execute kernel
  cl::NDRange global_work_size(SZ);
  
  // copy from local memory to buffer
  queue.enqueueWriteBuffer(buffer_in, CL_TRUE, 0, SZ*sizeof(float), input);

  for(int z=0;z<10;z++) {
    kernel.setArg(0,buffer_in);
    kernel.setArg(1,buffer_out);
    rv = queue.enqueueNDRangeKernel(kernel, cl::NullRange, global_work_size, cl::NullRange);
    if(rv != CL_SUCCESS) {
      std::cerr<<"ERROR"<<rv<<"\n";
    }
    queue.enqueueReadBuffer(buffer_out, CL_TRUE, 0, SZ*sizeof(float), output);
    queue.enqueueCopyBuffer(buffer_out, buffer_in, 0, 0, SZ*sizeof(float));
    // wait for completion
    queue.finish();

    for(int j=0; j<N; j++) {
      for(int i=0; i<N; i++) {
        int k = i+j*N;
        std::cout<<" "<<(int)(output[k]);
      }
      std::cout<<"\n";
    }
    std::cout<<"\n";
  }

  return 0;
}

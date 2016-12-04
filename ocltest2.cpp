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

  try {
    // load kernel from source
    cl::Kernel kernel(program, "dp_add1");

    const int N = 100;
    float data[N] = {0};
    cl::Buffer buffer(CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N*sizeof(float), data);

    kernel.setArg(0,buffer);
    
    // execute kernel
    cl::NDRange global_work_size(100);
    
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, global_work_size, cl::NullRange);
    queue.enqueueReadBuffer(buffer, CL_TRUE, 0, N*sizeof(float), data);
    // wait for completion
    queue.finish();
  }
  catch(cl::Error &er) {
    std::cerr<<"ERROR: "<<er.what()<<" : "<<err.er();
  }

  for(float v: data) {
    std::cout<<" "<<v;
  }
  std::cout<<"\n";

  return 0;
}

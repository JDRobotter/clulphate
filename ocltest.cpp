#include <iostream>

#include <CL/cl.hpp>

int main(int argc, char **argv) {

  std::cout<<"[+] listing platforms...\n";

  // get all platforms
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);

  if(all_platforms.size() == 0) {
    std::cout<<"[!] No platforms found.\n";
    exit(255);
  }

  for(auto platform: all_platforms) {
    std::cout<<" - "<<platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    std::cout<<" | [+] Fetching devices\n";

    std::vector<cl::Device> all_devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size() == 0) {
      std::cout<<" | [!] No devices found.\n";
    }

    for(auto device: all_devices) {
      std::cout<<" | - "
        <<device.getInfo<CL_DEVICE_NAME>()
        <<" : "<<device.getInfo<CL_DEVICE_VERSION>()
        <<" : "<<device.getInfo<CL_DRIVER_VERSION>()
        <<"\n";
    }
  }




  return 0;
}

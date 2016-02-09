#ifdef __APPLE__
      #include <OpenCL/opencl.h>
#elif __linux
     #include <CL/cl.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>

int show_info(cl_platform_id platform_id);
int load_file_to_memory(const char *filename, char **result);
int ocl_init(cl_platform_id& platform_id,  cl_device_id& device_id, cl_context& context, cl_command_queue& commands);
int loading_kernel_from_disk(const std::string& fileName,
				cl_program& program,
				cl_context& context,
				cl_device_id& device_id);
float getTimeDifference(cl_event event);

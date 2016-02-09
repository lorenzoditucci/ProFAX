#include "support.h"
#define SUCCESS 0
/* show information about the platform we are considering*/
				int show_info(cl_platform_id platform_id){
					int err;
					char cl_platform_vendor[1001];
					char cl_platform_name[1001];
					char cl_platform_version[1001];

					err = clGetPlatformInfo(platform_id,
						CL_PLATFORM_VENDOR,
						1000,
						(void *)cl_platform_vendor,
						NULL);
					if (err != CL_SUCCESS){
						printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					}
					printf("CL_PLATFORM_VENDOR %s\n",cl_platform_vendor);

					err = clGetPlatformInfo(platform_id,
						CL_PLATFORM_NAME,
						1000,
						(void *)cl_platform_name,
						NULL);
					if (err != CL_SUCCESS){
						printf("Error: clGetPlatformInfo(CL_PLATFORM_NAME) failed!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					}
					printf("CL_PLATFORM_NAME %s\n",cl_platform_name);

					err = clGetPlatformInfo(platform_id,
						CL_PLATFORM_VERSION,
						1000,
						(void *)cl_platform_version,
						NULL);
					if (err != CL_SUCCESS){
						printf("Error: clGetPlatformInfo(CL_PLATFORM_VERSION) failed!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					}
					printf("CL_PLATFORM_VERSION %s\n",cl_platform_version);
					return SUCCESS;
				}

				int load_file_to_memory(const char *filename, char **result)
				{ 
					printf("inside load_file_to_memory, filename value is %s \n", filename);
					int size = 0;
					FILE *f = fopen(filename, "rb");
					if (f == NULL) 
					{ 
						*result = NULL;
			    return -1; // -1 means file opening fail 
			} 
			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			*result = (char *)malloc(size+1);
			if (size != fread(*result, sizeof(char), size, f)) 
			{ 
				free(*result);
			    return -2; // -2 means file reading fail 
			} 
			fclose(f);
			(*result)[size] = 0;
			return size;
		}

		/*
			init the platform.
			In case of a GPU it checks how many devices are available and pick up the TESLA
			*/
			int ocl_init(cl_platform_id& platform_id,  cl_device_id& device_id, cl_context& context, cl_command_queue& commands){
				printf("initiation started...\n");
				#ifdef __GPU__
					/*
					If it's a GPU we could have more platforms because it recognizes also the CPU as a platform on ICHEC
					*/
					cl_uint num_platforms;
					cl_uint num_devices;
					cl_platform_id *platforms;
					cl_device_id *devices;
					
					int err = clGetPlatformIDs(
						0,
						NULL,
						&num_platforms);

					if (err != CL_SUCCESS){
						printf("Error: Failed to find an OpenCL platform!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					} 
					// Allocate enough space for each platform
					platforms = (cl_platform_id*) malloc(num_platforms * sizeof(platform_id));
					// Acquire the platforms
					err = clGetPlatformIDs(
						num_platforms,
						platforms,
						NULL);
					if (err != CL_SUCCESS){
						printf("Error: Failed acquiring platforms!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					} 



					//how many platforms we have?
					if(num_platforms > 1){
						printf("More than one platform detected! \n");
					}

					//we need to set the real platform that we want to use so...
					for(int i = 0; i< num_platforms; i++){
						err = clGetDeviceIDs(
							platforms[i],
							CL_DEVICE_TYPE_DEFAULT,
							0,
							NULL,
							&(num_devices));

						if (err != CL_SUCCESS){
							printf("Error: Failed acquiring platforms!\n");
							printf("Test failed\n");
							return EXIT_FAILURE;
						}

								// Allocate enough space for each device
						devices = (cl_device_id*) malloc(sizeof(cl_device_id)*num_devices);
								// Acquire the devices
						err = clGetDeviceIDs(
							platforms[i],
							CL_DEVICE_TYPE_DEFAULT,
							num_devices,
							devices,
							NULL);
						if (err != CL_SUCCESS){
							printf("Error: Failed acquiring platforms!\n");
							printf("Test failed\n");
							return EXIT_FAILURE;
						}

								//cl_char deviceName[10240] = {0};
						char deviceName[10240];
						    	// Get device name
						err = clGetDeviceInfo(devices[0],
							CL_DEVICE_NAME,
							sizeof(deviceName),
							&deviceName,
							NULL);

						if (err != CL_SUCCESS){
							printf("Error: could not get device information\n");
							return EXIT_FAILURE;
						} 
								//set the device we want to use
						printf("DEVICE %d NAME is %s \n",i, deviceName);  
						std::string device_name(deviceName);
						if (device_name.find("Tesla") != std::string::npos) {
							platform_id = platforms[i];
							device_id = devices[0];
						}

					}
				#else
					 // Connect to first platform
					int err = clGetPlatformIDs(1,&platform_id,NULL);
					if (err != CL_SUCCESS){
						printf("Error: Failed to find an OpenCL platform!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					} 

					if(show_info(platform_id) != SUCCESS){
						printf("Error: Showing information!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					}   
				#endif




		#ifdef __GPU__
					err = clGetDeviceIDs(platform_id,
						CL_DEVICE_TYPE_GPU,
						1,  
						&device_id,
						NULL);
		#elif __PHI__
					err = clGetDeviceIDs(platform_id,
						CL_DEVICE_TYPE_ACCELERATOR,
						1,  
						&device_id,
						NULL);
		#elif __FPGA__
					err = clGetDeviceIDs(platform_id,
						CL_DEVICE_TYPE_ACCELERATOR,
						1,  
						&device_id,
						NULL);
		#else
					err = clGetDeviceIDs(platform_id,
						CL_DEVICE_TYPE_CPU,
						1,  
						&device_id,
						NULL);
		#endif

					if (err != CL_SUCCESS){
						printf("Error: Failed to create a device group!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					}   

				//this is redundant, we could also remove it.
					cl_char string[10240] = {0};
		    // Get device name
					err = clGetDeviceInfo(device_id,
						CL_DEVICE_NAME,
						sizeof(string),
						&string,
						NULL);
					if (err != CL_SUCCESS){
						printf("Error: could not get device information\n");
						return EXIT_FAILURE;
					}   
					printf("Name of device: %s\n", string);
					cl_ulong globalMSize; 
					err = clGetDeviceInfo(device_id,
						CL_DEVICE_GLOBAL_MEM_SIZE,
						sizeof(cl_ulong),
						&globalMSize,
						NULL);
					if (err != CL_SUCCESS){
						printf("Error: could not get device information\n");
						return EXIT_FAILURE;
					}   
					printf("Global Memory size: %luMB\n", globalMSize/1024/1024);
		    // Create a compute context
					context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
					if (!context){
						printf("Error: Failed to create a compute context!\n");
						printf("Test failed\n");
						return EXIT_FAILURE;
					}   

		    // Create a command queue
					commands = clCreateCommandQueue(context,
						device_id,
						CL_QUEUE_PROFILING_ENABLE,
						&err);
					if (!commands){
						printf("Error: Failed to create a command commands!\n"); 
						printf("Error: code %i\n",err);
						printf("Test failed\n");
						return EXIT_FAILURE;
					}   
					return 0;
				}

			/*
			Load the kernel from the disk and create a program.
			Note that the program we create is different for each platform
			*/
			int loading_kernel_from_disk(const std::string& fileName,
				cl_program& program,
				cl_context& context,
				cl_device_id& device_id){
				int err;
	#ifdef __FPGA__
				printf("DEVICE: FPGA \n");
				unsigned char *kernelbinary;
				printf("loading binary [%s]\n", fileName.c_str());
				int n_i = load_file_to_memory(fileName.c_str(), (char **) &kernelbinary);
				if (n_i < 0) {
					printf("failed to load kernel from xclbin: %s\n", fileName.c_str());
					printf("Test failed\n");
					return EXIT_FAILURE;
				}
				size_t n = n_i;
				int status;
				printf("creating program...\n");
				program = clCreateProgramWithBinary(context,
					1,
					&device_id,
					&n,
					(const unsigned char **) &kernelbinary,
					&status,
					&err);
				printf("program created! \n");
				fflush(stdout);
	#else
				unsigned char *kernelsrc;
				printf("loading source [%s]\n", fileName.c_str());
				int n_i = load_file_to_memory(fileName.c_str(), (char **) &kernelsrc);
				if (n_i < 0) {
					printf("failed to load kernel from source: %s\n", fileName.c_str());
					printf("Test failed\n");
					return EXIT_FAILURE;
				}

				program = clCreateProgramWithSource(context,
					1,
					(const char **) &kernelsrc,
					NULL,
					&err);
	#endif
				if((!program) || (err!=CL_SUCCESS)){
					printf("Error: Failed to create compute program from binary %d!\n", err);
					printf("Test failed\n");
					return EXIT_FAILURE;
				}
				return 0;
			}

			
			/*
			Given an event, this function returns the kernel execution time in ms
			*/
			float getTimeDifference(cl_event event) {
				cl_ulong time_start = 0;
				cl_ulong time_end = 0;
				float total_time = 0.0f;

				clGetEventProfilingInfo(event,
					CL_PROFILING_COMMAND_START,
					sizeof(time_start),
					&time_start,
					NULL);
				clGetEventProfilingInfo(event, 
					CL_PROFILING_COMMAND_END,
					sizeof(time_end),
					&time_end,
					NULL);
				total_time = time_end - time_start;
				return total_time/ 1000000.0; // To convert nanoseconds to milliseconds
			}


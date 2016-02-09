
# Define the project for SDAccel
create_solution -name all -dir . -force
add_device xilinx:adm-pcie-7v3:1ddr:2.0

# Host Compiler Flags
set_property -name host_cflags -value "-g -Wall -D FPGA_DEVICE -D C_KERNEL"  -objects [current_solution]


# Host Source Files
add_files "test-cl.cpp"
#add_files "support.cpp"
#add_files "eef1.cpp"

# Header files
#add_files "support.h"
add_files "kernel.h"
#add_files "MatVec.hpp"
#add_files "RectDistCL.hpp"
#add_files "leafCL.hpp"
#add_files "eef1.hpp"
#add_files "RectDistCL.hpp"
#add_files "rotamers.hpp"

#set_property file_type "c header files" [get_files "support.h"]
set_property file_type "c header files" [get_files "kernel.h"]
#set_property file_type "c header files" [get_files "MatVec.hpp"]
#set_property file_type "c header files" [get_files "RectDistCL.hpp"]
#set_property file_type "c header files" [get_files "leafCL.hpp"]
#set_property file_type "c header files" [get_files "eef1.hpp"]
#set_property file_type "c header files" [get_files "RectDistCL.hpp"]
#set_property file_type "c header files" [get_files "rotamers.hpp"]

# Kernel Definition
create_kernel computePairEnergy -type c
#set_property max_memory_ports true [get_kernels computePairEnergy]
set_property memory_port_data_width 512 [get_kernels computePairEnergy]
add_files -kernel [get_kernels computePairEnergy] "kernel.cpp"
set_property -name kernel_flags -value {-g} -objects [get_kernels computePairEnergy]


# Define Binary Containers
create_opencl_binary computePairEnergy
set_property region "OCL_REGION_0" [get_opencl_binary computePairEnergy]
create_compute_unit -opencl_binary [get_opencl_binary computePairEnergy] -kernel [get_kernels computePairEnergy] -name k1

#preserve hls output so that we can have the report.
set_param compiler.preserveHlsOutput 1
# Compile the design for CPU based emulation
compile_emulation -flow cpu -opencl_binary [get_opencl_binary computePairEnergy]

# Run the compiled application in CPU based emulation mode
#run_emulation -flow cpu -args "-k computePairEnergy.xclbin -s1 -GVTGVCAGTAGVTGVCAGTAGVTGVCAGTAGVTGVCAGTAGVTGVCAGTAGVTGVCAGTAGVTGVCAGTAGVTGVCAGTAAGTV -i 10"
#run_emulation -flow cpu -args "-k computePairEnergy.xclbin -n 64 -m 128"

run_emulation -flow cpu -args "computePairEnergy.xclbin"

compile_emulation -flow hardware -opencl_binary [get_opencl_binary computePairEnergy]
run_emulation -flow hardware -args "computePairEnergy.xclbin"
#compile_emulation -flow hardware -opencl_binary [get_opencl_binary computePairEnergy]

#run_emulation -flow hardware -args "-k computePairEnergy.xclbin -n 64 -m 128"

#run_emulation -flow cpu -args "-k computePairEnergy.xclbin -n 16 -m 256"
#run_emulation -flow hardware -args "-k computePairEnergy.xclbin -n 16 -m 256"

#run_emulation -flow cpu -args "-k computePairEnergy.xclbin -n 256 -m 512"
#run_emulation -flow hardware -args "-k computePairEnergy.xclbin -n 256 -m 512"

#run_emulation -flow cpu -args "-k computePairEnergy.xclbin -n 256 -m 1000000000"
#run_emulation -flow hardware -args "-k computePairEnergy.xclbin -n 256 -m 1000000000"



#run_emulation -flow hardware -args "-k computePairEnergy.xclbin"
report_estimate
# Compile the application to run on the accelerator card
build_system

# Package the application binaries
package_system

#run_system -args "adder.xclbin"

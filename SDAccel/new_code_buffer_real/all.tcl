
# Define the project for SDAccel
create_solution -name all -dir . -force
add_device xilinx:adm-pcie-7v3:1ddr:2.0

# Host Compiler Flags
set_property -name host_cflags -value "-g -Wall -D FPGA_DEVICE -D C_KERNEL"  -objects [current_solution]


# Host Source Files
add_files "test-cl.cpp"

# Header files
add_files "kernel.h"

set_property file_type "c header files" [get_files "kernel.h"]

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

run_emulation -flow cpu -args "computePairEnergy.xclbin"

compile_emulation -flow hardware -opencl_binary [get_opencl_binary computePairEnergy]
run_emulation -flow hardware -args "computePairEnergy.xclbin"


#run_emulation -flow hardware -args "-k computePairEnergy.xclbin"
report_estimate
# Compile the application to run on the accelerator card
build_system

# Package the application binaries
package_system

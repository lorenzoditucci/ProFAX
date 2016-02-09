open_project protein_folding

set_top computePairEnergy

add_files kernel.cpp
add_files -tb main.cpp
add_file -tb kernel.h

open_solution "solution1"
set_part {xc7vx690tffg1761-2}
create_clock -period 5.00 -name default

csim_design  -clean
csynth_design
#cosim_design -tool modelsim -rtl verilog -trace_level all 
export_design -format ip_catalog -display_name "Compute Pair Energy function for Protein Folding" -evaluate verilog -vendor "Politecnico di Milano" -version "1.00"

exit


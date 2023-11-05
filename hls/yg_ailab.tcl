##############################################
#project settings

# Creat a project
open_project ai_lab_v[lindex $argv 0]

# The source file and test bench
add_files stream.h
add_files rocky-lib.h
add_files param.h
add_files misc.h
add_files matrix-vector-unit.h
add_files conv2d.h
add_files config.h
add_files block_top.h
add_files block_top.cpp
add_files block.h
add_files -tb input2.txt -cflags "-Wno-unknown-pragmas"
add_files -tb block_test.cpp -cflags "-Wno-unknown-pragmas"

# Specify the top-level function for synthesis
set_top do_compute

###########################
# Solution settings

# Create solution1
open_solution -reset solution1

# Specify a Xilinx device and clock period
# - Do not specify a clock uncertainty (margin)
set_part {xc7vx690tffg1761-2} -tool vivado
create_clock -period 4 -name default


# Verilator_RISCV
Verification and Coverage of the Verilator models of the CV32E40P processor which is based on the RISC-V ISA


## Verilator and Setup Instructions

This section discusses the Verilator, what it is and how to invoke and use it.</br>

Verilator is an open-source tool that transforms Verilog hardware description language (HDL) designs into highly efficient C++ or SystemC models. Unlike traditional event-driven simulators, Verilator creates cycle-accurate models that simulate much faster, making it suitable for verifying large designs and complex SoCs.

It can be installed using the link : https://verilator.org/guide/latest/install.html on Ubuntu

After successful installation of the tool, to invoke, follow the command shown below :

```
verilator --cc --exe --build -j -Wall --trace --trace-structs <related files> --top-module <top_module_name>  sim_main.cpp  --coverage  --Wno-fatal
```

 --cc : Verilator will translate the design into C++ code. Binary(--binary), C++(--cc) and SystemC(--sc) Generation.
    
-j : Verilation for hierarchy blocks runs in parallel.

--exe : generates an executable within obj_dir (directory)

--build : C++ compilation also runs as soon as a hierarchy block is Verilated.

--trace : Perform VCD tracing

--coverage : Using this tag we can use the Verilator’s inbuilt coverage tool.

--Wall : Enables warning messages but treats them as errors

--Wallno-fatal : Enables warnings but it ignores and allows full compilation generating the verilation report.

Use command: ```verilator_coverage vlt_coverage.dat --annotate annotate/``` to get coverage.


A hierarchical view of the processor with the modules and dependencies is as shown below. 

```
cv32e40p_core
├── cv32e40p_apu_core_pkg
├── cv32e40p_pkg
├── cv32e40p_sleep_unit
│   ├── cv32e40p_pkg
│   └── cv32e40p_clock_gating
├── cv32e40p_if_stage
│   ├── cv32e40p_pkg
│   ├── cv32e40p_prefetch_buffer
│   ├── cv32e40p_aligner
│   └── cv32e40p_compressed_decoder
├── cv32e40p_id_stage
│   ├── cv32e40p_pkg
│   ├── cv32e40p_apu_core_pkg
│   ├── cv32e40p_hwloop_regs
│   ├── cv32e40p_int_controller
│   ├── cv32e40p_controller
│   ├── cv32e40p_decoder
│   └── cv32e40p_registers_file
├── cv32e40p_ex_stage
│   ├── cv32e40p_pkg
│   ├── cv32e40p_apu_core_pkg
│   ├── cv32e40p_alu
│   ├── cv32e40p_mul
│   └── cv32e40p_alu_disp
├── cv32e40p_load_store_unit
│   └── cv32e40p_obi_interface
└── cv32e40p_cs_registers
    └── cv32e40p_pkg
cv32e40p_alu
├── cv32e40p_popcnt
├── cv32e40p_ff_one
└── cv32e40p_alu_div
cv32e40p_alu_disp
└── no other module instantiations
cv32e40p_compressed_decoder
└── cv32e40p_pkg
cv32e40p_controller
└── cv32e40p_pkg

cv32e40p_decoder
├── cv32e40p_pkg
├── cv32e40p_apu_core_pkg
└── cv32e40p_fpu_pkg

cv32e40p_ff_one
└── no other module instantiations
cv32e40p_fp_wrapper
├── cv32e40p_apu_core_pkg
├── cv32e40p_pkg
└── fpnew_pkg
cv32e40p_hwloop_regs
└── no other module instantiations
cv32e40p_if_stage
├── cv32e40p_pkg
├── cv32e40p_prefetch_buffer
│   ├── cv32e40p_prefetch_controller
│   ├── cv32e40p_fifo
│   └── cv32e40p_obi_interface
├── cv32e40p_aligner
└── cv32e40p_compressed_decoder
cv32e40p_int_controller
└── cv32e40p_pkg
cv32e40p_load_store_unit
└── cv32e40p_obi_interface
cv32e40p_mult
└── cv32e40p_pkg
cv32e40p_obi_interface
└── no other module instantiations
cv32e40p_popcnt
└── no other module instantiations
cv32e40p_prefetch_buffer
├── cv32e40p_prefetch_controller
├── cv32e40p_fifo
└── cv32e40p_obi_interface
cv32e40p_prefetch_controller
└── cv32e40p_pkg
cv32e40p_register_file_ff
└── no other module instantiations
cv32e40p_register_file_latch
└── no other module instantiations
cv32e40p_sleep_unit
├── cv32e40p_pkg
└── cv32e40p_clock_gating
cv32e40p_wrapper
├── cv32e40p_apu_core_pkg
├── cv32e40p_core
└── cv32e40p_fp_wrapper
```

## Verilator Coverage Analysis

As Discussed in the Verilator section to invoke the verilator’s inbuilt coverage tool one must add the --coverage tag with the invoking command. As per the documentation there are three types :

```
Functional coverage : --coverage
Line coverage : --coverage-line
Toggle coverage : --coverage-toggle
```

With our experimentation we observed that the verilator’s coverage algorithm does it in a way that if we manage to get good coverage with --coverage, the other two would also yield good results, most of the time they are better.
In order to perform the coverage analysis we have written randomized flat test benches in C++  which would be passed with the design when invoking the verilator.
We have done coverage analysis for most of the modules(designs) and got expected results(≥95%).But as the modules get more and more complex with more dependencies with other modules there chances of High coverage is very hard with the verilator’s coverage tool. This is indeed a limitation of the verilator. Some of the coverage analysis reports are shown below:

### Aligner module:
![aligner](https://github.com/emillal/Verilator_RISCV/assets/38190245/0a422e76-e1b9-486a-a97b-1bbd9437076c)

### alu_div module:
![alu_div](https://github.com/emillal/Verilator_RISCV/assets/38190245/1af00b78-5996-4786-9363-90deae36382c)

### obi_interface:
![obi](https://github.com/emillal/Verilator_RISCV/assets/38190245/8041def0-5c97-4860-a8cf-e97e0f8e33e6)

### prefetch_buffer:
![prefetch_buffer](https://github.com/emillal/Verilator_RISCV/assets/38190245/fff5af64-ea8a-469c-9335-70313416360d)

### register_file_latch:
![red_file_latch](https://github.com/emillal/Verilator_RISCV/assets/38190245/67787c72-9f26-4943-801b-91537073ba07)

### fifo module:
![fifo](https://github.com/emillal/Verilator_RISCV/assets/38190245/0c4d4eed-a11d-4506-9d33-3ca0a367d9f5)


Similarly for other modules verilator coverage analysis have been done.


As discussed, with the command verilator_coverage vlt_coverage.dat --annotate annotate/ 
We are able  to see where all in the design it is not getting covered properly. 

To properly get the coverage we would need to use a randomized testbench. In the case of verilator,it is written in C++. The inputs need to be randomized sufficiently to get good coverage. As discussed before the verilator’s inbuilt coverage tool is not that efficient as the other commercial tools which employ System Verilog testbenches. We observe that even after randomizing the input with sufficient simulation time. The whole code is not covered.

Due to a variety of reasons such as module parameters not randomizable, non-support of multiple genvar  etc. It is observed that most are due to the limitations of the coverage tool and not with the verilator itself.

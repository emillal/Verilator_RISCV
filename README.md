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



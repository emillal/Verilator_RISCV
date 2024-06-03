#include "Vcv32e40p_compressed_decoder.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 1000000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_compressed_decoder *dut = new Vcv32e40p_compressed_decoder;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_compressed_decoder_tb.vcd");

    // Main simulation loop
    while (sim_time < timeout) {
        // Randomize test vectors
        if (sim_time % 10 == 0) {
        dut->instr_i = rand();
        }
        // Evaluate DUT
        dut->eval();
        vcd->dump(sim_time);

        // Print output values for debugging
        /*std::cout << "Time: " << sim_time 
                  << " | instr_i: " << std::hex << dut->instr_i 
                  << " | instr_o: " << dut->instr_o
                  << " | is_compressed_o: " << dut->is_compressed_o
                  << " | illegal_instr_o: " << dut->illegal_instr_o 
                  << std::dec << std::endl;*/

        // Increment simulation time
        sim_time++;
    }

    #if VM_COVERAGE
    Verilated::threadContextp()->coveragep()->write("vlt_coverage.dat");
    #endif

    // Finalize simulation
    vcd->close();
    delete vcd;
    delete dut;

    return 0;
}


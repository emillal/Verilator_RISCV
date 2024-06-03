#include "Vcv32e40p_aligner.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    
    // Seed the random number generator
    std::srand(std::time(nullptr));
    
    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_aligner *dut = new Vcv32e40p_aligner;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_aligner_tb.vcd");

    // Reset the DUT
    dut->clk = 0;
    dut->rst_n = 0;
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Release reset
    dut->rst_n = 1;
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Main simulation loop
    while (sim_time < timeout) {
        // Toggle clock
        dut->clk = !dut->clk;
        
        // Randomize test vectors
        if (sim_time % 10 == 0) {
            //dut->clk = rand() % 2;
            dut->rst_n = rand() % 2;
            dut->fetch_valid_i = rand() % 2;  // Random value between 0 and 1
            dut->if_valid_i = rand() % 2;     // Random value between 0 and 1
            dut->fetch_rdata_i = rand();      // Random 32-bit value
            dut->branch_i = rand() % 2;       // Random value between 0 and 1
            dut->branch_addr_i = rand();      // Random 32-bit value
            dut->hwlp_update_pc_i = rand() % 2; // Random value between 0 and 1
            dut->hwlp_addr_i = rand();        // Random 32-bit value
        }

        // Evaluate DUT
        dut->eval();
        vcd->dump(sim_time);

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


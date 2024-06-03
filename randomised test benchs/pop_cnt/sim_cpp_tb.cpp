#include "Vcv32e40p_popcnt.h"
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
    Vcv32e40p_popcnt *dut = new Vcv32e40p_popcnt;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_popcnt_tb.vcd");

    // Main simulation loop
    while (sim_time < timeout) {
        // Toggle clock (although not necessary in this simple design, included for completeness)
        //dut->clk = !dut->clk;

        // Randomize test vectors
        if (sim_time % 2 == 0) {
            dut->in_i = rand();  // Random 32-bit value	       
        }

        // Evaluate DUT
        dut->eval();
        vcd->dump(sim_time);

        // Output the result for debug purposes
        /*std::cout << "Time: " << sim_time 
                  << " | Input: " << std::hex << dut->in_i 
                  << " | Popcnt Result: " << std::hex << dut->result_o << std::endl;*/

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


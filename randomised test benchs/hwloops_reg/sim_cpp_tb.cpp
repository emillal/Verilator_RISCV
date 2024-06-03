#include "Vcv32e40p_hwloop_regs.h"
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
    Vcv32e40p_hwloop_regs *dut = new Vcv32e40p_hwloop_regs;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_hwloop_regs_tb.vcd");

    // Reset the DUT
    dut->clk = 0;
    dut->rst_n = 0;
    dut->hwlp_start_data_i = 0;
    dut->hwlp_end_data_i = 0;
    dut->hwlp_cnt_data_i = 0;
    dut->hwlp_we_i = 0;
    dut->hwlp_regid_i = 0;
    dut->valid_i = 0;
    dut->hwlp_dec_cnt_i = 0;
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
            dut->rst_n = rand() % 2;  
            dut->hwlp_start_data_i = rand();             // Random 32-bit value
            dut->hwlp_end_data_i = rand();               // Random 32-bit value
            dut->hwlp_cnt_data_i = rand();               // Random 32-bit value
            dut->hwlp_we_i = rand() % 8;                 // Random value between 0 and 7 (3-bit)
            dut->hwlp_regid_i = rand() % 2;         // Random value based on number of registers
            dut->valid_i = rand() % 2;                   // Random value between 0 and 1
            dut->hwlp_dec_cnt_i = rand() % 4; // Random value for hwlp_dec_cnt_i
        }

        // Evaluate DUT
        dut->eval();
        vcd->dump(sim_time);

        // Output the result for debug purposes
        /*std::cout << "Time: " << sim_time 
                  << " | HWLP Start Data: " << std::hex << dut->hwlp_start_data_i 
                  << " | HWLP End Data: " << std::hex << dut->hwlp_end_data_i 
                  << " | HWLP Cnt Data: " << std::hex << dut->hwlp_cnt_data_i 
                  << " | HWLP WE: " << std::hex << dut->hwlp_we_i 
                  << " | HWLP RegID: " << std::hex << dut->hwlp_regid_i 
                  << " | Valid: " << dut->valid_i 
                  << " | HWLP Dec Cnt: " << std::hex << dut->hwlp_dec_cnt_i 
                  << std::endl;*/

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


#include "Vcv32e40p_alu.h"
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
    Vcv32e40p_alu *dut = new Vcv32e40p_alu;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_alu_tb.vcd");

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
        
        // Randomize inputs
        if (sim_time % 10 == 0) {
            //dut->clk = rand() % 2;
            dut->rst_n = rand() % 2;
            dut->enable_i = rand() % 2;
            dut->operator_i = rand() % 128;
            dut->operand_a_i = rand();
            dut->operand_b_i = rand();
            dut->operand_c_i = rand();
            dut->vector_mode_i = rand() % 4; // Assuming vector_mode_i is 2 bits wide
            dut->bmask_a_i = rand();
            dut->bmask_b_i = rand();
            dut->imm_vec_ext_i = rand() % 4; // Assuming imm_vec_ext_i is 2 bits wide
            dut->is_clpx_i = rand() % 2;
            dut->is_subrot_i = rand() % 2;
            dut->clpx_shift_i = rand() % 4; // Assuming clpx_shift_i is 2 bits wide
            dut->ex_ready_i = rand() % 2;
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


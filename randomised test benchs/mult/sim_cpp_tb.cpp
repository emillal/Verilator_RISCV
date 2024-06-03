#include "Vcv32e40p_mult.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Simulation time variables
vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    
    // Seed the random number generator
    std::srand(std::time(nullptr));
    
    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_mult *dut = new Vcv32e40p_mult;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_mult_tb.vcd");

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
        
        // Randomize test vectors at intervals
        if (sim_time % 10 == 0) {
            //dut->clk = rand() % 2;
            dut->rst_n = rand() % 2;
            dut->enable_i = rand() % 2;
            dut->operator_i = rand() % 8; // Assuming mul_opcode_e has 3 enums

            dut->short_subword_i = rand() % 2;
            dut->short_signed_i = rand() % 4; // 2-bit value

            dut->op_a_i = rand();
            dut->op_b_i = rand();
            dut->op_c_i = rand();
            dut->imm_i = rand() % 32; // 5-bit value

            dut->dot_signed_i = rand() % 4; // 2-bit value
            dut->dot_op_a_i = rand();
            dut->dot_op_b_i = rand();
            dut->dot_op_c_i = rand();
            dut->is_clpx_i = rand() % 2;
            dut->clpx_shift_i = rand() % 4; // 2-bit value
            dut->clpx_img_i = rand() % 2;

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


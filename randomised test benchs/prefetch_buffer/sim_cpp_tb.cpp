#include "Vcv32e40p_prefetch_buffer.h"
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
    Vcv32e40p_prefetch_buffer *dut = new Vcv32e40p_prefetch_buffer;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_prefetch_buffer_tb.vcd");

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

        if (sim_time % 10 == 0) {
            dut->rst_n = rand() % 2;
            dut->req_i = rand() % 2;
            dut->branch_i = rand() % 2;
            dut->branch_addr_i = rand();
            dut->hwlp_jump_i = rand() % 2;
            dut->hwlp_target_i = rand();
            dut->fetch_ready_i = rand() % 2;
            dut->instr_gnt_i = rand() % 2;
            dut->instr_rdata_i = rand();
            dut->instr_rvalid_i = rand() % 2;
            dut->instr_err_i = rand() % 2;
            dut->instr_err_pmp_i = rand() % 2;
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


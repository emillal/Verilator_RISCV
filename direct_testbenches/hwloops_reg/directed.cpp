#include "Vcv32e40p_hwloop_regs.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

// Helper function to apply directed stimulus
void apply_stimulus(Vcv32e40p_hwloop_regs* dut, int cycle) {
    switch (cycle) {
        case 0:
            // Initial state with no operations
            dut->hwlp_start_data_i = 0;
            dut->hwlp_end_data_i = 0;
            dut->hwlp_cnt_data_i = 0;
            dut->hwlp_we_i = 0;
            dut->hwlp_regid_i = 0;
            dut->valid_i = 0;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 1:
            // Write start address to register 0
            dut->hwlp_start_data_i = 0x1000;
            dut->hwlp_we_i = 1;
            dut->hwlp_regid_i = 0;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 2:
            // Write end address to register 0
            dut->hwlp_end_data_i = 0x2000;
            dut->hwlp_we_i = 2;
            dut->hwlp_regid_i = 0;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 3:
            // Write counter to register 0
            dut->hwlp_cnt_data_i = 10;
            dut->hwlp_we_i = 4;
            dut->hwlp_regid_i = 0;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 4:
            // Decrement counter in register 0
            dut->hwlp_we_i = 0;
            dut->hwlp_regid_i = 0;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 1;
            break;
        case 5:
            // Write start address to register 1
            dut->hwlp_start_data_i = 0x3000;
            dut->hwlp_we_i = 1;
            dut->hwlp_regid_i = 1;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 6:
            // Write end address to register 1
            dut->hwlp_end_data_i = 0x4000;
            dut->hwlp_we_i = 2;
            dut->hwlp_regid_i = 1;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 7:
            // Write counter to register 1
            dut->hwlp_cnt_data_i = 5;
            dut->hwlp_we_i = 4;
            dut->hwlp_regid_i = 1;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 0;
            break;
        case 8:
            // Decrement counter in register 1
            dut->hwlp_we_i = 0;
            dut->hwlp_regid_i = 1;
            dut->valid_i = 1;
            dut->hwlp_dec_cnt_i = 2;
            break;
        // Add more cases as needed to test various scenarios
        default:
            dut->hwlp_we_i = 0;
            dut->valid_i = 0;
            dut->hwlp_dec_cnt_i = 0;
            break;
    }
}

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
    for (int cycle = 0; cycle < timeout; ++cycle) {
        // Toggle clock
        dut->clk = !dut->clk;
        dut->eval();
        vcd->dump(sim_time);
        sim_time++;

        if (cycle % 10 == 0) {
            apply_stimulus(dut, cycle / 10);
        }

        
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


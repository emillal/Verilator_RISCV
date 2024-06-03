#include "Vcv32e40p_decoder.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 100000000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_decoder *dut = new Vcv32e40p_decoder;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 100);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_decoder_tb.vcd");


    // Main simulation loop
    while (sim_time < timeout) {
        // Toggle clock
        //dut->clk = !dut->clk;

        // Randomize test vectors
        if (sim_time % 10 == 0) {
            //dut->clk = rand() % 2;
            dut->rst_n = rand() % 2;
            dut->deassert_we_i = rand() % 2;

            dut->instr_rdata_i = rand();
            dut->illegal_c_insn_i = rand() % 2;

            dut->frm_i = rand() % 8; // Assuming 3-bit rounding mode
            dut->current_priv_lvl_i = rand() % 4; // Assuming 4 privilege levels
            dut->mcounteren_i = rand();

            dut->debug_mode_i = rand() % 2;
            dut->debug_wfi_no_sleep_i = rand() % 2;
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


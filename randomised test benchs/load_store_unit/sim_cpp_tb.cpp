#include "Vcv32e40p_load_store_unit.h"
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
    Vcv32e40p_load_store_unit *dut = new Vcv32e40p_load_store_unit;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_load_store_unit_tb.vcd");

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

        // Randomize test vectors every 10 cycles
        if (sim_time % 10 == 0) {
            dut->rst_n = rand() % 2;
            dut->data_we_ex_i = rand() % 2;
            dut->data_type_ex_i = rand() % 4; // 2-bit value
            dut->data_wdata_ex_i = rand();
            dut->data_reg_offset_ex_i = rand() % 4; // 2-bit value
            dut->data_load_event_ex_i = rand() % 2;
            dut->data_sign_ext_ex_i = rand() % 4; // 2-bit value
            dut->data_req_ex_i = rand() % 2;
            dut->operand_a_ex_i = rand();
            dut->operand_b_ex_i = rand();
            dut->addr_useincr_ex_i = rand() % 2;
            dut->data_misaligned_ex_i = rand() % 2;
            dut->data_atop_ex_i = rand() % 64; // 6-bit value
            dut->data_gnt_i = rand() % 2;
            dut->data_rvalid_i = rand() % 2;
            dut->data_err_i = rand() % 2;
            dut->data_err_pmp_i = rand() % 2;
            dut->data_rdata_i = rand();
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


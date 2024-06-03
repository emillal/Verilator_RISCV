#include "Vcv32e40p_sleep_unit.h" // Update with the correct module header file
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

// Helper function to apply directed stimulus
void apply_stimulus(Vcv32e40p_sleep_unit* dut, int cycle) {
    // Apply specific values to inputs based on the cycle number
    switch (cycle) {
        case 0:
            dut->scan_cg_en_i = 0;
            dut->fetch_enable_i = 1;
            dut->if_busy_i = 0;
            dut->ctrl_busy_i = 0;
            dut->lsu_busy_i = 0;
            dut->apu_busy_i = 0;
            dut->pulp_clock_en_i = 0;
            dut->p_elw_start_i = 0;
            dut->p_elw_finish_i = 0;
            dut->debug_p_elw_no_sleep_i = 0;
            dut->wake_from_sleep_i = 0;
            break;
        case 1:
            dut->scan_cg_en_i = 1;
            dut->fetch_enable_i = 0;
            dut->if_busy_i = 1;
            dut->ctrl_busy_i = 0;
            dut->lsu_busy_i = 0;
            dut->apu_busy_i = 0;
            dut->pulp_clock_en_i = 0;
            dut->p_elw_start_i = 0;
            dut->p_elw_finish_i = 0;
            dut->debug_p_elw_no_sleep_i = 0;
            dut->wake_from_sleep_i = 1;
            break;
        case 2:
            dut->scan_cg_en_i = 0;
            dut->fetch_enable_i = 1;
            dut->if_busy_i = 0;
            dut->ctrl_busy_i = 1;
            dut->lsu_busy_i = 0;
            dut->apu_busy_i = 1;
            dut->pulp_clock_en_i = 1;
            dut->p_elw_start_i = 1;
            dut->p_elw_finish_i = 0;
            dut->debug_p_elw_no_sleep_i = 1;
            dut->wake_from_sleep_i = 0;
            break;
        // Add more cases as needed for further testing
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_sleep_unit *dut = new Vcv32e40p_sleep_unit;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_sleep_unit_tb.vcd");

    // Reset the DUT
    dut->clk_ungated_i = 0;
    dut->rst_n = 0;
    dut->scan_cg_en_i = 0;
    dut->fetch_enable_i = 0;
    dut->if_busy_i = 0;
    dut->ctrl_busy_i = 0;
    dut->lsu_busy_i = 0;
    dut->apu_busy_i = 0;
    dut->pulp_clock_en_i = 0;
    dut->p_elw_start_i = 0;
    dut->p_elw_finish_i = 0;
    dut->debug_p_elw_no_sleep_i = 0;
    dut->wake_from_sleep_i = 0;
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
        dut->clk_ungated_i = !dut->clk_ungated_i;
        dut->eval();
        vcd->dump(sim_time);
        sim_time++;

        if (cycle % 10 == 0) {
            apply_stimulus(dut, cycle / 10);
        }

        // Print monitored signals at specific intervals
        if (sim_time % 20 == 0) {
            std::cout << "Time: " << sim_time
                      << ", clk_gated_o: " << dut->clk_gated_o
                      << ", core_sleep_o: " << dut->core_sleep_o
                      << ", fetch_enable_o: " << dut->fetch_enable_o
                      << std::endl;
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


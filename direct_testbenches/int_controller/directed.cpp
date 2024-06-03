#include "Vcv32e40p_int_controller.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

// Helper function to apply directed stimulus
void apply_stimulus(Vcv32e40p_int_controller* dut, int cycle) {
    switch (cycle) {
        case 0:
            // Initial state with no interrupts
            dut->irq_i = 0;
            dut->irq_sec_i = 0;
            dut->mie_bypass_i = 0;
            dut->m_ie_i = 0;
            dut->u_ie_i = 0;
            dut->current_priv_lvl_i = 0;
            break;
        case 1:
            // Issue a non-secure interrupt
            dut->irq_i = 1 << 3; // Interrupt at position 3
            dut->irq_sec_i = 0;
            dut->mie_bypass_i = 1 << 3; // Enable interrupt at position 3
            dut->m_ie_i = 1;
            dut->u_ie_i = 0;
            dut->current_priv_lvl_i = 3; // Machine mode
            break;
        case 2:
            // Issue a secure interrupt
            dut->irq_i = 1 << 7; // Interrupt at position 7
            dut->irq_sec_i = 1;
            dut->mie_bypass_i = 1 << 7; // Enable interrupt at position 7
            dut->m_ie_i = 1;
            dut->u_ie_i = 0;
            dut->current_priv_lvl_i = 3; // Machine mode
            break;
        case 3:
            // Issue multiple interrupts
            dut->irq_i = (1 << 3) | (1 << 7); // Interrupts at positions 3 and 7
            dut->irq_sec_i = 1;
            dut->mie_bypass_i = (1 << 3) | (1 << 7); // Enable interrupts at positions 3 and 7
            dut->m_ie_i = 1;
            dut->u_ie_i = 0;
            dut->current_priv_lvl_i = 3; // Machine mode
            break;
        case 4:
            // Disable global interrupt
            dut->irq_i = 1 << 3; // Interrupt at position 3
            dut->irq_sec_i = 0;
            dut->mie_bypass_i = 1 << 3; // Enable interrupt at position 3
            dut->m_ie_i = 0;
            dut->u_ie_i = 0;
            dut->current_priv_lvl_i = 3; // Machine mode
            break;
        case 5:
            // Issue an interrupt in user mode
            dut->irq_i = 1 << 3; // Interrupt at position 3
            dut->irq_sec_i = 0;
            dut->mie_bypass_i = 1 << 3; // Enable interrupt at position 3
            dut->m_ie_i = 0;
            dut->u_ie_i = 1;
            dut->current_priv_lvl_i = 0; // User mode
            break;
        // Add more cases as needed to test various scenarios
        default:
            dut->irq_i = 0;
            dut->irq_sec_i = 0;
            dut->mie_bypass_i = 0;
            dut->m_ie_i = 0;
            dut->u_ie_i = 0;
            dut->current_priv_lvl_i = 0;
            break;
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_int_controller *dut = new Vcv32e40p_int_controller;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_int_controller_tb.vcd");

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


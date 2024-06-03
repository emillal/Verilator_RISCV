#include "Vcv32e40p_ff_one.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;

void apply_test(Vcv32e40p_ff_one* dut, VerilatedVcdC* vcd, vluint64_t& sim_time, uint32_t input) {
    dut->in_i = input;
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Print the results
    std::cout << "Input: 0x" << std::hex << input 
              << " | First one index: " << std::dec << int(dut->first_one_o) 
              << " | No ones: " << dut->no_ones_o << std::endl;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_ff_one *dut = new Vcv32e40p_ff_one;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_ff_one_tb.vcd");

    // Directed Test Cases
    uint32_t test_inputs[] = {
        0x00000000, // No ones
        0x00000001, // First one at index 0
        0x80000000, // First one at index 31
        0x0F000000, // First one at index 24
        0x00000F00, // First one at index 8
        0x00F00000, // First one at index 20
        0x00000008  // First one at index 3
    };

    // Apply each test case
    for (uint32_t input : test_inputs) {
        apply_test(dut, vcd, sim_time, input);
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


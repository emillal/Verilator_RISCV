#include "Vcv32e40p_alu_div.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

// Simulation parameters
vluint64_t sim_time = 0;
vluint64_t timeout = 1000000;

// Helper function to apply a clock cycle
void apply_clock(Vcv32e40p_alu_div* dut, VerilatedVcdC* vcd) {
    dut->Clk_CI = !dut->Clk_CI;
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;
}

// Helper function to reset DUT
void reset_dut(Vcv32e40p_alu_div* dut, VerilatedVcdC* vcd) {
    dut->Rst_RBI = 0;
    apply_clock(dut, vcd);
    dut->Rst_RBI = 1;
    apply_clock(dut, vcd);
}

// Helper function to run a test case
void run_test_case(Vcv32e40p_alu_div* dut, VerilatedVcdC* vcd,
                   uint32_t OpA, uint32_t OpB, uint8_t OpBShift,
                   bool OpBIsZero, bool OpBSign, uint8_t OpCode,
                   bool InVld, bool OutRdy) {
    // Set inputs
    dut->OpA_DI = OpA;
    dut->OpB_DI = OpB;
    dut->OpBShift_DI = OpBShift;
    dut->OpBIsZero_SI = OpBIsZero;
    dut->OpBSign_SI = OpBSign;
    dut->OpCode_SI = OpCode;
    dut->InVld_SI = InVld;
    dut->OutRdy_SI = OutRdy;

    // Apply one clock cycle to process inputs
    apply_clock(dut, vcd);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_alu_div *dut = new Vcv32e40p_alu_div;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_alu_div_tb.vcd");

    // Initialize inputs
    dut->Clk_CI = 0;
    dut->Rst_RBI = 1; // Assume active low reset

    // Reset the DUT
    reset_dut(dut, vcd);

    // Run directed test cases
    std::cout << "Running Test Case 1: Unsigned Division" << std::endl;
    run_test_case(dut, vcd, 50, 10, 5, false, false, 0, true, true);

    std::cout << "Running Test Case 2: Signed Division" << std::endl;
    run_test_case(dut, vcd, 50, -10, 5, false, true, 1, true, true);

    std::cout << "Running Test Case 3: Unsigned Remainder" << std::endl;
    run_test_case(dut, vcd, 50, 7, 3, false, false, 2, true, true);

    std::cout << "Running Test Case 4: Signed Remainder" << std::endl;
    run_test_case(dut, vcd, 50, -7, 3, false, true, 3, true, true);

    // Wait for some cycles to complete operations
    for (int i = 0; i < 100; ++i) {
        apply_clock(dut, vcd);
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


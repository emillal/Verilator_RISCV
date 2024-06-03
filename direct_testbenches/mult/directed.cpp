#include "Vcv32e40p_mult.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

void apply_test_case(Vcv32e40p_mult* dut, VerilatedVcdC* vcd, vluint64_t& sim_time, 
                     uint8_t enable, uint8_t operator_i, 
                     uint32_t op_a, uint32_t op_b, uint32_t op_c, 
                     uint8_t short_subword, uint8_t short_signed, uint8_t imm, 
                     uint8_t dot_signed, uint32_t dot_op_a, uint32_t dot_op_b, uint32_t dot_op_c, 
                     uint8_t is_clpx, uint8_t clpx_shift, uint8_t clpx_img, 
                     uint8_t ex_ready) {

    dut->enable_i = enable;
    dut->operator_i = operator_i;
    dut->op_a_i = op_a;
    dut->op_b_i = op_b;
    dut->op_c_i = op_c;
    dut->short_subword_i = short_subword;
    dut->short_signed_i = short_signed;
    dut->imm_i = imm;
    dut->dot_signed_i = dot_signed;
    dut->dot_op_a_i = dot_op_a;
    dut->dot_op_b_i = dot_op_b;
    dut->dot_op_c_i = dot_op_c;
    dut->is_clpx_i = is_clpx;
    dut->clpx_shift_i = clpx_shift;
    dut->clpx_img_i = clpx_img;
    dut->ex_ready_i = ex_ready;

    // Evaluate DUT
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Print the results
    std::cout << "Time: " << sim_time 
              << " | enable: " << (int)enable
              << " | operator_i: " << (int)operator_i
              << " | op_a: " << op_a
              << " | op_b: " << op_b
              << " | op_c: " << op_c
              << " | result: " << dut->result_o
              << std::endl;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

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

    // Apply directed test cases

    // Test Case 1: Simple multiplication
    apply_test_case(dut, vcd, sim_time, 1, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1); // 2 * 3 = 6

    // Test Case 2: MAC operation
    apply_test_case(dut, vcd, sim_time, 1, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1); // 2 * 3 + 4 = 10

    // Test Case 3: Subword multiplication (unsigned)
    apply_test_case(dut, vcd, sim_time, 1, 2, 0x00020001, 0x00030001, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1); // (1 * 1) + (2 * 3) = 7

    // Test Case 4: Dot product operation
    apply_test_case(dut, vcd, sim_time, 1, 3, 0, 0, 0, 0, 0, 0, 0x00020001, 0x00030001, 0, 0, 0, 0, 0, 1); // Dot product

    // Test Case 5: Complex multiplication with shift
    apply_test_case(dut, vcd, sim_time, 1, 4, 0, 0, 0, 0, 0, 0, 0x00020001, 0x00030001, 0, 1, 2, 0, 1, 1); // Complex mult with shift

    // Continue simulation
    while (sim_time < timeout) {
        // Toggle clock
        dut->clk = !dut->clk;

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


#include "Vcv32e40p_alu.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <ctime>

vluint64_t sim_time = 0;
vluint64_t timeout = 1000; // Reduced timeout for directed testbench

void setInputs(Vcv32e40p_alu* dut, uint8_t enable, uint8_t op, uint32_t op_a, uint32_t op_b, uint32_t op_c, uint8_t vec_mode, uint8_t bmask_a, uint8_t bmask_b, uint8_t imm_vec_ext, uint8_t is_clpx, uint8_t is_subrot, uint8_t clpx_shift, uint8_t ex_ready) {
    dut->enable_i = enable;
    dut->operator_i = op;
    dut->operand_a_i = op_a;
    dut->operand_b_i = op_b;
    dut->operand_c_i = op_c;
    dut->vector_mode_i = vec_mode;
    dut->bmask_a_i = bmask_a;
    dut->bmask_b_i = bmask_b;
    dut->imm_vec_ext_i = imm_vec_ext;
    dut->is_clpx_i = is_clpx;
    dut->is_subrot_i = is_subrot;
    dut->clpx_shift_i = clpx_shift;
    dut->ex_ready_i = ex_ready;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_alu *dut = new Vcv32e40p_alu;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_alu_tb.vcd");

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

    // Test cases
    struct TestCase {
        uint8_t enable;
        uint8_t op;
        uint32_t op_a;
        uint32_t op_b;
        uint32_t op_c;
        uint8_t vec_mode;
        uint8_t bmask_a;
        uint8_t bmask_b;
        uint8_t imm_vec_ext;
        uint8_t is_clpx;
        uint8_t is_subrot;
        uint8_t clpx_shift;
        uint8_t ex_ready;
    };

    std::vector<TestCase> testCases = {
        {1, 0x00, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // AND operation
        {1, 0x01, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // OR operation
        {1, 0x02, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // XOR operation
        {1, 0x03, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // ADD operation
        {1, 0x04, 0x00000002, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // SUB operation
        {1, 0x05, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // SLL operation
        {1, 0x06, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // SRL operation
        {1, 0x07, 0x00000001, 0x00000001, 0x00000000, 0, 0, 0, 0, 0, 0, 0, 1}, // SRA operation
    };

    // Main simulation loop
    for (const auto& testCase : testCases) {
        // Apply test inputs
        setInputs(dut, testCase.enable, testCase.op, testCase.op_a, testCase.op_b, testCase.op_c, testCase.vec_mode, testCase.bmask_a, testCase.bmask_b, testCase.imm_vec_ext, testCase.is_clpx, testCase.is_subrot, testCase.clpx_shift, testCase.ex_ready);
        
        // Toggle clock and evaluate DUT
        for (int i = 0; i < 2; ++i) {
            dut->clk = !dut->clk;
            dut->eval();
            vcd->dump(sim_time);
            sim_time++;
        }

        // Print output values for debugging
        std::cout << "Test Case: " 
                  << " | enable: " << (int)testCase.enable
                  << " | operator: " << (int)testCase.op
                  << " | operand_a: " << std::hex << testCase.op_a 
                  << " | operand_b: " << testCase.op_b
                  << " | result: " << dut->result_o
                  << std::dec << std::endl;
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


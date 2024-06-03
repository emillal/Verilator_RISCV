#include "Vcv32e40p_aligner.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

void apply_reset(Vcv32e40p_aligner *dut, VerilatedVcdC *vcd) {
    dut->clk = 0;
    dut->rst_n = 0;
    dut->eval();
    vcd->dump(sim_time++);
    dut->clk = 1;
    dut->eval();
    vcd->dump(sim_time++);
    dut->clk = 0;
    dut->rst_n = 1;
    dut->eval();
    vcd->dump(sim_time++);
}

void apply_clock(Vcv32e40p_aligner *dut, VerilatedVcdC *vcd) {
    dut->clk = !dut->clk;
    dut->eval();
    vcd->dump(sim_time++);
}

void apply_test_case(Vcv32e40p_aligner *dut, VerilatedVcdC *vcd,
                     bool fetch_valid, bool if_valid, uint32_t fetch_rdata,
                     bool branch, uint32_t branch_addr, bool hwlp_update_pc, uint32_t hwlp_addr) {
    dut->fetch_valid_i = fetch_valid;
    dut->if_valid_i = if_valid;
    dut->fetch_rdata_i = fetch_rdata;
    dut->branch_i = branch;
    dut->branch_addr_i = branch_addr;
    dut->hwlp_update_pc_i = hwlp_update_pc;
    dut->hwlp_addr_i = hwlp_addr;
    
    // Apply one clock cycle
    apply_clock(dut, vcd);

    // Print expected outputs for verification

}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_aligner *dut = new Vcv32e40p_aligner;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_aligner_tb.vcd");

    // Apply reset
    apply_reset(dut, vcd);

    // Apply directed test cases
    apply_test_case(dut, vcd, 1, 0, 0x12345678, 0, 0, 0, 0);
    apply_test_case(dut, vcd, 0, 1, 0x9abcdef0, 1, 0x1000, 0, 0);
    apply_test_case(dut, vcd, 1, 1, 0xffffffff, 0, 0, 1, 0x2000);
    apply_test_case(dut, vcd, 0, 0, 0x00000000, 1, 0x3000, 1, 0x4000);
    apply_test_case(dut, vcd, 1, 0, 0x11111111, 1, 0x5000, 0, 0x6000);
    apply_test_case(dut, vcd, 0, 1, 0x22222222, 0, 0x7000, 1, 0x8000);

    // Complete the remaining simulation time
    while (sim_time < timeout) {
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


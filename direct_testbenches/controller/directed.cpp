#include "Vcv32e40p_controller.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;
vluint64_t timeout = 10000000;

void apply_test_case(Vcv32e40p_controller* dut, VerilatedVcdC* vcd, vluint64_t& sim_time, bool fetch_enable, bool is_fetch_failed, bool illegal_insn, bool ecall_insn, bool mret_insn, uint32_t pc_id) {
    dut->fetch_enable_i = fetch_enable;
    dut->is_fetch_failed_i = is_fetch_failed;
    dut->illegal_insn_i = illegal_insn;
    dut->ecall_insn_i = ecall_insn;
    dut->mret_insn_i = mret_insn;
    dut->pc_id_i = pc_id;

    // Evaluate DUT
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Print the results
    std::cout << "Time: " << sim_time 
              << " | fetch_enable: " << fetch_enable 
              << " | is_fetch_failed: " << is_fetch_failed 
              << " | illegal_insn: " << illegal_insn 
              << " | ecall_insn: " << ecall_insn 
              << " | mret_insn: " << mret_insn 
              << " | pc_id: " << std::hex << pc_id 
              << std::endl;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_controller *dut = new Vcv32e40p_controller;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_controller_tb.vcd");

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
    apply_test_case(dut, vcd, sim_time, 1, 0, 0, 0, 0, 0x00000000); // Normal fetch
    apply_test_case(dut, vcd, sim_time, 0, 1, 0, 0, 0, 0x00000004); // Fetch failed
    apply_test_case(dut, vcd, sim_time, 1, 0, 1, 0, 0, 0x00000008); // Illegal instruction
    apply_test_case(dut, vcd, sim_time, 1, 0, 0, 1, 0, 0x0000000C); // ECALL instruction
    apply_test_case(dut, vcd, sim_time, 1, 0, 0, 0, 1, 0x00000010); // MRET instruction

    #if VM_COVERAGE
        Verilated::threadContextp()->coveragep()->write("vlt_coverage.dat");
    #endif

    // Finalize simulation
    vcd->close();
    delete vcd;
    delete dut;

    return 0;
}


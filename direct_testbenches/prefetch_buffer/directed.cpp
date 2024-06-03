#include "Vcv32e40p_prefetch_buffer.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 100000;

// Helper function to apply directed stimulus
void apply_stimulus(Vcv32e40p_prefetch_buffer* dut, int cycle) {
    switch (cycle) {
        case 0:
            // Initial state with no operations
            dut->req_i = 0;
            dut->branch_i = 0;
            dut->branch_addr_i = 0;
            dut->hwlp_jump_i = 0;
            dut->hwlp_target_i = 0;
            dut->fetch_ready_i = 0;
            dut->instr_gnt_i = 0;
            dut->instr_rdata_i = 0;
            dut->instr_rvalid_i = 0;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
        case 1:
            // Issue a fetch request
            dut->req_i = 1;
            dut->branch_i = 0;
            dut->branch_addr_i = 0;
            dut->hwlp_jump_i = 0;
            dut->hwlp_target_i = 0;
            dut->fetch_ready_i = 1;
            dut->instr_gnt_i = 1;
            dut->instr_rdata_i = 0x12345678;
            dut->instr_rvalid_i = 1;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
        case 2:
            // Issue a branch request
            dut->req_i = 1;
            dut->branch_i = 1;
            dut->branch_addr_i = 0x1000;
            dut->hwlp_jump_i = 0;
            dut->hwlp_target_i = 0;
            dut->fetch_ready_i = 1;
            dut->instr_gnt_i = 1;
            dut->instr_rdata_i = 0;
            dut->instr_rvalid_i = 0;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
        case 3:
            // Handle branch request response
            dut->req_i = 1;
            dut->branch_i = 0;
            dut->branch_addr_i = 0x1000;
            dut->hwlp_jump_i = 0;
            dut->hwlp_target_i = 0;
            dut->fetch_ready_i = 1;
            dut->instr_gnt_i = 1;
            dut->instr_rdata_i = 0xCAFEBABE;
            dut->instr_rvalid_i = 1;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
        case 4:
            // Issue a hardware loop jump request
            dut->req_i = 1;
            dut->branch_i = 0;
            dut->branch_addr_i = 0;
            dut->hwlp_jump_i = 1;
            dut->hwlp_target_i = 0x2000;
            dut->fetch_ready_i = 1;
            dut->instr_gnt_i = 1;
            dut->instr_rdata_i = 0;
            dut->instr_rvalid_i = 0;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
        case 5:
            // Handle hardware loop jump request response
            dut->req_i = 1;
            dut->branch_i = 0;
            dut->branch_addr_i = 0;
            dut->hwlp_jump_i = 0;
            dut->hwlp_target_i = 0x2000;
            dut->fetch_ready_i = 1;
            dut->instr_gnt_i = 1;
            dut->instr_rdata_i = 0xDEADBEEF;
            dut->instr_rvalid_i = 1;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
        case 6:
            // Issue a fetch request and simulate an instruction bus error
            dut->req_i = 1;
            dut->branch_i = 0;
            dut->branch_addr_i = 0;
            dut->hwlp_jump_i = 0;
            dut->hwlp_target_i = 0;
            dut->fetch_ready_i = 1;
            dut->instr_gnt_i = 1;
            dut->instr_rdata_i = 0;
            dut->instr_rvalid_i = 1;
            dut->instr_err_i = 1;
            dut->instr_err_pmp_i = 0;
            break;
        // Add more cases as needed to test various scenarios
        default:
            dut->req_i = 0;
            dut->branch_i = 0;
            dut->hwlp_jump_i = 0;
            dut->fetch_ready_i = 0;
            dut->instr_gnt_i = 0;
            dut->instr_rvalid_i = 0;
            dut->instr_err_i = 0;
            dut->instr_err_pmp_i = 0;
            break;
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_prefetch_buffer *dut = new Vcv32e40p_prefetch_buffer;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_prefetch_buffer_tb.vcd");

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

        // Output the result for debug purposes
        
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


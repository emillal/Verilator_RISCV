#include "Vcv32e40p_if_stage.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

vluint64_t sim_time = 0;
vluint64_t timeout = 1000000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_if_stage *dut = new Vcv32e40p_if_stage;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_if_stage_tb.vcd");

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

        // Randomize test vectors at certain intervals
        if (sim_time % 10 == 0) {
            //dut->clk = rand() % 2;
            dut->rst_n = rand() % 2;

            dut->m_trap_base_addr_i = rand() & 0xFFFFFF;  // 24-bit random value
            dut->u_trap_base_addr_i = rand() & 0xFFFFFF;  // 24-bit random value
            dut->trap_addr_mux_i = rand() % 4;            // 2-bit random value

            dut->boot_addr_i = rand();                    // 32-bit random value
            dut->dm_exception_addr_i = rand();            // 32-bit random value
            dut->dm_halt_addr_i = rand();                 // 32-bit random value

            dut->req_i = rand() % 2;                      // Random value between 0 and 1

            dut->instr_gnt_i = rand() % 2;                // Random value between 0 and 1
            dut->instr_rvalid_i = rand() % 2;             // Random value between 0 and 1
            dut->instr_rdata_i = rand();                  // 32-bit random value
            dut->instr_err_i = rand() % 2;                // Random value between 0 and 1
            dut->instr_err_pmp_i = rand() % 2;            // Random value between 0 and 1

            dut->clear_instr_valid_i = rand() % 2;        // Random value between 0 and 1
            dut->pc_set_i = rand() % 2;                   // Random value between 0 and 1
            dut->mepc_i = rand();                         // 32-bit random value
            dut->uepc_i = rand();                         // 32-bit random value
            dut->depc_i = rand();                         // 32-bit random value
            dut->pc_mux_i = rand() % 16;                  // 4-bit random value
            dut->exc_pc_mux_i = rand() % 8;               // 3-bit random value

            dut->m_exc_vec_pc_mux_i = rand() % 32;        // 5-bit random value
            dut->u_exc_vec_pc_mux_i = rand() % 32;        // 5-bit random value

            dut->jump_target_id_i = rand();               // 32-bit random value
            dut->jump_target_ex_i = rand();               // 32-bit random value

            dut->hwlp_jump_i = rand() % 2;                // Random value between 0 and 1
            dut->hwlp_target_i = rand();                  // 32-bit random value

            dut->halt_if_i = rand() % 2;                  // Random value between 0 and 1
            dut->id_ready_i = rand() % 2;                 // Random value between 0 and 1
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


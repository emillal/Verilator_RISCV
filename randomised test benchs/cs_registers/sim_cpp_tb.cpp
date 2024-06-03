#include "Vcv32e40p_cs_registers.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 10000000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    
    // Seed the random number generator
    std::srand(std::time(nullptr));
    
    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_cs_registers *dut = new Vcv32e40p_cs_registers;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_cs_registers_tb.vcd");

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

        // Randomize test vectors
        if (sim_time % 10 == 0) {
            dut->rst_n =  rand() %2;
            dut->hart_id_i = rand();          // Random 32-bit value
            dut->mtvec_addr_i = rand();       // Random 32-bit value
            dut->csr_mtvec_init_i = rand() % 2; // Random value between 0 and 1

            //dut->csr_addr_i = (rand() % (1 << 12));  // Assuming csr_num_e is 10 bits
            dut->csr_addr_i = (rand() % 4096);
            dut->csr_wdata_i = rand();        // Random 32-bit value
            dut->csr_op_i = (rand() % 4);  // Assuming csr_opcode_e has 4 possible values

            /*for (int i = 0; i < N_HWLP; ++i) {
                dut->hwlp_start_i[i] = rand();  // Random 32-bit value
                dut->hwlp_end_i[i] = rand();    // Random 32-bit value
                dut->hwlp_cnt_i[i] = rand();    // Random 32-bit value
            }*/

            dut->fflags_i = rand() % 32;  // Assuming C_FFLAG bits
            dut->fflags_we_i = rand() % 2;    // Random value between 0 and 1
            dut->mip_i = rand();              // Random 32-bit value
            dut->csr_irq_sec_i = rand() % 2;  // Random value between 0 and 1
            dut->debug_mode_i = rand() % 2;   // Random value between 0 and 1
            dut->debug_cause_i = rand() % 8;  // Assuming 3 bits
            dut->debug_csr_save_i = rand() % 2; // Random value between 0 and 1
            dut->pc_if_i = rand();            // Random 32-bit value
            dut->pc_id_i = rand();            // Random 32-bit value
            dut->pc_ex_i = rand();            // Random 32-bit value

            dut->csr_save_if_i = rand() % 2;  // Random value between 0 and 1
            dut->csr_save_id_i = rand() % 2;  // Random value between 0 and 1
            dut->csr_save_ex_i = rand() % 2;  // Random value between 0 and 1
            dut->csr_restore_mret_i = rand() % 2; // Random value between 0 and 1
            dut->csr_restore_uret_i = rand() % 2; // Random value between 0 and 1
            dut->csr_restore_dret_i = rand() % 2; // Random value between 0 and 1
            dut->csr_cause_i = rand() % 64;   // Random 6-bit value
            dut->csr_save_cause_i = rand() % 2; // Random value between 0 and 1

            // Performance Counters
            dut->mhpmevent_minstret_i = rand() % 2; // Random value between 0 and 1
            dut->mhpmevent_load_i = rand() % 2;    // Random value between 0 and 1
            dut->mhpmevent_store_i = rand() % 2;   // Random value between 0 and 1
            dut->mhpmevent_jump_i = rand() % 2;    // Random value between 0 and 1
            dut->mhpmevent_branch_i = rand() % 2;  // Random value between 0 and 1
            dut->mhpmevent_branch_taken_i = rand() % 2; // Random value between 0 and 1
            dut->mhpmevent_compressed_i = rand() % 2;   // Random value between 0 and 1
            dut->mhpmevent_jr_stall_i = rand() % 2;     // Random value between 0 and 1
            dut->mhpmevent_imiss_i = rand() % 2;        // Random value between 0 and 1
            dut->mhpmevent_ld_stall_i = rand() % 2;     // Random value between 0 and 1
            dut->mhpmevent_pipe_stall_i = rand() % 2;   // Random value between 0 and 1
            dut->apu_typeconflict_i = rand() % 2;       // Random value between 0 and 1
            dut->apu_contention_i = rand() % 2;         // Random value between 0 and 1
            dut->apu_dep_i = rand() % 2;                // Random value between 0 and 1
            dut->apu_wb_i = rand() % 2;                 // Random value between 0 and 1
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


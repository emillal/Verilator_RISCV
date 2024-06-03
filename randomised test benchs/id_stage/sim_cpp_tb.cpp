#include "Vcv32e40p_id_stage.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 1000000;

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    
    // Seed the random number generator
    std::srand(std::time(nullptr));
    
    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_id_stage *dut = new Vcv32e40p_id_stage;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_id_stage_tb.vcd");

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
        dut->clk_ungated_i = !dut->clk_ungated_i;
        // Randomize test vectors
        if (sim_time % 10 == 0) {
            //dut->clk = rand() % 2;
            //dut->clk_ungated_i = rand() % 2;
            dut->rst_n = rand() % 2;
            dut->scan_cg_en_i = rand() % 2;
            dut->fetch_enable_i = rand() % 2;
            dut->instr_valid_i = rand() % 2;
            dut->instr_rdata_i = rand();
            dut->is_compressed_i = rand() % 2;
            dut->illegal_c_insn_i = rand() % 2;
            dut->branch_decision_i = rand() % 2;
            dut->is_fetch_failed_i = rand() % 2;
            dut->pc_id_i = rand();
            dut->ex_ready_i = rand() % 2;
            dut->wb_ready_i = rand() % 2;
            dut->ex_valid_i = rand() % 2;
            dut->apu_read_dep_i = rand() % 2;
            dut->apu_write_dep_i = rand() % 2;
            dut->apu_busy_i = rand() % 2;
            dut->frm_i = rand() % 8;
            dut->current_priv_lvl_i = rand() % 4;
            dut->irq_i = rand();
            dut->irq_sec_i = rand() % 2;
            dut->mie_bypass_i = rand();
            dut->m_irq_enable_i = rand() % 2;
            dut->u_irq_enable_i = rand() % 2;
            dut->debug_req_i = rand() % 2;
            dut->debug_single_step_i = rand() % 2;
            dut->debug_ebreakm_i = rand() % 2;
            dut->debug_ebreaku_i = rand() % 2;
            dut->trigger_match_i = rand() % 2;
            dut->regfile_waddr_wb_i = rand() % 64;
            dut->regfile_we_wb_i = rand() % 2;
            dut->regfile_wdata_wb_i = rand();
            dut->regfile_alu_waddr_fw_i = rand() % 64;
            dut->regfile_alu_we_fw_i = rand() % 2;
            dut->regfile_alu_wdata_fw_i = rand();
            dut->mult_multicycle_i = rand() % 2;
            dut->perf_imiss_i = rand() % 2;
            dut->mcounteren_i = rand();
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


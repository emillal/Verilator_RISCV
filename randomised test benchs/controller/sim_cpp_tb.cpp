#include "Vcv32e40p_controller.h"
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

    // Main simulation loop
    while (sim_time < timeout) {
        // Toggle clock
        dut->clk = !dut->clk;
        dut->clk_ungated_i = !dut->clk_ungated_i;
        
        // Randomize test vectors
        if (sim_time % 10 == 0) {
            dut->rst_n = rand() % 2;
            //dut->clk_ungated_i = rand() % 2;
            dut->fetch_enable_i = rand() % 2;
            dut->is_fetch_failed_i = rand() % 2;
            dut->illegal_insn_i = rand() % 2;
            dut->ecall_insn_i = rand() % 2;
            dut->mret_insn_i = rand() % 2;
            dut->uret_insn_i = rand() % 2;
            dut->dret_insn_i = rand() % 2;
            dut->mret_dec_i = rand() % 2;
            dut->uret_dec_i = rand() % 2;
            dut->dret_dec_i = rand() % 2;
            dut->wfi_i = rand() % 2;
            dut->ebrk_insn_i = rand() % 2;
            dut->fencei_insn_i = rand() % 2;
            dut->csr_status_i = rand() % 2;
            dut->instr_valid_i = rand() % 2;
            dut->pc_id_i = rand();
            dut->is_compressed_i = rand() % 2;
            
           //dut->hwlp_start_addr_i = rand() % 2;
                //dut->hwlp_end_addr_i[i] = rand();
                //dut->hwlp_counter_i[i] = rand();
            
            dut->data_req_ex_i = rand() % 2;
            dut->data_we_ex_i = rand() % 2;
            dut->data_misaligned_i = rand() % 2;
            dut->data_load_event_i = rand() % 2;
            dut->data_err_i = rand() % 2;
            dut->mult_multicycle_i = rand() % 2;
            dut->apu_en_i = rand() % 2;
            dut->apu_read_dep_i = rand() % 2;
            dut->apu_write_dep_i = rand() % 2;
            dut->branch_taken_ex_i = rand() % 2;
            dut->ctrl_transfer_insn_in_id_i = rand() % 4;
            dut->ctrl_transfer_insn_in_dec_i = rand() % 4;
            dut->irq_req_ctrl_i = rand() % 2;
            dut->irq_sec_ctrl_i = rand() % 2;
            dut->irq_id_ctrl_i = rand() % 32;
            dut->irq_wu_ctrl_i = rand() % 2;
            dut->current_priv_lvl_i = rand() % 4; // Assuming PrivLvl_t is 2 bits
            dut->debug_req_i = rand() % 2;
            dut->debug_single_step_i = rand() % 2;
            dut->debug_ebreakm_i = rand() % 2;
            dut->debug_ebreaku_i = rand() % 2;
            dut->trigger_match_i = rand() % 2;
            dut->regfile_we_id_i = rand() % 2;
            dut->regfile_alu_waddr_id_i = rand() % 64;
            dut->regfile_we_ex_i = rand() % 2;
            dut->regfile_waddr_ex_i = rand() % 64;
            dut->regfile_we_wb_i = rand() % 2;
            dut->regfile_alu_we_fw_i = rand() % 2;
            dut->reg_d_ex_is_reg_a_i = rand() % 2;
            dut->reg_d_ex_is_reg_b_i = rand() % 2;
            dut->reg_d_ex_is_reg_c_i = rand() % 2;
            dut->reg_d_wb_is_reg_a_i = rand() % 2;
            dut->reg_d_wb_is_reg_b_i = rand() % 2;
            dut->reg_d_wb_is_reg_c_i = rand() % 2;
            dut->reg_d_alu_is_reg_a_i = rand() % 2;
            dut->reg_d_alu_is_reg_b_i = rand() % 2;
            dut->reg_d_alu_is_reg_c_i = rand() % 2;
            dut->id_ready_i = rand() % 2;
            dut->id_valid_i = rand() % 2;
            dut->ex_valid_i = rand() % 2;
            dut->wb_ready_i = rand() % 2;
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


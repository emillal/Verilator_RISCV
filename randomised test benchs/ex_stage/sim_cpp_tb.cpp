#include "Vcv32e40p_ex_stage.h"
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
    Vcv32e40p_ex_stage *dut = new Vcv32e40p_ex_stage;
    
    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_ex_stage_tb.vcd");

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
            // ALU signals
            dut->rst_n = rand() % 2;
            dut->alu_operator_i = rand() % 128;
            dut->alu_operand_a_i = rand();
            dut->alu_operand_b_i = rand();
            dut->alu_operand_c_i = rand();
            dut->alu_en_i = rand() % 2;
            dut->bmask_a_i = rand() % 32;
            dut->bmask_b_i = rand() % 32;
            dut->imm_vec_ext_i = rand() % 4;
            dut->alu_vec_mode_i = rand() % 4;
            dut->alu_is_clpx_i = rand() % 2;
            dut->alu_is_subrot_i = rand() % 2;
            dut->alu_clpx_shift_i = rand() % 4;

            // Multiplier signals
            dut->mult_operator_i = rand() % 32;
            dut->mult_operand_a_i = rand();
            dut->mult_operand_b_i = rand();
            dut->mult_operand_c_i = rand();
            dut->mult_en_i = rand() % 2;
            dut->mult_sel_subword_i = rand() % 2;
            dut->mult_signed_mode_i = rand() % 4;
            dut->mult_imm_i = rand() % 32;
            dut->mult_dot_op_a_i = rand();
            dut->mult_dot_op_b_i = rand();
            dut->mult_dot_op_c_i = rand();
            dut->mult_dot_signed_i = rand() % 4;
            dut->mult_is_clpx_i = rand() % 2;
            dut->mult_clpx_shift_i = rand() % 4;
            dut->mult_clpx_img_i = rand() % 2;

            // APU signals
            dut->apu_en_i = rand() % 2;
            dut->apu_op_i = rand() % (1 << 6);
            dut->apu_lat_i = rand() % 4;
            /*for (int i = 0; i < 3; ++i) {
                dut->apu_operands_i[i] = rand();
            }*/
            dut->apu_waddr_i = rand() % 64;
            dut->apu_flags_i = rand() % (1 << 15);

            /*for (int i = 0; i < 3; ++i) {
                dut->apu_read_regs_i[i] = rand() % 64;
                dut->apu_read_regs_valid_i[i] = rand() % 2;
            }*/
            dut->apu_read_regs_valid_i = rand() % 8;
            dut->apu_read_dep_o = rand() % 2;

            /*for (int i = 0; i < 2; ++i) {
                dut->apu_write_regs_i[i] = rand() % 64;
                dut->apu_write_regs_valid_i[i] = rand() % 2;
            }*/
            dut->apu_write_regs_valid_i = rand() % 4;
            dut->apu_write_dep_o = rand() % 2;

            dut->apu_perf_type_o = rand() % 2;
            dut->apu_perf_cont_o = rand() % 2;
            dut->apu_perf_wb_o = rand() % 2;
            dut->apu_busy_o = rand() % 2;
            dut->apu_ready_wb_o = rand() % 2;

            dut->apu_req_o = rand() % 2;
            dut->apu_gnt_i = rand() % 2;
            /*for (int i = 0; i < 3; ++i) {
                dut->apu_operands_o[i] = rand();
            }*/
            dut->apu_op_o = rand() % (1 << 6);
            dut->apu_rvalid_i = rand() % 2;
            dut->apu_result_i = rand();

            // LSU signals
            dut->lsu_en_i = rand() % 2;
            dut->lsu_rdata_i = rand();

            // Other signals
            dut->branch_in_ex_i = rand() % 2;
            dut->regfile_alu_waddr_i = rand() % 64;
            dut->regfile_alu_we_i = rand() % 2;
            dut->regfile_we_i = rand() % 2;
            dut->regfile_waddr_i = rand() % 64;
            dut->csr_access_i = rand() % 2;
            dut->csr_rdata_i = rand();
            dut->is_decoding_i = rand() % 2;
            dut->lsu_ready_ex_i = rand() % 2;
            dut->lsu_err_i = rand() % 2;
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


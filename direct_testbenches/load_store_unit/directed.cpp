#include "Vcv32e40p_load_store_unit.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

// Helper function to apply directed stimulus
void apply_stimulus(Vcv32e40p_load_store_unit* dut, int cycle) {
    switch (cycle) {
        case 0:
            // Initial state with no operations
            dut->data_we_ex_i = 0;
            dut->data_type_ex_i = 0;
            dut->data_wdata_ex_i = 0;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 0;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 0;
            dut->operand_a_ex_i = 0;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 0;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 0;
            dut->data_rvalid_i = 0;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0;
            break;
        case 1:
            // Issue a store request
            dut->data_we_ex_i = 1;
            dut->data_type_ex_i = 0; // Word
            dut->data_wdata_ex_i = 0xDEADBEEF;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 0;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 1;
            dut->operand_a_ex_i = 0x1000;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 0;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 1;
            dut->data_rvalid_i = 0;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0;
            break;
        case 2:
            // Issue a load request
            dut->data_we_ex_i = 0;
            dut->data_type_ex_i = 0; // Word
            dut->data_wdata_ex_i = 0;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 1;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 1;
            dut->operand_a_ex_i = 0x1004;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 0;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 1;
            dut->data_rvalid_i = 1;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0xCAFEBABE;
            break;
        case 3:
            // Handle load response
            dut->data_we_ex_i = 0;
            dut->data_type_ex_i = 0; // Word
            dut->data_wdata_ex_i = 0;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 1;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 0;
            dut->operand_a_ex_i = 0;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 0;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 0;
            dut->data_rvalid_i = 1;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0xCAFEBABE;
            break;
        case 4:
            // Issue a misaligned load request
            dut->data_we_ex_i = 0;
            dut->data_type_ex_i = 0; // Word
            dut->data_wdata_ex_i = 0;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 1;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 1;
            dut->operand_a_ex_i = 0x1003;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 1;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 1;
            dut->data_rvalid_i = 0;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0;
            break;
        case 5:
            // Handle misaligned load response
            dut->data_we_ex_i = 0;
            dut->data_type_ex_i = 0; // Word
            dut->data_wdata_ex_i = 0;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 1;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 0;
            dut->operand_a_ex_i = 0;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 1;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 1;
            dut->data_rvalid_i = 1;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0xDEADBEEF;
            break;
        // Add more cases as needed to test various scenarios
        default:
            dut->data_we_ex_i = 0;
            dut->data_type_ex_i = 0;
            dut->data_wdata_ex_i = 0;
            dut->data_reg_offset_ex_i = 0;
            dut->data_load_event_ex_i = 0;
            dut->data_sign_ext_ex_i = 0;
            dut->data_req_ex_i = 0;
            dut->operand_a_ex_i = 0;
            dut->operand_b_ex_i = 0;
            dut->addr_useincr_ex_i = 0;
            dut->data_misaligned_ex_i = 0;
            dut->data_atop_ex_i = 0;
            dut->data_gnt_i = 0;
            dut->data_rvalid_i = 0;
            dut->data_err_i = 0;
            dut->data_err_pmp_i = 0;
            dut->data_rdata_i = 0;
            break;
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_load_store_unit *dut = new Vcv32e40p_load_store_unit;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_load_store_unit_tb.vcd");

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


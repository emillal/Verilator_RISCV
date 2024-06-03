#include "Vcv32e40p_apu_disp.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;
vluint64_t timeout = 1000; // Set appropriate timeout value

// Helper function to apply stimulus
void apply_stimulus(Vcv32e40p_apu_disp* dut, int cycle) {
    // Apply specific values to inputs based on the cycle number
    switch (cycle) {
        case 0:
            dut->enable_i = 1;
            dut->apu_lat_i = 0b01;
            dut->apu_waddr_i = 0x01;
            dut->is_decoding_i = 1;
            dut->read_regs_valid_i = 0b111;
            dut->read_regs_i = 0x01;
            dut->write_regs_valid_i = 0b11;
            dut->write_regs_i = 0x01;
            dut->apu_gnt_i = 1;
            dut->apu_rvalid_i = 1;
            break;
        case 1:
            dut->enable_i = 0;
            dut->apu_lat_i = 0b10;
            dut->apu_waddr_i = 0x02;
            dut->is_decoding_i = 0;
            dut->read_regs_valid_i = 0b011;
            dut->read_regs_i = 0x05;
            dut->write_regs_i = 0x05;
            dut->write_regs_valid_i = 0b01;
            dut->apu_gnt_i = 0;
            dut->apu_rvalid_i = 0;
            break;
        case 2:
            dut->enable_i = 1;
            dut->apu_lat_i = 0b11;
            dut->apu_waddr_i = 0x03;
            dut->is_decoding_i = 1;
            dut->read_regs_i = 0x03;
            dut->read_regs_valid_i = 0b101;
	    dut->write_regs_i = 0x03;
            dut->write_regs_valid_i = 0b10;
            dut->apu_gnt_i = 1;
            dut->apu_rvalid_i = 1;
            break;
        
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_apu_disp* dut = new Vcv32e40p_apu_disp;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_apu_disp_tb.vcd");

    // Initialize signals
    dut->clk_i = 0;
    dut->rst_ni = 0;
    dut->enable_i = 0;
    dut->apu_lat_i = 0;
    dut->apu_waddr_i = 0;
    dut->is_decoding_i = 0;

    dut->apu_gnt_i = 0;
    dut->apu_rvalid_i = 0;

    // Reset the DUT
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Release reset
    dut->rst_ni = 1;
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Main simulation loop
    for (int cycle = 0; cycle < timeout; ++cycle) {
        // Toggle clock
        dut->clk_i = !dut->clk_i;
        dut->eval();
        vcd->dump(sim_time);
        sim_time++;

        if (cycle % 2 == 0) {
            apply_stimulus(dut, cycle / 2);
        }

        // Print monitored signals at specific intervals
        /*if (sim_time % 20 == 0) {
            std::cout << "Time: " << sim_time
                      << ", apu_req_o: " << dut->apu_req_o
                      << ", apu_waddr_o: " << static_cast<int>(dut->apu_waddr_o)
                      << ", apu_multicycle_o: " << static_cast<int>(dut->apu_multicycle_o)
                      << ", apu_singlecycle_o: " << static_cast<int>(dut->apu_singlecycle_o)
                      << ", active_o: " << static_cast<int>(dut->active_o)
                      << ", stall_o: " << static_cast<int>(dut->stall_o)
                      << ", read_dep_o: " << static_cast<int>(dut->read_dep_o)
                      << ", write_dep_o: " << static_cast<int>(dut->write_dep_o)
                      << ", perf_type_o: " << static_cast<int>(dut->perf_type_o)
                      << ", perf_cont_o: " << static_cast<int>(dut->perf_cont_o)
                      << std::endl;
        }*/
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


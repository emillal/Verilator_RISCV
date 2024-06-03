#include "Vcv32e40p_obi_interface.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

vluint64_t sim_time = 0;
vluint64_t timeout = 10000;

// Helper function to apply directed stimulus
void apply_stimulus(Vcv32e40p_obi_interface* dut, int cycle) {
    switch (cycle) {
        case 0:
            // Initial state with no operations
            dut->trans_valid_i = 0;
            dut->trans_addr_i = 0;
            dut->trans_we_i = 0;
            dut->trans_be_i = 0;
            dut->trans_wdata_i = 0;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 0;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
        case 1:
            // Issue a valid transaction (write) and grant it immediately
            dut->trans_valid_i = 1;
            dut->trans_addr_i = 0x1000;
            dut->trans_we_i = 1;
            dut->trans_be_i = 0xF;
            dut->trans_wdata_i = 0xDEADBEEF;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 1;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
        case 2:
            // Issue a valid transaction (read) and grant it immediately
            dut->trans_valid_i = 1;
            dut->trans_addr_i = 0x2000;
            dut->trans_we_i = 0;
            dut->trans_be_i = 0xF;
            dut->trans_wdata_i = 0;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 1;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
        case 3:
            // Return a valid read response
            dut->trans_valid_i = 0;
            dut->trans_addr_i = 0;
            dut->trans_we_i = 0;
            dut->trans_be_i = 0;
            dut->trans_wdata_i = 0;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 0;
            dut->obi_rdata_i = 0xCAFEBABE;
            dut->obi_rvalid_i = 1;
            dut->obi_err_i = 0;
            break;
        case 4:
            // Issue another valid transaction (write) but do not grant it immediately
            dut->trans_valid_i = 1;
            dut->trans_addr_i = 0x3000;
            dut->trans_we_i = 1;
            dut->trans_be_i = 0xF;
            dut->trans_wdata_i = 0x12345678;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 0;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
        case 5:
            // Grant the pending transaction
            dut->trans_valid_i = 1;
            dut->trans_addr_i = 0x3000;
            dut->trans_we_i = 1;
            dut->trans_be_i = 0xF;
            dut->trans_wdata_i = 0x12345678;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 1;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
        case 6:
            // Issue another valid transaction (read) and return an error response
            dut->trans_valid_i = 1;
            dut->trans_addr_i = 0x4000;
            dut->trans_we_i = 0;
            dut->trans_be_i = 0xF;
            dut->trans_wdata_i = 0;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 1;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
        case 7:
            // Return an error response for the read transaction
            dut->trans_valid_i = 0;
            dut->trans_addr_i = 0;
            dut->trans_we_i = 0;
            dut->trans_be_i = 0;
            dut->trans_wdata_i = 0;
            dut->trans_atop_i = 0;
            dut->obi_gnt_i = 0;
            dut->obi_rdata_i = 0;
            dut->obi_rvalid_i = 1;
            dut->obi_err_i = 1;
            break;
        // Add more cases as needed to test various scenarios
        default:
            dut->trans_valid_i = 0;
            dut->obi_gnt_i = 0;
            dut->obi_rvalid_i = 0;
            dut->obi_err_i = 0;
            break;
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_obi_interface *dut = new Vcv32e40p_obi_interface;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_obi_interface_tb.vcd");

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
        if (sim_time % 20 == 0) {
            std::cout << "Time: " << sim_time
                      << " | trans_ready_o: " << dut->trans_ready_o
                      << " | resp_valid_o: " << dut->resp_valid_o
                      << " | resp_rdata_o: " << std::hex << dut->resp_rdata_o
                      << " | resp_err_o: " << dut->resp_err_o
                      << " | obi_req_o: " << dut->obi_req_o
                      << " | obi_addr_o: " << std::hex << dut->obi_addr_o
                      << " | obi_we_o: " << dut->obi_we_o
                      << " | obi_be_o: " << std::hex << dut->obi_be_o
                      << " | obi_wdata_o: " << std::hex << dut->obi_wdata_o
                      << " | obi_atop_o: " << std::hex << dut->obi_atop_o
                      << std::endl;
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


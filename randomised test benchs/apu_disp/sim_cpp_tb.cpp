#include "Vcv32e40p_apu_disp.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

vluint64_t sim_time = 0;
vluint64_t timeout = 1000000; // Set appropriate timeout value


// Helper function to generate random 6-bit values
uint8_t rand6() {
    return static_cast<uint8_t>(rand() % 64);
}

// Helper function to generate random 2-bit values
uint8_t rand2() {
    return static_cast<uint8_t>(rand() % 4);
}

// Helper function to generate random 3-bit values
uint8_t rand3() {
    return static_cast<uint8_t>(rand() % 8);
}

// Function to apply random stimulus
void apply_random_stimulus(Vcv32e40p_apu_disp* dut) {

 //IData read_regs_i[6];

    //dut->clk_i = rand() % 2;
    dut->rst_ni = rand() % 2;
    dut->enable_i = rand() % 2;
    dut->apu_lat_i = rand2();
    dut->apu_waddr_i = rand6();
    dut->is_decoding_i = rand() % 2;
    
    /*dut->read_regs_i[0] = rand6();
    dut->read_regs_i[1] = rand6();
    dut->read_regs_i[2] = rand6();
    dut->read_regs_i[3] = rand6();
    dut->read_regs_i[4] = rand6();
    dut->read_regs_i[5] = rand6();*/

   //dut->read_regs_i = rand6();
    //dut->read_regs_i = rand6();
    
    dut->read_regs_valid_i = rand3();
    //dut->read_regs_valid_i = rand3();
    //dut->read_regs_valid_i[2] = rand3();
 
    //dut->read_regs_valid_i[1] = rand() % 2;
    //dut->read_regs_valid_i[2] = rand() % 2;
    
    dut->write_regs_i = rand2();
    //dut->write_regs_i[1] = rand6();
    
    dut->write_regs_valid_i = rand2();
    //dut->write_regs_valid_i[1] = rand() % 2;
    
    dut->apu_gnt_i = rand() % 2;
    dut->apu_rvalid_i = rand() % 2;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    
    // Seed the random number generator
    std::srand(std::time(nullptr));
    
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
    
    dut->read_regs_i = 0;
    //dut->read_regs_i[1] = 0;
    //dut->read_regs_i[2] = 0;
    
    dut->read_regs_valid_i = 0;
    //dut->read_regs_valid_i[1] = 0;
    //dut->read_regs_valid_i[2] = 0;
    
    dut->write_regs_i = 0;
    //dut->write_regs_i[1] = 0;
    
    dut->write_regs_valid_i = 0;
    //dut->write_regs_valid_i[1] = 0;
    
    dut->apu_gnt_i = 0;
    //dut->apu_rvalid_i = 0;

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
    while (sim_time < timeout) {
        // Toggle clock
        dut->clk_i = !dut->clk_i;
        
        if (sim_time % 10 == 0) {
            apply_random_stimulus(dut);
        }

        // Evaluate DUT
        dut->eval();
        vcd->dump(sim_time);

        // Print monitored signals
        /*if (sim_time % 10 == 0) {
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


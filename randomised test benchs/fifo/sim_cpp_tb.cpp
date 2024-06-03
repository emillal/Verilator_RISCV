#include "Vcv32e40p_fifo.h"
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
    Vcv32e40p_fifo *dut = new Vcv32e40p_fifo;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_fifo_tb.vcd");

    // Reset the DUT
    //dut->clk_i = 0;
    dut->rst_ni = 0;
    dut->flush_i = 0;
    dut->flush_but_first_i = 0;
    dut->testmode_i = 0;
    dut->data_i = 0;
    dut->push_i = 0;
    dut->pop_i = 0;
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

        // Randomize test vectors
        if (sim_time % 10 == 0) {
            dut->rst_ni = rand(); 
            dut->flush_i = rand() % 2;
            dut->flush_but_first_i = rand() % 2;
            dut->testmode_i = rand() % 2;
            dut->data_i = rand();             // Random 32-bit value
            dut->push_i = rand() % 2;         // Random value between 0 and 1
            dut->pop_i = rand() % 2;          // Random value between 0 and 1
        }

        // Evaluate DUT
        dut->eval();
        vcd->dump(sim_time);

        // Output the result for debug purposes
        /*std::cout << "Time: " << sim_time 
                  << " | Push: " << dut->push_i 
                  << " | Pop: " << dut->pop_i 
                  << " | Data In: " << std::hex << dut->data_i 
                  << " | Data Out: " << std::hex << dut->data_o 
                  << " | Full: " << dut->full_o 
                  << " | Empty: " << dut->empty_o 
                  << " | Count: " << dut->cnt_o 
                  << std::endl;*/

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


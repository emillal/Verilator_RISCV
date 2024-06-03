#include "Vcv32e40p_alu_div.h"
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
    //std::srand(std::time(nullptr));

    // Initialize Verilator context and create the design under test (DUT)
    Vcv32e40p_alu_div *dut = new Vcv32e40p_alu_div;

    // Initialize VCD trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *vcd = new VerilatedVcdC;
    dut->trace(vcd, 99);  // Trace 99 levels of hierarchy
    vcd->open("cv32e40p_alu_div_tb.vcd");


    // Initialize inputs
    dut->Clk_CI = 0;
    dut->Rst_RBI = 0;
    dut->OpA_DI = 0;
    dut->OpB_DI = 0;
    dut->OpBShift_DI = 0;
    dut->OpBIsZero_SI = 0;
    dut->OpBSign_SI = 0;
    dut->OpCode_SI = 0;
    dut->InVld_SI = 0;
    dut->OutRdy_SI = 0;

    // Reset the DUT
    dut->eval();
    vcd->dump(sim_time);
    sim_time++;

    // Release reset
    dut->Rst_RBI = 1;
    dut->Clk_CI = 1;

    while (sim_time < timeout) {
        dut->Clk_CI = !dut->Clk_CI;

        if (sim_time % 10 == 0) {
            // Randomize inputs
            //dut->Clk_CI = rand() % 2;
            dut->Rst_RBI = rand() % 2;
            dut->OpA_DI = rand();
            dut->OpB_DI = rand();
            dut->OpBShift_DI = rand() % 64;
            //dut->OpBIsZero_SI = (dut->OpB_DI == 0);
            dut->OpBIsZero_SI = rand() % 2;
            dut->OpBSign_SI = rand() % 2;
            dut->OpCode_SI = rand() % 4;
            dut->InVld_SI = rand() % 2;
            dut->OutRdy_SI = rand() % 2;
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


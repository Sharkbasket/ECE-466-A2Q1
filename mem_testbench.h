#ifndef MEM_TESTBENCH_H
#define MEM_TESTBENCH_H

#include "systemc.h"
#include "mem_ctrl.h"

SC_MODULE(mem_testbench)
{
    // Ports
    sc_in<bool> complete;
    sc_in_clk clk;
    sc_inout< sc_lv<8> > data;
    sc_out<bool> new_comm, reset;
    sc_out<mem_ctrl::command> comm;
    sc_out< sc_uint<8> > addr;
    
    // Processes
    void mem_test();
    
    // Constructor
    mem_testbench(sc_module_name);
};

#endif
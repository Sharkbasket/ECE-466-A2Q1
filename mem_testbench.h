#ifndef MEM_TESTBENCH_H
#define MEM_TESTBENCH_H

#include "systemc.h"
#include "mem_ctrl.h"
#include <iostream>

SC_MODULE(mem_testbench)
{
    // Ports
    sc_in<bool> complete;
    sc_in_clk clk;
    sc_inout< sc_lv<8> > data;
    sc_out<bool> new_comm, reset;
    sc_out< sc_uint<2> > comm;
    sc_out< sc_uint<8> > addr;
    
    // Constructor
    mem_testbench(sc_module_name);
    
    // Processes
    void mem_test();
    
    // Functions
    void read_byte(sc_uint<8> r_addr);
    void read_block(sc_uint<8> r_addr);
    void write_byte(sc_uint<8> w_addr, sc_uint<8> w_data);
    void write_block(sc_uint<8> w_addr, sc_uint<8> w_data);
};

#endif
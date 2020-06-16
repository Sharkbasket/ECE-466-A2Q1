#ifndef MEM_CTRL_H
#define MEM_CTRL_H

#include "systemc.h"
#include <iostream>

SC_MODULE(mem_ctrl)
{
    // Memory commands
    enum command
    {
        RDBYT,
        RDBLK,
        WRBYT,
        WRBLK
    };
    
    // Ports
    sc_in<bool> new_comm, reset;
    sc_in<command> comm;
    sc_in< sc_uint<8> > addr;
    sc_in_clk clk;
    sc_inout< sc_lv<8> > data;
    sc_out<bool> complete;
    
    // Constructor
    mem_ctrl(sc_module_name);
    
    // Processes
    void mem_process();
    
    // 256-byte memory space
    sc_lv<8> memory[256];
};

#endif

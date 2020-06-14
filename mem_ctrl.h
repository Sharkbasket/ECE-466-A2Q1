#ifndef MEM_CTRL_H
#define MEM_CTRL_H

#include "systemc.h"

SC_MODULE(mem_ctrl)
{
    // Memory commands
    enum command : sc_uint<2> {RDBYT, RDBLK, WTBYT, WTBLK};
    
    // Ports
    sc_in<bool> reset, new_comm, complete;
    sc_in<command> comm;
    sc_in< sc_uint<8> > addr;
    sc_inout< sc_lv<8> > data;
    sc_in_clk clk;
    
    // 256-byte memory space
    sc_lv<8> memory[256];
    
    // Processes
    void mem_process();
    
    // Constructor
    mem_ctrl(sc_module_name);
};

#endif

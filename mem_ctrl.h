#include "systemc.h"


SC_MODULE(mem_ctrl)
{
    // Ports
    sc_in<bool> reset;
    sc_in<bool> new_comm;
    sc_in<bool> complete;
    sc_in< sc_uint<2> > comm;
    sc_in< sc_uint<8> > addr;
    sc_inout< sc_lv<8> > data;
    sc_in_clk clk;
    
    // 256-byte memory space
    sc_lv<8> memory[256];
    
    // Processes
    void mem_process();
    
    // Constructor
    mem_ctrl(sc_module_name n);
}

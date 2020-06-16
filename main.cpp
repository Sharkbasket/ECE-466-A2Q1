#include "systemc.h"
#include "mem_ctrl.h"
#include "mem_testbench.h"

int sc_main(int argc, char* argv[])
{
    // Channels
    sc_clock clk("clk", 10.0, SC_NS);
    sc_signal<bool> reset, new_comm, complete;
    sc_signal<mem_ctrl::command> comm;
    sc_signal< sc_uint<8> > addr;
    sc_signal_rv<8> data;
    
    // Module instantiation and interconnection
    mem_ctrl mc("mc");
    mc.addr(addr);
    mc.clk(clk);
    mc.comm(comm);
    mc.complete(complete);
    mc.data(data);
    mc.new_comm(new_comm);
    mc.reset(reset);
    
    mem_testbench tb("tb");
    tb.addr(addr);
    tb.clk(clk);
    tb.comm(comm);
    tb.complete(complete);
    tb.data(data);
    tb.new_comm(new_comm);
    tb.reset(reset);
    
    sc_start();
    
    return 0;
}
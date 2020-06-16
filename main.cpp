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
    
    // Set up waveform tracing
    sc_trace_file* tf = sc_create_vcd_trace_file("mem_ctrl_traces");
    tf->set_time_unit(1, SC_NS);
    sc_trace(tf, addr, "addr");
    sc_trace(tf, clk, "clk");
    sc_trace(tf, comm, "comm");
    sc_trace(tf, complete, "complete");
    sc_trace(tf, data, "data");
    sc_trace(tf, new_comm, "new_comm");
    sc_trace(tf, reset, "reset");
    
    sc_start();
    
    sc_close_vcd_trace_file(tf);
    
    return 0;
}
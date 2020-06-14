#include "systemc.h"
#include "mem_ctrl.h"
#include <iostream>

// Constructor
SC_HAS_PROCESS(mem_ctrl);
mem_ctrl::mem_ctrl(sc_module_name n) : sc_module(n)
{
    SC_CTHREAD(mem_process, clk.pos());
    reset_signal_is(reset);
}


// Process incoming commands
void mem_process()
{
    // Variables to store values sampled from input ports
    sc_uint<8> addr_s;
    command comm_s;
    sc_lv<8> data_s;
    
    // Reset actions
    if (reset->read() == true)
    {
        complete->write(false);
        wait();
    }
    
    // Main process loop
    while (true)
    {
        addr_s = addr->read();
        comm_s = comm->read();
        switch (comm_s)
        {
            // Read a single byte from memory
            case RDBYT:                
                // Retrieve data from memory
                data_s = memory[addr_s];
                std::cout << "@" << sc_time_stamp() << ": RDBYT, address = "
                          << addr_s << ", data = " << data_s << endl;
                wait();
                
                // Write data to bus
                data->write(data_s);
                break;
            
            // Read a 4-byte block from memory
            case RDBLK:
                for (int i = 0; (i < 4) && ((addr_s + i) < 256); i++)
                {
                    // Retrieve data from memory
                    data_s = memory[addr_s + i];
                    std::cout << "@" << sc_time_stamp()
                              << ": RDBLK-" << i << ", address = "
                              << addr_s + i << ", data = " << data_s << endl;
                    wait();
                    
                    // Write data to bus
                    data->write(data_s);
                }
                break;
            
            // Write a single byte to memory
            case WTBYT:
                // Retrieve data from the bus
                data_s = data->read();
                std::cout << "@" << sc_time_stamp() << ": WTBYT, address = "
                          << addr_s << ", data = " << data_s << endl;
                wait();
                
                // Write data to memory
                memory[addr_s] = data_s;
                break;
            
            // Write a 4-byte block to memory
            case WTBLK:
                for (int i = 0; (i < 4) && ((addr_s + i) < 256); i++)
                {
                    // Retrieve data from the bus
                    data_s = data->read();
                    std::cout << "@" << sc_time_stamp()
                              << ": WTBLK-" << i << ", address = "
                              << addr_s + i << ", data = " << data_s << endl;
                    wait();
                    
                    // Write data to memory
                    memory[addr_s + i] = data_s;
                }
                break;
        } // end switch
        
        // Assert complete and wait for handshake (de-assertion of new_comm)
        complete->write(true);
        while (new_comm->read() == true)
        {
            wait();
        }
        
        // If a read command was performed, release the bus
        if (comm_s == RDBYT || comm_s == RDBLK)
        {
            data->write("ZZZZZZZZ");
        }
        
        // De-assert complete to signal ready for next command
        complete->write(false);
        wait();
    } // end while
} // end mem_process

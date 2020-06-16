#include "mem_ctrl.h"

const sc_lv<8> Z("ZZZZZZZZ");

// Constructor
SC_HAS_PROCESS(mem_ctrl);
mem_ctrl::mem_ctrl(sc_module_name n) : sc_module(n)
{
    SC_CTHREAD(mem_process, clk.pos());
    reset_signal_is(reset, true);
}


// Process incoming commands
void mem_ctrl::mem_process()
{
    // Variables to store values sampled from input ports
    command comm_s;
    sc_uint<8> addr_s;
    sc_uint<8> data_s;
    
    // Reset actions
    if (reset->read() == true)
    {
        complete->write(false);
        wait();
    }
    
    // Main process loop
    while (true)
    {
        // Wait for arrival of new command
        while (new_comm->read() == false)
        {
            wait();
        }
        
        addr_s = addr->read();
        comm_s = comm->read();
        switch (comm_s)
        {
            // Read a single byte from memory
            case RDBYT:
                data_s = memory[addr_s];
                std::cout << "@" << sc_time_stamp()
                          << ": RDBYT, address = " << addr_s
                          << ", data = " << data_s << endl;
                wait();
                data->write(data_s);
                break;
            
            // Read a 4-byte block from memory
            case RDBLK:
                for (int i = 0; (i < 4) && ((addr_s + i) < 256); i++)
                {
                    data_s = memory[addr_s + i];
                    std::cout << "@" << sc_time_stamp() << ": RDBLK-" << i
                              << ", address = " << addr_s + i
                              << ", data = " << data_s << endl;
                    wait();
                    data->write(data_s);
                }
                break;
            
            // Write a single byte to memory
            case WRBYT:
                data_s = data->read();
                std::cout << "@" << sc_time_stamp()
                          << ": WRBYT, address = " << addr_s
                          << ", data = " << data_s << endl;
                wait();
                memory[addr_s] = data_s;
                break;
            
            // Write a 4-byte block to memory
            case WRBLK:
                for (int i = 0; (i < 4) && ((addr_s + i) < 256); i++)
                {
                    data_s = data->read();
                    std::cout << "@" << sc_time_stamp() << ": WRBLK-" << i
                              << ", address = " << addr_s + i
                              << ", data = " << data_s << endl;
                    wait();
                    memory[addr_s + i] = data_s;
                }   
                break;
            
            default:
                std::cout << "Unrecognized command: " << comm_s << endl;
                break;
        } // end switch
        
        // Assert complete and wait for handshake
        complete->write(true);
        while (new_comm->read() == true)
        {
            wait();
        }
        
        // Release data bus and signal ready for next command
        data->write(Z);
        complete->write(false);
        wait();
    } // end while
} // end mem_process

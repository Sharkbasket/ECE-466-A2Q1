#include "mem_testbench.h"

const sc_lv<8> Z("ZZZZZZZZ");

// Constructor
SC_HAS_PROCESS(mem_testbench);
mem_testbench::mem_testbench(sc_module_name n) : sc_module(n)
{
    SC_CTHREAD(mem_test, clk.pos());
}

// Test procedure
void mem_testbench::mem_test()
{
    reset->write(true);
    wait();
    reset->write(false);
    wait();
    
    write_byte(0, 0);
    write_byte(255, 255);
    read_byte(0);
    read_byte(255);
    write_block(170, 64);
    write_byte(85, 128);
    read_block(64);
    read_byte(128);
    
    sc_stop();
}

// Write a single byte to memory
void mem_testbench::write_byte(sc_uint<8> w_data, sc_uint<8> w_addr)
{
    while (complete->read() == true)
    {
        wait();
    }
    
    addr->write(w_addr);
    data->write(w_data);
    comm->write(mem_ctrl::WRBYT);
    new_comm->write(true);
    
    while (complete->read() == false)
    {
        wait();
    }
    
    data->write(Z);
    new_comm->write(false);
    wait();
}

// Write a 4-byte block to memory
void mem_testbench::write_block(sc_uint<8> w_data, sc_uint<8> w_addr)
{
    while (complete->read() == true)
    {
        wait();
    }
    
    addr->write(w_addr);
    data->write(w_data);
    comm->write(mem_ctrl::WRBLK);
    new_comm->write(true);
    
    while (complete->read() == false)
    {
        wait();
    }
    
    data->write(Z);
    new_comm->write(false);
    wait();
}

void mem_testbench::read_byte(sc_uint<8> r_addr)
{
    while (complete->read() == true)
    {
        wait();
    }
    
    addr->write(r_addr);
    comm->write(mem_ctrl::RDBYT);
    new_comm->write(true);
    
    while (complete->read() == false)
    {
        wait();
    }
    
    std::cout << "Read data " << data->read()
              << " from address " << r_addr << endl;
    
    data->write(Z);
    new_comm->write(false);
    wait();
}

void mem_testbench::read_block(sc_uint<8> r_addr)
{
    sc_uint<8> block[4];
    
    while (complete->read() == true)
    {
        wait();
    }
    
    addr->write(r_addr);
    comm->write(mem_ctrl::RDBLK);
    new_comm->write(true);
    
    while (data->read() == Z)
    {
        wait();
    }
    
    for (int i = 0; i < 4; i++)
    {
        block[i] = data->read();
        wait();
    }
    
    std::cout << "Read data [" << block[0] << ", " << block[1] << ", "
              << block[2] << ", " << block[3] 
              << "] from address " << r_addr << endl;
    
    data->write(Z);
    new_comm->write(false);
    wait();
}

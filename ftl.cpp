#include <iostream>
#include <vector>
#include <map>

// Constants for our simulated Flash
const int TOTAL_BLOCKS = 4;
const int PAGES_PER_BLOCK = 4;
const int INVALID_ADDR = -1;

struct PhysicalPage
{
    int data = 0;
    bool is_valid = false;
    bool is_free = true;
};

struct Block
{
    std::vector<PhysicalPage> pages;
    int erase_count = 0;

    Block() : pages(PAGES_PER_BLOCK) {}
};

class FTL_Simulator
{
private:
    std::vector<Block> flash_memory;
    std::map<int, int> l2p_table; // Logical-to-Physical Mapping

public:
    FTL_Simulator() : flash_memory(TOTAL_BLOCKS) {}

    // Simulate Writing Data (Out-of-place update logic)
    void write_data(int lba, int data)
    {
        // 1. Invalidate old mapping if it exists
        if (l2p_table.count(lba))
        {
            invalidate_old_page(l2p_table[lba]);
        }

        // 2. Find a free physical page (Simple Linear Search)
        for (int b = 0; b < TOTAL_BLOCKS; b++)
        {
            for (int p = 0; p < PAGES_PER_BLOCK; p++)
            {
                if (flash_memory[b].pages[p].is_free)
                {
                    // Write the data
                    flash_memory[b].pages[p].data = data;
                    flash_memory[b].pages[p].is_valid = true;
                    flash_memory[b].pages[p].is_free = false;

                    // Update L2P Table (The "Firmware" heart)
                    int physical_addr = (b * PAGES_PER_BLOCK) + p;
                    l2p_table[lba] = physical_addr;

                    std::cout << "LBA " << lba << " written to Physical Addr " << physical_addr << "\n";
                    return;
                }
            }
        }
        std::cout << "Disk Full! Garbage Collection required.\n";
    }

    void invalidate_old_page(int phys_addr)
    {
        int b = phys_addr / PAGES_PER_BLOCK;
        int p = phys_addr % PAGES_PER_BLOCK;
        flash_memory[b].pages[p].is_valid = false;
        std::cout << "  [Info] Physical Addr " << phys_addr << " marked INVALID (stale).\n";
    }

    void read_data(int lba)
    {
        if (l2p_table.count(lba))
        {
            int phys_addr = l2p_table[lba];
            int b = phys_addr / PAGES_PER_BLOCK;
            int p = phys_addr % PAGES_PER_BLOCK;
            std::cout << "Read LBA " << lba << " -> Value: " << flash_memory[b].pages[p].data << "\n";
        }
        else
        {
            std::cout << "LBA " << lba << " not found!\n";
        }
    }
};

int main()
{
    FTL_Simulator mySSD;

    std::cout << "--- Step 1: Writing Data ---\n";
    mySSD.write_data(10, 500); // Write LBA 10
    mySSD.write_data(20, 700); // Write LBA 20

    std::cout << "\n--- Step 2: Overwriting LBA 10 (Demonstrating Out-of-Place Update) ---\n";
    mySSD.write_data(10, 501); // Update LBA 10

    std::cout << "\n--- Step 3: Reading Data ---\n";
    mySSD.read_data(10);
    mySSD.read_data(20);

    return 0;
}
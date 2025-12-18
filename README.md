## Flash Translation Layer (FTL) Simulator

A lightweight C++ simulation of a NAND Flash Controller's firmware logic. This project demonstrates the core complexities of managing non-volatile memory, specifically handling the "erase-before-write" constraint through logical-to-physical address indirection.

## 🚀 Overview

Unlike traditional Hard Disk Drives (HDDs), NAND Flash memory cannot be overwritten directly. Data must be written to empty pages, and old data must be marked "invalid" until the entire block is erased. This simulator implements a Page-Level FTL to manage these hardware limitations transparently from the host OS.

## ✨ Key Features

Logical-to-Physical (L2P) Mapping: Implemented a dynamic mapping table using std::map to decouple host logical addresses from physical NAND locations.

Out-of-Place Updates: Simulated the standard SSD write strategy where data updates are written to new free pages, invalidating the previous physical location.

Status Tracking: Each physical page tracks state metadata: Free, Valid, or Invalid (Stale).

Wear Leveling (Concept): Architecture designed to support erase-count tracking to ensure even wear across the flash blocks.

## 🛠️ System Architecture

The simulator is structured to mirror real-world SSD hierarchy:

SSD Controller (FTL): The intelligence layer that handles read/write requests.

Blocks: The smallest unit of erasure.

Pages: The smallest unit of programming (writing) and reading.

## 💻 Technical Implementation

The core logic resides in the FTL_Simulator class:

Write Logic: 1. Check if the Logical Block Address (LBA) exists. 2. If yes, mark the old Physical Page as Invalid. 3. Search for the next available Free page. 4. Update the L2P table with the new physical coordinate.

Read Logic: 1. Perform a constant-time lookup in the L2P table. 2. Direct access to the physical memory array.

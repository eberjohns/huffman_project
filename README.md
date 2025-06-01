# 🌳 Huffman Compressor/Decompressor 🌳

This project implements a classic Huffman Coding algorithm for file compression and decompression in C. It's designed to be a lightweight, command-line tool that allows you to compress text files into a more compact binary format and then decompress them back to their original form.

The primary goal of this project is to demonstrate the core principles of Huffman coding, including:

- Frequency Analysis: How character frequencies are used to build an optimal compression tree.
- Tree Construction: Utilizing a min-priority queue to build the Huffman tree.
- Code Generation: Deriving variable-length binary codes for each character based on their frequencies.
- Bit-level I/O: Efficiently reading and writing individual bits to achieve true compression.
- Separate Key (Map File): Demonstrating how the encoding "key" (the character-to-code map) can be externalized, offering transparency and flexibility for users.
---

## 🚀 Getting Started
To use this project, you'll need a C compiler (like GCC) installed on your system.

**⬇️ Download & Setup**
1. Clone this repository to your local machine (or download the source code).
2. Navigate into the `c_logic` folder where all the source `.c` and header `.h` files are located.

**📦 Pre-Built Executables (Quick Start)**

You have the pre-built `huffman_compressor.exe` and `huffman_decompressor.exe` (or just `huffman_compressor` and `huffman_decompressor` on Linux/macOS) in the root folder, you can use them directly without compiling.

**🖥️ How to Use**

This project consists of two main programs: `huffman_compressor` and `huffman_decompressor`.

**1. Compressing a File**

The compressor takes an input text file, generates a compressed binary file, and creates a separate map file containing the Huffman codes.

**Command:**
```Bash
huffman_compressor <input_text_file> <output_compressed_file> <output_map_file>
```

- `<input_text_file>`: The path to the text file you want to compress (e.g., `my_document.txt`).
- `<output_compressed_file>`: The path where the compressed binary data will be saved (e.g., `my_document.bin`).
- `<output_map_file>`: The path where the Huffman character-to-code map will be saved (e.g., `my_document_map.txt`). This file is crucial for decompression!

**Example:**

```Bash
huffman_compressor input.txt compressed.bin huffman_map.txt
```

**2. Decompressing a File**

The decompressor uses the compressed binary file and the corresponding map file to reconstruct the original text file.

**Command:**

```Bash
huffman_decompressor <compressed_input_file> <map_file> <decompressed_output_file>
```
- `<compressed_input_file>`: The path to the compressed binary file (e.g., `compressed.bin`).
- `<map_file>`: The path to the Huffman map file generated during compression (e.g., `huffman_map.txt`).
- `<decompressed_output_file>`: The path where the original decompressed text will be saved (e.g., `decompressed.txt`).

**Example:**
```Bash
huffman_decompressor compressed.bin huffman_map.txt decompressed.txt
```

## 🛠️ Building the Project from Source

If you want to compile the project yourself (or modify it), follow these steps from within the `c_logic` directory.

**🏗️ Compilation Commands**

You'll use gcc to compile the source files into executables.

**For the Compressor:**
```Bash
gcc compress_main.c encoder.c linked_list.c min_priority_queue.c -o huffman_compressor
```

**For the Decompressor:**
```Bash
gcc decompress_main.c decoder.c huffman_node.c min_priority_queue.c -o huffman_decompressor
```

After successful compilation, you will find the `huffman_compressor` and `huffman_decompressor` executables in your `c_logic` directory. You can then move them to your root folder as you've already done!

**✍️ How to Modify the Code**

The project is modularized into several `.c` and `.h` files, making it easier to navigate and modify.

- Header Files (`.h`): These files contain declarations of data structures (like `struct HuffmanNode`) and function prototypes. They act as blueprints for what's available.
- Source Files (`.c`): These files contain the actual implementations (the code blocks `{...}`) for the functions declared in the header files.

Here's a breakdown of what each file does:

- `compress_main.c`: Contains the main function for the compression executable. It orchestrates the entire compression process, from reading the input file and building the frequency table to invoking the tree construction, code generation, and bit-writing functions.
- `decompress_main.c`: Contains the main function for the decompression executable. It manages reading the compressed file and the map file, reconstructing the decoding tree, and then performing the bit-by-bit decoding.
- `huffman_node.h`: Defines the core HuffmanNode structure used to build the Huffman tree. It also declares the create_huffman_node function.
- `min_priority_queue.h`: Declares the MinPriorityQueue structure and its associated functions (create_min_pq, insert_pq, extract_min_pq, is_empty_pq, free_min_pq), which are fundamental for building the Huffman tree efficiently.
- `min_priority_queue.c`: Implements all the functions declared in min_priority_queue.h, including the create_huffman_node definition and the heap operations (sifting up/down, swapping nodes).
- `linked_list.h`: Declares the Node structure for a simple linked list and its functions (create_node, linked_list_push, linked_list_pop, linked_list_free). This was used to group characters by frequency before building the priority queue.
- `linked_list.c`: Implements the linked list functions declared in linked_list.h.
- `encoder.h`: Declares the global huffman_codes array and the functions specific to encoding (init_huffman_codes_array, build_huffman_codes, print_huffman_codes, encode_and_write_file, write_huffman_map_to_file). It also declares free_huffman_tree.
- `encoder.c`: Implements all the encoding-related functions declared in encoder.h, including the recursive DFS for code generation and the bit-packing logic for writing the compressed file and the map file.
- `decoder.h`: Declares functions specific to decoding (build_decoding_tree_from_map_file, decode_and_write_file).
- `decoder.c`: Implements the decoding logic, including reading the map file to reconstruct the Huffman tree and then reading the compressed bits to traverse the tree and output characters.

Feel free to explore the code, understand how each component contributes to the overall process, and even experiment with modifications! Happy compressing! 🎉❤✨

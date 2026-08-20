# CS-300: Data Structures and Algorithms
## ABCU Advising Assistance Program — Portfolio Reflection

**Zachary Wheeler | Southern New Hampshire University | CS-300**

---

## Project Artifacts

- **Project One:** Runtime and memory analysis of three data structures (Linked List, Hash Table, Binary Search Tree)
- **Project Two:** `CS 300 Project Two Final Source.cpp` — Working C++ advising program that loads, sorts, and displays ABCU course data

---

## Reflection

### What was the problem you were solving in the projects for this course?

The goal was to build a practical advising tool for the Computer Science department at ABCU — a program that could load course data, present it in a clean alphanumeric list, and let advisors look up individual courses along with their prerequisites. The input files were bare-bones: raw CSV data with course numbers, titles, and prerequisite relationships. My approach was to treat it less like a data exercise and more like an advisor guiding a student. If this were my own university and I had more time, the richer the information the better — but the foundation had to be solid first: accurate data, clean output, and a structure that made sense to the person using it.

### How did you approach the problem? Consider why data structures are important to understand.

I chose a Binary Search Tree as the core data structure, partly because it was one I had prior experience with, and partly because it was the right tool for what this program needed to do. The decision came down to what happens at print time. A linked list would require a separate sort on every print. A hash table offers fast lookup but has no inherent order — you'd still need to sort before displaying. A BST inserts data in sorted order and prints it alphanumerically through an in-order traversal, with no additional sorting step required. For a program that loads once and gets read many times, the BST does the sorting work at insert time so you never pay for it again. Understanding data structures means understanding those tradeoffs before you write a single line of code.

### How did you overcome any roadblocks you encountered while going through the activities or project?

File paths and directory management have been a consistent challenge throughout this course. Even with assistance, nearly every project hit errors that traced back to the program not finding the right file in the right place — incorrect relative paths, mismatched filenames, working directory assumptions that didn't hold across environments. Pushing through that meant slowing down, reading error messages carefully, and learning to think about where the program is running from, not just what it's trying to do. It's a small thing that costs a lot of time until it finally clicks.

### How has your work on this project expanded your approach to designing software and developing programs?

This course reinforced something I now consider essential: diagram before you code. Having a visual map of what data needs to be entered, how it flows through the structure, and how it gets presented on the other end made the implementation dramatically clearer. It's the difference between wandering and navigating. Before CS-300, I would often start writing and figure out the structure as I went. Now I think about the shape of the solution first — inputs, queries, outputs — and let the code follow from that.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

Maintainability, readability, and adaptability all come down to discipline in three areas: comments, naming, and format. Comments placed with intention — explaining the *why*, not just the *what* — make code recoverable months later without having to reverse-engineer your own thinking. Consistent naming conventions make the logic speak for itself. And format matters more than people expect. As a novelist, I already understood that structure carries meaning — that a paragraph break, an indent, a line of white space tells the reader something. In code, that principle is not just stylistic, it's functional. Strategic indentation and spacing were essential to following my own logic through this project, and they're now habits I won't write without.

---

## AI Usage Acknowledgment

Generative AI tools (Google Gemini) were used to assist in the development of portions of this project, in accordance with SNHU's AI usage guidelines. All AI-assisted code was reviewed, tested, and understood before submission. Citation guidance followed the [Shapiro Library AI Citation Guide](https://libguides.snhu.edu/artificialintelligence).

---

*CS-300 | Data Structures and Algorithms | 2026 C-4 (Jun–Aug)*

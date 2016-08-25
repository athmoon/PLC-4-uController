# PLC-4-uController
Project created for my thesis of the bachelor degree of electrical engineering.

A set of software which is capable of represent, in an efficient manner, a finite-state machine from a descriptor file, to serve as basis for the software implementation of a PLC (Programmable Logic Controller).

Two programs have been developed. The first program created absorbs the logic of the state machine described and generates a representation for the machine in binary decision diagrams (BDD), which is a minimum, ordered and canonical representation. This description is exported in two different formats (C and XML). 

The first format (C), can be easily imported by any program written in C, but it is committed to describe the finite-state machine at compile time. The second format (XML), brings the flexibility of being used by software in any language, with proper treatment. The format also ensures the possibility of changing the system description at runtime.

The second program developed can be embedded in computers and microcontrollers which contain compiler for the C language. It is able to import the logical descriptions from both C and XML files. From the imported BDD, the software can also effectively (by the very nature of BDD representation), estimate all system’s outputs as well as its next state, based on the current state and the system’s inputs, implementing so the desired finite-state machine. Such implementation avoids redundant and disordered verifications of the inputs and states of the system, very present in textual implementations with if-then-else, for example.

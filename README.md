# PLC-4-uController
Efficient conversion of graphical finite-state machines to C-code implementation, using binary decision diagrams.

This project is constitued by a set of software which is capable of represent, in an efficient manner, a finite-state machine from a descriptor file (espresso format), to serve as basis for the software implementation of a PLC (Programmable Logic Controller). The espresso file is being obtained by a user-friendly program called Orange Cad, in which the user can represent his finite state-machine graphically.

Two programs have been developed. The first one (floresta dir) absorbs the logic of the state machine described and generates a representation for the machine in (ordered) binary decision diagrams (BDD), which is a minimum, ordered and canonical representation. This description is exported in two different formats (C and XML). 

The first format (C), can be easily imported by any program written in C, but it is committed to describe the finite-state machine at compile time. The second format (XML), brings the flexibility of being used by software in any language, with proper treatment. The format also ensures the possibility of changing the system description at runtime.

The second program developed (plcengine dir) is an engine that can be embedded in computers and microcontrollers which contain compilers for the C language. The engine is able to import the logical descriptions from both C and XML files. From the imported BDD, the software can also effectively (by the very nature of BDD representation), estimate all system’s outputs as well as its next state, based on the current state and the system’s inputs, implementing so the desired finite-state machine. Such implementation avoids redundant and disordered verifications of the inputs and states of the system, very present in textual implementations with if-then-else, for example. The engine does not depend on the third-party BDD library used in the first program, i.e., it just needs to include the .C or the .XML file. 

The project was developed in my bachelor degree thesis, that is available in PT-BR (Portuguese) in: https://github.com/vitorroriz/PLC-4-uController/blob/master/Implementação%20eﬁciente%20de%20máquinas%20de%20estados%20ﬁnitos%20para%20aplicações%20de%20controle.pdf

This project utilizes:

* The BuDDy BDD package, for BDD implementation. Available in: https://sourceforge.net/projects/buddy/. With docummentation in: http://buddy.sourceforge.net/manual/main.html
* The Mini-XML library, for XML manipulation. Available in: http://www.msweet.org/projects.php?Z3
* The OrangeCad Web tool to design graphical finite-state machines and generate the espresso files. Available in: https://blog.ufes.br/orangecad/

# Implementation LR(1)-analysis

The objective was to develop a software implementation of a parser based on LR(1) 
grammar, and built on it the table analysis. The result of the analysis present in the 
form of a sequence of numbers grammar rules applied in the analysis process.

THE TABLE ANALYSIS
------------

Grammar:

	<S>::= c a <A>

	<A>::= ( <L> )

	<A>::= lambda

	<L>::= e , <L>

	<L>::= e

D - end

E - error

S - Shift

R - Reduce

/*-----(S)---(A)---(L)---(c)---(a)---(,)---(e)---(()---())---($)---

/*(0)*/ - 'D'---'E'---'E'---'S'1--'E'---'E'---'E'---'E'---'E'---'E'

/*(1)*/	- 'E'---'E'---'E'---'E'---'S'2--'E'---'E'---'E'---'E'---'E' 

/*(2)*/	- 'E'---'S'3--'E'---'E'---'E'---'E'---'E'---'S'4--'E'---'R'2

/*(3)*/	- 'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E'---'R'0

/*(4)*/	- 'E'---'E'---'S'5--'E'---'E'---'E'---'S'---'E'---'E'---'E' 

/*(5)*/	- 'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E'---'S'6--'E'

/*(6)*/	- 'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E'---'R'1

/*(7)*/	- 'E'---'S'8--'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E' 

/*(8)*/	- 'E'---'E'---'E'---'E'---'E'---'S'9--'E'---'E'---'R'4--'E'

/*(9)*/	- 'E'---'E'---'S'---'E'---'E'---'E'---'S'---'E'---'E'---'E'

/*(10)*/ -'E'---'E'---'E'---'E'---'E'---'E'---'E'---'E'---'R'3--'E'


ABOUT LR(1)-analysis
-----------

For internal states of the automaton performing the LR (1) parsing, convenient to use 
an alternate character set that does not intersect with the plurality of terminal and 
non-terminal symbols of the grammar. Information about the current internal state of the 
machine and a history of contact with this state is stored in a stack of states in the 
form of additional characters from this set, and the same symbols are used as row headers 
control table. In addition, in the process of parsing it makes sense to follow the 
transformations of sentential forms. To do this, the machine provides a stack of characters 
that can be placed in the terminal and non-terminal symbols of the grammar. The content of 
the stack of unread characters of the input string at every stage of analysis constitute 
the current sentential form. The content of the stack state and the content of the stack 
characters during parsing change synchronously. Therefore, the software implementation of 
the automaton can be constructed with a stack of records consisting of two fields.

Run the command control table, the machine carries out the transitions between internal 
states. A new internal state for the next transition may be indicated in the tables of the 
selected team, as well as in transition table state machine. Teams of this kind are called 
"shift." Transition to a new state in this case is accompanied by a recording of this state 
to the state stack and moving to the next character of the input string with a record 
reading stack symbol characters. Thus in the stacks accumulation history of behavior of the 
machine. The ability to take advantage of this story appears at the machine after running a 
different type, called "reduction". On the command "cast" of characters are removed from 
the stack symbols, forming the right side of the rule, the number of which is specified in 
the command. Synchronous change contents stacks removes the same number of records from the 
states of the stack, resulting in machine passes into the inner state in which it has 
already been several steps back, and which now remains in the top of the stack after 
removal conditions. Remote right side of the rules in the sentential form should be changed 
to the left side of the rule. This change is manifested in the work of the machine in the 
form of imitation of the non-terminal read from the input string after changing the 
contents of the stack above removal.

The recognition process ends successfully, ie the input string is considered to belong 
language, took a gun, if the input string is completely read, the machine is in the initial 
internal state of the input string and simulates reading axioms. The conclusion is 
incorrect recognizable line automatic detection team "mistake" in situations similar to 
those that have been mentioned in the description of LL (1) parsing.

More info here:

https://en.wikipedia.org/wiki/LR_parser

https://en.wikipedia.org/wiki/LL_parser

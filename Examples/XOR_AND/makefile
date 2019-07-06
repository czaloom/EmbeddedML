objects = xor_and_example.o embeddedML.o 

test :	$(objects)
	cc -o xorand $(objects)


embeddedML.o : embeddedML.h

.PHONY:	clean
clean :
	rm xorand $(objects)

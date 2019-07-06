objects = xor_example.o embeddedML.o 

test :	$(objects)
	cc -o xor $(objects)


embeddedML.o : embeddedML.h

.PHONY:	clean
clean :
	rm xor $(objects)

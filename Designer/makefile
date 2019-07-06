objects = designer.o util.o 

test :	$(objects)
	cc -o designer $(objects)


util.o : util.h

.PHONY:	clean
clean :
	rm designer $(objects)

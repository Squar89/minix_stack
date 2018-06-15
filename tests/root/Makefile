PROG=	test_io.o write.o read.o

all: $(PROG:.o=)

%: %.o
	clang $< -o $@

clean:
	rm -f $(PROG) $(PROG:.o=)


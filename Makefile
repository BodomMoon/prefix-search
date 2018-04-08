TESTS = \
    test_cpy \
    test_ref

CFLAGS = -Wall -Werror -g

# Control the build verbosity                                                   
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

GIT_HOOKS := .git/hooks/applied

.PHONY: all clean

all: $(GIT_HOOKS) $(TESTS)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

OBJS_LIB = \
    tst.o \
    ref.o

OBJS := \
    $(OBJS_LIB) \
    test_cpy.o \
    test_ref.o

deps := $(OBJS:%.o=.%.o.d)

test_%: test_%.o $(OBJS_LIB)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

clean:
	$(RM) $(TESTS) $(OBJS)
	$(RM) $(deps)

bench:$(TESTS) test.txt
	gcc bench.c `pkg-config --cflags --libs glib-2.0` -o bench
	./bench
	gcc caculate.c -o caculate -lm
	./caculate
	gcc caculate_ref.c -o caculate_ref -lm
	./caculate_ref

plot: bench
	gnuplot scripts/runtime.gp
	gnuplot scripts/runtime2.gp

cache-test: $(TESTS)
	perf stat --repeat 5 \
		-e cache-misses,cache-references,instructions,cycles \
		./test_cpy<test.txt>-
	perf stat --repeat 5 \
		-e cache-misses,cache-references,instructions,cycles \
		./test_ref<test.txt>-

-include $(deps)

vpath %.h include
vpath %.o objects

flags := -g -O2 -Wall 

outdir := out
incdir := include
objdir := objects

main = hashio

target ?= schain_hash
assert ?= false

assert_suf = _assert

objs = $(addprefix $(objdir)/, $(main).o $(target).o)

ifeq ($(assert), true)
	objs += $(addprefix $(objdir)/, $(target)$(assert_suf).o)
endif

$(outdir)/$(main): $(objs) | $(outdir)
	cc $(flags) $^ -o $@

$(objs): | $(objdir)

$(objdir):
	mkdir $(objdir)

$(outdir):
	mkdir $(outdir)

$(objdir)/$(main).o: $(main).c $(main).h
	cc $(flags) -c $< -o $@

$(objdir)/$(target).o: $(target)/$(target).c $(target).h
	cc $(flags) -c $< -o $@

$(objdir)/$(target)$(assert_suf).o: $(objdir)/$(target).o $(target)/$(target)$(assert_suf).c $(target)$(assert_suf).h
	cc $(flags) -c $(target)/$(target)$(assert_suf).c -o $@

.PHONY: clean
clean:
	rm -f -r $(objdir)
	rm -f -r $(outdir)

print: *.c
	lpr -p $?
	touch print

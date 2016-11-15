# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Local variables

HEADERS_$(d)	:= $(shell find $(d) -type f -iname '*.h')
SRCS_$(d)	:= $(shell find $(d) -type f -iname '*.c')
OBJS_$(d)	:= $(patsubst $(d)/%.c,$(BUILDDIR)/$(d)/%.o, $(filter %.c,$(SRCS_$(d))))

CLEAN		:= $(CLEAN) $(OBJS_$(d))

# Local rules

$(BUILDDIR)/$(d):
	mkdir -p $@

$(OBJS_$(d)):	CFLAGS_TGT = $(INCLUDES_$(PACKAGE)) $(CFLAGS_$(PACKAGE)) $(INCLUDES_sqlite3) $(CXXFLAGS_sqlite3)
$(OBJS_$(d)):	$(HEADERS_$(d)) | $(BUILDDIR)/$(d)

TGT_$(d) := $(BUILDDIR)/$(d)/test
CLEAN		:= $(CLEAN) $(TGT_$(d))

$(TGT_$(d)):	LDFLAGS_TGT = $(LINKFLAGS_$(PACKAGE)) $(LINKFLAGS_sqlite3)
$(TGT_$(d)):	LDLIBS_TGT = $(LIBPATH_$(PACKAGE)) $(LIB_$(PACKAGE)) $(LIBPATH_sqlite3)  $(LIB_sqlite3)
$(TGT_$(d)): $(SHAREDLIB_$(PACKAGE)) $(OBJS_$(d))
	$(LINK)

TESTS := TEST_$(d) $(TESTS)
TEST_$(d): $(TGT_$(d))
	@echo -n "Running $< ... "
	@LD_LIBRARY_PATH="$(PATH_$(PACKAGE)):$${LD_LIBRARY_PATH}" $<  &> $<.log
	@echo ok
	@echo -n "Running $< with Valgrind memcheck ... "
	@LD_LIBRARY_PATH="$(PATH_$(PACKAGE)):$${LD_LIBRARY_PATH}" valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-leak-kinds=all --errors-for-leak-kinds=all --leak-check-heuristics=all --show-reachable=yes --show-possibly-lost=yes --undef-value-errors=yes --track-origins=yes --partial-loads-ok=yes --expensive-definedness-checks=yes --keep-stacktraces=alloc-and-free --freelist-vol=20000000 --freelist-big-blocks=1000000 --show-mismatched-frees=yes --malloc-fill=0XEE --free-fill=0x33 $< &> $<.memcheck.log
	@echo ok
	@echo -n "Running $< with Valgrind helgrind ... "
	@LD_LIBRARY_PATH="$(PATH_$(PACKAGE)):$${LD_LIBRARY_PATH}" valgrind --tool=helgrind --free-is-write=yes --track-lockorders=yes --history-level=full --conflict-cache-size=1000000 --check-stack-refs=yes --ignore-thread-creation=no $< &> $<.helgrind.log
	@echo ok

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

# Compiler
CFLAGS := -std=c11 -W -Wall $(if $(DEBUG),-O0 -g,-O3 -DNDEBUG)
LDFLAGS := $(if $(DEBUG),,-O3)
LDLIBS := -lpthread
DEPFLAGS = -MT $@ -MMD -MP -MF $(obj_dir)/$*.d

# Project
name := morse
src_dir := src
obj_dir := build
bin_dir := bin

# Files
srcs := $(wildcard $(src_dir)/*.c)
objs := $(srcs:$(src_dir)/%.c=$(obj_dir)/%.o)
deps := $(objs:.o=.d)
exec := $(bin_dir)/$(name)

.PHONY: all clean

all: $(exec)

clean:
	rm -f $(objs) $(deps) $(exec)

$(exec): $(objs) | $(bin_dir)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(obj_dir)/%.o: $(src_dir)/%.c | $(obj_dir)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(obj_dir) $(bin_dir):
	mkdir $@

-include $(deps)


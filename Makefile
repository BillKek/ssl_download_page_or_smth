
EXENAME := browser.exe
PKGS=--static openssl
# -D_WIN32_WINNT=0x0501 "AcquireSRWlockExclusive" not found in KERNEL32.dll https://github.com/winfsp/cgofuse/commit/35bcf037030dcadcd247618c75c00c6cd17482d7 https://github.com/rclone/rclone/issues/1481
#  -DLWS_USE_IPV6=0 https://github.com/warmcat/libwebsockets/issues/100
CFLAGS = -g -O0 -Wall  $(shell pkg-config --cflags $(PKGS) ) -I. -I./include -std=c99 -D _WIN32_IE=0x0600 -DWINVER=0x0600 -D_WIN32_WINNT=0x0600
LIBS = -static $(shell pkg-config --libs $(PKGS) ) -lm -lcomctl32 -lws2_32 -lwinmm

CSRCSCAN := $(wildcard ./*.c) 
HSRCSCAN := $(wildcard ./*.h)
CSRCS := $(filter %.c,$(CSRCSCAN))
objects := $(patsubst %.c,obj/%.o,$(CSRCS))

OBJSCAN_PRE1 := $(wildcard ./obj/**/*.o) $(wildcard ./obj/*.o) $(wildcard ./obj/**/**/*.o)
OBJSCAN := $(EXENAME) $(OBJSCAN_PRE1)

all:	clean $(objects) 
	gcc -o $(EXENAME) $(objects) $(LIBS)

# Build the resources
# obj/Resource.o: res/Resource.rc res/Application.manifest res/Application.ico include/Resource.h | obj
# 	windres -I./include -I./res -i "$<" -o "$@"

$(objects):	obj/%.o: %.c
	$(shell sh.exe -c "mkdir -p ./$(dir $@)  2> /dev/null")
	gcc -g -O0 -Wall $(CFLAGS) -c $< -o $@

clean:
#$(shell cmd.exe /c "echo $(CSRCS) > test.txt")
#	$(shell cmd.exe /c "echo $(OBJSCAN_PRE1) > test2.txt")
#	$(shell cmd.exe /c "echo $(HSRCSCAN) > test3.txt")
	$(foreach objFile,$(OBJSCAN),$(shell sh.exe -c "rm $(objFile)  2> /dev/null"))

ifdef OS #OS is only set on Windows, so we can use it to detect whether we're on Windows or *nix.
	FILE = makefiles\BattleHummer.cbp.mak.windows
else
	ifeq ($(shell uname), Linux)
		FILE = makefiles/BattleHummer.cbp.mak.unix
	endif
endif

all: main
main:
	make -f $(FILE)
	
.PHONY : clean
clean :
	make -f $(FILE) clean
	

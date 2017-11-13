ifneq ($(MODEL),template_1)
MODEL=template_0
endif

EPSTOOL=../../bin/epstool

ifeq ($(OS),Windows_NT)
PLATFORM= Windows
else
PLATFORM= $(shell uname -s)
endif
#Darwin, Linux, Windows_NT

ifeq ($(PLATFORM),Linux)
	VIEWER=xdg-open
	PIC_FORMAT=eps
endif
ifeq ( $(PLATFORM),Darwin)
	VIEWER=open
	PIC_FORMAT=eps
endif
ifeq ( $(PLATFORM),Windows_NT)
	VIEWER=cygstart
	PIC_FORMAT=jpg
endif

all: execute

execute: SETUP SYNTHESIZABLE_WITHOUT_COUNTERS NON_SYNTHESIZABLE_WITHOUT_COUNTERS

SETUP:
	mkdir -p output_dir TuLiP
	
check_OS:
	@echo Detected operating system: $(PLATFORM)

check_viewer:
	@echo Detected file viewer: $(VIEWER)


SYNTHESIZABLE_WITHOUT_COUNTERS:
	@echo "Generating TuLiP description for the topology with the \
		following requirements (press q to exit requirements visualization)."
	@read -r dummy
	@less dynamic_specs/$(MODEL)_synthesizable_nc.txt
	@$(EPSTOOL) -a architecture_specs/$(MODEL).txt \
		-b dynamic_specs/$(MODEL)_synthesizable_nc.txt \
		-o TuLiP/$(MODEL)_synthesizable_nc.py
	@echo "Generated TuLiP specification... press Enter to see the file"
	@echo "(then press q when finished)."
	@read -r dummy
	@less TuLiP/$(MODEL)_synthesizable_nc.py
	@echo "Execute TuLiP"
	@python TuLiP/$(MODEL)_synthesizable_nc.py \
		-g -f output_dir/$(MODEL).$(PIC_FORMAT) -i -p output_dir/$(MODEL)_nc.py
	@echo
	@echo
	@echo "Automaton created..."
	@echo "Python implementation of the strategy stored in output_dir/$(MODEL).py"
	@echo "Press Enter to open it (press q to exit python visualization)."
	@read -r dummy
	@less output_dir/$(MODEL)_nc.py
	@echo "Graphical representation of the strategy stored in"
	@echo "output_dir/$(MODEL).$(PIC_FORMAT)"
	@echo "Press Enter to open it."
	@read -r dummy
	@$(VIEWER) output_dir/$(MODEL).$(PIC_FORMAT)
	

NON_SYNTHESIZABLE_WITHOUT_COUNTERS:
	@echo "Modify the requirements, inserting an unrealizable constraint."
	@echo "Press Enter to see the differences" 
	@echo "(press q to exit requirements visualization)"
	@read -r dummy
	@sdiff dynamic_specs/$(MODEL)_synthesizable_nc.txt \
          dynamic_specs/$(MODEL)_not_synthesizable_nc.txt | less
	@$(EPSTOOL) -a architecture_specs/$(MODEL).txt \
		-b dynamic_specs/$(MODEL)_not_synthesizable_nc.txt \
		-o TuLiP/$(MODEL)_not_synthesizable_nc.py
	@echo "Generated TuLiP specification... press Enter to see the file"
	@echo "(then press q when finished)."
	@read -r dummy
	@less TuLiP/$(MODEL)_not_synthesizable_nc.py
	@echo "Execute TuLiP"
	@python TuLiP/$(MODEL)_not_synthesizable_nc.py \
		-g -f output_dir/$(MODEL).$(PIC_FORMAT) -i -p output_dir/$(MODEL).py

clean:
	@rm -f output_dir/*.py output_dir/*.eps output_dir/*.jpg
	@rm -f TuLiP/*.py


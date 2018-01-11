ifneq ($(MODEL),template_1)
MODEL=template_0
endif


TOOL=../../bin/chase

ifeq ($(OS),Windows_NT)
PLATFORM= Windows
else
PLATFORM= $(shell uname -s)
endif
#Darwin, Linux, Windows_NT

ifeq ($(PLATFORM),Linux)
	VIEWER=xdg-open
	PIC_FORMAT=jpg
endif
ifeq ($(PLATFORM),Darwin)
	VIEWER=open
	PIC_FORMAT=eps
endif
ifeq ($(PLATFORM),Windows_NT)
	VIEWER=cygstart
	PIC_FORMAT=jpg
endif

all: execute


execute: SETUP SYNTHESIZABLE_WITHOUT_COUNTERS NON_SYNTHESIZABLE_WITHOUT_COUNTERS

SETUP:
	mkdir -p output_dir TuLiP counterstrategies
	
check_OS:
	@echo Detected operating system: $(PLATFORM)

check_viewer:
	@echo Detected file viewer: $(VIEWER)


SYNTHESIZABLE_WITHOUT_COUNTERS:
	@clear
	@echo "Visualize system specification (press Enter to read and q when finished)."
	@read -r dummy
	@less specs/$(MODEL)_synthesizable.txt
	@echo "CHASE: Contract-Based Hierarchical Analysis and System Exploration."
	@echo "Press enter to start."	
	@read -r dummy
	@echo "Generating contract from specification…"
	@ $(TOOL) -b tulip -i specs/$(MODEL)_synthesizable.txt \
		-o TuLiP/$(MODEL)_synthesizable.py -V | less -R
	@echo "Generated contract in TuLiP format (press Enter to read and q when finished)."
	@read -r dummy
	@less TuLiP/$(MODEL)_synthesizable.py
	@echo "Check Contract Consistency"
	@python TuLiP/$(MODEL)_synthesizable.py \
		-g -f output_dir/$(MODEL).$(PIC_FORMAT) -i -p output_dir/$(MODEL).py
	@echo
	@echo
	@echo "Python implementation of the controller (press Enter to read and q when finished)."
	@read -r dummy
	@less output_dir/$(MODEL).py
	@echo Graphical representation in output_dir/$(MODEL).$(PIC_FORMAT)
	@read -r dummy
	@clear
	

NON_SYNTHESIZABLE_WITHOUT_COUNTERS:
	@echo "Inserted additional requirement (press Enter to read and q when finished)."
	@read -r dummy
		@sdiff specs/$(MODEL)_synthesizable.txt \
			  specs/$(MODEL)_failing.txt | less
	@echo "Generating contract from specification…"
	@$(TOOL) -b tulip -i specs/$(MODEL)_failing.txt \
		-o TuLiP/$(MODEL)_failing.py -V
	@echo "Generated contract in TuLiP format (press Enter to read and q when finished)."
	@read -r dummy
	@less TuLiP/$(MODEL)_failing.py
	@echo "Check Contract Consistency"
	@python TuLiP/$(MODEL)_failing.py \
		-g -f output_dir/$(MODEL).$(PIC_FORMAT) -i -p output_dir/$(MODEL).py \
		-c -csp counterstrategies/

clean:
	@rm -f output_dir/*.py output_dir/*.eps output_dir/*.jpg
	@rm -f TuLiP/*.py
	@rm -f counterstrategies/*.py

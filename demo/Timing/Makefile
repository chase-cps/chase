ifneq ($(MODEL),template_1)
ifneq ($(MODEL),template_2)	
MODEL=template_0
endif
endif


EXECUTABLE=../../bin/chase

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
ifeq ($(PLATFORM),Darwin)
	VIEWER=open
	PIC_FORMAT=eps
endif
ifeq ($(PLATFORM),Windows_NT)
	VIEWER=cygstart
	PIC_FORMAT=jpg
endif

all: execute


execute: SETUP SYNTHESIZABLE_WITH_COUNTERS NON_SYNTHESIZABLE_WITH_COUNTERS

SETUP:
	mkdir -p output_dir TuLiP
	
check_OS:
	@echo Detected operating system: $(PLATFORM)

check_viewer:
	@echo Detected file viewer: $(VIEWER)


SYNTHESIZABLE_WITH_COUNTERS:
	@clear
	@echo "Visualize system specification (press Enter to read and q when finished)."
	@read -r dummy
	@less specs/$(MODEL)_synthesizable.txt
	@echo "CHASE: Contract-Based Hierarchical Analysis and System Exploration."
	@echo "Press enter to start."	
	@read -r dummy
	@echo "Generating contract from specification…"
	@$(EXECUTABLE) -b tulip -i specs/$(MODEL)_synthesizable.txt \
		-o TuLiP/$(MODEL)_synthesizable.py -V | less -R
	@echo "Generated contract in TuLiP format (press Enter to read and q when finished)."
	@read -r dummy
	@less TuLiP/$(MODEL)_synthesizable.py
	@echo "Check Contract Consistency"
	@python TuLiP/$(MODEL)_synthesizable.py \
		-i -p output_dir/$(MODEL).py
	@echo
	@echo
	@echo "Python implementation of the controller (press Enter to read and q when finished)."
	@read -r dummy
	@less output_dir/$(MODEL).py
	@echo Graphical representation in output_dir/$(MODEL).$(PIC_FORMAT)
	@read -r dummy
	@clear
	

NON_SYNTHESIZABLE_WITH_COUNTERS:
	@echo "Inserted additional requirement (press Enter to read and q when finished)."
	@read -r dummy
		@sdiff specs/$(MODEL)_synthesizable.txt \
			  specs/$(MODEL)_unrealizable.txt | less
	@echo "Generating contract from specification…"
	@$(EXECUTABLE) -b tulip -i specs/$(MODEL)_unrealizable.txt \
		-o TuLiP/$(MODEL)_unrealizable.py -V
	@echo "Generated contract in TuLiP format (press Enter to read and q when finished)."
	@read -r dummy
	@less TuLiP/$(MODEL)_unrealizable.py
	@echo "Check Contract Consistency"
	@python TuLiP/$(MODEL)_unrealizable.py \
		-i -p output_dir/$(MODEL).py

clean:
	@rm -f output_dir/*.py output_dir/*.eps output_dir/*.jpg
	@rm -f TuLiP/*.py

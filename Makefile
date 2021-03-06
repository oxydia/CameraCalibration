GXX = g++
FLAGS = -lOpenKN-math -lOpenKN-image -lglut -lGLU -lGLEW -L.   -O2 -Wall -g -I.
SRCDIR = src
EXE = calibration


$(EXE) : $(SRCDIR)/*
	@echo "Generating executable, this may take a while."
	@$(GXX) $^ $(FLAGS) -o $(EXE)
	@echo "Executable generated successfully."
	@echo "Wanna launch it ? Just type './calibration -h' for help."
	@mkdir out

 
.PHONY : clean ultraclean fresh

clean :
	@echo "-- Clean --"
	@rm -f $(SRCDIR)/*.o $(SRCDIR)/*~ *~
	@echo "Cleaning generated files done."
    
ultraclean : clean
	@rm -f $(EXE)
	@echo "Cleaning executabe done."
	
fresh : ultraclean
	@rm -f out/*
	@echo "Cleaning out directory done."

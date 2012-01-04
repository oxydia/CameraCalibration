GXX = g++
FLAGS = -Wall -g -lOpenKN-math -lOpenKN-image -lglut -lGLU -lGLEW -L. -I. -O2
SRCDIR = src
EXE = cameraCalibration

$(EXE): $(SRCDIR)/*.cpp
	@$(GXX) $^ -o $(EXE) $(FLAGS)
	@echo "Executable generated successfully."
	@echo "Wanna launch it ? Just type './cameraCalibration' without argument for help."
    
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

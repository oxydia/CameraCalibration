GXX = g++
FLAGS = -Wall -g
SRCDIR = src
EXE = cameraCalibration

$(EXE): $(SRCDIR)/main.cpp $(SRCDIR)/Image.hpp $(SRCDIR)/Camera.hpp
	@$(GXX) $^ -o $(EXE) $(FLAGS)
	@echo "Executable generated successfully."
	@echo "Wanna launch it ? Just type './cameraCalibration path/img0.jpg path/img1.jpg'."
	@echo "Type './cameraCalibration help' for help."
    
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

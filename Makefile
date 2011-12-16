GXX = g++
FLAGS = -Wall
EXE = cameraCalibration

$(EXE): main.cpp
	$(GXX) $^ -o $(EXE) $(FLAGS)
    
.PHONY : clean

clean :
	@echo "-- Clean --"
	@rm -f *.o *~
	@echo "Cleaning generated files done."
    
ultraclean : clean
	@rm -f $(EXE)
	@echo "Cleaning executabe done."

CXX = g++
CXXFLAGS = -std=c++17 -O3 

solution: solution.cpp
	$(CXX) $(CXXFLAGS) solution.cpp -o solution

clean:
ifeq ($(OS),Windows_NT)
	@echo Cleaning project...
	@for /R %%f in (*.o) do del /F /Q "%%f"
	@for /R %%f in (*.exe) do del /F /Q "%%f"
	@echo Clean complete.
else
	@echo Cleaning project...
	@find . -name "*.o" -delete
	@rm -f $(TARGET) $(TEST_TARGET)
	@echo Clean complete.
endif

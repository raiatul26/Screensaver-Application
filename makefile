all: 	
	@g++ -std=c++11 main.cpp -lGL -lGLU -lglut -pthread -o screensaver
	@./screensaver 12
clean:
	@rm -r screensaver

execute:
	@g++ -std=c++11 main.cpp -lGL -lGLU -lglut -pthread -o screensaver
	@./screensaver $(num_thread)

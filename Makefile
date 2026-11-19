build:
	gcc main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL3 -o framebuffer
run:
	./framebuffer

portscanner:
	mkdir -p build
	gcc -o build/portscanner main.c

clean:
	rm -rf build/

all:
	make -C lib all
	make -C tests all

clean:
	make -C lib clean
	make -C tests clean

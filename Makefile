DEPS=main2.o bomb/normal.o map_object/bomb.o map_object/wall.o map_object/nothing.o

all: foobar

foobar: $(DEPS)
	$(CC) $(DEPS) -o $@

clean:
	rm -f foobar *.o */*.o

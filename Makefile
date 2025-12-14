GCC = gcc
SOURCE = main.c
TARGET = main
GCC_FLAGS = 

$(TARGET): $(SOURCE)
	$(GCC) $(SOURCE) -o $(TARGET) $(GCC_FLAGS)

clean:
	rm -f $(TARGET)

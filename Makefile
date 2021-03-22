SRC_DIR := ./src
OBJ_DIR := ./build
INC := ./include
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := 
CFLAGS := -I$(INC)

all: main

main: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -r build/* main
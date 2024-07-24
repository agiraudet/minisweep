NAME			:=	minisweep

BUILD_DIR	:=	./build

INC_DIR		:=	./inc

SRCS_DIR	:=	./src

SRCS			:=	main.c \
							menu.c \
							minisweep.c \
							win.c \
							grid.c \
							theme.c

OBJS			:=	$(SRCS:%.c=$(BUILD_DIR)/%.o)

INC_FLAGS	:=	$(addprefix -I, $(INC_DIR))

CXXFLAGS	:=	-MD -Wall -Wextra -Werror $(INC_FLAGS) -g

CXX				:=	gcc

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	DEPS := -lraylib
	CXXFLAGS += $(DEPS)
else ifeq ($(UNAME_S),Darwin)
	LIB_DIR := /opt/homebrew/opt/raylib/lib
	LIB_INC_DIR := /opt/homebrew/opt/raylib/include
	LDFLAGS := -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR) -lraylib
	INC_FLAGS += -I$(LIB_INC_DIR)
endif

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.c | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

all: $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(HDR_ASM)

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re

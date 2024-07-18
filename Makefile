NAME			:=	minisweep

DEPS			:=	-lraylib

BUILD_DIR	:=	./build

INC_DIR		:=	./inc

SRCS_DIR	:=	./src

SRCS			:=	main.c \
							menu.c \
							win.c \
							grid.c

OBJS			:=	$(SRCS:%.c=$(BUILD_DIR)/%.o)

INC_FLAGS	:=	$(addprefix -I, $(INC_DIR))

CXXFLAGS	:=	-MD -Wall -Wextra -Werror $(INC_FLAGS)

CXX				:=	gcc

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(DEPS) -o $@ $(DEPS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.c | $(BUILD_DIR) $(HDR_ASM)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

all: $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(HDR_ASM)

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re

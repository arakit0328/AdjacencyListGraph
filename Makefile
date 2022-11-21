CC = c++
CFLAGS = -Wall -g
FLAGS = -Wall -g
LIBS = -lm
OBJS = adjacency_list_graph.o graph_main.o


graph_main: $(OBJS)
	$(CC) $(FLAGS) -o graph_main $(OBJS) $(LIBS)
.cpp.o:
	$(CC) $(CFLAGS) -c $<
clean:
	/bin/rm -rf *.o *~ graph_main $(OBJS) $(TARGET)

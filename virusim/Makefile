CXX       = g++
CXXFLAGS  = -Wall -Wno-unknown-pragmas -I.

PROGRAM   = virusim
PROGRAM1  = virusim_parallel_probs
PROGRAM2  = virusim_parallel_trials
PROGRAM3  = virusim_parallel_probs_n_trials
PROGRAM4  = virusim_tasks
OPTS      = -fopenmp
SRCS      = virusim.cpp Random.cpp Population.cpp
OBJS      = $(SRCS:.cpp=.o)
OUT       = ../output

.SUFFIXES: .cpp .o

.cpp.o: 
	$(CXX) -c $(CXXFLAGS) $<

all: $(PROGRAM) $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) $(PROGRAM4)

$(PROGRAM): $(OBJS)
	mkdir -p $(OUT)
	$(CXX) -o $(OUT)/$(PROGRAM) $(SRCS) $(CXXFLAGS) $(LDFLAGS)

$(PROGRAM1): $(OBJS)
	mkdir -p $(OUT)
	$(CXX) $(OPTS) -o $(OUT)/$(PROGRAM1) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -DPARALLEL_PROBS

$(PROGRAM2): $(OBJS)
	mkdir -p $(OUT)
	$(CXX) $(OPTS) -o $(OUT)/$(PROGRAM2) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -DPARALLEL_TRIALS

$(PROGRAM3): $(OBJS)
	mkdir -p $(OUT)
	$(CXX) $(OPTS) -o $(OUT)/$(PROGRAM3) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -DPARALLEL_PROBS -DPARALLEL_TRIALS

$(PROGRAM4): $(OBJS)
	mkdir -p $(OUT)
	$(CXX) $(OPTS) -o $(OUT)/$(PROGRAM3) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -DTASKS

clean:
	/bin/rm -f $(OBJS) $(OUT)/$(PROGRAM) $(OUT)/$(PROGRAM1) $(OUT)/$(PROGRAM2) $(OUT)/$(PROGRAM3) $(OUT)/$(PROGRAM4)
	/bin/rmdir $(OUT)

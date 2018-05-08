# -------------------------------------------------------------------------------------------------
BIN=bin
OBJ=obj
INCLUDE=-I/usr/local/cuda/include -ICommon -Iinclude -Isrc

FLAGS=-O3 -fopenmp -DHAVE_CUBLAS -c
LINK=-lmagma -lcublas -lcudart
MAGMA_LIB=magma-2.3.0/lib/libmagma.so
# -------------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------
COMPILE_BP=icpc $(FLAGS) $(INCLUDE) -IBipartite -o $(OBJ)/bp.o Bipartite.cpp $(LINK) --std=c++11 && \
		   icpc $(OBJ)/bp.o -o $(BIN)/bp.x $(MAGMA_LIB)

COMPILE_BP_D=icpc $(FLAGS) $(INCLUDE) -IBipartite -DDEBUG -o $(OBJ)/bp_d.o Bipartite.cpp $(LINK) --std=c++11 && \
		     icpc $(OBJ)/bp_d.o -o $(BIN)/bp_d.x $(MAGMA_LIB)
# -------------------------------------------------------------------------------------------------
COMPILE_BPG=icpc $(FLAGS) $(INCLUDE) -o $(OBJ)/bpg.o BipartiteGeneral.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bpg.o -o $(BIN)/bpg.x $(MAGMA_LIB)

COMPILE_BPG_D=icpc $(FLAGS) $(INCLUDE) -DDEBUG -o $(OBJ)/bpg_d.o BipartiteGeneral.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bpg_d.o -o $(BIN)/bpg_d.x $(MAGMA_LIB)
# -------------------------------------------------------------------------------------------------
COMPILE_BPGL=icpc $(FLAGS) $(INCLUDE) -o $(OBJ)/bpgl.o BipartiteGeneralLindblad.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bpgl.o -o $(BIN)/bpgl.x $(MAGMA_LIB)

COMPILE_BPGL_D=icpc $(FLAGS) $(INCLUDE) -DDEBUG -o $(OBJ)/bpgl_d.o BipartiteGeneralLindblad.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bpgl_d.o -o $(BIN)/bpgl_d.x $(MAGMA_LIB)
# -------------------------------------------------------------------------------------------------
COMPILE_BPG2=icpc $(FLAGS) $(INCLUDE) -o $(OBJ)/bpg2.o Bipartite2General.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bpg2.o -o $(BIN)/bpg2.x $(MAGMA_LIB)

COMPILE_BPG2_D=icpc $(FLAGS) $(INCLUDE) -DDEBUG -o $(OBJ)/bpg2_d.o Bipartite2General.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bpg2_d.o -o $(BIN)/bpg2_d.x $(MAGMA_LIB)
# -------------------------------------------------------------------------------------------------
COMPILE_BP_N=icpc $(FLAGS) $(INCLUDE) -IBipartite_n -DDEBUG -o $(OBJ)/bp_n.o Bipartite_n.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bp_n.o -o $(BIN)/bp_n.x $(MAGMA_LIB)

COMPILE_BP_N_G=icpc $(FLAGS) $(INCLUDE) -IBipartite_n_g -DDEBUG -o $(OBJ)/bp_n_g.o Bipartite_n_g.cpp $(LINK) --std=c++11 && \
		icpc $(OBJ)/bp_n_g.o -o $(BIN)/bp_n_g.x $(MAGMA_LIB)
# -------------------------------------------------------------------------------------------------

compile_bp:
	$(COMPILE_BP)

compile_bp_d:
	$(COMPILE_BP_D)
# -----------------
compile_bpg:
	$(COMPILE_BPG)

compile_bpg_d:
	$(COMPILE_BPG_D)
# ------------------
compile_bpg2:
	$(COMPILE_BPG2)

compile_bpg2_d:
	$(COMPILE_BPG2_D)

compile_bp_n:
	$(COMPILE_BP_N)

compile_bp_n_g:
	$(COMPILE_BP_N_G)
# -------------------------------------------------------------------------------------------------
COMPILE_TEST=icpc $(FLAGS) $(INCLUDE) -o $(OBJ)/Test.o Test.cpp $(LINK) --std=c++11 && \
		   icpc $(OBJ)/Test.o -o $(BIN)/Test.x $(MAGMA_LIB)

compile_test:
	$(COMPILE_TEST)

Test:
	$(COMPILE_TEST)

	rm Test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=Test.out run ./$(BIN)/Test.x
# -------------------------------------------------------------------------------------------------
bp:
	$(COMPILE_BP)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bp.x

bp_d:
	$(COMPILE_BP_D)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bp_d.x

bp_clean:
	rm -r Bipartite/out/* 2>/dev/null || true
# ------------------------------------------------------
bpg:
	$(COMPILE_BPG)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bpg.x

bpg_d:
	$(COMPILE_BPG_D)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bpg_d.x

bpg_clean:
	rm -r BipartiteGeneral/out/* 2>/dev/null || true
# -------------------------------------------------------
bpgl:
	$(COMPILE_BPGL)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bpgl.x

bpgl_d:
	$(COMPILE_BPGL_D)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bpgl_d.x
# -------------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------
bpg2:
	$(COMPILE_BPG2)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bpg2.x

bpg2_d:
	$(COMPILE_BPG2_D)

	rm test.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=test.out run ./$(BIN)/bpg2_d.x
# -------------------------------------------------------------------------------------------------
bp_n:
	$(COMPILE_BP_N)

	rm bp_n.out 2>/dev/null || echo >/dev/null;

	srun -N 1 -p gputest --output=bp_n.out run ./$(BIN)/bp_n.x

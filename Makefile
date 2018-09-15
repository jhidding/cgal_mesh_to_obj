build_dir = ./build

cgal_cflags = -frounding-math
cgal_libs = -lm -lCGAL -lgmp -lboost_thread -lmpfr

compile = g++
compile_flags = -std=c++17 -O3 -Wall $(cgal_cflags)

link = g++
link_flags = $(cgal_libs)

cc_files = src/mesh_to_obj.cc
obj_files = $(cc_files:%.cc=$(build_dir)/%.o)
dep_files = $(obj_files:%.o=%.d)

$(build_dir)/%.o : %.cc
	mkdir -p $(@D)
	$(compile) $(compile_flags) -MMD -c $< -o $@

$(build_dir)/mesh_to_obj : $(obj_files)
	mkdir -p $(@D)
	$(link) $^ $(link_flags) -o $@


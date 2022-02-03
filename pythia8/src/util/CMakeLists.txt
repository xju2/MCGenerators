# add pythia path
if (NOT DEFINED PYTHIA8_DIR)
	set(PYTHIA8_DIR /usr/local/include/Pythia8)
endif ()

include_directories(${PYTHIA8_DIR}/include)
link_directories(${PYTHIA8_DIR}/lib)

if (NOT DEFINED HEPMC_DIR)
	set(HEPMC_DIR  /usr/local/include/HepMC2)
endif()

# include_directories(${HEPMC_DIR}/include)
# link_directories   (${HEPMC_DIR}/lib)

# add pythia executables
add_executable(pythiaToHepMC2 pythiaToHepMC2.cxx)
set_target_properties(pythiaToHepMC2 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(pythiaToHepMC2 pythia8 HepMC)
# target_include_directories(
# 	pythiaToHepMC2 PRIVATE )
install(TARGETS pythiaToHepMC2 DESTINATION bin)

add_executable(pythiaToHepMC3 pythiaToHepMC3.cxx)
set_target_properties(pythiaToHepMC3 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(pythiaToHepMC3 pythia8 HepMC3)
install(TARGETS pythiaToHepMC3 DESTINATION bin)
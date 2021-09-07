add_subdirectory(libDelphesUtils)
add_subdirectory(libDelphesParquet)

add_executable(AnaDelphes AnaDelphes.cxx)
set_target_properties(AnaDelphes PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(AnaDelphes DelphesUtils)

find_package(Parquet)
if ( Parquet_FOUND )
	add_executable(Delphes2Parquet Delphes2Parquet.cxx)
	set_target_properties(Delphes2Parquet PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(Delphes2Parquet DelphesParquet DelphesUtils)


	add_executable(TestParquet TestParquet.cxx)
	set_target_properties(TestParquet PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(TestParquet DelphesParquet)
endif()
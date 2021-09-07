find_package(ROOT 6.18 REQUIRED COMPONENTS Core Hist Tree TreePlayer)
find_package(Delphes)
# find_package(parquet)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

file(GLOB SrcFiles *.cxx)
add_library(DelphesParquet SHARED ${SrcFiles})
target_include_directories(DelphesParquet 
    PUBLIC ${Delphes_INCLUDE_DIR}
    INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(DelphesParquet
    Delphes
    ROOT::Core ROOT::Tree 
    ROOT::TreePlayer ROOT::Physics
    parquet
)

install(TARGETS DelphesParquet 
        DESTINATION lib
        EXPORT DelphesParquetTargets)

file(GLOB HeaderFiles include/DelphesParquet/*.hpp)
install(FILES ${HeaderFiles}
    DESTINATION include/DelphesParquet)
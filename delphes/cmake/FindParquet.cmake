# Find the Parquet includes and libraries.
#
# This module defines the `Parquet` imported target that encodes all
# necessary information in its target properties.
include(FindPackageHandleStandardArgs)

find_library(Parquet_LIBRARY
  NAMES parquet
  HINTS ${Parquet_DIR}
  PATHS /usr/local /usr/lib
  PATH_SUFFIXES lib lib64 x86_64-linux-gnu
  DOC "The Parquet library")

find_path(Parquet_INCLUDE_DIR
  NAMES parquet/column_writer.h
  HINTS ${Parquet_DIR}
  PATHS /usr/local /usr
  PATH_SUFFIXES include
  DOC "The Parquet include directory")

find_package_handle_standard_args(Parquet
  REQUIRED_VARS Parquet_LIBRARY Parquet_INCLUDE_DIR)

add_library(Parquet SHARED IMPORTED)
set_property(TARGET Parquet PROPERTY IMPORTED_LOCATION ${Parquet_LIBRARY})
set_property(TARGET Parquet PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Parquet_INCLUDE_DIR})

mark_as_advanced(Parquet_FOUND Parquet_INCLUDE_DIR Parquet_LIBRARY)
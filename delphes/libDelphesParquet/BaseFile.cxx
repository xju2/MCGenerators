#include "DelphesParquet/ObjectFile.hpp"

#include <parquet/exception.h>
#include <parquet/properties.h>



void ObjectFile::init(std::shared_prt<arrow::fs::FileSystem> fs, const std::string& path) {
  std::shared_ptr<arrow::io::OutputStream> outfile;
  PARQUET_ASSIGN_OR_THROW(
			  outfile,
			  fs->OpenOutputStream(path));

  //
  // File Schema
  std::vector<std::shared_ptr<arrow::Field>> fields = {
    arrow::field("evt"   , arrow::int32())
  };

  const std::vector<std::shared_ptr<arrow::Field>>& colfields=this->fields();
  fields.insert(fields.end(), colfields.begin(), colfields.end());

  _schema=arrow::schema(fields);

  if(_numrowsingroup<0)
    { _numrowsingroup=32*1024*1024/(4*fields.size()); }

  // Create writer
  parquet::arrow::FileWriter::Open(*_schema, ::arrow::default_memory_pool(),
                                   outfile,
                                   parquet::default_writer_properties(),
                                   parquet::default_arrow_writer_properties(),
                                   &_writer);
 
}
#pragma once

#include <string>

#include <arrow/api.h>
#include <arrow/filesystem/filesystem.h>
#include <arrow/io/api.h>

#include <parquet/arrow/writer.h>

//! Base class for storing `LCObject`'s to a Parquet file.
class ObjectFile
{
public:
  ObjectFile() =default;
  virtual ~ObjectFile() =default;

  virtual void init(std::shared_ptr<arrow::fs::FileSystem> fs, const std::string& path);
  virtual void fill(const TObject* obj, const int32_t evtid);
  virtual void close();

protected:
  virtual std::vector<std::shared_ptr<arrow::Field>> fields() =0;
  virtual std::vector<std::shared_ptr<arrow::Array>> arrays() =0;

  void endrow();
  void write();

private:
  //! Writer
  std::unique_ptr<parquet::arrow::FileWriter> _writer;

  //! Schema
  std::shared_ptr<arrow::Schema> _schema;
  
  //! Number of entries in row group
  int32_t _numrowsingroup=-1;

  //! Row counter
  int32_t _rows=0;

  //! Builders
  arrow::Int32Builder _evt_builder;
  arrow::Int64Builder _colidx_builder;
};

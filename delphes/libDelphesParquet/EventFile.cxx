#include "DelphesParquet/EventFile.hpp"

#include <parquet/exception.h>
#include <parquet/properties.h>


void EventFile::init(const std::string& path)
{
  PARQUET_ASSIGN_OR_THROW(_outfile, FileClass::Open(path.c_str()));

  // Setup the parquet schema
  std::shared_ptr<GroupNode> schema = setup_schema();

  // Add writer properties
  parquet::WriterProperties::Builder builder;
  builder.compression(parquet::Compression::SNAPPY);
  std::shared_ptr<parquet::WriterProperties> props = builder.build();

  _writer = parquet::ParquetFileWriter::Open(_outfile, schema, props);

  // int file_size = 4*32;
  // if(_numrowsingroup<0) { 
  //   _numrowsingroup=32*1024*1024/(4*file_size);
  // }
  _numrowsingroup = 10000;

  clear();
}

void EventFile::close()
{
  write();
  _writer->Close();
  DCHECK(_outfile->Close().ok());
}

void EventFile::fill(const Data& data)
{
  _evt_b          .push_back(data._evt);
  _nTruthJets_b   .push_back(data._nTruthJet);
  _nTruthBJets_b  .push_back(data._nTruthBJets);
  _nTruthTauJets_b.push_back(data._nTruthTauJets);

  EventFile::endrow();
}

void EventFile::endrow()
{
  _rows++;
  if((_rows%_numrowsingroup)==0)
    {
      write();
    }
}

void EventFile::write()
{
  if (_evt_b.size() < 1) return;

  parquet::RowGroupWriter* rg_writer = _writer->AppendRowGroup();

  int num_rows = (int) _evt_b.size();

  parquet::Int32Writer* int32_writer =
      static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
  int32_writer->WriteBatch(num_rows, nullptr, nullptr, &_evt_b[0]);

  int32_writer = static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
  int32_writer->WriteBatch(num_rows, nullptr, nullptr, &_nTruthJets_b[0]);

  int32_writer = static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
  int32_writer->WriteBatch(num_rows, nullptr, nullptr, &_nTruthBJets_b[0]);

  int32_writer = static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
  int32_writer->WriteBatch(num_rows, nullptr, nullptr, &_nTruthTauJets_b[0]);

  rg_writer->Close();

  clear();
}

void EventFile::clear(){
  _evt_b.clear();
  _nTruthJets_b.clear();
  _nTruthBJets_b.clear();
  _nTruthTauJets_b.clear();
}
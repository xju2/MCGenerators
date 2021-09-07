#pragma once

#include <string>

#include <arrow/io/file.h>
#include <arrow/util/logging.h>
#include <parquet/api/reader.h>
#include <parquet/api/writer.h>

using parquet::ConvertedType;
using parquet::Repetition;
using parquet::Type;
using parquet::schema::GroupNode;
using parquet::schema::PrimitiveNode;

//! Base class for storing data to a Parquet file.
//! this saves event level information
class EventFile
{
public:

  static std::shared_ptr<GroupNode> setup_schema() {
    parquet::schema::NodeVector fields;

    // book columns for event data frame.
    fields.push_back(PrimitiveNode::Make(
      "evt", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      "nTruthJets", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      "nTruthBJets", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      "nTruthTauJets", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE));

    // Create a GroupNode named 'schema' using the primitive nodes defined above
    // This GroupNode is the root node of the schema tree
    return std::static_pointer_cast<GroupNode>(
        GroupNode::Make("Events", Repetition::REQUIRED, fields));
  }

  struct Data
  {
    /* data */
    int32_t _evt;
    int32_t _nTruthJet      = -1;
    int32_t _nTruthBJets    = -1;
    int32_t _nTruthTauJets  = -1;
  };
  
  EventFile() =default;
  virtual ~EventFile() =default;

  virtual void init(const std::string& path);
  virtual void fill(const Data& evtid);
  virtual void close();

protected:

  void endrow();
  void write();
  void clear();

private:

  //! Writer
  // std::unique_ptr<parquet::arrow::FileWriter> _writer;
  std::shared_ptr<parquet::ParquetFileWriter> _writer;

  using FileClass = ::arrow::io::FileOutputStream;
  std::shared_ptr<FileClass> _outfile;

  //! Schema
  std::shared_ptr<arrow::Schema> _schema;
  
  //! Number of entries in row group
  int32_t _numrowsingroup=-1;

  //! Row counter
  int32_t _rows=0;

  //! Builders
  std::vector<int32_t> _evt_b;
  std::vector<int32_t> _nTruthJets_b;
  std::vector<int32_t> _nTruthBJets_b;
  std::vector<int32_t> _nTruthTauJets_b;
};

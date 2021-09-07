#ifndef __DELPHES_PARQUET_H__
#define __DELPHES_PARQUET_H__
// https://github.com/apache/arrow/blob/master/cpp/examples/parquet/low_level_api/reader_writer.h


#include <arrow/io/file.h>
#include <arrow/util/logging.h>
#include <parquet/api/reader.h>
#include <parquet/api/writer.h>


using parquet::ConvertedType;
using parquet::Repetition;
using parquet::Type;
using parquet::schema::GroupNode;
using parquet::schema::PrimitiveNode;

// constexpr int FIXED_LENGTH = 10;

static std::shared_ptr<GroupNode> SetupSchema() {
  parquet::schema::NodeVector fields;
  // // Create a primitive node named 'boolean_field' with type:BOOLEAN,
  // // repetition:REQUIRED
  // fields.push_back(PrimitiveNode::Make(
  //   "boolean_field", Repetition::REQUIRED, Type::BOOLEAN, ConvertedType::NONE));

  // // Create a primitive node named 'int32_field' with type:INT32, repetition:REQUIRED,
  // fields.push_back(PrimitiveNode::Make(
  //   "int32_field", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE));

  fields.push_back(PrimitiveNode::Make(
    "float_field", Repetition::REPEATED, Type::FLOAT, ConvertedType::NONE));

  // Create a GroupNode named 'schema' using the primitive nodes defined above
  // This GroupNode is the root node of the schema tree
  return std::static_pointer_cast<GroupNode>(
      GroupNode::Make("schema", Repetition::REQUIRED, fields));
}

class DelphesParquet {
  public:
  DelphesParquet(std::string& filename);
  ~DelphesParquet();

  private:
  // std::shared_ptr<parquet::ParquetFileWriter> file_writer;
};

#endif
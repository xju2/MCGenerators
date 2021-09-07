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

//! this saves Reconstructed Jet information
//! Including pT, eta, phi, and E.
class RecoJetFile
{
public:
  static std::shared_ptr<GroupNode> setup_schema(std::string& pname="Jet") {
    parquet::schema::NodeVector fields;

    // book columns for event data frame.
    fields.push_back(PrimitiveNode::Make(
      pname+"Pt", Repetition::REQUIRED, Type::FLOAT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      pname+"Eta", Repetition::REQUIRED, Type::FLOAT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      pname+"Phi", Repetition::REQUIRED, Type::FLOAT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      pname+"E", Repetition::REQUIRED, Type::FLOAT32, ConvertedType::NONE));
    fields.push_back(PrimitiveNode::Make(
      pname+"IsBtagged", Repetition::REQUIRED, Type::BOOLEAN, ConvertedType::NONE
    ));
    fields.push_back(PrimitiveNode::Make(
      pname+"IsTautagged", Repetition::REQUIRED, Type::BOOLEAN, ConvertedType::NONE
    ));
    fields.push_back(PrimitiveNode::Make(
      pname+"TowerN", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE
    ));
    fields.push_back(PrimitiveNode::Make(
      pname+"TrackN", Repetition::REQUIRED, Type::INT32, ConvertedType::NONE
    ));

    // Create a GroupNode named 'RecoJets' using the primitive nodes defined above
    // This GroupNode is the root node of the schema tree
    return std::static_pointer_cast<GroupNode>(
        GroupNode::Make("RecoJets", Repetition::REQUIRED, fields));
  }
};
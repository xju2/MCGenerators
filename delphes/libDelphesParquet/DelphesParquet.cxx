#include "DelphesParquet/DelphesParquet.hpp"

#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

constexpr int NUM_ROWS_PER_ROW_GROUP = 500;

using namespace std;

DelphesParquet::DelphesParquet(std::string& filename) {
    // create parque file
    using FileClass = ::arrow::io::FileOutputStream;
    std::shared_ptr<FileClass> out_file;
    PARQUET_ASSIGN_OR_THROW(out_file, FileClass::Open(filename.c_str()));
    cout << "File created" << endl;

    // Setup the parquet schema
    std::shared_ptr<GroupNode> schema = SetupSchema();
    cout << "schema created" << endl;

    // Add writer properties
    parquet::WriterProperties::Builder builder;
    builder.compression(parquet::Compression::SNAPPY);
    std::shared_ptr<parquet::WriterProperties> props = builder.build();
    cout << "properties created" << endl;

    // Create a ParquetFileWriter instance
    std::shared_ptr<parquet::ParquetFileWriter> file_writer =
        parquet::ParquetFileWriter::Open(out_file, schema, props);
    cout << "ParquetFileWriter created" << endl;

    // Append a RowGroup with a specific number of rows.
    parquet::RowGroupWriter* rg_writer = file_writer->AppendRowGroup();
    cout << "RowGroup created" << endl;

    // // Write the Bool column
    // parquet::BoolWriter* bool_writer =
    //     static_cast<parquet::BoolWriter*>(rg_writer->NextColumn());
    // for (int i = 0; i < NUM_ROWS_PER_ROW_GROUP; i++) {
    //   bool value = ((i % 2) == 0) ? true : false;
    //   bool_writer->WriteBatch(1, nullptr, nullptr, &value);
    // }

    // // Write the Int32 column
    // parquet::Int32Writer* int32_writer =
    //   static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
    // std::vector<int32_t> data_int;
    // for (int i = 0; i < NUM_ROWS_PER_ROW_GROUP; i++) {
    //   int32_t value = i;
    //   data_int.push_back(value);
    // }
    // int32_writer->WriteBatch(NUM_ROWS_PER_ROW_GROUP, nullptr, nullptr, &data_int[0]);

    // Write the Float column
    parquet::FloatWriter* float_writer =
        static_cast<parquet::FloatWriter*>(rg_writer->NextColumn());
    std::vector<float> data_float;
    std::vector<int16_t> data_def;
    std::vector<int16_t> data_rep;
    // int16_t definition_level = 1;
    // int16_t repetition_level = 0;
    for (int i = 0; i < 2* NUM_ROWS_PER_ROW_GROUP; i++) {
      float value = static_cast<float>(i) * 1.0f;

      int16_t definition_level = 1;
      int16_t repetition_level = 0; // start of a new record
      if (((i+1) % 4) == 0 || ((i+1) % 3) == 0) {
        repetition_level = 1; // this will be attached to the existing record.
      }

      data_float.push_back(value);
      data_def.push_back(definition_level);
      data_rep.push_back(repetition_level);

      // float_writer->WriteBatch(1, &definition_level, &repetition_level, &value);
    }
    float_writer->WriteBatch(NUM_ROWS_PER_ROW_GROUP, &data_def[0], &data_rep[0], &data_float[0]);

    // Close the ParquetFileWriter
    file_writer->Close();

    // Write the bytes to file
    DCHECK(out_file->Close().ok());
}

DelphesParquet::~DelphesParquet() {

}
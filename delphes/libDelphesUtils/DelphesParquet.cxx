#include "DelphesUtils/DelphesParquet.hpp"

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

    // Write the Bool column
    parquet::BoolWriter* bool_writer =
        static_cast<parquet::BoolWriter*>(rg_writer->NextColumn());
    for (int i = 0; i < NUM_ROWS_PER_ROW_GROUP; i++) {
      bool value = ((i % 2) == 0) ? true : false;
      bool_writer->WriteBatch(1, nullptr, nullptr, &value);
    }

    // Write the Int32 column
    parquet::Int32Writer* int32_writer =
        static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
    for (int i = 0; i < NUM_ROWS_PER_ROW_GROUP; i++) {
      int32_t value = i;
      int32_writer->WriteBatch(1, nullptr, nullptr, &value);
    }

    // Write the Float column
    parquet::FloatWriter* float_writer =
        static_cast<parquet::FloatWriter*>(rg_writer->NextColumn());
    for (int i = 0; i < NUM_ROWS_PER_ROW_GROUP; i++) {
      float value = static_cast<float>(i) * 1.1f;
      float_writer->WriteBatch(1, nullptr, nullptr, &value);
    }

    // Close the ParquetFileWriter
    file_writer->Close();

    // Write the bytes to file
    DCHECK(out_file->Close().ok());
}

DelphesParquet::~DelphesParquet() {

}
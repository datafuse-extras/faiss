/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

//  Copyright 2004-present Facebook. All Rights Reserved
// -*- c++ -*-
// I/O code for indexes

#include "index_io_c.h"
#include <faiss/index_io.h>
#include "../faiss/impl/io.h"
#include "macros_impl.h"

using faiss::Index;

int faiss_write_index(const FaissIndex* idx, FILE* f) {
    try {
        faiss::write_index(reinterpret_cast<const Index*>(idx), f);
    }
    CATCH_AND_HANDLE
}

int faiss_write_index_fname(const FaissIndex* idx, const char* fname) {
    try {
        faiss::write_index(reinterpret_cast<const Index*>(idx), fname);
    }
    CATCH_AND_HANDLE
}

int faiss_read_index(FILE* f, int io_flags, FaissIndex** p_out) {
    try {
        auto out = faiss::read_index(f, io_flags);
        *p_out = reinterpret_cast<FaissIndex*>(out);
    }
    CATCH_AND_HANDLE
}

int faiss_read_index_fname(
        const char* fname,
        int io_flags,
        FaissIndex** p_out) {
    try {
        auto out = faiss::read_index(fname, io_flags);
        *p_out = reinterpret_cast<FaissIndex*>(out);
    }
    CATCH_AND_HANDLE
}

template<class T>
void forget(T&& v) {
	alignas(T) char buf[sizeof(T)];
	new(buf) T(std::move(v));
}

int serialize_index(const FaissIndex* idx, uint8_t** p_out, size_t* p_size,size_t *p_capacity){
    try{
        faiss::VectorIOWriter writer;
        writer.name = "VectorIOWriter";
        write_index(reinterpret_cast<const Index*>(idx), &writer);
        *p_out = writer.data.data();
        *p_size = writer.data.size();
        *p_capacity = writer.data.capacity();
        forget(std::move(writer.data));
    }
    CATCH_AND_HANDLE
}

int deserialize_index(const uint8_t* data, size_t size, FaissIndex** p_out){
    try{
        faiss::VectorIOReader reader;
        reader.name = "VectorIOReader";
        reader.data = std::vector<uint8_t>(data, data + size);
        auto out = read_index(&reader);
        *p_out = reinterpret_cast<FaissIndex*>(out);
    }
    CATCH_AND_HANDLE
}
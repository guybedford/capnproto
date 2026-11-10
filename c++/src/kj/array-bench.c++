// Copyright (c) 2026 Cloudflare, Inc. and contributors
// Licensed under the MIT License:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <benchmark/benchmark.h>

#include "array.h"
#include "string.h"

namespace kj {
namespace {

void sizeArguments(benchmark::Benchmark* benchmark) {
  benchmark->RangeMultiplier(8)->Range(16, 64 * 1024);
}

void bench_ArrayPtr_copyFrom(benchmark::State& state) {
  size_t size = state.range(0);
  auto src = heapArray<byte>(size);
  src.asPtr().fill(0x5a);
  auto dst = heapArray<byte>(size);
  for (auto _: state) {
    dst.asPtr().copyFrom(src);
    benchmark::DoNotOptimize(dst.begin());
    benchmark::ClobberMemory();
  }
  state.SetBytesProcessed(state.iterations() * size);
}
BENCHMARK(bench_ArrayPtr_copyFrom)->Apply(sizeArguments);

void bench_heapString(benchmark::State& state) {
  size_t size = state.range(0);
  auto src = heapArray<char>(size);
  src.asPtr().fill('a');
  for (auto _: state) {
    auto str = heapString(src.begin(), size);
    benchmark::DoNotOptimize(str.begin());
  }
  state.SetBytesProcessed(state.iterations() * size);
}
BENCHMARK(bench_heapString)->Apply(sizeArguments);

}  // namespace
}  // namespace kj

BENCHMARK_MAIN();

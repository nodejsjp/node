// Copyright Joyent, Inc. and other Node contributors.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "node_counters.h"
#include "uv.h"

#include <string.h>


namespace node {

using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::GCCallbackFlags;
using v8::GCEpilogueCallback;
using v8::GCPrologueCallback;
using v8::GCType;
using v8::Handle;
using v8::HandleScope;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

static uint64_t counter_gc_start_time;
static uint64_t counter_gc_end_time;


void COUNTER_NET_SERVER_CONNECTION(const FunctionCallbackInfo<Value>&) {
  NODE_COUNT_SERVER_CONN_OPEN();
}


void COUNTER_NET_SERVER_CONNECTION_CLOSE(const FunctionCallbackInfo<Value>&) {
  NODE_COUNT_SERVER_CONN_CLOSE();
}


void COUNTER_HTTP_SERVER_REQUEST(const FunctionCallbackInfo<Value>&) {
  NODE_COUNT_HTTP_SERVER_REQUEST();
}


void COUNTER_HTTP_SERVER_RESPONSE(const FunctionCallbackInfo<Value>&) {
  NODE_COUNT_HTTP_SERVER_RESPONSE();
}


void COUNTER_HTTP_CLIENT_REQUEST(const FunctionCallbackInfo<Value>&) {
  NODE_COUNT_HTTP_CLIENT_REQUEST();
}


void COUNTER_HTTP_CLIENT_RESPONSE(const FunctionCallbackInfo<Value>&) {
  NODE_COUNT_HTTP_CLIENT_RESPONSE();
}


static void counter_gc_start(GCType type, GCCallbackFlags flags) {
  counter_gc_start_time = NODE_COUNT_GET_GC_RAWTIME();
}


static void counter_gc_done(GCType type, GCCallbackFlags flags) {
  uint64_t endgc = NODE_COUNT_GET_GC_RAWTIME();
  if (endgc != 0) {
    uint64_t totalperiod = endgc - counter_gc_end_time;
    uint64_t gcperiod = endgc - counter_gc_start_time;

    if (totalperiod > 0) {
      unsigned int percent = static_cast<unsigned int>(
          (gcperiod * 100) / totalperiod);

      NODE_COUNT_GC_PERCENTTIME(percent);
      counter_gc_end_time = endgc;
    }
  }
}


void InitPerfCounters(Handle<Object> target) {
  HandleScope scope(node_isolate);

  static struct {
    const char* name;
    void (*func)(const FunctionCallbackInfo<Value>&);
  } tab[] = {
#define NODE_PROBE(name) #name, name
    { NODE_PROBE(COUNTER_NET_SERVER_CONNECTION) },
    { NODE_PROBE(COUNTER_NET_SERVER_CONNECTION_CLOSE) },
    { NODE_PROBE(COUNTER_HTTP_SERVER_REQUEST) },
    { NODE_PROBE(COUNTER_HTTP_SERVER_RESPONSE) },
    { NODE_PROBE(COUNTER_HTTP_CLIENT_REQUEST) },
    { NODE_PROBE(COUNTER_HTTP_CLIENT_RESPONSE) }
#undef NODE_PROBE
  };

  for (int i = 0; i < ARRAY_SIZE(tab); i++) {
    Local<String> key = String::New(tab[i].name);
    Local<Value> val = FunctionTemplate::New(tab[i].func)->GetFunction();
    target->Set(key, val);
  }

  // Only Windows performance counters supported
  // To enable other OS, use conditional compilation here
  InitPerfCountersWin32();

  // init times for GC percent calculation and hook callbacks
  counter_gc_start_time = NODE_COUNT_GET_GC_RAWTIME();
  counter_gc_end_time = counter_gc_start_time;

  v8::V8::AddGCPrologueCallback(counter_gc_start);
  v8::V8::AddGCEpilogueCallback(counter_gc_done);
}


void TermPerfCounters(Handle<Object> target) {
  // Only Windows performance counters supported
  // To enable other OS, use conditional compilation here
  TermPerfCountersWin32();
}

}  // namespace node

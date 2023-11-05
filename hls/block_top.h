#pragma once
#include <hls_stream.h>
#include "stream.h"
using namespace hls;

void do_compute(stream<trans_pkt >& in,stream<trans_pkt >& out, const unsigned reps);
//void do_compute_test(stream<axis_test >& in,stream< axis_test >& out, const unsigned reps);

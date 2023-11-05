#pragma once
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <ap_int.h>
using namespace hls;
using namespace std;

//struct ap_axis{
//	ap_uint<32> data;
//	ap_uint<1> last;
//	ap_uint<8> keep;
//};
//
//struct axis_test{
//	ap_uint<32> data;
//	ap_uint<1> last;
//	ap_uint<8> keep;
//};
#define DWIDTH 32
typedef ap_axiu<DWIDTH,0,0,0> trans_pkt;

template <	unsigned InStreamW,
			unsigned OutStreamW,
			unsigned NumLines>
void ReduceWidth(
	stream<ap_uint<InStreamW> > & in,
	stream<ap_uint<OutStreamW> > & out,
	const unsigned reps)
{
	const unsigned parts = InStreamW/OutStreamW;

	for (unsigned rep = 0; rep < reps*NumLines; rep++) {  //400*400*3*3
#pragma HLS loop_tripcount min=NumLines max=NumLines
#pragma HLS PIPELINE II=InStreamW/OutStreamW

		ap_uint<InStreamW> temp_in = in.read();
		for (unsigned p = 0; p < parts; p++) {

			ap_uint<OutStreamW> temp_out = temp_in(OutStreamW-1, 0);
			out.write( temp_out );
			temp_in = temp_in >> OutStreamW;
		}
	}
}

template <	unsigned Ibit,
			unsigned Cin,
			unsigned InP,
			unsigned K,
			unsigned NumLines>
void ReduceWidth_P(
	stream<ap_uint<Cin*Ibit*K*K> > & in,
	stream<ap_uint<InP*Ibit*K*K> > & out,
	const unsigned reps)
{
	const unsigned parts = Cin/InP;
	ap_uint<InP*Ibit*K*K> temp = 0;
//	unsigned temp_p = 0;
	for (unsigned rep = 0; rep < reps*NumLines; rep++) {
#pragma HLS loop_tripcount min=NumLines max=NumLines
#pragma HLS PIPELINE II=parts
		ap_uint<Cin*Ibit*K*K> temp_in = in.read();
		ap_uint<Cin*Ibit> temp_in_0 = temp_in(Cin*Ibit-1,0);
		ap_uint<Cin*Ibit> temp_in_1 = temp_in(2*Cin*Ibit-1,Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_2 = temp_in(3*Cin*Ibit-1,2*Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_3 = temp_in(4*Cin*Ibit-1,3*Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_4 = temp_in(5*Cin*Ibit-1,4*Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_5 = temp_in(6*Cin*Ibit-1,5*Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_6 = temp_in(7*Cin*Ibit-1,6*Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_7 = temp_in(8*Cin*Ibit-1,7*Cin*Ibit);
		ap_uint<Cin*Ibit> temp_in_8 = temp_in(9*Cin*Ibit-1,8*Cin*Ibit);

		for (unsigned p = 0; p < parts; p++) {
			temp(InP*Ibit-1,0) = temp_in_0((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(2*InP*Ibit-1,1*InP*Ibit) = temp_in_1((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(3*InP*Ibit-1,2*InP*Ibit) = temp_in_2((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(4*InP*Ibit-1,3*InP*Ibit) = temp_in_3((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(5*InP*Ibit-1,4*InP*Ibit) = temp_in_4((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(6*InP*Ibit-1,5*InP*Ibit) = temp_in_5((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(7*InP*Ibit-1,6*InP*Ibit) = temp_in_6((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(8*InP*Ibit-1,7*InP*Ibit) = temp_in_7((p+1)*InP*Ibit-1,p*(InP*Ibit));
			temp(9*InP*Ibit-1,8*InP*Ibit) = temp_in_8((p+1)*InP*Ibit-1,p*(InP*Ibit));
			out.write(temp);
		}
	}
}
/*
 * 2021/03/10 ??????
 */
template <	unsigned InStreamW,
			unsigned OutStreamW,
			unsigned NumLines>
void ExpandWidth(
	stream<ap_uint<InStreamW> > & in,
	stream<ap_uint<OutStreamW> > & out,
	const unsigned reps = 1)
{
	const unsigned long long parts = OutStreamW/InStreamW;
	ap_uint<OutStreamW> buffer;

	for (unsigned long long rep = 0; rep < reps*NumLines; rep++) {  //400*400
#pragma HLS loop_tripcount min=NumLines max=NumLines
		for (unsigned p = 0; p < parts; p++) {  //4
#pragma HLS PIPELINE II=1
			ap_uint<InStreamW> temp = in.read();
			buffer( (p+1)*InStreamW-1, p*InStreamW ) = temp;
		}

		out.write(buffer);

	}

}


template <	unsigned InStreamW,
			unsigned NumLines>
void DuplicateStreams(
	stream<ap_uint<InStreamW> > & in,
	stream<ap_uint<InStreamW> > & out1,
	stream<ap_uint<InStreamW> > & out2,
	const unsigned reps = 1)
{
	for (unsigned long long rep = 0; rep < reps*NumLines; rep++) {
#pragma HLS loop_tripcount min=NumLines max=NumLines
#pragma HLS PIPELINE II=1
		ap_uint<InStreamW> temp = in.read();
		out1.write(temp);
		out2.write(temp);
	}
}


template <	unsigned first_Abit,
			unsigned second_Abit,
			unsigned Cin,
			unsigned out_bit,
			unsigned NumLines>
void AddStreams_SIGN(
	stream<ap_uint<first_Abit*Cin> > & in1,
	stream<ap_uint<second_Abit*Cin> > & in2,
	stream<ap_uint<Cin*out_bit> > & out,
	const unsigned reps = 1)
{
	ap_uint<Cin*out_bit> temp_out;

	for (unsigned long long rep = 0; rep < reps*NumLines; rep++) {
#pragma HLS loop_tripcount min=NumLines max=NumLines
#pragma HLS PIPELINE II=1
		ap_uint<first_Abit*Cin> temp1 = in1.read();
		ap_uint<second_Abit*Cin> temp2 = in2.read();
		for(unsigned i = 0; i < Cin; i++)
		{
			ap_int<first_Abit> temp1_result = temp1((i+1)*first_Abit-1,i*first_Abit);
			ap_int<second_Abit> temp2_result = temp2((i+1)*second_Abit-1,i*second_Abit);
			temp_out((i+1)*out_bit-1,i*out_bit) = temp1_result + temp2_result;
		}
		out.write(temp_out);
	}
}


template <	unsigned NumLines>
void AddLast_test(
	stream<ap_uint<32> >& in,
	stream<trans_pkt >& out,
	const unsigned reps = 1)
{
	trans_pkt temp;
	temp.keep = "0xf";

	for (unsigned long long i = 0; i < reps*NumLines-1; i++) {
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=NumLines-1 max=NumLines-1
		temp.data(32-1, 0) = in.read();
	//	temp.data(32-1, 32) = 0;
		temp.last = 0;
		out.write(temp);
	}

	temp.data(32-1, 0) = in.read();
	temp.last = 1;
	out.write(temp);
}

template <	unsigned NumLines>
void AddLast(
	stream<ap_uint<32> >& in,
	stream<trans_pkt >& out,
	const unsigned reps = 1)
{
	trans_pkt temp;
	temp.keep = "0xf";

	for (unsigned long long i = 0; i < reps*NumLines-1; i++) {
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=NumLines-1 max=NumLines-1
		temp.data = in.read();
		temp.last = 0;
		out.write(temp);
	}

	temp.data = in.read();
	temp.last = 1;
	out.write(temp);
}

template <	unsigned OutStreamW,
			unsigned NumLines>
void ExtractPixels(
	stream<trans_pkt > & in,
	stream<ap_uint<OutStreamW> >& out,
	const unsigned reps = 1)
{
	trans_pkt temp;

	for (unsigned long long rep = 0; rep < reps*NumLines; rep++) {
#pragma HLS loop_tripcount min=NumLines max=NumLines
		temp = in.read();
		out.write( temp.data(OutStreamW-1, 0) );
	}
}

template <	unsigned OutStreamW,
			unsigned NumLines>
void ExtractPixels_test(
	stream<trans_pkt > & in,
	stream<ap_uint<OutStreamW> >& out,
	const unsigned reps = 1)
{
	trans_pkt temp;

	for (unsigned long long rep = 0; rep < reps*NumLines; rep++) {
#pragma HLS loop_tripcount min=NumLines max=NumLines
		temp = in.read();
		out.write( temp.data(OutStreamW-1, 0) );
	}
}

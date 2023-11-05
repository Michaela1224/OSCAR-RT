#pragma once
#include <hls_stream.h>
#include "conv2d.h"
using namespace hls;
//  # define CONV9_DEBUG
// # define CONV10_DEBUG
// # define CONV12_DEBUG
////// # define CONV13_DEBUG
//# define CONV18_DEBUG
//# define CONV19_DEBUG
//# define CONV21_DEBUG
//# define CONV22_DEBUG
//# define CONV24_DEBUG
//# define CONV25_DEBUG


template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT,

			unsigned out_bit>
void First_2DSP_1(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(first_Cin/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(first_Cin/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(first_Cin/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*out_bit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW
	stream<ap_uint<first_MVTU_InP*first_Ibit> > in_out1("res_in_out1");
#pragma HLS STREAM variable=in_out1 depth=1000
	stream<ap_uint<first_MVTU_InP*first_Ibit> > in_out2("res_in_out2");
	stream<ap_uint<third_MVTU_OutP*third_Abit> > in_out2_2("res_in_out2_2");
	//?latency????,???????????
	const unsigned first_latency = (10+first_Din*first_Cin/first_MVTU_InP);
	const unsigned second_latency = (10+first_Din*(second_K-1))*first_Cin/first_MVTU_InP;   //?????????????
	const unsigned third_latency = (10+first_Din*first_Cin/first_MVTU_InP);
#pragma HLS STREAM variable=in_out2 depth=first_latency+second_latency+third_latency+1000    //??????????????????????
#pragma HLS STREAM variable=in_out2_2 depth=first_latency+second_latency+third_latency+1000    //??????????????????????

	DuplicateStreams<first_MVTU_InP*first_Ibit, first_Din*first_Din*first_Cin/first_MVTU_InP>(in, in_out1, in_out2, reps);

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");        //Relu6??Abit?????3
#pragma HLS STREAM variable=first_out depth=1000

CONV_1x1_ACT_SIGN_in_DSP<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in_out1,first_weights,first_inc,first_bias,first_out,reps);

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");        //Relu6??Abit?????3
#pragma HLS STREAM variable=second_out depth=1000
DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	stream<ap_uint<third_MVTU_OutP*third_Abit> > third_out("third_out");
#pragma HLS STREAM variable=third_out depth=1000
	CONV_1x1_NoACT_in_DSP<first_Din,first_Cout,first_Cin,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,third_out,reps);

	//------------------------------third_out test passed!-------------------------------
	//-----------------------------------debug-------------------------------------------
//		ap_uint<24> tmp;
//		ap_uint<8> tmp1;
//		FILE* fp = fopen("stream1_out.txt", "wb");
//		for(int i = 0;i < 128*128;i++){
//				tmp=in_out2.read();
//				in_out2.write(tmp);
//				for(int j=0;j<3;j++){      //out channel
//					tmp1=tmp(7,0);        //out Abit
//					tmp=tmp>>8;
//					fprintf(fp, "%lf\n", double(tmp1));
//				}
//		}
	//----------------------------------------------------------------------------------
	
	ExpandWidth<first_MVTU_InP*first_Ibit, third_MVTU_OutP*first_Ibit, first_Din*first_Din*third_MVTU_OutP/first_MVTU_InP>(in_out2, in_out2_2, reps);
	
	AddStreams_SIGN<first_Ibit,third_Abit,third_MVTU_OutP,out_bit,first_Din*first_Din*first_Cin/third_MVTU_OutP>(in_out2_2, third_out, out, reps);
}



template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT,

			unsigned out_bit>
void First_2DSP_E1(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(first_Cin/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(first_Cin/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(first_Cin/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*out_bit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW
	stream<ap_uint<first_MVTU_InP*first_Ibit> > in_out1("res_in_out1");
#pragma HLS STREAM variable=in_out1 depth=1000
	stream<ap_uint<first_MVTU_InP*first_Ibit> > in_out2("res_in_out2");
	stream<ap_uint<12*third_Abit> > in_out2_2("res_in_out2_2");
	stream<ap_uint<third_MVTU_OutP*third_Abit> > in_out2_3("res_in_out2_3");
	//?latency????,???????????
	const unsigned first_latency = (10+first_Din*first_Cin/first_MVTU_InP);
	const unsigned second_latency = (10+first_Din*(second_K-1))*first_Cin/first_MVTU_InP;   //?????????????
	const unsigned third_latency = (10+first_Din*first_Cin/first_MVTU_InP);
#pragma HLS STREAM variable=in_out2 depth=first_latency+second_latency+third_latency+1000    //??????????????????????
#pragma HLS STREAM variable=in_out2_2 depth=first_latency+second_latency+third_latency+1000    //??????????????????????
#pragma HLS STREAM variable=in_out2_3 depth=first_latency+second_latency+third_latency+1000    //??????????????????????

	DuplicateStreams<first_MVTU_InP*first_Ibit, first_Din*first_Din*first_Cin/first_MVTU_InP>(in, in_out1, in_out2, reps);

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");        //Relu6??Abit?????3
#pragma HLS STREAM variable=first_out depth=1000

CONV_1x1_ACT_SIGN_in_DSP<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in_out1,first_weights,first_inc,first_bias,first_out,reps);

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");        //Relu6??Abit?????3
#pragma HLS STREAM variable=second_out depth=1000
DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	stream<ap_uint<third_MVTU_OutP*third_Abit> > third_out("third_out");
#pragma HLS STREAM variable=third_out depth=1000
	CONV_1x1_NoACT_in_DSP<first_Din,first_Cout,first_Cin,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,third_out,reps);

	//------------------------------third_out test passed!-------------------------------
	//-----------------------------------debug-------------------------------------------
//		ap_uint<24> tmp;
//		ap_uint<8> tmp1;
//		FILE* fp = fopen("stream1_out.txt", "wb");
//		for(int i = 0;i < 128*128;i++){
//				tmp=in_out2.read();
//				in_out2.write(tmp);
//				for(int j=0;j<3;j++){      //out channel
//					tmp1=tmp(7,0);        //out Abit
//					tmp=tmp>>8;
//					fprintf(fp, "%lf\n", double(tmp1));
//				}
//		}
	//----------------------------------------------------------------------------------
	
	ExpandWidth<first_MVTU_InP*first_Ibit, 12*first_Ibit, first_Din*first_Din*4>(in_out2, in_out2_2, reps);
	
	ReduceWidth<12*first_Ibit, third_MVTU_OutP*first_Ibit ,first_Din*first_Din*4>(in_out2_2,in_out2_3,reps);
	
	AddStreams_SIGN<first_Ibit,third_Abit,third_MVTU_OutP,out_bit,first_Din*first_Din*first_Cin/third_MVTU_OutP>(in_out2_3, third_out, out, reps);
}


template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT,

			unsigned out_bit>
void First_Block_2DSP(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(first_Cin/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(first_Cin/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(first_Cin/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*out_bit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW
	stream<ap_uint<first_MVTU_InP*first_Ibit> > in_out1("res_in_out1");
#pragma HLS STREAM variable=in_out1 depth=1000
	stream<ap_uint<first_MVTU_InP*first_Ibit> > in_out2("res_in_out2");

	//?latency????,???????????
	const unsigned first_latency = (10+first_Din*first_Cin/first_MVTU_InP);
	const unsigned second_latency = (10+first_Din*(second_K-1))*first_Cin/first_MVTU_InP;   //?????????????
	const unsigned third_latency = (10+first_Din*first_Cin/first_MVTU_InP);
#pragma HLS STREAM variable=in_out2 depth=first_latency+second_latency+third_latency+1000    //??????????????????????

	DuplicateStreams<first_MVTU_InP*first_Ibit, first_Din*first_Din*first_Cin/first_MVTU_InP>(in, in_out1, in_out2, reps);

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");        //Relu6??Abit?????3
#pragma HLS STREAM variable=first_out depth=1000

CONV_1x1_ACT_SIGN_in_DSP<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in_out1,first_weights,first_inc,first_bias,first_out,reps);

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");        //Relu6??Abit?????3
#pragma HLS STREAM variable=second_out depth=1000
DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	stream<ap_uint<third_MVTU_OutP*third_Abit> > third_out("third_out");
#pragma HLS STREAM variable=third_out depth=1000
	CONV_1x1_NoACT_in_DSP<first_Din,first_Cout,first_Cin,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,third_out,reps);

	//------------------------------third_out test passed!-------------------------------
	//-----------------------------------debug-------------------------------------------
//		ap_uint<24> tmp;
//		ap_uint<8> tmp1;
//		FILE* fp = fopen("stream1_out.txt", "wb");
//		for(int i = 0;i < 128*128;i++){
//				tmp=in_out2.read();
//				in_out2.write(tmp);
//				for(int j=0;j<3;j++){      //out channel
//					tmp1=tmp(7,0);        //out Abit
//					tmp=tmp>>8;
//					fprintf(fp, "%lf\n", double(tmp1));
//				}
//		}
	//----------------------------------------------------------------------------------

	AddStreams_SIGN<first_Ibit,third_Abit,third_MVTU_OutP,out_bit,first_Din*first_Din*first_Cin/first_MVTU_InP>(in_out2, third_out, out, reps);
}




template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Cout,
			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT>
void Second_Block_2DSP(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(third_Cout/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(third_Cout/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(third_Cout/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*third_Abit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");
#pragma HLS STREAM variable=first_out depth=1000
	CONV_1x1_ACT_SIGN_in_DSP<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in,first_weights,first_inc,first_bias,first_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV9_DEBUG
			ap_uint<first_MVTU_OutP*3> tmp_9;
			ap_uint<3> tmp1_9;
			FILE* fp_9 = fopen("conv_9_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/first_MVTU_OutP;i++){
				tmp_9=first_out.read();
				first_out.write(tmp_9);
				for(int j=0;j<first_MVTU_OutP;j++){      //out channel
					tmp1_9=tmp_9(3-1,0);        //out Abit
					tmp_9=tmp_9>>3;
					fprintf(fp_9, "%lf\n", double(tmp1_9));
				}
			}
			fclose(fp_9);
#endif
	//----------------------------------------------------------------------------------

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");
#pragma HLS STREAM variable=second_out depth=1000
	DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV10_DEBUG
			ap_uint<second_MVTU_InP*3> tmp_10;
			ap_uint<3> tmp1_10;
			FILE* fp_10 = fopen("conv_10_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/(4*second_MVTU_InP);i++){
				tmp_10=second_out.read();
				second_out.write(tmp_10);
				for(int j=0;j<second_MVTU_InP;j++){      //out channel
					tmp1_10=tmp_10(3-1,0);        //out Abit
					tmp_10=tmp_10>>3;
					fprintf(fp_10, "%lf\n", double(tmp1_10));
				}
			}
			fclose(fp_10);
#endif
	//----------------------------------------------------------------------------------

	const unsigned third_Din = first_Din/second_S + (first_Din%second_S > 0);
	CONV_1x1_NoACT_in_DSP<third_Din,first_Cout,third_Cout,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,out,reps);


}

template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Cout,
			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT>
void Second_Block_2DSP_test(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(third_Cout/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(third_Cout/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(third_Cout/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*third_Abit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");
#pragma HLS STREAM variable=first_out depth=1000
	CONV_1x1_ACT_SIGN_in_DSP<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in,first_weights,first_inc,first_bias,first_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV24_DEBUG
			ap_uint<first_MVTU_OutP*3> tmp_9;
			ap_uint<3> tmp1_9;
			FILE* fp_9 = fopen("conv_24_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/first_MVTU_OutP;i++){
				tmp_9=first_out.read();
				first_out.write(tmp_9);
				for(int j=0;j<first_MVTU_OutP;j++){      //out channel
					tmp1_9=tmp_9(3-1,0);        //out Abit
					tmp_9=tmp_9>>3;
					fprintf(fp_9, "%lf\n", double(tmp1_9));
				}
			}
			fclose(fp_9);
#endif
	//----------------------------------------------------------------------------------

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");
#pragma HLS STREAM variable=second_out depth=1000
	DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV25_DEBUG
			ap_uint<second_MVTU_InP*3> tmp_10;
			ap_uint<3> tmp1_10;
			FILE* fp_10 = fopen("conv_25_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/(second_MVTU_InP);i++){
				tmp_10=second_out.read();
				second_out.write(tmp_10);
				for(int j=0;j<second_MVTU_InP;j++){      //out channel
					tmp1_10=tmp_10(3-1,0);        //out Abit
					tmp_10=tmp_10>>3;
					fprintf(fp_10, "%lf\n", double(tmp1_10));
				}
			}
			fclose(fp_10);
#endif
	//----------------------------------------------------------------------------------

	const unsigned third_Din = first_Din/second_S + (first_Din%second_S > 0);
	CONV_1x1_NoACT_in_DSP<third_Din,first_Cout,third_Cout,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,out,reps);


}


template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Cout,
			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT>
void Second_Block_1DSP_test(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(third_Cout/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(third_Cout/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(third_Cout/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*third_Abit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");
#pragma HLS STREAM variable=first_out depth=1000
	CONV_1x1_ACT_SIGN_in<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in,first_weights,first_inc,first_bias,first_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV18_DEBUG
			ap_uint<first_MVTU_OutP*3> tmp_9;
			ap_uint<3> tmp1_9;
			FILE* fp_9 = fopen("conv_18_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/first_MVTU_OutP;i++){
				tmp_9=first_out.read();
				first_out.write(tmp_9);
				for(int j=0;j<first_MVTU_OutP;j++){      //out channel
					tmp1_9=tmp_9(3-1,0);        //out Abit
					tmp_9=tmp_9>>3;
					fprintf(fp_9, "%lf\n", double(tmp1_9));
				}
			}
			fclose(fp_9);
#endif
	//----------------------------------------------------------------------------------

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");
#pragma HLS STREAM variable=second_out depth=1000
	DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV19_DEBUG
			ap_uint<second_MVTU_InP*3> tmp_10;
			ap_uint<3> tmp1_10;
			FILE* fp_10 = fopen("conv_19_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/(second_MVTU_InP);i++){
				tmp_10=second_out.read();
				second_out.write(tmp_10);
				for(int j=0;j<second_MVTU_InP;j++){      //out channel
					tmp1_10=tmp_10(3-1,0);        //out Abit
					tmp_10=tmp_10>>3;
					fprintf(fp_10, "%lf\n", double(tmp1_10));
				}
			}
			fclose(fp_10);
#endif
	//----------------------------------------------------------------------------------

	const unsigned third_Din = first_Din/second_S + (first_Din%second_S > 0);
	CONV_1x1_NoACT_in_DSP<third_Din,first_Cout,third_Cout,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,out,reps);


}



template<   unsigned first_Din,
			unsigned first_Cin,
			unsigned first_Cout,
			unsigned first_Ibit,
			unsigned first_Wbit,
			unsigned first_IM,
			unsigned first_MVTU,
			unsigned first_Abit,
			unsigned first_MVTU_InP,
			unsigned first_MVTU_OutP,
			unsigned first_INC_BIT,
			unsigned first_BIAS_BIT,
			unsigned first_LSHIFT,

			unsigned second_K,
			unsigned second_S,
			unsigned second_Wbit,
			unsigned second_IM,
			unsigned second_MVTU,
			unsigned second_MVTU_InP,
			unsigned second_INC_BIT,
			unsigned second_BIAS_BIT,
			unsigned second_LSHIFT,

			unsigned third_Cout,
			unsigned third_Wbit,
			unsigned third_IM,
			unsigned third_MVTU,
			unsigned third_Abit,
			unsigned third_MVTU_InP,
			unsigned third_MVTU_OutP,
			unsigned third_INC_BIT,
			unsigned third_BIAS_BIT,
			unsigned third_LSHIFT>
void Second_Block_1DSP(
		stream<ap_uint<first_MVTU_InP*first_Ibit> >& in,

		const ap_uint<first_MVTU_InP*first_Wbit> first_weights[first_MVTU_OutP][(first_Cin/first_MVTU_InP)*(first_Cout/first_MVTU_OutP)],
		const ap_uint<first_INC_BIT> first_inc[first_MVTU][(first_Cout/first_MVTU)],
		const ap_uint<first_BIAS_BIT> first_bias[first_MVTU][(first_Cout/first_MVTU)],

		const ap_uint<second_K*second_K*second_MVTU_InP*second_Wbit> second_weights[((first_Cout*second_K*second_K)/second_MVTU_InP)],
		const ap_uint<second_INC_BIT> second_inc[second_MVTU][first_Cout/second_MVTU],
		const ap_uint<second_BIAS_BIT> second_bias[second_MVTU][first_Cout/second_MVTU],

		const ap_uint<third_MVTU_InP*third_Wbit> third_weights[third_MVTU_OutP][(first_Cout/third_MVTU_InP)*(third_Cout/third_MVTU_OutP)],
		const ap_uint<third_INC_BIT> third_inc[third_MVTU][(third_Cout/third_MVTU)],
		const ap_uint<third_BIAS_BIT> third_bias[third_MVTU][(third_Cout/third_MVTU)],

		stream<ap_uint<third_MVTU_OutP*third_Abit> >& out,
		const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	stream<ap_uint<first_MVTU_OutP*3> > first_out("first_out");
#pragma HLS STREAM variable=first_out depth=1000
	CONV_1x1_ACT_SIGN_in<first_Din,first_Cin,first_Cout,first_Ibit,first_Wbit,first_IM,first_MVTU,first_Abit,first_MVTU_InP,first_MVTU_OutP,first_INC_BIT,first_BIAS_BIT,first_LSHIFT>
	(in,first_weights,first_inc,first_bias,first_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV18_DEBUG
			ap_uint<first_MVTU_OutP*3> tmp_9;
			ap_uint<3> tmp1_9;
			FILE* fp_9 = fopen("conv_18_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/first_MVTU_OutP;i++){
				tmp_9=first_out.read();
				first_out.write(tmp_9);
				for(int j=0;j<first_MVTU_OutP;j++){      //out channel
					tmp1_9=tmp_9(3-1,0);        //out Abit
					tmp_9=tmp_9>>3;
					fprintf(fp_9, "%lf\n", double(tmp1_9));
				}
			}
			fclose(fp_9);
#endif
	//----------------------------------------------------------------------------------

	stream<ap_uint<second_MVTU_InP*3> > second_out("second_out");
#pragma HLS STREAM variable=second_out depth=1000
	DSC_ACT_Inp<second_K,second_S,first_Din,first_Cout,3,second_Wbit,second_IM,second_MVTU,3,second_MVTU_InP,second_INC_BIT,second_BIAS_BIT,second_LSHIFT>
	(first_out,second_weights,second_inc,second_bias,second_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV18_DEBUG
			ap_uint<second_MVTU_InP*3> tmp_10;
			ap_uint<3> tmp1_10;
			FILE* fp_10 = fopen("conv_19_hls_out.txt", "wb");
			for(int i = 0;i < first_Din*first_Din*first_Cout/(second_MVTU_InP);i++){
				tmp_10=second_out.read();
				second_out.write(tmp_10);
				for(int j=0;j<second_MVTU_InP;j++){      //out channel
					tmp1_10=tmp_10(3-1,0);        //out Abit
					tmp_10=tmp_10>>3;
					fprintf(fp_10, "%lf\n", double(tmp1_10));
				}
			}
			fclose(fp_10);
#endif
	//----------------------------------------------------------------------------------

	const unsigned third_Din = first_Din/second_S + (first_Din%second_S > 0);
	CONV_1x1_NoACT_in_DSP<third_Din,first_Cout,third_Cout,3,third_Wbit,third_IM,third_MVTU,third_Abit,third_MVTU_InP,third_MVTU_OutP,third_INC_BIT,third_BIAS_BIT,third_LSHIFT>
	(second_out,third_weights,third_inc,third_bias,out,reps);


}

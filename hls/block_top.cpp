#define AP_INT_MAX_W 5000
#include "rocky-lib.h"
#include "config.h"
#include "param.h"

//#define CONV1_DEBUG
//#define CONV2_DEBUG
//#define CONV5_DEBUG
//#define CONV8_DEBUG
//#define CONV11_DEBUG
//#define CONV14_DEBUG
//#define CONV17_DEBUG
//#define CONV20_DEBUG
//#define CONV23_DEBUG
//#define CONV26_DEBUG
//#define CONV27_DEBUG

void do_compute(stream<trans_pkt >& in,stream< trans_pkt >& out, const unsigned reps){

#pragma HLS INTERFACE s_axilite port=reps bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE axis register both port=in
#pragma HLS INTERFACE axis register both port=out
//---------------------------------------参数切分-----------------------------------------

#pragma HLS DATAFLOW

#pragma HLS ARRAY_PARTITION variable = conv_0_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_0_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_0_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_1_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable=conv_1_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_1_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_2_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_2_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_2_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_3_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_3_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_3_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_4_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_4_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_4_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_5_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_5_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_5_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_6_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_6_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_6_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_7_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_7_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_7_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_8_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_8_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_8_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_9_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_9_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_9_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_10_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_10_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_10_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_11_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_11_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_11_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_12_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_12_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_12_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_13_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_13_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_13_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_14_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_14_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_14_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_15_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_15_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_15_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_16_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_16_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_16_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_17_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_17_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_17_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_18_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_18_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_18_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_19_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_19_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_19_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_20_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_20_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_20_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_21_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_21_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_21_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_22_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_22_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_22_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_23_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_23_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_23_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_24_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_24_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_24_bias complete dim = 1

//#pragma HLS ARRAY_PARTITION variable = conv_25_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_25_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_25_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_26_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_26_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_26_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_27_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_27_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_27_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_28_w complete dim = 1

	stream<ap_uint<32> > in_stream_extract("in_stream_extract");
#pragma HLS STREAM variable=in_stream_extract depth=1000
	ExtractPixels<32,43264> (in,in_stream_extract,reps);       //NumLines=800*800/4

	stream<ap_uint<8> > conv0_in("conv_in");
#pragma HLS STREAM variable=conv0_in depth=1000
	ReduceWidth<32, 8 , 43264 >(in_stream_extract,conv0_in,reps);

#ifdef CONV1_DEBUG
			ap_uint<8> tmp_in;
			FILE* fp = fopen("in_hls_out.txt", "wb");
			for(int i = 0;i < 416*416;i++){
				tmp_in=conv0_in.read();
				conv0_in.write(tmp_in);
				fprintf(fp, "%d\n", int(tmp_in));

			}
			fclose(fp);
#endif

	stream<ap_uint<CONV_0_OUT_BIT * CONV_0_OUP> >  conv_0_out("conv_0_out");
#pragma HLS STREAM variable=conv_0_out depth=1000
	CONV2D_ACT_P<CONV_0_K,CONV_0_S,CONV_0_IN_ROW,CONV_0_IN_CH,CONV_0_OUT_CH,
	               CONV_0_IN_BIT,CONV_0_W_BIT,CONV_0_IM,CONV_0_MVTU,CONV_0_OUT_BIT,CONV_0_INP,
				   CONV_0_OUP,CONV_0_INC_BIT,CONV_0_BIAS_BIT,CONV_0_L_SHIFT>
		           (conv0_in,conv_0_w,conv_0_inc,conv_0_bias,conv_0_out,reps);  //outbuf

#ifdef CONV1_DEBUG
			ap_uint<96> tmp_0;
			ap_uint<3> tmp1_0;
			FILE* fp_i = fopen("conv0_hls_out_1.txt", "wb");
			for(int i = 0;i < 208*208;i++){
				tmp_0=conv_0_out.read();
				conv_0_out.write(tmp_0);
				for(int j=0;j<32;j++){      //out channel
					tmp1_0=tmp_0(2,0);        //out Abit
					tmp_0=tmp_0>>3;
					fprintf(fp_i, "%lf\n", double(tmp1_0));
				}
			}
			fclose(fp_i);
#endif

	stream<ap_uint<CONV_1_OUT_BIT * CONV_1_INP> >  conv_1_out("conv_1_out");
#pragma HLS STREAM variable=conv_1_out depth=1000
	DSC_ACT_Inp<CONV_1_K,CONV_1_S,CONV_1_IN_ROW,CONV_1_IN_CH,CONV_1_IN_BIT,CONV_1_W_BIT,
				CONV_1_IM,CONV_1_MVTU,CONV_1_OUT_BIT,CONV_1_INP,CONV_1_INC_BIT,CONV_1_BIAS_BIT,CONV_1_L_SHIFT>
				(conv_0_out,conv_1_w,conv_1_inc,conv_1_bias,conv_1_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV1_DEBUG
			ap_uint<96> tmp;
			ap_uint<3> tmp1;
			FILE* fp_0 = fopen("conv1_hls_out_1.txt", "wb");
			for(int i = 0;i < 208*208;i++){
				tmp=conv_1_out.read();
				conv_1_out.write(tmp);
				for(int j=0;j<32;j++){      //out channel
					tmp1=tmp(2,0);        //out Abit
					tmp=tmp>>3;
					fprintf(fp_0, "%lf\n", double(tmp1));
				}
			}
			fclose(fp_0);
#endif

	stream<ap_uint<CONV_2_OUT_BIT * CONV_2_OUP> >  conv_2_out("conv_2_out");
#pragma HLS STREAM variable=conv_2_out depth=1000
	CONV_1x1_NoACT_in_DSP<CONV_2_IN_ROW,CONV_2_IN_CH,CONV_2_OUT_CH,CONV_2_IN_BIT,CONV_2_W_BIT,CONV_2_IM,CONV_2_MVTU,
						BNOutBit,CONV_2_INP,CONV_2_OUP,CONV_2_INC_BIT,CONV_2_BIAS_BIT,CONV_2_L_SHIFT>
						(conv_1_out,conv_2_w,conv_2_inc,conv_2_bias,conv_2_out,reps);
//-----------------------------------debug-------------------------------------------
#ifdef CONV2_DEBUG
			ap_uint<CONV_2_OUT_BIT * CONV_2_OUP> tmp_2;
			ap_int<CONV_2_OUT_BIT> tmp1_2;
			FILE* fp2 = fopen("conv2_hls_out.txt", "wb");
			for(int i = 0;i < CONV_2_OUT_ROW*CONV_2_OUT_COL*CONV_2_OUT_CH/CONV_2_OUP;i++){
				tmp_2=conv_2_out.read();
				conv_2_out.write(tmp_2);
				for(int j=0;j<CONV_2_OUP;j++){      //out channel
					tmp1_2=tmp_2(CONV_2_OUT_BIT-1,0);        //out Abit
					tmp_2=tmp_2>>CONV_2_OUT_BIT;
					fprintf(fp2, "%lf\n", double(tmp1_2));
				}
			}
			fclose(fp2);
#endif

	stream<ap_uint<CONV_5_OUT_BIT * CONV_5_OUP> >  block_0_out("block_0_out");
#pragma HLS STREAM variable=block_0_out depth=1000
	Second_Block_2DSP<CONV_3_IN_ROW,CONV_3_IN_CH,CONV_3_OUT_CH,CONV_3_IN_BIT,CONV_3_W_BIT,CONV_3_IM,CONV_3_MVTU,CONV_3_OUT_BIT,
				 CONV_3_INP,CONV_3_OUP,CONV_3_INC_BIT,CONV_3_BIAS_BIT,CONV_3_L_SHIFT,
				 CONV_4_K,CONV_4_S,CONV_4_W_BIT,CONV_4_IM,CONV_4_MVTU,CONV_4_INP,CONV_4_INC_BIT,CONV_4_BIAS_BIT,CONV_4_L_SHIFT,
				 CONV_5_OUT_CH,CONV_5_W_BIT,CONV_5_IM,CONV_5_MVTU,CONV_5_OUT_BIT,CONV_5_INP,CONV_5_OUP,CONV_5_INC_BIT,CONV_5_BIAS_BIT,
				 CONV_5_L_SHIFT>
			(conv_2_out,conv_3_w,conv_3_inc,conv_3_bias,conv_4_w,conv_4_inc,conv_4_bias,conv_5_w,conv_5_inc,conv_5_bias,block_0_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV5_DEBUG
			ap_uint<CONV_5_OUT_BIT * CONV_5_OUP> tmp_5;
			ap_int<CONV_5_OUT_BIT> tmp1_5;
			FILE* fp_5 = fopen("conv_5_hls_out.txt", "wb");
			for(int i = 0;i < CONV_5_OUT_ROW*CONV_5_OUT_COL*CONV_5_OUT_CH/CONV_5_OUP;i++){
				tmp_5=block_0_out.read();
				block_0_out.write(tmp_5);
				for(int j=0;j<CONV_5_OUP;j++){      //out channel
					tmp1_5=tmp_5(CONV_5_OUT_BIT-1,0);        //out Abit
					tmp_5=tmp_5>>CONV_5_OUT_BIT;
					fprintf(fp_5, "%lf\n", double(tmp1_5));
				}
			}
			fclose(fp_5);
#endif

	stream<ap_uint<FIRST_BLOCK_OUTBIT * CONV_8_OUP> >  block_1_out("block_1_out");
#pragma HLS STREAM variable=block_1_out depth=1000
	First_Block_2DSP<CONV_6_IN_ROW,CONV_6_IN_CH,CONV_6_OUT_CH,CONV_6_IN_BIT,CONV_6_W_BIT,CONV_6_IM,CONV_6_MVTU,CONV_6_OUT_BIT,CONV_6_INP,CONV_6_OUP,CONV_6_INC_BIT,
				CONV_6_BIAS_BIT,CONV_6_L_SHIFT,
				CONV_7_K,CONV_7_S,CONV_7_W_BIT,CONV_7_IM,CONV_7_MVTU,CONV_7_INP,CONV_7_INC_BIT,CONV_7_BIAS_BIT,CONV_7_L_SHIFT,
				CONV_8_W_BIT,CONV_8_IM,CONV_8_MVTU,CONV_8_OUT_BIT,CONV_8_INP,CONV_8_OUP,CONV_8_INC_BIT,CONV_8_BIAS_BIT,CONV_8_L_SHIFT,
				FIRST_BLOCK_OUTBIT>
		(block_0_out,conv_6_w,conv_6_inc,conv_6_bias,conv_7_w,conv_7_inc,conv_7_bias,conv_8_w,conv_8_inc,conv_8_bias,block_1_out,reps);

	//-----------------------------------debug-------------------------------------------
#ifdef CONV8_DEBUG
			ap_uint<FIRST_BLOCK_OUTBIT * CONV_8_OUP> tmp_8;
			ap_int<FIRST_BLOCK_OUTBIT> tmp1_8;
			FILE* fp_8 = fopen("conv_8_hls_out.txt", "wb");
			for(int i = 0;i < CONV_8_OUT_ROW*CONV_8_OUT_COL*CONV_8_OUT_CH/CONV_8_OUP;i++){
				tmp_8=block_1_out.read();
				block_1_out.write(tmp_8);
				for(int j=0;j<CONV_8_OUP;j++){      //out channel
					tmp1_8=tmp_8(FIRST_BLOCK_OUTBIT-1,0);        //out Abit
					tmp_8=tmp_8>>FIRST_BLOCK_OUTBIT;
					fprintf(fp_8, "%lf\n", double(tmp1_8));
				}
			}
			fclose(fp_8);
#endif

			//----------------------------------------------------------------------------------

	stream<ap_uint<CONV_11_OUT_BIT * CONV_11_OUP> >  block_2_out("block_2_out");
#pragma HLS STREAM variable=block_2_out depth=1000
	Second_Block_2DSP<CONV_9_IN_ROW,CONV_9_IN_CH,CONV_9_OUT_CH,CONV_9_IN_BIT,CONV_9_W_BIT,CONV_9_IM,CONV_9_MVTU,CONV_9_OUT_BIT,CONV_9_INP,CONV_9_OUP,CONV_9_INC_BIT,
				 CONV_9_BIAS_BIT,CONV_9_L_SHIFT,
				 CONV_10_K,CONV_10_S,CONV_10_W_BIT,CONV_10_IM,CONV_10_MVTU,CONV_10_INP,CONV_10_INC_BIT,CONV_10_BIAS_BIT,CONV_10_L_SHIFT,
				 CONV_11_OUT_CH,CONV_11_W_BIT,CONV_11_IM,CONV_11_MVTU,CONV_11_OUT_BIT,CONV_11_INP,CONV_11_OUP,CONV_11_INC_BIT,CONV_11_BIAS_BIT,CONV_11_L_SHIFT>
				(block_1_out,conv_9_w,conv_9_inc,conv_9_bias,conv_10_w,conv_10_inc,conv_10_bias,conv_11_w,conv_11_inc,conv_11_bias,block_2_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV11_DEBUG
			ap_uint<CONV_11_OUT_BIT * CONV_11_OUP> tmp_11;
			ap_int<CONV_11_OUT_BIT> tmp1_11;
			FILE* fp_11 = fopen("conv_11_hls_out.txt", "wb");
			for(int i = 0;i < CONV_11_OUT_ROW*CONV_11_OUT_COL*CONV_11_OUT_CH/CONV_11_OUP;i++){
				tmp_11=block_2_out.read();
				block_2_out.write(tmp_11);
				for(int j=0;j<CONV_11_OUP;j++){      //out channel
					tmp1_11=tmp_11(CONV_11_OUT_BIT-1,0);        //out Abit
					tmp_11=tmp_11>>CONV_11_OUT_BIT;
					fprintf(fp_11, "%lf\n", double(tmp1_11));
				}
			}
			fclose(fp_11);
#endif
			//----------------------------------------------------------------------------------

	stream<ap_uint<FIRST_BLOCK_OUTBIT * CONV_14_OUP> >  block_3_out("block_3_out");
#pragma HLS STREAM variable=block_3_out depth=1000
		First_Block_2DSP<CONV_12_IN_ROW,CONV_12_IN_CH,CONV_12_OUT_CH,CONV_12_IN_BIT,CONV_12_W_BIT,CONV_12_IM,CONV_12_MVTU,CONV_12_OUT_BIT,CONV_12_INP,CONV_12_OUP,CONV_12_INC_BIT,
		            CONV_12_BIAS_BIT,CONV_12_L_SHIFT,
				 	CONV_13_K,CONV_13_S,CONV_13_W_BIT,CONV_13_IM,CONV_13_MVTU,CONV_13_INP,CONV_13_INC_BIT,CONV_13_BIAS_BIT,CONV_13_L_SHIFT,
					CONV_14_W_BIT,CONV_14_IM,CONV_14_MVTU,CONV_14_OUT_BIT,CONV_14_INP,CONV_14_OUP,CONV_14_INC_BIT,CONV_14_BIAS_BIT,CONV_14_L_SHIFT,
					FIRST_BLOCK_OUTBIT>
				   (block_2_out,conv_12_w,conv_12_inc,conv_12_bias,conv_13_w,conv_13_inc,conv_13_bias,conv_14_w,conv_14_inc,conv_14_bias,block_3_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV14_DEBUG
			ap_uint<FIRST_BLOCK_OUTBIT * CONV_14_OUP> tmp_14;
			ap_int<FIRST_BLOCK_OUTBIT> tmp1_14;
			FILE* fp_14 = fopen("conv_14_hls_out.txt", "wb");
			for(int i = 0;i < CONV_14_OUT_ROW*CONV_14_OUT_COL*CONV_14_OUT_CH/CONV_14_OUP;i++){
				tmp_14=block_3_out.read();
				block_3_out.write(tmp_14);
				for(int j=0;j<CONV_14_OUP;j++){      //out channel
					tmp1_14=tmp_14(FIRST_BLOCK_OUTBIT-1,0);        //out Abit
					tmp_14=tmp_14>>FIRST_BLOCK_OUTBIT;
					fprintf(fp_14, "%lf\n", double(tmp1_14));
				}
			}
			fclose(fp_14);
#endif


	stream<ap_uint<Two_Bypass_Outbit * CONV_17_OUP> >  block_4_out("block_4_out");
#pragma HLS STREAM variable=block_4_out depth=1000
		First_Block_2DSP<CONV_15_IN_ROW,CONV_15_IN_CH,CONV_15_OUT_CH,CONV_15_IN_BIT,CONV_15_W_BIT,CONV_15_IM,CONV_15_MVTU,CONV_12_OUT_BIT,CONV_15_INP,CONV_15_OUP,CONV_15_INC_BIT,
			        CONV_15_BIAS_BIT,CONV_15_L_SHIFT,
					CONV_16_K,CONV_16_S,CONV_16_W_BIT,CONV_16_IM,CONV_16_MVTU,CONV_16_INP,CONV_16_INC_BIT,CONV_16_BIAS_BIT,CONV_16_L_SHIFT,
					CONV_17_W_BIT,CONV_17_IM,CONV_17_MVTU,CONV_17_OUT_BIT,CONV_17_INP,CONV_17_OUP,CONV_17_INC_BIT,CONV_17_BIAS_BIT,CONV_17_L_SHIFT,
					Two_Bypass_Outbit>
				    (block_3_out,conv_15_w,conv_15_inc,conv_15_bias,conv_16_w,conv_16_inc,conv_16_bias,conv_17_w,conv_17_inc,conv_17_bias,block_4_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV17_DEBUG
			ap_uint<Two_Bypass_Outbit * CONV_17_OUP> tmp_17;
			ap_int<Two_Bypass_Outbit> tmp1_17;
			FILE* fp_17 = fopen("conv_17_hls_out.txt", "wb");
			for(int i = 0;i < CONV_17_OUT_ROW*CONV_17_OUT_COL*CONV_17_OUT_CH/CONV_17_OUP;i++){
				tmp_17=block_4_out.read();
				block_4_out.write(tmp_17);
				for(int j=0;j<CONV_17_OUP;j++){      //out channel
					tmp1_17=tmp_17(Two_Bypass_Outbit-1,0);        //out Abit
					tmp_17=tmp_17>>Two_Bypass_Outbit;
					fprintf(fp_17, "%lf\n", double(tmp1_17));
				}
			}
			fclose(fp_17);
#endif

	stream<ap_uint<CONV_20_OUT_BIT * CONV_20_OUP> >  block_5_out("block_5_out");
#pragma HLS STREAM variable=block_5_out depth=1000
		Second_Block_1DSP<CONV_18_IN_ROW,CONV_18_IN_CH,CONV_18_OUT_CH,CONV_18_IN_BIT,CONV_18_W_BIT,CONV_18_IM,CONV_18_MVTU,CONV_18_OUT_BIT,CONV_18_INP,CONV_18_OUP,
		             CONV_18_INC_BIT,CONV_18_BIAS_BIT,CONV_18_L_SHIFT,
					 CONV_19_K,CONV_19_S,CONV_19_W_BIT,CONV_19_IM,CONV_19_MVTU,CONV_19_INP,CONV_19_INC_BIT,CONV_19_BIAS_BIT,CONV_19_L_SHIFT,
					 CONV_20_OUT_CH,CONV_20_W_BIT,CONV_20_IM,CONV_20_MVTU,CONV_20_OUT_BIT,CONV_20_INP,CONV_20_OUP,CONV_20_INC_BIT,CONV_20_BIAS_BIT,CONV_20_L_SHIFT>
				     (block_4_out,conv_18_w,conv_18_inc,conv_18_bias,conv_19_w,conv_19_inc,conv_19_bias,conv_20_w,conv_20_inc,conv_20_bias,block_5_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV20_DEBUG
			ap_uint<CONV_20_OUT_BIT * CONV_20_OUP> tmp_20;
			ap_int<CONV_20_OUT_BIT> tmp1_20;
			FILE* fp_20 = fopen("conv_20_hls_out.txt", "wb");
			for(int i = 0;i < CONV_20_OUT_ROW*CONV_20_OUT_COL*CONV_20_OUT_CH/CONV_20_OUP;i++){
				tmp_20=block_5_out.read();
				block_5_out.write(tmp_20);
				for(int j=0;j<CONV_20_OUP;j++){      //out channel
					tmp1_20=tmp_20(CONV_20_OUT_BIT-1,0);        //out Abit
					tmp_20=tmp_20>>CONV_20_OUT_BIT;
					fprintf(fp_20, "%lf\n", double(tmp1_20));
				}
			}
			fclose(fp_20);
#endif

	stream<ap_uint<FIRST_BLOCK_OUTBIT * CONV_23_OUP> >  block_6_out("block_6_out");
#pragma HLS STREAM variable=block_6_out depth=1000
		First_2DSP_E1<CONV_21_IN_ROW,CONV_21_IN_CH,CONV_21_OUT_CH,CONV_21_IN_BIT,CONV_21_W_BIT,CONV_21_IM,CONV_21_MVTU,CONV_21_OUT_BIT,CONV_21_INP,CONV_21_OUP,
		            CONV_21_INC_BIT,CONV_21_BIAS_BIT,CONV_21_L_SHIFT,
				    CONV_22_K,CONV_22_S,CONV_22_W_BIT,CONV_22_IM,CONV_22_MVTU,CONV_22_INP,CONV_22_INC_BIT,CONV_22_BIAS_BIT,CONV_22_L_SHIFT,
				    CONV_23_W_BIT,CONV_23_IM,CONV_23_MVTU,CONV_23_OUT_BIT,CONV_23_INP,CONV_23_OUP,CONV_23_INC_BIT,CONV_23_BIAS_BIT,CONV_23_L_SHIFT,
				    FIRST_BLOCK_OUTBIT>
				    (block_5_out,conv_21_w,conv_21_inc,conv_21_bias,conv_22_w,conv_22_inc,conv_22_bias,conv_23_w,conv_23_inc,conv_23_bias,block_6_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV23_DEBUG
			ap_uint<FIRST_BLOCK_OUTBIT * CONV_23_OUP> tmp_23;
			ap_int<FIRST_BLOCK_OUTBIT> tmp1_23;
			FILE* fp_23 = fopen("conv_23_hls_out.txt", "wb");
			for(int i = 0;i < CONV_23_OUT_ROW*CONV_23_OUT_COL*CONV_23_OUT_CH/CONV_23_OUP;i++){
				tmp_23=block_6_out.read();
				block_6_out.write(tmp_23);
				for(int j=0;j<CONV_23_OUP;j++){      //out channel
					tmp1_23=tmp_23(FIRST_BLOCK_OUTBIT-1,0);        //out Abit
					tmp_23=tmp_23>>FIRST_BLOCK_OUTBIT;
					fprintf(fp_23, "%lf\n", double(tmp1_23));
				}
			}
			fclose(fp_23);
#endif




	stream<ap_uint<CONV_26_OUT_BIT * CONV_26_OUP> >  block_7_out("block_7_out");
#pragma HLS STREAM variable=block_7_out depth=1000
		Second_Block_2DSP<CONV_24_IN_ROW,CONV_24_IN_CH,CONV_24_OUT_CH,CONV_24_IN_BIT,CONV_24_W_BIT,CONV_24_IM,CONV_24_MVTU,CONV_24_OUT_BIT,CONV_24_INP,CONV_24_OUP,CONV_24_INC_BIT,
					 CONV_24_BIAS_BIT,CONV_24_L_SHIFT,
					 CONV_25_K,CONV_25_S,CONV_25_W_BIT,CONV_25_IM,CONV_25_MVTU,CONV_25_INP,CONV_25_INC_BIT,CONV_25_BIAS_BIT,CONV_25_L_SHIFT,
					 CONV_26_OUT_CH,CONV_26_W_BIT,CONV_26_IM,CONV_26_MVTU,CONV_26_OUT_BIT,CONV_26_INP,CONV_26_OUP,CONV_26_INC_BIT,CONV_26_BIAS_BIT,CONV_26_L_SHIFT>
				    (block_6_out,conv_24_w,conv_24_inc,conv_24_bias,conv_25_w,conv_25_inc,conv_25_bias,conv_26_w,conv_26_inc,conv_26_bias,block_7_out,reps);
	
	//-----------------------------------debug-------------------------------------------
#ifdef CONV26_DEBUG
			ap_uint<CONV_26_OUT_BIT * CONV_26_OUP> tmp_26;
			ap_int<CONV_26_OUT_BIT> tmp1_26;
			FILE* fp_26 = fopen("conv_26_hls_out.txt", "wb");
			for(int i = 0;i < CONV_26_OUT_ROW*CONV_26_OUT_COL*CONV_26_OUT_CH/CONV_26_OUP;i++){
				tmp_26=block_7_out.read();
				block_7_out.write(tmp_26);
				for(int j=0;j<CONV_26_OUP;j++){      //out channel
					tmp1_26=tmp_26(CONV_26_OUT_BIT-1,0);        //out Abit
					tmp_26=tmp_26>>CONV_26_OUT_BIT;
					fprintf(fp_26, "%lf\n", double(tmp1_26));
				}
			}
			fclose(fp_26);
#endif
	
// 		stream<ap_uint<CONV_26_OUT_BIT * CONV_27_INP> >  conv_27_in("conv_27_in");
// #pragma HLS STREAM variable=block_7_out depth=640 dim=1
// ExpandWidth<CONV_26_OUT_BIT * CONV_26_OUP, CONV_26_OUT_BIT * CONV_27_INP, CONV_26_OUT_ROW*CONV_26_OUT_COL*CONV_26_OUT_CH/CONV_27_INP>(block_7_out, conv_27_in,reps );
	
	stream<ap_uint<CONV_27_OUT_BIT * CONV_27_OUP> >  conv_27_out("conv_27_out");
#pragma HLS STREAM variable=conv_27_out depth=1000
	CONV_1x1_ACT_SIGN_in_DSP<CONV_27_IN_ROW,CONV_27_IN_CH,CONV_27_OUT_CH,CONV_27_IN_BIT,CONV_27_W_BIT,CONV_27_IM,CONV_27_MVTU,CONV_27_OUT_BIT,CONV_27_INP,CONV_27_OUP,
	             CONV_27_INC_BIT,CONV_27_BIAS_BIT,CONV_27_L_SHIFT>
				(block_7_out,conv_27_w,conv_27_inc,conv_27_bias,conv_27_out,reps);

//-----------------------------------debug-------------------------------------------
#ifdef CONV27_DEBUG
			ap_uint<CONV_27_OUT_BIT * CONV_27_OUP> tmp_27;
			ap_uint<CONV_27_OUT_BIT> tmp1_27;
			FILE* fp_27 = fopen("conv_27_hls_out.txt", "wb");
			for(int i = 0;i < CONV_27_OUT_ROW*CONV_27_OUT_COL*CONV_27_OUT_CH/CONV_27_OUP;i++){
				tmp_27=conv_27_out.read();
				conv_27_out.write(tmp_27);
				for(int j=0;j<CONV_27_OUP;j++){      //out channel
					tmp1_27=tmp_27(CONV_27_OUT_BIT-1,0);        //out Abit
					tmp_27=tmp_27>>CONV_27_OUT_BIT;
					fprintf(fp_27, "%lf\n", double(tmp1_27));
				}
			}
			fclose(fp_27);
#endif

	stream<ap_uint<LastOutBit * CONV_28_PE> >  last_out("last_out");
#pragma HLS STREAM variable=last_out depth=1000
	CONV2D_1x1_NOACT_NoP_DSP<CONV_28_IFM_ROW,CONV_28_IFM_CH,CONV_28_OFM_CH,CONV_28_IN_BIT,CONV_28_W_BIT,LastOutBit,CONV_28_SIMD,CONV_28_PE>
			(conv_27_out,conv_28_w,last_out,reps);

	stream<ap_uint<32> >  single_out("single_out");
#pragma HLS STREAM variable=single_out depth=1000 dim=1
  ReduceWidth<LastOutBit * CONV_28_PE, 32 , CONV_28_IFM_ROW*CONV_28_IFM_ROW*CONV_28_OFM_CH / CONV_28_PE>(last_out,single_out,reps);

	AddLast<25*CONV_28_IFM_ROW*CONV_28_IFM_ROW>(single_out,out,reps);
}




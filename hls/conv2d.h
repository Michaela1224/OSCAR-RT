#pragma once
#include "matrix-vector-unit.h"
#include <hls_stream.h>
#include <ap_int.h>
using namespace hls;

//#define ReduceWidth_P_DEBUG
//#define SWU_P0_DEBUG
//2020/3/10 已经查证
//1*1卷积，无BN，无激活
template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP>
void CONV2D_1x1_NOACT_NoP(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	stream<ap_uint<MVTU_OutP*Mbit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

//	stream<ap_uint<MVTU_InP*Ibit> > swu_out_reduced("swu_out_reduced");
//	ReduceWidth<Cin*Ibit, MVTU_InP*Ibit, K*K*Dout*Dout> (in, swu_out_reduced, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
#endif

//	stream<ap_uint<MVTU_OutP*Mbit> > out_raw("out_raw");
	MVU_rowfirst<Dout*Dout, Ibit, Wbit, Mbit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP>
	(in, weights, out, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

//	ExpandWidth<MVTU_OutP*Mbit, Cout*Mbit, Dout*Dout>
//	(out_raw, out, reps);
}




template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP>
void CONV2D_1x1_NOACT_NoP_DSP(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	stream<ap_uint<MVTU_OutP*Mbit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

//	stream<ap_uint<MVTU_InP*Ibit> > swu_out_reduced("swu_out_reduced");
//	ReduceWidth<Cin*Ibit, MVTU_InP*Ibit, K*K*Dout*Dout> (in, swu_out_reduced, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
#endif

//	stream<ap_uint<MVTU_OutP*Mbit> > out_raw("out_raw");
	MVU_rowfirst_DSP<Dout*Dout, Ibit, Wbit, Mbit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP>
	(in, weights, out, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

//	ExpandWidth<MVTU_OutP*Mbit, Cout*Mbit, Dout*Dout>
//	(out_raw, out, reps);
}
/***************************/
/*函数名：CONV1x1_NoACT_out
IFM复用。输出缓存
激活函数为+-X 例如4bit 正负8
2021/03/20已经查证
*/
/***************************/

template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV1x1_NoACT_out(
	stream<ap_uint<Cin*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU_OutP][(Cout/MVTU_OutP)],
	const ap_uint<BIAS_BIT> bias[MVTU_OutP][(Cout/MVTU_OutP)],
	stream<ap_uint<Cout*Abit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

	stream<ap_uint<MVTU_InP*Ibit> > swu_out_reduced("swu_out_reduced");
	ReduceWidth<Cin*Ibit, MVTU_InP*Ibit, K*K*Dout*Dout> (in, swu_out_reduced, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
#endif

	stream<ap_uint<MVTU_OutP*Abit> > out_raw("out_raw");
	MVAU_outbuf_BN<Dout*Dout, Ibit, Wbit, Mbit, Abit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
	(swu_out_reduced, weights, inc, bias, out_raw, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

	ExpandWidth<MVTU_OutP*Abit, Cout*Abit, Dout*Dout>(out_raw, out, reps);
}

/***************************/
/*函数名：CONV_1x1_NoACT_in(CONV2D_1x1_NoACT)
IFM复用。输入缓存
激活函数为+-X 例如4bit 正负8
输入为无符号
2021/03/20已经查证
*/
/***************************/

template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV_1x1_NoACT_in(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU_OutP][(Cout/MVTU_OutP)],
	const ap_uint<BIAS_BIT> bias[MVTU_OutP][(Cout/MVTU_OutP)],
	stream<ap_uint<MVTU_OutP*Abit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

//	stream<ap_uint<MVTU_InP*Ibit> > swu_out_reduced("swu_out_reduced");
//	ReduceWidth<Cin*Ibit, MVTU_InP*Ibit, K*K*Dout*Dout> (in, swu_out_reduced, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
#endif

//	stream<ap_uint<MVTU_OutP*Abit> > out_raw("out_raw");
	MVAU_inbuf_BN<Dout*Dout, Ibit, Wbit, Mbit, Abit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
	(in, weights, inc, bias, out, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

//	ExpandWidth<MVTU_OutP*Abit, Cout*Abit, Dout*Dout>(out_raw, out, reps);
}

template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV_1x1_NoACT_in_DSP(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU][(Cout/MVTU)],
	const ap_uint<BIAS_BIT> bias[MVTU][(Cout/MVTU)],
	stream<ap_uint<MVTU_OutP*Abit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

	const unsigned InputFold = Cin*K*K/MVTU_InP;
	const unsigned OutputFold = Cout/MVTU_OutP;
	const unsigned totalReps = Dout*Dout*OutputFold;

stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
MVAU_inbuf_USIGN_DSP<totalReps, Ibit, Wbit, IM, MVTU, Cin*K*K, InputFold,Cout, OutputFold,MVTU_InP, MVTU_OutP>(in, weights, out_raw,reps);

#ifdef TEST_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
	ap_uint<MVTU*IM> tmp;
	ap_int<IM> tmp1;
	FILE* fp1 = fopen("result_MVAU_10.txt", "wb");
	for(int i = 0;i < Dout*Dout*(Cout/MVTU_OutP);i++){
		for(int k= 0;k < MVTU_OutP/MVTU;k++){
			tmp=out_raw.read();
			out_raw.write(tmp);
				for(int j=0;j<MVTU;j++){      //out channel
					tmp1=tmp(IM-1,0);        //out Abit
					tmp=tmp>>IM;
					fprintf(fp1, "%lf\n", double(tmp1));
				}
		}
	}
	fclose(fp1);
#endif

BN_ReUse<totalReps,Ibit, Wbit,IM, MVTU, Abit, Cin*K*K, InputFold,Cout,OutputFold, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT> (out_raw, inc, bias, out,reps);

// //	stream<ap_uint<MVTU_OutP*Abit> > out_raw("out_raw");
// 	MVAU_inbuf_BN_DSP<Dout*Dout, Ibit, Wbit, Mbit, Abit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
// 	(in, weights, inc, bias, out, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

//	ExpandWidth<MVTU_OutP*Abit, Cout*Abit, Dout*Dout>(out_raw, out, reps);
}








/***************************/
/*函数名：DSC_ACT_P_v1
//3*3Dconv,并行度提高版，带BN和Relu6
若使用该函数，并行度设为MVTU_InP，实际为MVTU_InP*K*K
问题：  解决SWU的问题，FIFO
2021/03/12已经查证
*/
/***************************/

template <	unsigned K,
			unsigned S,
			unsigned Din,
			unsigned Cin,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DSC_ACT_P_v1(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*K*K*Wbit> weights[Cin/MVTU_InP],
	const ap_uint<INC_BIT> inc[MVTU][(Cin/MVTU)],
	const ap_uint<BIAS_BIT> bias[MVTU][(Cin/MVTU)],
	stream<ap_uint<MVTU_InP*Abit> >& out,     //对Relu6而言，Abit恒为3
	const unsigned reps)
{
#pragma HLS DATAFLOW
	const unsigned Dout = Din/S + (Din%S > 0);
	const unsigned IntermediateDout = S*(Dout-1) + K;
#ifdef DSC_DEBUG
	cout << "Dout: " << Dout << endl;
	cout << "IntermediateDout: " << IntermediateDout << endl;
#endif
	const unsigned BottomRightPad = (IntermediateDout - Din)/2;
	const unsigned TopLeftPad = (IntermediateDout - Din) - BottomRightPad;
#ifdef DSC_DEBUG
	cout << "TopLeftPad: " << TopLeftPad << endl;
	cout << "BottomRightPad: " << BottomRightPad << endl;
#endif
	stream<ap_uint<Cin*Ibit> > in_raw("in_raw");
#pragma HLS STREAM variable=in_raw depth=2
	ExpandWidth<MVTU_InP*Ibit, Cin*Ibit, Din*Din>(in, in_raw, reps);

	stream<ap_uint<Cin*Ibit> > samepad_out("samepad_out");
#pragma HLS STREAM variable=samepad_out depth=2
	SAMEPAD<TopLeftPad, BottomRightPad, Din, Cin, Ibit>(in_raw, samepad_out, reps);
#ifdef DSC_DEBUG
	cout << "samepad_out.size(): " << samepad_out.size() << endl;
#endif

	stream<ap_uint<MVTU_InP*Ibit*K*K> > swu_out("swu_out");
#pragma HLS STREAM variable=swu_out depth=2
	SWU_DSC<K, S, IntermediateDout, IntermediateDout, Cin, Ibit,MVTU_InP> (samepad_out, swu_out, reps);
#ifdef SWU_P0_DEBUG
		ap_uint<32*3*3*3> tmp0;
		ap_uint<3> tmp10;
		FILE* fp0 = fopen("dsc_swu_p_out.txt", "wb");
		for(int i = 0;i < Dout*Dout;i++){
			tmp0=swu_out.read();
			swu_out.write(tmp0);
			for(int j=0;j<32*3*3;j++){      //out channel
				tmp10=tmp0(2,0);        //out Abit
				tmp0=tmp0>>3;
				fprintf(fp0, "%lf\n", double(tmp10));
			}
		}
#endif

	const unsigned ChannelFold = Cin/MVTU_InP;
	const unsigned totalReps = Dout*Dout*ChannelFold;

	stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
	DSC_MVAU_KKP_1<totalReps,Ibit,K,Wbit,IM,MVTU,Cin,ChannelFold,MVTU_InP,INC_BIT,BIAS_BIT,L_SHIFT>
	    (swu_out,weights,out_raw,reps);

#ifdef DW_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
	ap_uint<MVTU*IM> tmp;
	ap_int<IM> tmp1;
	FILE* fp = fopen("dot_MVAU_10_2.txt", "wb");
	for(int i = 0;i < Dout*Dout*(Cin/MVTU_InP);i++){
		for(int k= 0;k < MVTU_InP/MVTU;k++){
			tmp=out_raw.read();
			out_raw.write(tmp);
				for(int j=0;j<MVTU;j++){      //out channel
					tmp1=tmp(IM-1,0);        //out Abit
					tmp=tmp>>IM;
					fprintf(fp, "%lf\n", double(tmp1));
				}
		}
	}
	fclose(fp);
#endif

	DWURelu6<totalReps,Ibit, Wbit, IM,MVTU, Abit, Cin, ChannelFold, MVTU_InP, INC_BIT,BIAS_BIT,L_SHIFT> (out_raw, inc, bias, out,reps);


}

template <	unsigned K,
			unsigned S,
			unsigned Din,
			unsigned Cin,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DSC_ACT_Inp(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*K*K*Wbit> weights[Cin/MVTU_InP],
	const ap_uint<INC_BIT> inc[MVTU][(Cin/MVTU)],
	const ap_uint<BIAS_BIT> bias[MVTU][(Cin/MVTU)],
	stream<ap_uint<MVTU_InP*Abit> >& out,     //对Relu6而言，Abit恒为3
	const unsigned reps)
{
#pragma HLS DATAFLOW
	const unsigned Dout = Din/S + (Din%S > 0);
	const unsigned IntermediateDout = S*(Dout-1) + K;
#ifdef DSC_DEBUG
	cout << "Dout: " << Dout << endl;
	cout << "IntermediateDout: " << IntermediateDout << endl;
#endif
	const unsigned BottomRightPad = (IntermediateDout - Din)/2;
	const unsigned TopLeftPad = (IntermediateDout - Din) - BottomRightPad;
#ifdef DSC_DEBUG
	cout << "TopLeftPad: " << TopLeftPad << endl;
	cout << "BottomRightPad: " << BottomRightPad << endl;
#endif
	stream<ap_uint<Cin*Ibit> > in_raw("in_raw");
#pragma HLS STREAM variable=in_raw depth=2
	ExpandWidth<MVTU_InP*Ibit, Cin*Ibit, Din*Din>(in, in_raw, reps);

	stream<ap_uint<Cin*Ibit> > samepad_out("samepad_out");
#pragma HLS STREAM variable=samepad_out depth=2
	SAMEPAD<TopLeftPad, BottomRightPad, Din, Cin, Ibit>(in_raw, samepad_out, reps);
#ifdef DSC_DEBUG
	cout << "samepad_out.size(): " << samepad_out.size() << endl;
#endif

	stream<ap_uint<MVTU_InP*Ibit*K*K> > swu_out("swu_out");
#pragma HLS STREAM variable=swu_out depth=2
	SWU_DSC<K, S, IntermediateDout, IntermediateDout, Cin, Ibit,MVTU_InP> (samepad_out, swu_out, reps);
#ifdef SWU_P0_DEBUG
		ap_uint<32*3*3*3> tmp0;
		ap_uint<3> tmp10;
		FILE* fp0 = fopen("dsc_swu_p_out.txt", "wb");
		for(int i = 0;i < Dout*Dout;i++){
			tmp0=swu_out.read();
			swu_out.write(tmp0);
			for(int j=0;j<32*3*3;j++){      //out channel
				tmp10=tmp0(2,0);        //out Abit
				tmp0=tmp0>>3;
				fprintf(fp0, "%lf\n", double(tmp10));
			}
		}
#endif

	const unsigned ChannelFold = Cin/MVTU_InP;
	const unsigned totalReps = Dout*Dout*ChannelFold;

	stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
	DSC_MVAU_P_2<totalReps,Ibit,K,Wbit,IM,MVTU,Cin,ChannelFold,MVTU_InP,INC_BIT,BIAS_BIT,L_SHIFT>
	    (swu_out,weights,out_raw,reps);

#ifdef DW_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
	ap_uint<MVTU*IM> tmp;
	ap_int<IM> tmp1;
	FILE* fp = fopen("dot_MVAU_10_2.txt", "wb");
	for(int i = 0;i < Dout*Dout*(Cin/MVTU_InP);i++){
		for(int k= 0;k < MVTU_InP/MVTU;k++){
			tmp=out_raw.read();
			out_raw.write(tmp);
				for(int j=0;j<MVTU;j++){      //out channel
					tmp1=tmp(IM-1,0);        //out Abit
					tmp=tmp>>IM;
					fprintf(fp, "%lf\n", double(tmp1));
				}
		}
	}
	fclose(fp);
#endif

	DWURelu6<totalReps,Ibit, Wbit, IM,MVTU, Abit, Cin, ChannelFold, MVTU_InP, INC_BIT,BIAS_BIT,L_SHIFT> (out_raw, inc, bias, out,reps);

}

template <	unsigned K,
			unsigned S,
			unsigned Din,
			unsigned Cin,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DSC_ACT_P_DSP(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*K*K*Wbit> weights[Cin/MVTU_InP],
	const ap_uint<INC_BIT> inc[Cin],
	const ap_uint<BIAS_BIT> bias[Cin],
	stream<ap_uint<MVTU_InP*Abit> >& out,     //对Relu6而言，Abit恒为3
	const unsigned reps)
{
#pragma HLS DATAFLOW
	const unsigned Dout = Din/S + (Din%S > 0);
	const unsigned IntermediateDout = S*(Dout-1) + K;
#ifdef DSC_DEBUG
	cout << "Dout: " << Dout << endl;
	cout << "IntermediateDout: " << IntermediateDout << endl;
#endif
	const unsigned BottomRightPad = (IntermediateDout - Din)/2;
	const unsigned TopLeftPad = (IntermediateDout - Din) - BottomRightPad;
#ifdef DSC_DEBUG
	cout << "TopLeftPad: " << TopLeftPad << endl;
	cout << "BottomRightPad: " << BottomRightPad << endl;
#endif
	stream<ap_uint<Cin*Ibit> > in_raw("in_raw");
	ExpandWidth<MVTU_InP*Ibit, Cin*Ibit, Din*Din>(in, in_raw, reps);

	stream<ap_uint<Cin*Ibit> > samepad_out("samepad_out");
	SAMEPAD<TopLeftPad, BottomRightPad, Din, Cin, Ibit>(in_raw, samepad_out, reps);
#ifdef DSC_DEBUG
	cout << "samepad_out.size(): " << samepad_out.size() << endl;
#endif

	stream<ap_uint<MVTU_InP*Ibit*K*K> > swu_out("swu_out");
	SWU_DSC<K, S, IntermediateDout, IntermediateDout, Cin, Ibit,MVTU_InP> (samepad_out, swu_out, reps);
#ifdef SWU_P0_DEBUG
		ap_uint<32*3*3*3> tmp0;
		ap_uint<3> tmp10;
		FILE* fp0 = fopen("dsc_swu_p_out.txt", "wb");
		for(int i = 0;i < Dout*Dout;i++){
			tmp0=swu_out.read();
			swu_out.write(tmp0);
			for(int j=0;j<32*3*3;j++){      //out channel
				tmp10=tmp0(2,0);        //out Abit
				tmp0=tmp0>>3;
				fprintf(fp0, "%lf\n", double(tmp10));
			}
		}
#endif



//	stream<ap_uint<MVTU_InP*Abit> > out_raw("out_raw");
//	DSC_MVAU_P<Dout*Dout,Ibit,K,Wbit,Mbit,Abit,Cin,MVTU_InP,INC_BIT,BIAS_BIT,L_SHIFT>
 //   (swu_out,weights,inc,bias,out_raw,reps);
	DSC_MVAU_KP_DSP<Dout*Dout,Ibit,K,Wbit,Mbit,Abit,Cin,MVTU_InP,INC_BIT,BIAS_BIT,L_SHIFT>
	    (swu_out,weights,inc,bias,out,reps);
//	ExpandWidth<MVTU_InP*Abit, Cin*Abit, Dout*Dout>
//		(out_raw, out, reps);
}
/***************************/
/*函数名：CONV2D_ACT_KKP
适用于通道数较少的3*3卷积，将K*K的卷积核全部展开做并行处理

在Rocky比赛中，800*800*1->400*400*32中使用

2021/03/10已经查证
*/
/***************************/
template <	unsigned K,
			unsigned S,
			unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV2D_ACT_KKP(
	stream<ap_uint<Cin*Ibit> >& in,
	const ap_uint<MVTU_InP*K*K*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU][Cout/MVTU],
	const ap_uint<BIAS_BIT> bias[MVTU][Cout/MVTU],
	stream<ap_uint<MVTU_OutP*Abit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned Dout = Din/S + (Din%S > 0);
	const unsigned IntermediateDout = S*(Dout-1) + K;
#ifdef CONV2_DEBUG
	cout << "Dout: " << Dout << endl;
	cout << "IntermediateDout: " << IntermediateDout << endl;
#endif
	const unsigned BottomRightPad = (IntermediateDout - Din)/2;
	const unsigned TopLeftPad = (IntermediateDout - Din) - BottomRightPad;
#ifdef CONV2_DEBUG
	cout << "TopLeftPad: " << TopLeftPad << endl;
	cout << "BottomRightPad: " << BottomRightPad << endl;
#endif

	stream<ap_uint<Cin*Ibit> > samepad_out("samepad_out");
#pragma HLS STREAM variable=samepad_out depth=2
	SAMEPAD<TopLeftPad, BottomRightPad, Din, Cin, Ibit>(in, samepad_out, reps);
#ifdef CONV2_DEBUG
	cout << "samepad_out.size(): " << samepad_out.size() << endl;
#endif

	stream<ap_uint<MVTU_InP*K*K*Ibit> > swu_out("swu_out");
#pragma HLS STREAM variable=swu_out depth=2
	SWU_KK<K, S, IntermediateDout, IntermediateDout, Cin, Ibit,MVTU_InP> (samepad_out, swu_out, reps);

#ifdef SWU_P_DEBUG
		ap_uint<1*3*3*8> tmp;
		ap_uint<8> tmp1;
		FILE* fp = fopen("swu_p_out.txt", "wb");
		for(int i = 0;i < 400;i++){
			tmp=swu_out.read();
			swu_out.write(tmp);
			for(int j=0;j<9;j++){      //out channel
				tmp1=tmp(7,0);        //out Abit
				tmp=tmp>>8;
				fprintf(fp, "%lf\n", double(tmp1));
			}
		}
#endif

	const unsigned InputFold = 1;
	const unsigned OutputFold = 1;
	const unsigned totalReps = Dout*Dout;

	stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
	MVAU_inbuf_v2<totalReps, Ibit, Wbit, IM,MVTU, Cin*K*K, InputFold,Cout,OutputFold, Cin*K*K, MVTU_OutP>
	(swu_out, weights, out_raw, reps);
#ifdef CONV2_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

	URelu6_ReUse<totalReps,Ibit, Wbit, IM,MVTU, Abit, Cin*K*K, InputFold,Cout,OutputFold, Cin*K*K, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
	 (out_raw, inc, bias, out,reps);

}

template <	unsigned K,
			unsigned S,
			unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV2D_ACT_P(
	stream<ap_uint<Cin*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin*K*K/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU][Cout/MVTU],
	const ap_uint<BIAS_BIT> bias[MVTU][Cout/MVTU],
	stream<ap_uint<MVTU_OutP*Abit> >& out,
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned Dout = Din/S + (Din%S > 0);
	const unsigned IntermediateDout = S*(Dout-1) + K;
#ifdef CONV2_DEBUG
	cout << "Dout: " << Dout << endl;
	cout << "IntermediateDout: " << IntermediateDout << endl;
#endif
	const unsigned BottomRightPad = (IntermediateDout - Din)/2;
	const unsigned TopLeftPad = (IntermediateDout - Din) - BottomRightPad;
#ifdef CONV2_DEBUG
	cout << "TopLeftPad: " << TopLeftPad << endl;
	cout << "BottomRightPad: " << BottomRightPad << endl;
#endif

	stream<ap_uint<Cin*Ibit> > samepad_out("samepad_out");
#pragma HLS STREAM variable=samepad_out depth=2
	SAMEPAD<TopLeftPad, BottomRightPad, Din, Cin, Ibit>(in, samepad_out, reps);
#ifdef CONV2_DEBUG
	cout << "samepad_out.size(): " << samepad_out.size() << endl;
#endif

	stream<ap_uint<Cin*K*K*Ibit> > swu_out("swu_out");
#pragma HLS STREAM variable=swu_out depth=2
	SWU_KK<K, S, IntermediateDout, IntermediateDout, Cin, Ibit,MVTU_InP> (samepad_out, swu_out, reps);

#ifdef SWU_P_DEBUG
		ap_uint<1*3*3*8> tmp;
		ap_uint<8> tmp1;
		FILE* fp = fopen("swu_p_out.txt", "wb");
		for(int i = 0;i < 400;i++){
			tmp=swu_out.read();
			swu_out.write(tmp);
			for(int j=0;j<9;j++){      //out channel
				tmp1=tmp(7,0);        //out Abit
				tmp=tmp>>8;
				fprintf(fp, "%lf\n", double(tmp1));
			}
		}
#endif
	stream<ap_uint<MVTU_InP*Ibit> > swu_out_reduced("swu_out_reduced");
#pragma HLS STREAM variable=swu_out_reduced depth=2
	ReduceWidth<Cin*Ibit*K*K, MVTU_InP*Ibit, Dout*Dout> (swu_out, swu_out_reduced, reps);
	const unsigned InputFold = 9;
	const unsigned OutputFold = 1;
	const unsigned totalReps = Dout*Dout;

	stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
	MVAU_inbuf_v2<totalReps, Ibit, Wbit, IM,MVTU, Cin*K*K, InputFold,Cout,OutputFold, MVTU_InP, MVTU_OutP>
	(swu_out_reduced, weights, out_raw, reps);
#ifdef CONV2_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

	URelu6_ReUse<totalReps,Ibit, Wbit, IM,MVTU, Abit, Cin*K*K, InputFold,Cout,OutputFold, Cin*K*K, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
	 (out_raw, inc, bias, out,reps);

}


template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV_1x1_ACT_SIGN_in(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU][(Cout/MVTU)],
	const ap_uint<BIAS_BIT> bias[MVTU][(Cout/MVTU)],
	stream<ap_uint<MVTU_OutP*Abit> >& out,                      //Relu6 3bit
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

	const unsigned InputFold = Cin*K*K/MVTU_InP;
	const unsigned OutputFold = Cout/MVTU_OutP;
	const unsigned totalReps = Dout*Dout*OutputFold;

stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
MVAU_inbuf_SIGN<totalReps, Ibit, Wbit, IM,MVTU, Cin*K*K, InputFold,Cout, OutputFold,MVTU_InP, MVTU_OutP>(in, weights, out_raw,reps);

#ifdef TEST_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
	ap_uint<MVTU*IM> tmp;
	ap_int<IM> tmp1;
	FILE* fp = fopen("result_MVAU_9.txt", "wb");
	for(int i = 0;i < Dout*Dout*(Cout/MVTU_OutP);i++){
		for(int k= 0;k < MVTU_OutP/MVTU;k++){
			tmp=out_raw.read();
			out_raw.write(tmp);
				for(int j=0;j<MVTU;j++){      //out channel
					tmp1=tmp(IM-1,0);        //out Abit
					tmp=tmp>>IM;
					fprintf(fp, "%lf\n", double(tmp1));
				}
		}
	}
	fclose(fp);
#endif

Relu6_ReUse<totalReps,Ibit, Wbit, IM,MVTU, Abit, Cin*K*K, InputFold,Cout,OutputFold, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT> (out_raw, inc, bias, out,reps);
// //	stream<ap_uint<MVTU_OutP*Abit> > out_raw("out_raw");
// 	MVAU_in_Relu6_SIGN_DSP<Dout*Dout, Ibit, Wbit, Mbit,Abit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
// 	(in, weights, inc, bias, out, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

//	ExpandWidth<MVTU_OutP*Abit, Cout*Abit, Dout*Dout>(out_raw, out, reps);
}


template <	unsigned Din,
			unsigned Cin,
			unsigned Cout,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MVTU_InP,
			unsigned MVTU_OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void CONV_1x1_ACT_SIGN_in_DSP(
	stream<ap_uint<MVTU_InP*Ibit> >& in,
	const ap_uint<MVTU_InP*Wbit> weights[MVTU_OutP][(Cin/MVTU_InP)*(Cout/MVTU_OutP)],
	const ap_uint<INC_BIT> inc[MVTU][(Cout/MVTU)],
	const ap_uint<BIAS_BIT> bias[MVTU][(Cout/MVTU)],
	stream<ap_uint<MVTU_OutP*Abit> >& out,                      //Relu6 3bit
	const unsigned reps = 1)
{
#pragma HLS DATAFLOW

	const unsigned K = 1;
	const unsigned Dout = Din;

	const unsigned InputFold = Cin*K*K/MVTU_InP;
	const unsigned OutputFold = Cout/MVTU_OutP;
	const unsigned totalReps = Dout*Dout*OutputFold;

stream<ap_uint<MVTU*IM> > out_raw("out_raw");
#pragma HLS STREAM variable=out_raw depth=2
MVAU_inbuf_SIGN_DSP<totalReps, Ibit, Wbit, IM,MVTU, Cin*K*K, InputFold,Cout, OutputFold,MVTU_InP, MVTU_OutP>(in, weights, out_raw,reps);

#ifdef TEST_DEBUG
	cout << "out_raw.size(): " << out_raw.size() << endl;
	ap_uint<MVTU*IM> tmp;
	ap_int<IM> tmp1;
	FILE* fp = fopen("result_MVAU_9.txt", "wb");
	for(int i = 0;i < Dout*Dout*(Cout/MVTU_OutP);i++){
		for(int k= 0;k < MVTU_OutP/MVTU;k++){
			tmp=out_raw.read();
			out_raw.write(tmp);
				for(int j=0;j<MVTU;j++){      //out channel
					tmp1=tmp(IM-1,0);        //out Abit
					tmp=tmp>>IM;
					fprintf(fp, "%lf\n", double(tmp1));
				}
		}
	}
	fclose(fp);
#endif

Relu6_ReUse<totalReps,Ibit, Wbit, IM,MVTU, Abit, Cin*K*K, InputFold,Cout,OutputFold, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT> (out_raw, inc, bias, out,reps);
// //	stream<ap_uint<MVTU_OutP*Abit> > out_raw("out_raw");
// 	MVAU_in_Relu6_SIGN_DSP<Dout*Dout, Ibit, Wbit, Mbit,Abit, Cin*K*K, Cout, MVTU_InP, MVTU_OutP, INC_BIT,BIAS_BIT,L_SHIFT>
// 	(in, weights, inc, bias, out, reps);
#ifdef CONV2_DEBUG
	cout << "swu_out_reduced.size(): " << swu_out_reduced.size() << endl;
	cout << "out_raw.size(): " << out_raw.size() << endl;
#endif

//	ExpandWidth<MVTU_OutP*Abit, Cout*Abit, Dout*Dout>(out_raw, out, reps);
}


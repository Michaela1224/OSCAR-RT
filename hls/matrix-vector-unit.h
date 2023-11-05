#pragma once
#include "misc.h"
#include "stream.h"
using namespace std;
//FILE* fp_test = fopen("test_out.txt", "wb");
// for DSP48E1
//support 
template <	unsigned Wbit,
			unsigned Ibit,
			unsigned Mbit,
			unsigned P>
ap_int<Mbit> DOT_dspopt_v7(
	ap_uint<2*Wbit> weights,
	ap_uint<2*Ibit> in)
{
	ap_int<Mbit> accumulation = 0;
	ap_uint<2> C_reg = 0;
	ap_uint<Mbit> C_port = 0;
	ap_int<25> A_port;
	ap_int<25> D_port;
	ap_uint<18> B_port = 0;
	ap_uint<48> P_port;
#pragma HLS BIND_OP variable=P_port op=mul impl=dsp
//#pragma HLS RESOURCE variable=P_port core=DSP_Macro
	bool in1zero,in2zero;
	ap_int<2*Wbit> temp_W;
	ap_int<Wbit> temp_w1;
	ap_int<Wbit> temp_w2;
	ap_uint<2*Ibit> temp_IN;
	ap_int<8> result1;
	ap_int<8> result2;
	ap_int<9> result;
	for (unsigned p = 0; p < (P>>1); p++) {
#pragma HLS UNROLL
		temp_W = weights( (p+1)*2*Wbit-1, 2*p*Wbit );
		temp_w1 = temp_W(Wbit-1, 0);
		temp_w2 = temp_W(2*Wbit-1, Wbit );
		temp_IN = in((p+1)*2*Ibit-1, 2*p*Ibit);
		D_port(24,20) = temp_w1;
		D_port(19,0) = 0;
		A_port = (ap_int<25>)temp_w2;
		B_port(3,0) = temp_IN(Ibit-1, 0);
		B_port(13,10) = temp_IN( 2*Ibit-1, Ibit);

		if(temp_IN(Ibit-1, 0)!=0) {
			in1zero=true;
		}
		else{
			in1zero=false;
		}
		if(temp_IN( 2*Ibit-1, Ibit)!=0) {
			in2zero=true;
		}
		else{
			in2zero=false;
		}

		if(A_port(24,24)==1){
			if(!in1zero && !in2zero){
				C_reg = 0;
			}else if(!in1zero && in2zero){
				C_reg =1;
			}else{
				C_reg = 2;
			}
		}else{
			C_reg = 0;
		}

		P_port = (A_port+D_port)*B_port;
		result1 = P_port(27,20);
		result2 = P_port(17,10);
		result= (result1+result2);
		accumulation += result;
		C_port += C_reg;

	}

	return accumulation+C_port;
}

/*
//P must be the factor of 2
max(Wbit)=4(signed)
max(Ibit)=5(signed)
*/
template <	unsigned Wbit,
			unsigned Ibit>
ap_int<11>  DSP_reuse_unsigned(ap_int<Wbit> weight0, ap_int<Wbit> weight1,ap_uint<Ibit> in0,ap_uint<Ibit> in1){
	ap_uint<48>  P_port;
#pragma HLS BIND_OP variable=P_port op=mul impl=dsp
//#pragma HLS RESOURCE variable=P_port core=DSP_Macro
	ap_int<25>   A_port;
	ap_int<25>   D_port;
	ap_int<18>  B_port;
	ap_int<4> w0;     //signed
	ap_int<4> w1;
	ap_int<5> a0;     //signed
	ap_int<5> a1;
	ap_uint<1> reg=0;
	w0 = weight0;
	w1 = weight1;
	a0 = in0;
	a1=in1;

	bool  A0=(a0==0)?1:0;
   bool   A1=(a1==0)?1:0;
   bool  B0=(w0==0)?1:0;
   bool  B1=(w1==0)?1:0;

   bool  flag= (a0(4,4) &~w1(3,3) )|(~a0(4,4) &w1(3,3) );

   bool  flag_a1w0=(a1(4,4) &~w0(3,3) )|(~a1(4,4) &w0(3,3) );

	bool  flag_0=a1(4,4)^w0(3,3);
	bool  flag_1=a0(4,4)^w1(3,3);
	bool  flag_2=a0(4,4)^w0(3,3);

	D_port(24,20)=w1;
	D_port(19,0)=0;
	A_port=(ap_int<25>)w0;
    B_port(17,10)=a1;
	B_port(9,0)=0;
	B_port=B_port+(ap_int<18>)a0;

	P_port=((A_port+D_port)*B_port); 

	if((A0&~A1&~B0&~B1)|(B1&~A0&~A1&~B0)){
		if(flag_a1w0){
			reg=1;
		}else{
			reg=0;
		}
	} else if((A1&~A0&~B0&~B1)|(B0&~A0&~A1&~B1)|(A1==0&A0==0&B0==0&B1==0)){
		if(flag){
			reg=1;
		}else{
			reg=0;
		}
	}else if(A0&~A1&~B0&B1){
		if(flag_0){
			reg=1;
		}else{
			reg=0;
		}
	}else if(~A0&A1&B0&~B1){
		if(flag_1){
			reg=1;
		}else{
			reg=0;
		}
	}else if(~A0&A1&~B0&B1){
		if(flag_2){
			reg=1;
		}else{
			reg=0;
		}
	}

	return (ap_int<9>)P_port(8,0)+((ap_int<9>)P_port(38,30))+reg;

}

template <	unsigned Wbit,
			unsigned Ibit>
ap_int<11>  DSP_reuse_signed(ap_int<Wbit> weight0, ap_int<Wbit> weight1,ap_int<Ibit> in0,ap_int<Ibit> in1){
	ap_uint<48>  P_port;
#pragma HLS BIND_OP variable=P_port op=mul impl=dsp
//#pragma HLS RESOURCE variable=P_port core=DSP_Macro
	ap_int<25>   A_port;
	ap_int<25>   D_port;
	ap_int<18>  B_port;
	ap_int<4> w0;     //signed
	ap_int<4> w1;
	ap_int<5> a0;     //signed
	ap_int<5> a1;
	ap_uint<1> reg=0;
	w0 = weight0;
	w1 = weight1;
	a0 = in0;
	a1=in1;
	bool  A0=(a0==0)?1:0;
   bool   A1=(a1==0)?1:0;
   bool  B0=(w0==0)?1:0;
   bool  B1=(w1==0)?1:0;

   bool  flag= (a0(4,4) &~w1(3,3) )|(~a0(4,4) &w1(3,3) );

   bool  flag_a1w0=(a1(4,4) &~w0(3,3) )|(~a1(4,4) &w0(3,3) );

	bool  flag_0=a1(4,4)^w0(3,3);
	bool  flag_1=a0(4,4)^w1(3,3);
	bool  flag_2=a0(4,4)^w0(3,3);

	D_port(24,20)=w1;
	D_port(19,0)=0;
	A_port=(ap_int<25>)w0;
    B_port(17,10)=a1;
	B_port(9,0)=0;
	B_port=B_port+(ap_int<18>)a0;

	P_port=((A_port+D_port)*B_port); 

	if((A0&~A1&~B0&~B1)|(B1&~A0&~A1&~B0)){
		if(flag_a1w0){
			reg=1;
		}else{
			reg=0;
		}
	} else if((A1&~A0&~B0&~B1)|(B0&~A0&~A1&~B1)|(A1==0&A0==0&B0==0&B1==0)){
		if(flag){
			reg=1;
		}else{
			reg=0;
		}
	}else if(A0&~A1&~B0&B1){
		if(flag_0){
			reg=1;
		}else{
			reg=0;
		}
	}else if(~A0&A1&B0&~B1){
		if(flag_1){
			reg=1;
		}else{
			reg=0;
		}
	}else if(~A0&A1&~B0&B1){
		if(flag_2){
			reg=1;
		}else{
			reg=0;
		}
	}
	// fprintf(fp_test, "w:   %f     a:   %f 	wa:  %f   \n", (double)weight0,  (double)in0,(double)((ap_int<9>)P_port(8,0))   )  ;
	// fprintf(fp_test, "w:   %f     a:   %f 	wa:  %f   \n", (double)weight1,  (double)in1,(double)(   ((ap_int<9>)P_port(38,30))+reg        ) )  ;
	// // cout <<"w0a0:   " <<(ap_int<9>)P_port(8,0)<<"  ";
	// cout <<" w1a1:   " <<((ap_int<9>)P_port(38,30))+reg<<"  ";
	// cout <<"sum:   " <<((ap_int<9>)P_port(38,30))+reg<<endl;
	return (ap_int<9>)P_port(8,0)+((ap_int<9>)P_port(38,30))+reg;

}


template <	unsigned Wbit,
			unsigned Ibit>
void  DW_reuse_unsigned(ap_int<Wbit> weight0, ap_int<Wbit> weight1,ap_uint<Ibit> in0,ap_uint<Ibit> in1,ap_int<9>* output_0,ap_int<9>* output_1 ){
	ap_uint<48>  P_port;
#pragma HLS BIND_OP variable=P_port op=mul impl=dsp
//#pragma HLS RESOURCE variable=P_port core=DSP_Macro
	ap_int<25>   A_port;
	ap_int<25>   D_port;
	ap_int<18>  B_port;
	ap_int<4> w0;     //signed
	ap_int<4> w1;
	ap_int<5> a0;     //signed
	ap_int<5> a1;
	ap_uint<1> reg=0;
	w0 = weight0;
	w1 = weight1;
	a0 = in0;
	a1=in1;

	bool  A0=(a0==0)?1:0;
   bool   A1=(a1==0)?1:0;
   bool  B0=(w0==0)?1:0;
   bool  B1=(w1==0)?1:0;

   bool  flag= (a0(4,4) &~w1(3,3) )|(~a0(4,4) &w1(3,3) );

   bool  flag_a1w0=(a1(4,4) &~w0(3,3) )|(~a1(4,4) &w0(3,3) );

	bool  flag_0=a1(4,4)^w0(3,3);
	bool  flag_1=a0(4,4)^w1(3,3);
	bool  flag_2=a0(4,4)^w0(3,3);

	D_port(24,20)=w1;
	D_port(19,0)=0;
	A_port=(ap_int<25>)w0;
    B_port(17,10)=a1;
	B_port(9,0)=0;
	B_port=B_port+(ap_int<18>)a0;

	P_port=((A_port+D_port)*B_port); 

	if((A0&~A1&~B0&~B1)|(B1&~A0&~A1&~B0)){
		if(flag_a1w0){
			reg=1;
		}else{
			reg=0;
		}
	} else if((A1&~A0&~B0&~B1)|(B0&~A0&~A1&~B1)|(A1==0&A0==0&B0==0&B1==0)){
		if(flag){
			reg=1;
		}else{
			reg=0;
		}
	}else if(A0&~A1&~B0&B1){
		if(flag_0){
			reg=1;
		}else{
			reg=0;
		}
	}else if(~A0&A1&B0&~B1){
		if(flag_1){
			reg=1;
		}else{
			reg=0;
		}
	}else if(~A0&A1&~B0&B1){
		if(flag_2){
			reg=1;
		}else{
			reg=0;
		}
	}
	*output_0=(ap_int<9>)P_port(8,0);
	*output_1=((ap_int<9>)P_port(38,30))+reg;

}



template <	unsigned Wbit,
			unsigned Ibit,
			unsigned Mbit,
			unsigned P>
ap_int<Mbit> DOT_DSP_unsigned(
	ap_uint<P*Wbit> weights,
	ap_uint<P*Ibit> in)
{
	ap_int<Mbit> accumulation = 0;

	for (unsigned p = 0; p < (P>>1); p++) {
#pragma HLS UNROLL

		ap_int<11> result;


		ap_uint<2*Wbit> temp_w = weights( (p+1)*2*Wbit-1, 2*p*Wbit );
		ap_uint<2*Ibit> temp_in = in( (p+1)*2*Ibit-1, 2*p*Ibit );
		ap_int<Wbit> temp_w0=temp_w( Wbit-1, 0 );
		ap_int<Wbit> temp_w1=temp_w( 2*Wbit-1, Wbit );
		ap_uint<Ibit> temp_in0= temp_in( Ibit-1, 0 );
		ap_uint<Ibit> temp_in1= temp_in( 2*Ibit-1, Ibit );
//		cout << "temp_w:" << temp_w << "  ";
//		cout << "temp_in:" << temp_in << endl;
		result = DSP_reuse_unsigned<Wbit,Ibit>(temp_w0,temp_w1,temp_in0,temp_in1);

		accumulation += result;
	}

	return accumulation;
}

template <	unsigned Wbit,
			unsigned Ibit,
			unsigned Mbit,
			unsigned P>
ap_int<Mbit> DOT_DSP_signed(
	ap_uint<P*Wbit> weights,
	ap_uint<P*Ibit> in)
{
	ap_int<Mbit> accumulation = 0;

	for (unsigned p = 0; p < (P>>1); p++) {
#pragma HLS UNROLL

		ap_int<11> result;


		ap_uint<2*Wbit> temp_w = weights( (p+1)*2*Wbit-1, 2*p*Wbit );
		ap_uint<2*Ibit> temp_in = in( (p+1)*2*Ibit-1, 2*p*Ibit );
		ap_int<Wbit> temp_w0=temp_w( Wbit-1, 0 );
		ap_int<Wbit> temp_w1=temp_w( 2*Wbit-1, Wbit );
		ap_int<Ibit> temp_in0= temp_in( Ibit-1, 0 );
		ap_int<Ibit> temp_in1= temp_in( 2*Ibit-1, Ibit );
		// cout << "temp_w0:" << temp_w0 << "  ";
		// cout << "temp_w1:" << temp_w1 << endl;
		// cout << "temp_in0:" << temp_in0 << "  ";
		// cout << "temp_in1:" << temp_in1 << endl;
		result = DSP_reuse_signed<Wbit,Ibit>(temp_w0,temp_w1,temp_in0,temp_in1);

		accumulation += result;
	}

	return accumulation;
}
/***************************/
/*
2021/03/10 閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷
*/
/***************************/
template <	unsigned Wbit,
			unsigned Ibit,
			unsigned Mbit,
			unsigned P>
ap_int<Mbit> DOT(
	ap_uint<P*Wbit> weights,
	ap_uint<P*Ibit> in)
{
	ap_int<Mbit> accumulation = 0;

	for (unsigned p = 0; p < P; p++) {
#pragma HLS UNROLL
		ap_int<Mbit> result;

		ap_int<Wbit> temp_w = weights( (p+1)*Wbit-1, p*Wbit );
		ap_int<Ibit+1> temp_in = in( (p+1)*Ibit-1, p*Ibit );
//		cout << "temp_w:" << temp_w << "  ";
//		cout << "temp_in:" << temp_in << endl;
		result = temp_w*temp_in;

		accumulation += result;
	}

	return accumulation;
}

/***************************/
/*
2021/03/20 閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷
//闂佸憡纰嶉崹鍧楀闯濞差亜绀冮柛娑卞瀺濠靛柈鐔煎灳閾忣偄浠撮梺鎸庣☉閻倿鏁撻懞銉ュ闁稿缍侀幆鍕⒐閹邦噮鍟嬪┑顔炬嚀閸婃悂骞忛敓锟?,闁哄鐗婇幐鎼佸矗閸℃鈻旈柛婵嗗閺併劎绱掑Δ瀣暠鐟滄澘娼″顐︽嚋椤掑﹥瀚归柨鐕傛嫹
*/
/***************************/

template <	unsigned Wbit,
			unsigned Ibit,
			unsigned Mbit,
			unsigned P>
ap_int<Mbit> DOT_SIGN(
	ap_uint<P*Wbit> weights,
	ap_uint<P*Ibit> in)
{
	ap_int<Mbit> accumulation = 0;

	for (unsigned p = 0; p < P; p++) {
#pragma HLS UNROLL
		ap_int<Mbit> result;

		ap_int<Wbit> temp_w = weights( (p+1)*Wbit-1, p*Wbit );
		ap_int<Ibit> temp_in = in( (p+1)*Ibit-1, p*Ibit );
		result = temp_w*temp_in;

		accumulation += result;
	}

	return accumulation;
}





/***************************/
/*
2021/03/20 閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷  闂佹寧绋戝鎭唗a_bit婵炴垶鎸鹃崕銈呂涢妶鍥╃當闁挎洍鍋撶憸鐗堝閹峰懐鎹勯妸锔芥闂佹寧鍐婚幏锟?
bn_res = (bn_res + (D >> 1)) >> (W_BIT - 1 + DATA_BIT + L_SHIFT);
*/
/***************************/
template <	unsigned IN_BIT,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned DATA_BIT,
			unsigned W_BIT,
			unsigned L_SHIFT
			>
ap_uint<3> BN_Relu6( ap_int<IN_BIT> in,
                ap_uint<INC_BIT> inc,
                ap_uint<BIAS_BIT> bias ) {
 //#pragma HLS inline off
	const unsigned D = 1 << (W_BIT - 1 + DATA_BIT + L_SHIFT);  //濠电偛顦崝宥夊礈閿燂拷
	const unsigned   Inter_BIT=IN_BIT+INC_BIT+1;
	ap_int<INC_BIT> inc_tmp=inc;
	ap_int<BIAS_BIT> bias_tmp=bias;
	ap_int<Inter_BIT> bn_res = in * inc_tmp + bias_tmp;
	ap_uint<3> res;
	ap_int<Inter_BIT+1> bn_res_tmp;
	if (bn_res > 0) {
		bn_res_tmp=(bn_res + (D >> 1)) ;
		bn_res_tmp = bn_res_tmp>> (W_BIT - 1 + DATA_BIT + L_SHIFT);   //闂佸憡鐗曢幖顐﹀春閿燂拷
		if (bn_res_tmp > 6){
			res = 6;
		} else {
			res = bn_res_tmp;
		}
	} else {
		res = 0;
	}
	return res;

}




template <	unsigned IN_BIT,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned DATA_BIT,
			unsigned W_BIT,
			unsigned L_SHIFT
			>
ap_uint<3> BN_Relu6_SIGN( ap_int<IN_BIT> in,
                ap_uint<INC_BIT> inc,
                ap_uint<BIAS_BIT> bias ) {
//#pragma HLS inline off
	const unsigned D = 1 << (W_BIT - 1 + DATA_BIT -1 + L_SHIFT);   //濠电偛顦崝宥夊礈閿燂拷
	const unsigned   Inter_BIT=IN_BIT+INC_BIT+1;
	ap_int<INC_BIT> inc_tmp=inc;
	ap_int<BIAS_BIT> bias_tmp=bias;
	ap_int<Inter_BIT> bn_res = in * inc_tmp + bias_tmp;
	ap_uint<3> res;
	ap_int<Inter_BIT+1> bn_res_tmp;
	if (bn_res > 0) {
		bn_res_tmp=(bn_res + (D >> 1)) ;
		bn_res_tmp = bn_res_tmp>> (W_BIT - 1 + DATA_BIT -1 + L_SHIFT);  //濠电偛顦崝宥夊礈閿燂拷
		if (bn_res_tmp > 6){
			res = 6;
		} else {
			res = bn_res_tmp;
		}
	} else {
		res = 0;
	}
	return res;

}

/***************************/
/*
2021/03/20 閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷  闁哄鐗婇幐鎼佸吹椤撶喓鈻旈柣鎴炆戠粻鎺旂磼濡ゅ鐣辩憸甯嫹
*/
/***************************/
//婵炲濮寸€电拺闂佹寧绋戦懟顖炲箚閸喐缍囬柟鎯у暱濮ｅ〃it闂傚倸瀚崝鏇㈠春濡ゅ懎违闁跨噦鎷?
template <	unsigned IN_BIT,
			unsigned OUT_BIT,
			unsigned INC_BIT,
			unsigned BIAS_BIT,

			unsigned DATA_BIT,
			unsigned W_BIT,
			unsigned L_SHIFT
			>
ap_int<OUT_BIT> Batch_Norm( ap_int<IN_BIT> in,
                ap_uint<INC_BIT> inc,
                ap_uint<BIAS_BIT> bias ) {
//#pragma HLS inline off
	const unsigned D = 1 << (W_BIT - 1 + DATA_BIT + L_SHIFT);
	const ap_uint<OUT_BIT-1> limit = (1 << (OUT_BIT-1))-1;
	const unsigned   Inter_BIT=IN_BIT+INC_BIT+1;
	ap_int<INC_BIT> inc_tmp=inc;
	ap_int<BIAS_BIT> bias_tmp=bias;
	ap_int<Inter_BIT> bn_res = in * inc_tmp + bias_tmp;
	ap_int<OUT_BIT> res;
	ap_int<Inter_BIT+1> bn_res_tmp;
	bn_res_tmp=(bn_res + (D >> 1)) ;
	bn_res_tmp = bn_res_tmp >> (W_BIT - 1 + DATA_BIT + L_SHIFT);
	if (bn_res_tmp > limit){
		res = limit;
	}
	else if(bn_res_tmp < -limit)
	{
		res = -limit;
	}
	else
	{
		res = bn_res_tmp;
	}

	return res;

}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned Cin,
			unsigned ChannelFold,
			unsigned InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DWURelu6(
	stream<ap_uint<MVTU*IM> >& vec,
	const ap_uint<INC_BIT> inc[MVTU][Cin/MVTU],
	const ap_uint<BIAS_BIT> bias[MVTU][Cin/MVTU],
	stream<ap_uint<InP*Abit> >& out,
	const unsigned reps = 1)
{

	const unsigned  CycleNum= InP/MVTU;
	const unsigned  MvtuFold= Cin/MVTU;
#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*CycleNum;
	ap_uint<MVTU*IM> tempVec;
	ap_int<IM> tempin;

	unsigned wMat = 0;
	unsigned wVec = 0;
	ap_uint<MVTU*Abit> outBuf_tmp;
	ap_uint<InP*Abit> outBuf;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*CycleNum max=NumVecs*CycleNum
#pragma HLS PIPELINE II= 1
		tempVec = vec.read();
		for (unsigned j = 0; j<MVTU; j++) { 
#pragma HLS UNROLL
			tempin=ap_int<IM>(tempVec((j+1)*IM-1, j*IM));

	//		cout <<(double)tempin <<endl;
//			outBuf_tmp=outBuf_tmp>>Abit;
			outBuf_tmp((j+1)*Abit-1,Abit*j) = BN_Relu6<IM,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(tempin, inc[j][wMat], bias[j][wMat]);

			//  cout <<(double)(outBuf_tmp(MVTU*Abit-1,MVTU*Abit-Abit)) <<endl;

//			tempVec=tempVec>>IM;
			
		}
		//  cout <<(double)outBuf_tmp<<endl;
		outBuf=outBuf>>(MVTU*Abit);	
		outBuf(InP*Abit-1,InP*Abit-MVTU*Abit)=outBuf_tmp;
		

		if(wVec==CycleNum-1){
			out.write(outBuf);
			//  cout <<(double)outBuf<<endl;
		}

		if(wMat==MvtuFold-1)
			wMat=0;
		else
			wMat++;
	
		if(wVec==CycleNum-1){
			wVec=0;
		}
		else{
			wVec++;
		}
	}
}
template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void URelu6_ReUse(
	stream<ap_uint<MVTU*IM> >& vec,
	const ap_uint<INC_BIT> inc[MVTU][MatrixW/MVTU],
	const ap_uint<BIAS_BIT> bias[MVTU][MatrixW/MVTU],
	stream<ap_uint<OutP*Abit> >& out,
	const unsigned reps = 1)
{
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;
	const unsigned  MvtuFold= MatrixW/MVTU;
#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*CycleNum;
	ap_uint<MVTU*IM> tempVec;
	ap_int<IM> tempin;

	unsigned wMat = 0;
	unsigned wVec = 0;
	unsigned wRec = 0;
	ap_uint<MVTU*Abit> outBuf_tmp;
	ap_uint<OutP*Abit> outBuf;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*CycleNum max=NumVecs*CycleNum
#pragma HLS PIPELINE II= 1
		tempVec = vec.read();
		for (unsigned j = 0; j<MVTU; j++) { 
#pragma HLS UNROLL
			tempin=ap_int<IM>(tempVec((j+1)*IM-1, j*IM));

	//		cout <<(double)tempin <<endl;
//			outBuf_tmp=outBuf_tmp>>Abit;
			outBuf_tmp((j+1)*Abit-1,Abit*j) = BN_Relu6<IM,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(tempin, inc[j][wMat], bias[j][wMat]);

			//  cout <<(double)(outBuf_tmp(MVTU*Abit-1,MVTU*Abit-Abit)) <<endl;

//			tempVec=tempVec>>IM;
			
		}
		//  cout <<(double)outBuf_tmp<<endl;
		outBuf=outBuf>>(MVTU*Abit);	
		outBuf(OutP*Abit-1,OutP*Abit-MVTU*Abit)=outBuf_tmp;
		

		if(wVec==CycleNum-1){
			out.write(outBuf);
			//  cout <<(double)outBuf<<endl;
		}

		if(wMat==MvtuFold-1)
			wMat=0;
		else
			wMat++;
	
		if(wVec==CycleNum-1){
			wVec=0;
		}
		else{
			wVec++;
		}
	}
}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void BN_ReUse(
	stream<ap_uint<MVTU*IM> >& vec,
	const ap_uint<INC_BIT> inc[MVTU][MatrixW/MVTU],
	const ap_uint<BIAS_BIT> bias[MVTU][MatrixW/MVTU],
	stream<ap_uint<OutP*Abit> >& out,
	const unsigned reps = 1)
{
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;
	const unsigned  MvtuFold= MatrixW/MVTU;
#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*CycleNum;
	ap_uint<MVTU*IM> tempVec;
	ap_int<IM> tempin;

	unsigned wMat = 0;
	unsigned wVec = 0;
	unsigned wRec = 0;
	ap_uint<MVTU*Abit> outBuf_tmp;
	ap_uint<OutP*Abit> outBuf;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*CycleNum max=NumVecs*CycleNum
#pragma HLS PIPELINE II= 1
		tempVec = vec.read();
		for (unsigned j = 0; j<MVTU; j++) { 
#pragma HLS UNROLL



			tempin=ap_int<IM>(tempVec((j+1)*IM-1, j*IM));
			outBuf_tmp((j+1)*Abit-1,Abit*j) = Batch_Norm<IM,Abit,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(tempin, inc[j][wMat], bias[j][wMat]);

			// cout <<(double)(outBuf_tmp(MVTU*Abit-1,MVTU*Abit-Abit)) <<endl;

	//		tempVec=tempVec>>IM;

		}
		outBuf=outBuf>>(MVTU*Abit);	
		outBuf(OutP*Abit-1,OutP*Abit-MVTU*Abit)=outBuf_tmp;
		

		if(wVec==CycleNum-1)
			out.write(outBuf);
			// cout <<(double)outBuf<<endl;

		if(wMat==MvtuFold-1)
			wMat=0;
		else
			wMat++;
	
		if(wVec==CycleNum-1){
			wVec=0;
		}
		else{
			wVec++;
		}
	}
}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Abit,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void Relu6_ReUse(
	stream<ap_uint<MVTU*IM> >& vec,
	const ap_uint<INC_BIT> inc[MVTU][MatrixW/MVTU],
	const ap_uint<BIAS_BIT> bias[MVTU][MatrixW/MVTU],
	stream<ap_uint<OutP*Abit> >& out,
	const unsigned reps = 1)
{
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;
	const unsigned  MvtuFold= MatrixW/MVTU;
#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*CycleNum;
	ap_uint<MVTU*IM> tempVec;
	ap_int<IM> tempin;

	unsigned wMat = 0;
	unsigned wVec = 0;
	unsigned wRec = 0;
	ap_uint<MVTU*Abit> outBuf_tmp;
	ap_uint<OutP*Abit> outBuf;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*CycleNum max=NumVecs*CycleNum
#pragma HLS PIPELINE II= 1
		tempVec = vec.read();
		for (unsigned j = 0; j<MVTU; j++) { 
#pragma HLS UNROLL
			tempin=ap_int<IM>(tempVec((j+1)*IM-1, j*IM));

	//		cout <<(double)tempin <<endl;
//			outBuf_tmp=outBuf_tmp>>Abit;
			outBuf_tmp((j+1)*Abit-1,Abit*j) = BN_Relu6_SIGN<IM,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(tempin, inc[j][wMat], bias[j][wMat]);

			//  cout <<(double)(outBuf_tmp(MVTU*Abit-1,MVTU*Abit-Abit)) <<endl;

//			tempVec=tempVec>>IM;
			
		}
		//  cout <<(double)outBuf_tmp<<endl;
		outBuf=outBuf>>(MVTU*Abit);	
		outBuf(OutP*Abit-1,OutP*Abit-MVTU*Abit)=outBuf_tmp;
		

		if(wVec==CycleNum-1){
			out.write(outBuf);
			//  cout <<(double)outBuf<<endl;
		}

		if(wMat==MvtuFold-1)
			wMat=0;
		else
			wMat++;
	
		if(wVec==CycleNum-1){
			wVec=0;
		}
		else{
			wVec++;
		}
	}
}






/***************************/
/*闂佸憡鍨兼慨銈夊汲閻旂厧瑙︾€广儱绻掔粣妗猄C_MVAU_KP_v1

闂佸吋鐪归崕鐗堢箾閸ヮ剚鍋ㄩ柕濠庣厛閸ゅ鏌涢幋锝呅撻柡鍡欏枛閺佸秶浠﹂崐鐔风彲闁荤偞绋戦懟顖溾偓鐟扮－閹峰宕ｉ妷銊ь槹InP闂佹寧绋戦懟顖炴偪閸曨垱鈷旈柛娑卞墲缁€濠璶P*K*K闂佹寧绋戦悧濠囧极椤曗偓楠炴劖绋夐　纾?<=Cin闂佹寧鍐婚幏锟?

婵烇絽娴傞崰妤咁敆濞ｇ寛U FIFO闂佸憡鐟﹂敋闁活煈鍓熷畷銉︽償閳ュ磭鍩嶉梻浣规緲缁夎泛鈻撻幋鐘冲闁硅鍔﹂崥鈧?
闁哄鐗婇幐鎼佸矗閸℃稒鏅繛鍡欐櫒P*Ibit*K*K
闂佸搫顦崯鎾闯閹间礁绠洪柛鈩兩戦弶褰掓煥濞戞瑥鍝烘繛鍫暜np闂佹眹鍔岀€氼剟濡甸崶鈺€鐒婇煫鍥ㄧ啲閹烽浠﹂挊澶岀М ap_uint<InP*K*K*Wbit> weights[Cin/InP]

2021/03/15 婵烇絽娴傞崰妤咁敆閿燂拷
*/
/***************************/
template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned K,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Cin,
			unsigned ChannelFold,
			unsigned InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DSC_MVAU_KKP_1(
	stream<ap_uint<InP*Ibit*K*K> >& vec,
	const ap_uint<InP*K*K*Wbit> weights[Cin/InP],
	stream<ap_uint<MVTU*IM> >& out,
	const unsigned reps)
{

	const unsigned long long totalReps = reps*NumVecs;
	ap_uint<InP*Ibit*K*K> tempCin;


//	闂佸憡甯掑Λ妤€顭囬敓锟?
	ap_uint<IM*InP> dot_result_Vec;
	ap_uint<IM*InP> dot_result_Vec_tmp;
	ap_int<IM> result;
	ap_uint<InP*K*K*Wbit> tempMat ;

	unsigned wMat = 0;
	for (unsigned long long rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs max=NumVecs
#pragma HLS PIPELINE II=1

			tempCin = vec.read();
			// cout <<"resultVec   " <<(double)tempCin <<" \n ";
			tempMat = weights[wMat];
			// cout <<"resultVec   " <<double(tempMat)<<" \n ";

		for(unsigned out_loop = 0;out_loop < K*K;out_loop++){
			for(unsigned in_loop = 0;in_loop < InP;in_loop++){
	#pragma HLS UNROLL
				ap_int<Wbit> temp_w = tempMat( out_loop*InP*Wbit+(in_loop+1)*Wbit-1, in_loop*Wbit+out_loop*InP*Wbit );
				ap_int<Ibit+1> temp_in = tempCin( out_loop*InP*Ibit+(in_loop+1)*Ibit-1, in_loop*Ibit+out_loop*InP*Ibit );
				ap_int<IM> acc=temp_w * temp_in;
				if(out_loop == 0)
					dot_result_Vec((in_loop+1)*IM-1,in_loop*IM) = acc;
				else{
					result=ap_int<IM>(dot_result_Vec((in_loop+1)*IM-1,in_loop*IM))+acc;
					dot_result_Vec((in_loop+1)*IM-1,in_loop*IM) = result;
				}
			}
		}

		if(rep >=  1 ){
			out.write(dot_result_Vec_tmp);

		// test-begin
		// for (unsigned i=0;i<MVTU;i++){
		// 	ap_int<IM> test=resultVec_tmp1(IM-1,0);
		// 	cout <<(double)test <<"   ";
		// 	resultVec_tmp1=resultVec_tmp1>>IM;
		// 	if (i==MVTU-1){
		// 		cout <<" \n";
		// 	}
		// }
		// test-end
	}

	dot_result_Vec_tmp=dot_result_Vec;

	if(wMat==ChannelFold-1)
		wMat=0;
	else
		wMat++;			
	}
	out.write(dot_result_Vec_tmp);
}



template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned K,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned Cin,
			unsigned ChannelFold,
			unsigned InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DSC_MVAU_P_2(
	stream<ap_uint<InP*Ibit*K*K> >& vec,
	const ap_uint<InP*K*K*Wbit> weights[Cin/InP],
	stream<ap_uint<MVTU*IM> >& out,
	const unsigned reps)
{

	const unsigned long long totalReps = reps*NumVecs*K*K;
	const unsigned  CycleNum= InP/MVTU;
	ap_uint<InP*Ibit*K*K> tempCin;


//	闂佸憡甯掑Λ妤€顭囬敓锟?
	ap_uint<IM*InP> dot_result_Vec;
	ap_uint<IM*InP> dot_result_Vec_tmp;
	ap_uint<MVTU*IM> resultVec_tmp1;
	ap_int<IM> result;
	ap_uint<InP*K*K*Wbit> tempMat ;

	unsigned wVec = 0;
	unsigned wMat = 0;
	for (unsigned long long rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs*K*K max=NumVecs*K*K
#pragma HLS PIPELINE II=1
		if (wVec== 0  ) {
			tempCin = vec.read();
			// cout <<"resultVec   " <<(double)tempCin <<" \n ";
			tempMat = weights[wMat];
			// cout <<"resultVec   " <<double(tempMat)<<" \n ";
		}

		for(unsigned in_loop = 0;in_loop < InP;in_loop++){
#pragma HLS UNROLL
			// ap_int<Wbit> temp_w = tempMat( wVec*InP*Wbit+(in_loop+1)*Wbit-1, in_loop*Wbit+wVec*InP*Wbit );
			// ap_int<Ibit+1> temp_in = tempCin( wVec*InP*Ibit+(in_loop+1)*Ibit-1, in_loop*Ibit+wVec*InP*Ibit );
			ap_int<Wbit> temp_w = tempMat( (in_loop+1)*Wbit-1,  in_loop*Wbit );
			ap_int<Ibit+1> temp_in = tempCin( (in_loop+1)*Ibit-1, in_loop*Ibit );
			ap_int<IM> acc=temp_w * temp_in;
			if(wVec == 0)
				dot_result_Vec((in_loop+1)*IM-1,in_loop*IM) = acc;
			else{
				result=ap_int<IM>(dot_result_Vec((in_loop+1)*IM-1,in_loop*IM))+acc;
				dot_result_Vec((in_loop+1)*IM-1,in_loop*IM) = result;
			}
		}
		tempMat=tempMat>>(InP*Wbit);
		tempCin=tempCin>>(InP*Ibit);

	if(rep >=  K*K & wVec<CycleNum){
		resultVec_tmp1=dot_result_Vec_tmp(MVTU*IM-1,0 );
		out.write(resultVec_tmp1);

		// test-begin
		// for (unsigned i=0;i<MVTU;i++){
		// 	ap_int<IM> test=resultVec_tmp1(IM-1,0);
		// 	cout <<(double)test <<"   ";
		// 	resultVec_tmp1=resultVec_tmp1>>IM;
		// 	if (i==MVTU-1){
		// 		cout <<" \n";
		// 	}
		// }
		// test-end

		dot_result_Vec_tmp=dot_result_Vec_tmp>>(MVTU*IM);
	}
		if (wVec == K*K-1) {
			dot_result_Vec_tmp=dot_result_Vec;
		}

		if (wVec == K*K-1) {
			wVec=0;
			if(wMat==ChannelFold-1)
				wMat=0;
			else
				wMat++;			
		}
		else
			wVec++;

	}
	for (unsigned i = 0; i < CycleNum; i++) {  //2
#pragma HLS PIPELINE II= 1
	resultVec_tmp1=dot_result_Vec_tmp(MVTU*IM-1,0 );
	out.write(resultVec_tmp1);
	dot_result_Vec_tmp=dot_result_Vec_tmp>>(MVTU*IM);
				// cout <<"resultVec   " <<(double)resultVec_tmp[i] <<" \n ";
	}

}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned K,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned Cin,
			unsigned InP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void DSC_MVAU_KP_DSP(
	stream<ap_uint<InP*Ibit*K*K> >& vec,
	const ap_uint<InP*K*K*Wbit> weights[Cin/InP],
	const ap_int<INC_BIT> inc[Cin],
	const ap_int<BIAS_BIT> bias[Cin],
	stream<ap_uint<InP*Abit> >& out,
	const unsigned reps)
{
	const unsigned long long ChannelFold = Cin/InP;
	const unsigned long long totalReps = reps*NumVecs;

	ap_uint<InP*Ibit*K*K> tempCin;

//	闂佸憡甯掑Λ妤€顭囬敓锟?
	ap_int<Mbit> dot_result_Vec[InP];
#pragma HLS ARRAY_PARTITION variable=dot_result_Vec complete dim=0
	ap_uint<InP*K*K*Wbit> tempMat ;
	ap_uint<InP*Abit> outBuf;
	ap_int<9> output_0;
	ap_int<9> output_1;
	for (unsigned rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs max=NumVecs
#pragma HLS PIPELINE II=Cin/Inp
		for (unsigned loop = 0; loop < ChannelFold; loop++) {
			tempCin = vec.read();
			tempMat = weights[loop];
			for(unsigned out_loop = 0;out_loop < K*K;out_loop++){
				for(unsigned in_loop = 0;in_loop < InP;in_loop=in_loop+2){
#pragma HLS UNROLL
					// ap_int<Wbit> temp_w = tempMat( out_loop*InP*Wbit+(in_loop+1)*Wbit-1, in_loop*Wbit+out_loop*InP*Wbit );
					// ap_int<Ibit+1> temp_in = tempCin( out_loop*InP*Ibit+(in_loop+1)*Ibit-1, in_loop*Ibit+out_loop*InP*Ibit );
					ap_int<2*Wbit> temp_w = tempMat( out_loop*InP*Wbit+(in_loop+2)*Wbit-1, in_loop*Wbit+out_loop*InP*Wbit );
					ap_uint<2*Ibit> temp_in = tempCin( out_loop*InP*Ibit+(in_loop+2)*Ibit-1, in_loop*Ibit+out_loop*InP*Ibit );
					ap_int<Wbit> temp_w0=temp_w( Wbit-1, 0 );
					ap_int<Wbit> temp_w1=temp_w( 2*Wbit-1, Wbit );
					ap_uint<Ibit> temp_in0= temp_in( Ibit-1, 0 );
					ap_uint<Ibit> temp_in1= temp_in( 2*Ibit-1, Ibit );
					DW_reuse_unsigned<Wbit,Ibit>( temp_w0, temp_w1, temp_in0, temp_in1, &output_0,&output_1 );
					if(out_loop == 0){
						dot_result_Vec[in_loop] = output_0;
						dot_result_Vec[in_loop+1] = output_1;
					}
					else{
						dot_result_Vec[in_loop] += output_0;
						dot_result_Vec[in_loop+1] += output_1;
					}
					outBuf(Abit*in_loop+Abit-1,Abit*in_loop)= BN_Relu6<Mbit,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(dot_result_Vec[in_loop], inc[loop*InP+in_loop], bias[loop*InP+in_loop]);
					outBuf(Abit*in_loop+Abit+Abit-1,Abit*in_loop+Abit)= BN_Relu6<Mbit,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(dot_result_Vec[in_loop+1], inc[loop*InP+in_loop+1], bias[loop*InP+in_loop+1]);
				}
			}
			out.write(outBuf);
		}
		//	flag=flag+1;
	}
//	cout << "flag:" << flag << endl;
}




/***************************/
/*闂佸憡鍨兼慨銈夊汲閻旂厧瑙︾€广儱绻掔粣妗礦AU_inbuf_BN
//1闁艰揪鎷?1闂佸憡顨婇ˉ鎾光叾婵炴垶鎼╅崢楣冨极閵堝绀嗛柟娈垮枟閻ｉ亶鏌ｉ娑欐珚婵☆垪鍋撻梺鍛婄閸ㄥ潡宕抽悜妯尖枙婵☆垵宕甸妴濠囨煕濡や焦绀€闁告ɑ鎹囬弫宥囦沪閼规壆顦伴柡澶婄墛閹告悂宕ｉ崱娑欑厒鐎广儱鎳庨悞娲煟閵忊晝鍘滅紒杈ㄧ箘閺侇噣銆冮埈鐔兼煥濞戞ɑ婀版俊顐elu6闂侀潧妫撮幏锟?(闁哄鐗婇幐鎼佸吹椤撱垹瀚夐柛顐ょit闂佹眹鍔岀€氭澘顬婂ú顏勭闁跨噦鎷?)
濠电姷顣幏閿嬬箾閼奸鍤欓柛銈変憾瀵偅锛愭担鐣岊槹+-X 婵炴挻鑹鹃鍛淬€呴敓锟?4bit 濠殿喗绻愮紞鈧紒鈧敓锟?8
2021/03/20閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷
*/
/***************************/

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned MatrixH,
			unsigned MatrixW,
			unsigned InP,
			unsigned OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void MVAU_inbuf_BN(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	const ap_int<INC_BIT> inc[OutP][MatrixW/OutP],
	const ap_int<BIAS_BIT> bias[OutP][MatrixW/OutP],
	stream<ap_uint<OutP*Abit> >& out,
	const unsigned reps = 1)
{
	const unsigned long long InputFold = MatrixH/InP;
	const unsigned long long OutputFold = MatrixW/OutP;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold*OutputFold;

	ap_uint<InP*Ibit> rowstore[InputFold];
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM

	ap_int<Mbit> resultVec[OutP];
#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=0
	unsigned wVec = 0;
	unsigned wMat = 0;
	ap_uint<InP*Ibit> tempVec;
	ap_uint<OutP*Abit> outBuf;

	unsigned index = 0;
	for (unsigned rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs*MatrixH*MatrixW/InP/OutP max=NumVecs*MatrixH*MatrixW/InP/OutP
#pragma HLS PIPELINE II=1

		if (wMat == 0) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;
		for (unsigned p = 0; p < OutP; p++) {
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];

			ap_int<Mbit> acc = DOT<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );

			if (wVec == 0)
				resultVec[p] = acc;
			else
				resultVec[p] += acc;

			outBuf( (p+1)*Abit-1, p*Abit ) = Batch_Norm<Mbit,Abit,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(resultVec[p], inc[p][wMat], bias[p][wMat]);
		}

		if (wVec == InputFold-1){
			out.write(outBuf);
		}

		if (wVec == InputFold-1) {
			wVec = 0;
			if (wMat == OutputFold-1)
				wMat = 0;
			else
				wMat++;
		}
		else
			wVec++;
	}
}




/***************************/
/*闂佸憡鍨兼慨銈夊汲閻旂厧瑙︾€广儱绻掔粣妗礦AU_outbuf_BN
//1闁艰揪鎷?1闂佸憡顨婇ˉ鎾光叾婵炴垶鎼╅崢楣冨极閵堝绀嗛柟娈垮枟閻ｉ亶鏌ｉ娑欐珚婵☆垪鍋撻梺鍛婄閸ㄥ潡宕抽悜妯尖枙婵☆垵宕甸妴濠囨煕濡や焦绀€闁告ɑ鎹囬弫宥囦沪閼规壆顦伴柡澶婄墛閹告悂宕ｉ崱娑欑厒鐎广儱鎳庨悞娲煟閵忊晝鍘滅紒杈ㄧ箘閺侇噣銆冮埈鐔兼煥濞戞ɑ婀版俊顐elu6闂侀潧妫撮幏锟?(闁哄鐗婇幐鎼佸吹椤撱垹瀚夐柛顐ょit闂佹眹鍔岀€氭澘顬婂ú顏勭闁跨噦鎷?)
濠电姷顣幏閿嬬箾閼奸鍤欓柛銈変憾瀵偅锛愭担鐣岊槹+-X 婵炴挻鑹鹃鍛淬€呴敓锟?4bit 濠殿喗绻愮紞鈧紒鈧敓锟?8
2021/03/20閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷
*/
/***************************/
template <	unsigned NumVecs,   //Dout*Dout
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned MatrixH, //Cin*K*K
			unsigned MatrixW,  //Cout
			unsigned InP,  //闂佸憡鐟崹浼村Φ閸ャ劎顩茬€广儺妲抧
			unsigned OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void MVAU_outbuf_BN(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	const ap_int<INC_BIT> inc[OutP][MatrixW/OutP],
	const ap_int<BIAS_BIT> bias[OutP][MatrixW/OutP],
	stream<ap_uint<OutP*Abit> >& out,
	const unsigned reps = 1)
{
	//濠电偛顦崝宥夊礈娴煎瓨鏅慨妯虹－缁犲綊鏌涜椤ㄥ牓寮ˇ鎶﹏g long闂佹眹鍔岀€氼剛鏁锕€鐐婇柣妯挎珪绗慛umVecs闁荤偞绋忛崝搴ㄥΦ濮樿埖鍎嶉柛鏇ㄥ枤缂堝鏌涢幋婵囨儓鐟滅増鐓￠幊妤呭箣閹烘梻鐛ラ悗鍨緲閻楀棝濡甸敓锟?
	const unsigned long long InputFold = MatrixH/InP;
	const unsigned long long OutputFold = MatrixW/OutP;

	const unsigned long long totalReps = reps*NumVecs*InputFold*OutputFold;
/* solution1_FF婵炶揪缍€濞夋洟寮妶鍡樹氦婵炲棗閰ｉ崵瀣磼閹惧懘妾繛纭锋嫹*/
	ap_int<Mbit> resultVec[OutP][OutputFold];
	#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=0
/* solution2_BRAM婵炶揪缍€濞夋洟寮妶鍡樹氦婵炲棗閰ｉ崵瀣磼閹惧懘妾繛纭锋嫹
 * 闁哄鏅滈悷锕傛偋闁秵鍋ㄩ柕濞垮€楅悷婵嗩熆妫版繀绨绘い鏇ㄥ櫍閺佸秶浠﹂挊澶嬫灳闂佹悶鍎虫慨鍓ф崲閺囥垹鍙婃い鏍ㄦ皑閸欐垿鏌ら崘娴嬪亾閻旀粍甯￠弻灞剧節閸愵亶浼曞┑掳鍊ч幏锟?
 * */
//	ap_int<Mbit> resultVec[OutP][OutputFold];
//#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=1
//#pragma HLS RESOURCE variable resultVec core=RAM_2P
	unsigned wVec = 0;
	unsigned wMat = 0;
	ap_uint<InP*Ibit> tempVec;
	ap_uint<OutP*Abit> outBuf;

	unsigned index = 0;
	for (unsigned rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs*MatrixH*MatrixW/InP/OutP max=NumVecs*MatrixH*MatrixW/InP/OutP
	#pragma HLS PIPELINE II=1

			if (wMat == 0) {
				tempVec = vec.read();
			}

			index = wVec*OutputFold+wMat;
			for (unsigned p = 0; p < OutP; p++) {  //闁哄鐗婇幐鎼佸吹椤撱垻宓侀柟顖炲亰閺€浠嬪箹鏉堝墽绱扮紒妤€鍊绘禒锕傚即閻愮數锛?
	#pragma HLS UNROLL
				ap_uint<InP*Wbit> tempMat = weights[p][index];

				ap_int<Mbit> acc = DOT<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );
//婵炴垶鎸哥粔鐑藉礂濡粯濯撮悹鎭掑妽閺嗗粓RAM闂佹眹鍔岀€氼剛鏁锕€鐐婇柣妯挎珪绗戦梺鎼炲劤婵磭鎷归悢鍛婁氦婵炲棙鎸稿▍锟?
				if (wVec == 0)
					resultVec[p][wMat] = acc;
				else
					resultVec[p][wMat] += acc;

				outBuf( (p+1)*Abit-1, p*Abit ) = Batch_Norm<Mbit,Abit,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(resultVec[p], inc[p][wMat], bias[p][wMat]);
			}

			if (wVec == InputFold-1){   //wVec=inputFold-1闂佸搫瀚鍢檃t婵炲濯撮幏锟?0闂佸憡甯炲绨刾utFold-1濠电偛鏋撮妶鍥╃倳闂佸憡鍨煎▍锝囧垝閵娾晛鍑犻柨鐕傛嫹
				out.write(outBuf);
			}

			if (wMat == OutputFold-1) {
				wMat = 0;
				if (wVec == InputFold-1)
					wVec = 0;
				else
					wVec++;
			}
			else
				wMat++;
		}
}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP>
void MVAU_inbuf_SIGN(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	stream<ap_uint<MVTU*IM> >& out,
	const unsigned reps = 1){
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold;


	ap_uint<InP*Ibit> rowstore[InputFold];
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
	ap_int<IM> result;
	ap_uint<IM*OutP> resultVec;
	ap_uint<IM*OutP> resultVec_tmp;

	ap_uint<MVTU*IM> resultVec_tmp1;

	unsigned wVec = 0;
	unsigned wMat = 0;
	unsigned index = 0;
	unsigned flag = 0;
	ap_uint<InP*Ibit> tempVec;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*InputFold max=NumVecs*InputFold
#pragma HLS PIPELINE II= 1
		if (wMat== 0  ) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;

		for (unsigned p = 0; p < OutP; p++) {  //2
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];

			ap_int<IM> acc = DOT_SIGN<Wbit, Ibit, IM, InP>( tempMat, tempVec );
			if (wVec == 0)
				resultVec((p+1)*IM-1,p*IM) = acc;
			else{
				result=ap_int<IM>(resultVec((p+1)*IM-1,p*IM))+acc;
				resultVec((p+1)*IM-1,p*IM) = result;
			}
		}


		if(rep >= InputFold & wVec<CycleNum){
				resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
				out.write(resultVec_tmp1);
				//test-begin
				// for (unsigned i=0;i<MVTU;i++){
				// 	ap_int<IM> test=resultVec_tmp1(IM-1,0);
				// 	cout <<(double)test <<"   ";
				// 	resultVec_tmp1=resultVec_tmp1>>IM;
				// 	if (i==MVTU-1){
				// 		cout <<" \n";
				// 	}
				// }
				//test-end
				resultVec_tmp=resultVec_tmp>>(MVTU*IM);
				
		}

		if(wVec ==InputFold-1)
			resultVec_tmp=resultVec;

		if (wVec == InputFold-1) {
			wVec=0;
			if(wMat==OutputFold-1)
				wMat=0;
			else
				wMat++;
		}
		else
				wVec++;
	}

	for (unsigned i = 0; i < CycleNum; i++) {  //2
#pragma HLS PIPELINE II= 1
		resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
		out.write(resultVec_tmp1);
		resultVec_tmp=resultVec_tmp>>(MVTU*IM);
					// cout <<"resultVec   " <<(double)resultVec_tmp[i] <<" \n ";
		}
}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP>
void MVAU_inbuf_SIGN_DSP(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	stream<ap_uint<MVTU*IM> >& out,
	const unsigned reps = 1){
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold;


	ap_uint<InP*Ibit> rowstore[InputFold];
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
	ap_int<IM> result;
	ap_uint<IM*OutP> resultVec;
	ap_uint<IM*OutP> resultVec_tmp;

	ap_uint<MVTU*IM> resultVec_tmp1;

	unsigned wVec = 0;
	unsigned wMat = 0;
	unsigned index = 0;
	unsigned flag = 0;
	ap_uint<InP*Ibit> tempVec;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*InputFold max=NumVecs*InputFold
#pragma HLS PIPELINE II= 1
		if (wMat== 0  ) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;

		for (unsigned p = 0; p < OutP; p++) {  //2
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];
//			ap_int<Mbit> acc = DOT<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );
			ap_int<IM> acc = DOT_DSP_signed<Wbit, Ibit, IM, InP>( tempMat, tempVec );
			if (wVec == 0)
				resultVec((p+1)*IM-1,p*IM) = acc;
			else{
				result=ap_int<IM>(resultVec((p+1)*IM-1,p*IM))+acc;
				resultVec((p+1)*IM-1,p*IM) = result;
			}
		}


		if(rep >= InputFold & wVec<CycleNum){
				resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
				out.write(resultVec_tmp1);
				//test-begin
				// for (unsigned i=0;i<MVTU;i++){
				// 	ap_int<IM> test=resultVec_tmp1(IM-1,0);
				// 	cout <<(double)test <<"   ";
				// 	resultVec_tmp1=resultVec_tmp1>>IM;
				// 	if (i==MVTU-1){
				// 		cout <<" \n";
				// 	}
				// }
				//test-end
				resultVec_tmp=resultVec_tmp>>(MVTU*IM);
				
		}

		if(wVec ==InputFold-1)
			resultVec_tmp=resultVec;

		if (wVec == InputFold-1) {
			wVec=0;
			if(wMat==OutputFold-1)
				wMat=0;
			else
				wMat++;
		}
		else
				wVec++;
	}

	for (unsigned i = 0; i < CycleNum; i++) {  //2
#pragma HLS PIPELINE II= 1
		resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
		out.write(resultVec_tmp1);
		resultVec_tmp=resultVec_tmp>>(MVTU*IM);
					// cout <<"resultVec   " <<(double)resultVec_tmp[i] <<" \n ";
		}
}













/***************************/
/*
2021/03/20 閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷
//闂佸搫鐗為幏鐑芥煕濮橆剚鎹ｆ繛鍫嫹1闁艰揪鎷?1闂佸憡顨婇ˉ鎾光叾婵炴垶鎼╅崢楣冨极閵堝绀嗛柟娈垮枟閻ｉ亶鏌ｉ娑欐珚婵☆垪鍋撻梺鍛婄閸ㄥ潡宕抽悜妯尖枙婵☆垵宕甸妴濠囨煕濡や焦绀€闁告ɑ鎹囬弫宥囦沪閻愵剨楠廈N闂佹寧绋戦張顒€螞閵堝應鏀介柨鐔绘鐓ら柨鐕傛嫹
*/
/***************************/

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned MatrixH,
			unsigned MatrixW,
			unsigned InP,
			unsigned OutP>
void MVU_rowfirst(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	stream<ap_uint<OutP*Mbit> >& out,
	const unsigned reps = 1)
{
	const unsigned long long InputFold = MatrixH/InP;
	const unsigned long long OutputFold = MatrixW/OutP;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold*OutputFold;

	ap_uint<InP*Ibit> rowstore[InputFold];
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
	ap_int<Mbit> resultVec[OutP];
#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=0
	unsigned wVec = 0;
	unsigned wMat = 0;
	ap_uint<InP*Ibit> tempVec;
	ap_uint<OutP*Mbit> outBuf;

	unsigned index = 0;
	for (unsigned rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs*MatrixH*MatrixW/InP/OutP max=NumVecs*MatrixH*MatrixW/InP/OutP
#pragma HLS PIPELINE II=1

		if (wMat == 0) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;
		for (unsigned p = 0; p < OutP; p++) {
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];

			ap_int<Mbit> acc = DOT<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );

			if (wVec == 0)
				resultVec[p] = acc;
			else
				resultVec[p] += acc;

			outBuf((p+1)*Mbit-1, p*Mbit) = resultVec[p];
		}

		if (wVec == InputFold-1)
			out.write(outBuf);

		if (wVec == InputFold-1) {
			wVec = 0;
			if (wMat == OutputFold-1)
				wMat = 0;
			else
				wMat++;
		}
		else
			wVec++;
	}
}


template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned MatrixH,
			unsigned MatrixW,
			unsigned InP,
			unsigned OutP>
void MVU_rowfirst_DSP(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	stream<ap_uint<OutP*Mbit> >& out,
	const unsigned reps = 1)
{
	const unsigned long long InputFold = MatrixH/InP;
	const unsigned long long OutputFold = MatrixW/OutP;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold*OutputFold;

	ap_uint<InP*Ibit> rowstore[InputFold];
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
	ap_int<Mbit> resultVec[OutP];
#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=0
	unsigned wVec = 0;
	unsigned wMat = 0;
	ap_uint<InP*Ibit> tempVec;
	ap_uint<OutP*Mbit> outBuf;

	unsigned index = 0;
	for (unsigned long long rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs*MatrixH*MatrixW/InP/OutP max=NumVecs*MatrixH*MatrixW/InP/OutP
#pragma HLS PIPELINE II=1

		if (wMat == 0) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;
		for (unsigned p = 0; p < OutP; p++) {
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];

			ap_int<Mbit> acc = DOT_DSP_unsigned<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );

			if (wVec == 0)
				resultVec[p] = acc;
			else
				resultVec[p] += acc;

			outBuf((p+1)*Mbit-1, p*Mbit) = resultVec[p];
		}

		if (wVec == InputFold-1)
			out.write(outBuf);

		if (wVec == InputFold-1) {
			wVec = 0;
			if (wMat == OutputFold-1)
				wMat = 0;
			else
				wMat++;
		}
		else
			wVec++;
	}
}







template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP>
void MVAU_inbuf_v2(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	stream<ap_uint<MVTU*IM> >& out,
	const unsigned reps = 1){
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold;


	ap_uint<InP*Ibit> rowstore[InputFold];
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
	ap_int<IM> result;
	ap_uint<IM*OutP> resultVec;
	ap_uint<IM*OutP> resultVec_tmp;

	ap_uint<MVTU*IM> resultVec_tmp1;

	unsigned wVec = 0;
	unsigned wMat = 0;
	unsigned index = 0;
	unsigned flag = 0;
	ap_uint<InP*Ibit> tempVec;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*InputFold max=NumVecs*InputFold
#pragma HLS PIPELINE II= 1
		if (wMat== 0  ) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;

		for (unsigned p = 0; p < OutP; p++) {  //2
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];
			ap_int<IM> acc = DOT<Wbit, Ibit, IM, InP>( tempMat, tempVec );
			// ap_int<IM> acc = DOT_DSP_signed<Wbit, Ibit, IM, InP>( tempMat, tempVec );
			if (wVec == 0)
				resultVec((p+1)*IM-1,p*IM) = acc;
			else{
				result=ap_int<IM>(resultVec((p+1)*IM-1,p*IM))+acc;
				resultVec((p+1)*IM-1,p*IM) = result;
			}
		}


		if(rep >= InputFold & wVec<CycleNum){
				resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
				out.write(resultVec_tmp1);
				//test-begin
				// for (unsigned i=0;i<MVTU;i++){
				// 	ap_int<IM> test=resultVec_tmp1(IM-1,0);
				// 	cout <<(double)test <<"   ";
				// 	resultVec_tmp1=resultVec_tmp1>>IM;
				// 	if (i==MVTU-1){
				// 		cout <<" \n";
				// 	}
				// }
				//test-end
				resultVec_tmp=resultVec_tmp>>(MVTU*IM);
				
		}

		if(wVec ==InputFold-1)
			resultVec_tmp=resultVec;

		if (wVec == InputFold-1) {
			wVec=0;
			if(wMat==OutputFold-1)
				wMat=0;
			else
				wMat++;
		}
		else
				wVec++;
	}

	for (unsigned i = 0; i < CycleNum; i++) {  //2
#pragma HLS PIPELINE II= 1
		resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
		out.write(resultVec_tmp1);
		resultVec_tmp=resultVec_tmp>>(MVTU*IM);
					// cout <<"resultVec   " <<(double)resultVec_tmp[i] <<" \n ";
		}
}

template <	unsigned NumVecs,
			unsigned Ibit,
			unsigned Wbit,
			unsigned IM,
			unsigned MVTU,
			unsigned MatrixH,
			unsigned InputFold,
			unsigned MatrixW,
			unsigned OutputFold,
			unsigned InP,
			unsigned OutP>
void MVAU_inbuf_USIGN_DSP(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	stream<ap_uint<MVTU*IM> >& out,
	const unsigned reps = 1){
//	const unsigned InputFold = MatrixH/InP;
//	const unsigned OutputFold = MatrixW/OutP;
	const unsigned  CycleNum= OutP/MVTU;

#ifdef MVU_DEBUG
	cout << "InputFold: " << InputFold << endl;
	cout << "OutputFold: " << OutputFold << endl;
#endif

	const unsigned long long totalReps = reps*NumVecs*InputFold;


	ap_uint<InP*Ibit> rowstore[InputFold];
//#pragma HLS RESOURCE variable=rowstore core=RAM_2P_BRAM
#pragma HLS BIND_STORAGE variable=rowstore type=ram_2p
	ap_int<IM> result;
	ap_uint<IM*OutP> resultVec;
	ap_uint<IM*OutP> resultVec_tmp;

	ap_uint<MVTU*IM> resultVec_tmp1;

	unsigned wVec = 0;
	unsigned wMat = 0;
	unsigned index = 0;
	unsigned flag = 0;
	ap_uint<InP*Ibit> tempVec;

	for (unsigned long long rep = 0; rep <totalReps; rep++) {  //50*50
#pragma HLS loop_tripcount min=NumVecs*InputFold max=NumVecs*InputFold
#pragma HLS PIPELINE II= 1
		if (wMat== 0  ) {
			tempVec = vec.read();
			rowstore[wVec] = tempVec;
		}
		else {
			tempVec = rowstore[wVec];
		}

		index = wVec*OutputFold+wMat;

		for (unsigned p = 0; p < OutP; p++) {  //2
#pragma HLS UNROLL
			ap_uint<InP*Wbit> tempMat = weights[p][index];
//			ap_int<Mbit> acc = DOT<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );
			ap_int<IM> acc = DOT_DSP_unsigned<Wbit, Ibit, IM, InP>( tempMat, tempVec );
			if (wVec == 0)
				resultVec((p+1)*IM-1,p*IM) = acc;
			else{
				result=ap_int<IM>(resultVec((p+1)*IM-1,p*IM))+acc;
				resultVec((p+1)*IM-1,p*IM) = result;
			}
		}


		if(rep >= InputFold & wVec<CycleNum){
				resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
				out.write(resultVec_tmp1);
				//test-begin
				// for (unsigned i=0;i<MVTU;i++){
				// 	ap_int<IM> test=resultVec_tmp1(IM-1,0);
				// 	cout <<(double)test <<"   ";
				// 	resultVec_tmp1=resultVec_tmp1>>IM;
				// 	if (i==MVTU-1){
				// 		cout <<" \n";
				// 	}
				// }
				//test-end
				resultVec_tmp=resultVec_tmp>>(MVTU*IM);
				
		}

		if(wVec ==InputFold-1)
			resultVec_tmp=resultVec;

		if (wVec == InputFold-1) {
			wVec=0;
			if(wMat==OutputFold-1)
				wMat=0;
			else
				wMat++;
		}
		else
				wVec++;
	}

	for (unsigned i = 0; i < CycleNum; i++) {  //2
#pragma HLS PIPELINE II= 1
		resultVec_tmp1=resultVec_tmp(MVTU*IM-1,0 );
		out.write(resultVec_tmp1);
		resultVec_tmp=resultVec_tmp>>(MVTU*IM);
					// cout <<"resultVec   " <<(double)resultVec_tmp[i] <<" \n ";
		}
}
/***************************/
/*闂佸憡鍨兼慨銈夊汲閻旂厧瑙︾€广儱绻掔粣妗礦AU_outbuf
IFM婵犮垼娉涚粔鍫曞极閵堝憘鐔煎灳瀹曞洨顢呴梺闈涙濞村洨娆㈤銏犵闁惧繐婀辨径鍕倵濞戣櫕瀚?
2021/03/10 閻庤鐡曠亸娆戝垝閿熺姴钃熼柕澶堝妿濡诧拷
*/
/***************************/
template <	unsigned NumVecs,   //Dout*Dout
			unsigned Ibit,
			unsigned Wbit,
			unsigned Mbit,
			unsigned Abit,
			unsigned MatrixH, //Cin*K*K
			unsigned MatrixW,  //Cout
			unsigned InP,  //闂佸憡鐟崹浼村Φ閸ャ劎顩茬€广儺妲抧
			unsigned OutP,
			unsigned INC_BIT,
			unsigned BIAS_BIT,
			unsigned L_SHIFT>
void MVAU_outbuf(
	stream<ap_uint<InP*Ibit> >& vec,
	const ap_uint<InP*Wbit> weights[OutP][(MatrixH/InP)*(MatrixW/OutP)],
	const ap_int<INC_BIT> inc[OutP][MatrixW/OutP],
	const ap_int<BIAS_BIT> bias[OutP][MatrixW/OutP],
	stream<ap_uint<OutP*Abit> >& out,
	const unsigned reps = 1)
{
	//濠电偛顦崝宥夊礈娴煎瓨鏅慨妯虹－缁犲綊鏌涜椤ㄥ牓寮ˇ鎶﹏g long闂佹眹鍔岀€氼剛鏁锕€鐐婇柣妯挎珪绗慛umVecs闁荤偞绋忛崝搴ㄥΦ濮樿埖鍎嶉柛鏇ㄥ枤缂堝鏌涢幋婵囨儓鐟滅増鐓￠幊妤呭箣閹烘梻鐛ラ悗鍨緲閻楀棝濡甸敓锟?
	const unsigned long long InputFold = MatrixH/InP;
	const unsigned long long OutputFold = MatrixW/OutP;

	const unsigned long long totalReps = reps*NumVecs*InputFold*OutputFold;
/* solution1_FF婵炶揪缍€濞夋洟寮妶鍡樹氦婵炲棗閰ｉ崵瀣磼閹惧懘妾繛纭锋嫹*/
	ap_int<Mbit> resultVec[OutP][OutputFold];
	#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=0
/* solution2_BRAM婵炶揪缍€濞夋洟寮妶鍡樹氦婵炲棗閰ｉ崵瀣磼閹惧懘妾繛纭锋嫹
 * 闁哄鏅滈悷锕傛偋闁秵鍋ㄩ柕濞垮€楅悷婵嗩熆妫版繀绨绘い鏇ㄥ櫍閺佸秶浠﹂挊澶嬫灳闂佹悶鍎虫慨鍓ф崲閺囥垹鍙婃い鏍ㄦ皑閸欐垿鏌ら崘娴嬪亾閻旀粍甯￠弻灞剧節閸愵亶浼曞┑掳鍊ч幏锟?
 * */
//	ap_int<Mbit> resultVec[OutP][OutputFold];
//#pragma HLS ARRAY_PARTITION variable=resultVec complete dim=1
//#pragma HLS RESOURCE variable resultVec core=RAM_2P
	unsigned wVec = 0;
	unsigned wMat = 0;
	ap_uint<InP*Ibit> tempVec;
	ap_uint<OutP*Abit> outBuf;

	unsigned index = 0;
	for (unsigned rep = 0; rep < totalReps; rep++) {
#pragma HLS loop_tripcount min=NumVecs*MatrixH*MatrixW/InP/OutP max=NumVecs*MatrixH*MatrixW/InP/OutP
	#pragma HLS PIPELINE II=1

			if (wMat == 0) {
				tempVec = vec.read();
			}

			index = wVec*OutputFold+wMat;
			for (unsigned p = 0; p < OutP; p++) {  //闁哄鐗婇幐鎼佸吹椤撱垻宓侀柟顖炲亰閺€浠嬪箹鏉堝墽绱扮紒妤€鍊绘禒锕傚即閻愮數锛?
	#pragma HLS UNROLL
				ap_uint<InP*Wbit> tempMat = weights[p][index];

				ap_int<Mbit> acc = DOT<Wbit, Ibit, Mbit, InP>( tempMat, tempVec );
//婵炴垶鎸哥粔鐑藉礂濡粯濯撮悹鎭掑妽閺嗗粓RAM闂佹眹鍔岀€氼剛鏁锕€鐐婇柣妯挎珪绗戦梺鎼炲劤婵磭鎷归悢鍛婁氦婵炲棙鎸稿▍锟?
				if (wVec == 0)
					resultVec[p][wMat] = acc;
				else
					resultVec[p][wMat] += acc;

				outBuf( (p+1)*Abit-1, p*Abit ) = BN_Relu6<Mbit,INC_BIT,BIAS_BIT,Ibit,Wbit,L_SHIFT>(resultVec[p][wMat], inc[p][wMat], bias[p][wMat]);
			}

			if (wVec == InputFold-1){   //wVec=inputFold-1闂佸搫瀚鍢檃t婵炲濯撮幏锟?0闂佸憡甯炲绨刾utFold-1濠电偛鏋撮妶鍥╃倳闂佸憡鍨煎▍锝囧垝閵娾晛鍑犻柨鐕傛嫹
				out.write(outBuf);
			}

			if (wMat == OutputFold-1) {
				wMat = 0;
				if (wVec == InputFold-1)
					wVec = 0;
				else
					wVec++;
			}
			else
				wMat++;
		}
}

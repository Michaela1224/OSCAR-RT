#include "block_top.h"
#include <ap_axi_sdata.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <iostream>

#define IN_ROW      416      //输入图像行数
#define IN_COL      416      //输入图像列数
#define IN_CH       1      //输入图像通道数
#define BIT         8      //输入比特数
#define SIZE IN_ROW*IN_COL*IN_CH

typedef ap_uint< 8 > T_img_data_in;


bool loadFile_txt(const char* name1, int reps,stream<trans_pkt >& in);

void load_data(const char *path, char *ptr, unsigned int size);



int main(void){
//	T_img_data_in img[IN_ROW][IN_COL][IN_CH];
	ap_uint<32> in_data;


	trans_pkt in_tmp;
	int reps=1;

	//load_data("test.bin", (char *) img,sizeof(img));
	
	//T_img_data_in * data = (T_img_data_in *) img;
	stream<trans_pkt > in_stream("in_stream");

	loadFile_txt("input2.txt", 1,in_stream);
	// FILE* fp_input = fopen("input_verfy.txt", "wb");
	// for(unsigned m=0;m<reps;m++){
	// 	for(int i = 0;i < (SIZE/4);i++){
	// 		for(int j = 0;j < 4;j ++)
	// 		{
	// 			in_data(BIT*(j+1)-1, BIT*j) = data[i*4+j];
	// 			fprintf(fp_input, "%lf\n", double(data[i*4+j]));
	// 		}
	// 		in_tmp.data=in_data;
	// 		in_stream.write(in_tmp);
	// 	}
	// }
	// fclose(fp_input);
	stream<trans_pkt > out_stream("out_stream");
	trans_pkt temp_stream;
	ap_int<32> tmp;
	do_compute(in_stream,out_stream,reps);
	FILE* fp = fopen("output_v24_1.txt", "wb");
	for(int i = 0;i < 25*52*52*reps;i++){
		temp_stream=out_stream.read();
		tmp=temp_stream.data;
		fprintf(fp, "%lf\n", double(tmp));
	}
	fclose(fp);
/*	stream<axis_test > out_stream("out_stream");
	do_compute(in_stream,out_stream,reps);

	axis_test temp_stream;
	ap_uint<24*5> tmp;
	ap_int<5> tmp1;
	FILE* fp = fopen("output.txt", "wb");
	for(int i = 0;i < 200*200*reps;i++){
		temp_stream=out_stream.read();
		tmp=temp_stream.data;
		for(int j=0;j<24;j++){
			tmp1=tmp(4,0);        //out Abit
			tmp=tmp>>5;
			fprintf(fp, "%lf\n", double(tmp1));
		}
	}
	fclose(fp);
//	FILE* fp = fopen("output.txt", "wb");
//	for(int i = 0;i < 25*50*50*reps;i++){
//		temp_stream=out_stream.read();
//		tmp=temp_stream.data;
//		fprintf(fp, "%lf\n", double(tmp));
//	}
*/
	return 0;
}


void load_data(const char *path, char *ptr, unsigned int size)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f)
    {
        std::cout << "no such file,please check the file name!/n";
        exit(0);
    }
    f.read(ptr, size);
    f.close();
    std::cout << "load data successful/n" << std::endl;
}

bool loadFile_txt(const char* name1, int reps,stream<trans_pkt >& in){
	FILE* fp1 = fopen(name1, "rb");
	int i = 0;
	int j = 0;
	ap_uint<8> tmp;  //输入为3bit
	ap_uint<32> in_data;
	trans_pkt in_tmp;
	int rep;
	double temp;
	FILE* fp_input = fopen("input_verfy.txt", "wb");
	if ((fp1 == NULL)) {
		std::cout << "Load Error!" << std::endl;
		return false;
	}
	for(rep=0;rep<reps;rep++){
		for (i = 0; i<SIZE/4; i++) {
			for (j = 0; j<4; j++) {
				fscanf(fp1, "%lf", &temp);  //数据格式为double
				
				tmp = (ap_uint<8>)temp;  //数据转化为输入bit
				fprintf(fp_input, "%d\n", int(tmp));
	//			cout << "start:" << tmp <<endl;
				in_data(BIT*(j+1)-1, BIT*j) = tmp;
			}
			in_tmp.data=in_data;
			in.write(in_tmp);
		}
	}
	fflush(fp1);  //清除读写缓冲区。强迫将缓冲区的数据写回参数stream指定的文件中
	fclose(fp1);
	fclose(fp_input);
	std::cout << "Load Success!" << std::endl;
	return true;
}







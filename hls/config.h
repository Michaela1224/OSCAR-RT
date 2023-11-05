#define BNOutBit 4
#define FIRST_BLOCK_OUTBIT 5
#define LastOutBit 32
#define Two_Bypass_Outbit 6
// conv_0_3*3 
//Res_DSP = 39 latency_mvu= 389376 
#define CONV_0_K 3 
#define CONV_0_S 2 
#define CONV_0_P 1 
#define CONV_0_IN_CH 1 
#define CONV_0_IN_ROW 416 
#define CONV_0_IN_COL 416 
#define CONV_0_OUT_CH 32 
#define CONV_0_OUT_ROW 208 
#define CONV_0_OUT_COL 208 
#define CONV_0_INP 1 
#define CONV_0_OUP 32 
#define CONV_0_IN_BIT 8 
#define CONV_0_OUT_BIT 3 
#define CONV_0_W_BIT 4 
#define CONV_0_MVTU 4 
#define CONV_0_IM 16 
#define CONV_0_INC_BIT 23 
#define CONV_0_BIAS_BIT 31 
#define CONV_0_W_TILES 9 
#define CONV_0_A_TILES 8 
#define CONV_0_L_SHIFT 16 

// conv_1_DW
//Res_DSP = 8 latency_mvu= 389376 
#define CONV_1_K 3 
#define CONV_1_S 1 
#define CONV_1_P 1 
#define CONV_1_IN_CH 32 
#define CONV_1_IN_ROW 208 
#define CONV_1_IN_COL 208 
#define CONV_1_OUT_CH 32 
#define CONV_1_OUT_ROW 208 
#define CONV_1_OUT_COL 208 
#define CONV_1_INP 32 
#define CONV_1_OUP 32 
#define CONV_1_IN_BIT 3 
#define CONV_1_OUT_BIT 3 
#define CONV_1_W_BIT 4 
#define CONV_1_MVTU 4 
#define CONV_1_IM 11 
#define CONV_1_INC_BIT 26 
#define CONV_1_BIAS_BIT 26 
#define CONV_1_W_TILES 1 
#define CONV_1_A_TILES 8 
#define CONV_1_L_SHIFT 16 

// conv_2_1*1 
//Res_DSP = 36 latency_mvu= 259584 
#define CONV_2_K 1 
#define CONV_2_S 1 
#define CONV_2_P 0 
#define CONV_2_IN_CH 32 
#define CONV_2_IN_ROW 208 
#define CONV_2_IN_COL 208 
#define CONV_2_OUT_CH 12 
#define CONV_2_OUT_ROW 208 
#define CONV_2_OUT_COL 208 
#define CONV_2_INP 32 
#define CONV_2_OUP 2 
#define CONV_2_IN_BIT 3 
#define CONV_2_OUT_BIT 4 
#define CONV_2_W_BIT 4 
#define CONV_2_MVTU 2 
#define CONV_2_IM 13 
#define CONV_2_INC_BIT 21 
#define CONV_2_BIAS_BIT 26 
#define CONV_2_W_TILES 6 
#define CONV_2_A_TILES 6 
#define CONV_2_L_SHIFT 16 

// conv_3_1*1 
//Res_DSP = 42 latency_mvu= 259584 
#define CONV_3_K 1 
#define CONV_3_S 1 
#define CONV_3_P 0 
#define CONV_3_IN_CH 12 
#define CONV_3_IN_ROW 208 
#define CONV_3_IN_COL 208 
#define CONV_3_OUT_CH 32 
#define CONV_3_OUT_ROW 208 
#define CONV_3_OUT_COL 208 
#define CONV_3_INP 2 
#define CONV_3_OUP 32 
#define CONV_3_IN_BIT 4 
#define CONV_3_OUT_BIT 3 
#define CONV_3_W_BIT 4 
#define CONV_3_MVTU 8 
#define CONV_3_IM 12 
#define CONV_3_INC_BIT 20 
#define CONV_3_BIAS_BIT 26 
#define CONV_3_W_TILES 6 
#define CONV_3_A_TILES 4 
#define CONV_3_L_SHIFT 16 

// conv_4_DW
//Res_DSP = 8 latency_mvu= 97344 
#define CONV_4_K 3 
#define CONV_4_S 2 
#define CONV_4_P 1 
#define CONV_4_IN_CH 32 
#define CONV_4_IN_ROW 208 
#define CONV_4_IN_COL 208 
#define CONV_4_OUT_CH 32 
#define CONV_4_OUT_ROW 104 
#define CONV_4_OUT_COL 104 
#define CONV_4_INP 32 
#define CONV_4_OUP 32 
#define CONV_4_IN_BIT 3 
#define CONV_4_OUT_BIT 3 
#define CONV_4_W_BIT 4 
#define CONV_4_MVTU 4 
#define CONV_4_IM 11 
#define CONV_4_INC_BIT 21 
#define CONV_4_BIAS_BIT 27 
#define CONV_4_W_TILES 1 
#define CONV_4_A_TILES 8 
#define CONV_4_L_SHIFT 16 

// conv_5_1*1 
//Res_DSP = 36 latency_mvu= 97344 
#define CONV_5_K 1 
#define CONV_5_S 1 
#define CONV_5_P 0 
#define CONV_5_IN_CH 32 
#define CONV_5_IN_ROW 104 
#define CONV_5_IN_COL 104 
#define CONV_5_OUT_CH 18 
#define CONV_5_OUT_ROW 104 
#define CONV_5_OUT_COL 104 
#define CONV_5_INP 32 
#define CONV_5_OUP 2 
#define CONV_5_IN_BIT 3 
#define CONV_5_OUT_BIT 4 
#define CONV_5_W_BIT 4 
#define CONV_5_MVTU 2 
#define CONV_5_IM 13 
#define CONV_5_INC_BIT 21 
#define CONV_5_BIAS_BIT 27 
#define CONV_5_W_TILES 9 
#define CONV_5_A_TILES 9 
#define CONV_5_L_SHIFT 16 

// conv_6_1*1 
//Res_DSP = 26 latency_mvu= 292032 
#define CONV_6_K 1 
#define CONV_6_S 1 
#define CONV_6_P 0 
#define CONV_6_IN_CH 18 
#define CONV_6_IN_ROW 104 
#define CONV_6_IN_COL 104 
#define CONV_6_OUT_CH 60 
#define CONV_6_OUT_ROW 104 
#define CONV_6_OUT_COL 104 
#define CONV_6_INP 2 
#define CONV_6_OUP 20 
#define CONV_6_IN_BIT 4 
#define CONV_6_OUT_BIT 3 
#define CONV_6_W_BIT 4 
#define CONV_6_MVTU 4 
#define CONV_6_IM 13 
#define CONV_6_INC_BIT 20 
#define CONV_6_BIAS_BIT 26 
#define CONV_6_W_TILES 27 
#define CONV_6_A_TILES 15 
#define CONV_6_L_SHIFT 16 

// conv_7_DW
//Res_DSP = 8 latency_mvu= 292032 
#define CONV_7_K 3 
#define CONV_7_S 1 
#define CONV_7_P 1 
#define CONV_7_IN_CH 60 
#define CONV_7_IN_ROW 104 
#define CONV_7_IN_COL 104 
#define CONV_7_OUT_CH 60 
#define CONV_7_OUT_ROW 104 
#define CONV_7_OUT_COL 104 
#define CONV_7_INP 20 
#define CONV_7_OUP 20 
#define CONV_7_IN_BIT 3 
#define CONV_7_OUT_BIT 3 
#define CONV_7_W_BIT 4 
#define CONV_7_MVTU 4 
#define CONV_7_IM 11 
#define CONV_7_INC_BIT 22 
#define CONV_7_BIAS_BIT 26 
#define CONV_7_W_TILES 3 
#define CONV_7_A_TILES 15 
#define CONV_7_L_SHIFT 16 

// conv_8_1*1 
//Res_DSP = 23 latency_mvu= 292032 
#define CONV_8_K 1 
#define CONV_8_S 1 
#define CONV_8_P 0 
#define CONV_8_IN_CH 60 
#define CONV_8_IN_ROW 104 
#define CONV_8_IN_COL 104 
#define CONV_8_OUT_CH 18 
#define CONV_8_OUT_ROW 104 
#define CONV_8_OUT_COL 104 
#define CONV_8_INP 20 
#define CONV_8_OUP 2 
#define CONV_8_IN_BIT 3 
#define CONV_8_OUT_BIT 4 
#define CONV_8_W_BIT 4 
#define CONV_8_MVTU 1 
#define CONV_8_IM 13 
#define CONV_8_INC_BIT 21 
#define CONV_8_BIAS_BIT 26 
#define CONV_8_W_TILES 27 
#define CONV_8_A_TILES 18 
#define CONV_8_L_SHIFT 16 

// conv_9_1*1 
//Res_DSP = 34 latency_mvu= 292032 
#define CONV_9_K 1 
#define CONV_9_S 1 
#define CONV_9_P 0 
#define CONV_9_IN_CH 18 
#define CONV_9_IN_ROW 104 
#define CONV_9_IN_COL 104 
#define CONV_9_OUT_CH 84 
#define CONV_9_OUT_ROW 104 
#define CONV_9_OUT_COL 104 
#define CONV_9_INP 2 
#define CONV_9_OUP 28 
#define CONV_9_IN_BIT 5 
#define CONV_9_OUT_BIT 3 
#define CONV_9_W_BIT 4 
#define CONV_9_MVTU 4 
#define CONV_9_IM 14 
#define CONV_9_INC_BIT 20 
#define CONV_9_BIAS_BIT 27 
#define CONV_9_W_TILES 27 
#define CONV_9_A_TILES 21 
#define CONV_9_L_SHIFT 16 

// conv_10_DW
//Res_DSP = 8 latency_mvu= 73008 
#define CONV_10_K 3 
#define CONV_10_S 2 
#define CONV_10_P 1 
#define CONV_10_IN_CH 84 
#define CONV_10_IN_ROW 104 
#define CONV_10_IN_COL 104 
#define CONV_10_OUT_CH 84 
#define CONV_10_OUT_ROW 52 
#define CONV_10_OUT_COL 52 
#define CONV_10_INP 28 
#define CONV_10_OUP 28 
#define CONV_10_IN_BIT 3 
#define CONV_10_OUT_BIT 3 
#define CONV_10_W_BIT 4 
#define CONV_10_MVTU 4 
#define CONV_10_IM 11 
#define CONV_10_INC_BIT 21 
#define CONV_10_BIAS_BIT 27 
#define CONV_10_W_TILES 3 
#define CONV_10_A_TILES 21 
#define CONV_10_L_SHIFT 16 

// conv_11_1*1 
//Res_DSP = 31 latency_mvu= 97344 
#define CONV_11_K 1 
#define CONV_11_S 1 
#define CONV_11_P 0 
#define CONV_11_IN_CH 84 
#define CONV_11_IN_ROW 52 
#define CONV_11_IN_COL 52 
#define CONV_11_OUT_CH 24 
#define CONV_11_OUT_ROW 52 
#define CONV_11_OUT_COL 52 
#define CONV_11_INP 28 
#define CONV_11_OUP 2 
#define CONV_11_IN_BIT 3 
#define CONV_11_OUT_BIT 4 
#define CONV_11_W_BIT 4 
#define CONV_11_MVTU 1 
#define CONV_11_IM 14 
#define CONV_11_INC_BIT 19 
#define CONV_11_BIAS_BIT 26 
#define CONV_11_W_TILES 36 
#define CONV_11_A_TILES 24 
#define CONV_11_L_SHIFT 16 

// conv_12_1*1 
//Res_DSP = 15 latency_mvu= 389376 
#define CONV_12_K 1 
#define CONV_12_S 1 
#define CONV_12_P 0 
#define CONV_12_IN_CH 24 
#define CONV_12_IN_ROW 52 
#define CONV_12_IN_COL 52 
#define CONV_12_OUT_CH 144 
#define CONV_12_OUT_ROW 52 
#define CONV_12_OUT_COL 52 
#define CONV_12_INP 2 
#define CONV_12_OUP 12 
#define CONV_12_IN_BIT 4 
#define CONV_12_OUT_BIT 3 
#define CONV_12_W_BIT 4 
#define CONV_12_MVTU 1 
#define CONV_12_IM 13 
#define CONV_12_INC_BIT 20 
#define CONV_12_BIAS_BIT 26 
#define CONV_12_W_TILES 144 
#define CONV_12_A_TILES 144 
#define CONV_12_L_SHIFT 16 

// conv_13_DW
//Res_DSP = 6 latency_mvu= 292032 
#define CONV_13_K 3 
#define CONV_13_S 1 
#define CONV_13_P 1 
#define CONV_13_IN_CH 144 
#define CONV_13_IN_ROW 52 
#define CONV_13_IN_COL 52 
#define CONV_13_OUT_CH 144 
#define CONV_13_OUT_ROW 52 
#define CONV_13_OUT_COL 52 
#define CONV_13_INP 12 
#define CONV_13_OUP 12 
#define CONV_13_IN_BIT 3 
#define CONV_13_OUT_BIT 3 
#define CONV_13_W_BIT 4 
#define CONV_13_MVTU 2 
#define CONV_13_IM 11 
#define CONV_13_INC_BIT 22 
#define CONV_13_BIAS_BIT 26 
#define CONV_13_W_TILES 12 
#define CONV_13_A_TILES 72 
#define CONV_13_L_SHIFT 16 

// conv_14_1*1 
//Res_DSP = 15 latency_mvu= 389376 
#define CONV_14_K 1 
#define CONV_14_S 1 
#define CONV_14_P 0 
#define CONV_14_IN_CH 144 
#define CONV_14_IN_ROW 52 
#define CONV_14_IN_COL 52 
#define CONV_14_OUT_CH 24 
#define CONV_14_OUT_ROW 52 
#define CONV_14_OUT_COL 52 
#define CONV_14_INP 12 
#define CONV_14_OUP 2 
#define CONV_14_IN_BIT 3 
#define CONV_14_OUT_BIT 4 
#define CONV_14_W_BIT 4 
#define CONV_14_MVTU 1 
#define CONV_14_IM 15 
#define CONV_14_INC_BIT 19 
#define CONV_14_BIAS_BIT 26 
#define CONV_14_W_TILES 144 
#define CONV_14_A_TILES 24 
#define CONV_14_L_SHIFT 16 

// conv_15_1*1 
//Res_DSP = 15 latency_mvu= 389376 
#define CONV_15_K 1 
#define CONV_15_S 1 
#define CONV_15_P 0 
#define CONV_15_IN_CH 24 
#define CONV_15_IN_ROW 52 
#define CONV_15_IN_COL 52 
#define CONV_15_OUT_CH 144 
#define CONV_15_OUT_ROW 52 
#define CONV_15_OUT_COL 52 
#define CONV_15_INP 2 
#define CONV_15_OUP 12 
#define CONV_15_IN_BIT 5 
#define CONV_15_OUT_BIT 3 
#define CONV_15_W_BIT 4 
#define CONV_15_MVTU 1 
#define CONV_15_IM 14 
#define CONV_15_INC_BIT 20 
#define CONV_15_BIAS_BIT 26 
#define CONV_15_W_TILES 144 
#define CONV_15_A_TILES 144 
#define CONV_15_L_SHIFT 16 

// conv_16_DW
//Res_DSP = 6 latency_mvu= 292032 
#define CONV_16_K 3 
#define CONV_16_S 1 
#define CONV_16_P 1 
#define CONV_16_IN_CH 144 
#define CONV_16_IN_ROW 52 
#define CONV_16_IN_COL 52 
#define CONV_16_OUT_CH 144 
#define CONV_16_OUT_ROW 52 
#define CONV_16_OUT_COL 52 
#define CONV_16_INP 12 
#define CONV_16_OUP 12 
#define CONV_16_IN_BIT 3 
#define CONV_16_OUT_BIT 3 
#define CONV_16_W_BIT 4 
#define CONV_16_MVTU 2 
#define CONV_16_IM 11 
#define CONV_16_INC_BIT 21 
#define CONV_16_BIAS_BIT 26 
#define CONV_16_W_TILES 12 
#define CONV_16_A_TILES 72 
#define CONV_16_L_SHIFT 16 

// conv_17_1*1 
//Res_DSP = 15 latency_mvu= 389376 
#define CONV_17_K 1 
#define CONV_17_S 1 
#define CONV_17_P 0 
#define CONV_17_IN_CH 144 
#define CONV_17_IN_ROW 52 
#define CONV_17_IN_COL 52 
#define CONV_17_OUT_CH 24 
#define CONV_17_OUT_ROW 52 
#define CONV_17_OUT_COL 52 
#define CONV_17_INP 12 
#define CONV_17_OUP 2 
#define CONV_17_IN_BIT 3 
#define CONV_17_OUT_BIT 4 
#define CONV_17_W_BIT 4 
#define CONV_17_MVTU 1 
#define CONV_17_IM 15 
#define CONV_17_INC_BIT 20 
#define CONV_17_BIAS_BIT 26 
#define CONV_17_W_TILES 144 
#define CONV_17_A_TILES 24 
#define CONV_17_L_SHIFT 16 

// conv_18_1*1 
//Res_DSP = 15 latency_mvu= 389376 
#define CONV_18_K 1 
#define CONV_18_S 1 
#define CONV_18_P 0 
#define CONV_18_IN_CH 24 
#define CONV_18_IN_ROW 52 
#define CONV_18_IN_COL 52 
#define CONV_18_OUT_CH 144 
#define CONV_18_OUT_ROW 52 
#define CONV_18_OUT_COL 52 
#define CONV_18_INP 2 
#define CONV_18_OUP 12 
#define CONV_18_IN_BIT 6 
#define CONV_18_OUT_BIT 3 
#define CONV_18_W_BIT 4 
#define CONV_18_MVTU 1 
#define CONV_18_IM 15 
#define CONV_18_INC_BIT 20 
#define CONV_18_BIAS_BIT 28 
#define CONV_18_W_TILES 144 
#define CONV_18_A_TILES 144 
#define CONV_18_L_SHIFT 16 

// conv_19_DW
//Res_DSP = 6 latency_mvu= 292032 
#define CONV_19_K 3 
#define CONV_19_S 1 
#define CONV_19_P 1 
#define CONV_19_IN_CH 144 
#define CONV_19_IN_ROW 52 
#define CONV_19_IN_COL 52 
#define CONV_19_OUT_CH 144 
#define CONV_19_OUT_ROW 52 
#define CONV_19_OUT_COL 52 
#define CONV_19_INP 12 
#define CONV_19_OUP 12 
#define CONV_19_IN_BIT 3 
#define CONV_19_OUT_BIT 3 
#define CONV_19_W_BIT 4 
#define CONV_19_MVTU 2 
#define CONV_19_IM 11 
#define CONV_19_INC_BIT 23 
#define CONV_19_BIAS_BIT 29 
#define CONV_19_W_TILES 12 
#define CONV_19_A_TILES 72 
#define CONV_19_L_SHIFT 18 

// conv_20_1*1 
//Res_DSP = 27 latency_mvu= 389376 
#define CONV_20_K 1 
#define CONV_20_S 1 
#define CONV_20_P 0 
#define CONV_20_IN_CH 144 
#define CONV_20_IN_ROW 52 
#define CONV_20_IN_COL 52 
#define CONV_20_OUT_CH 48 
#define CONV_20_OUT_ROW 52 
#define CONV_20_OUT_COL 52 
#define CONV_20_INP 12 
#define CONV_20_OUP 4 
#define CONV_20_IN_BIT 3 
#define CONV_20_OUT_BIT 4 
#define CONV_20_W_BIT 4 
#define CONV_20_MVTU 1 
#define CONV_20_IM 15 
#define CONV_20_INC_BIT 21 
#define CONV_20_BIAS_BIT 28 
#define CONV_20_W_TILES 144 
#define CONV_20_A_TILES 48 
#define CONV_20_L_SHIFT 18 

// conv_21_1*1 
//Res_DSP = 32 latency_mvu= 389376 
#define CONV_21_K 1 
#define CONV_21_S 1 
#define CONV_21_P 0 
#define CONV_21_IN_CH 48 
#define CONV_21_IN_ROW 52 
#define CONV_21_IN_COL 52 
#define CONV_21_OUT_CH 168 
#define CONV_21_OUT_ROW 52 
#define CONV_21_OUT_COL 52 
#define CONV_21_INP 4 
#define CONV_21_OUP 14 
#define CONV_21_IN_BIT 4 
#define CONV_21_OUT_BIT 3 
#define CONV_21_W_BIT 4 
#define CONV_21_MVTU 2 
#define CONV_21_IM 14 
#define CONV_21_INC_BIT 19 
#define CONV_21_BIAS_BIT 26 
#define CONV_21_W_TILES 144 
#define CONV_21_A_TILES 84 
#define CONV_21_L_SHIFT 16 

// conv_22_DW
//Res_DSP = 6 latency_mvu= 292032 
#define CONV_22_K 3 
#define CONV_22_S 1 
#define CONV_22_P 1 
#define CONV_22_IN_CH 168 
#define CONV_22_IN_ROW 52 
#define CONV_22_IN_COL 52 
#define CONV_22_OUT_CH 168 
#define CONV_22_OUT_ROW 52 
#define CONV_22_OUT_COL 52 
#define CONV_22_INP 14 
#define CONV_22_OUP 14 
#define CONV_22_IN_BIT 3 
#define CONV_22_OUT_BIT 3 
#define CONV_22_W_BIT 4 
#define CONV_22_MVTU 2 
#define CONV_22_IM 11 
#define CONV_22_INC_BIT 25 
#define CONV_22_BIAS_BIT 26 
#define CONV_22_W_TILES 12 
#define CONV_22_A_TILES 84 
#define CONV_22_L_SHIFT 16 

// conv_23_1*1 
//Res_DSP = 45 latency_mvu= 259584 
#define CONV_23_K 1 
#define CONV_23_S 1 
#define CONV_23_P 0 
#define CONV_23_IN_CH 168 
#define CONV_23_IN_ROW 52 
#define CONV_23_IN_COL 52 
#define CONV_23_OUT_CH 48 
#define CONV_23_OUT_ROW 52 
#define CONV_23_OUT_COL 52 
#define CONV_23_INP 14 
#define CONV_23_OUP 6 
#define CONV_23_IN_BIT 3 
#define CONV_23_OUT_BIT 4 
#define CONV_23_W_BIT 4 
#define CONV_23_MVTU 1 
#define CONV_23_IM 15 
#define CONV_23_INC_BIT 20 
#define CONV_23_BIAS_BIT 26 
#define CONV_23_W_TILES 96 
#define CONV_23_A_TILES 48 
#define CONV_23_L_SHIFT 16 

// conv_24_1*1 
//Res_DSP = 27 latency_mvu= 346112 
#define CONV_24_K 1 
#define CONV_24_S 1 
#define CONV_24_P 0 
#define CONV_24_IN_CH 48 
#define CONV_24_IN_ROW 52 
#define CONV_24_IN_COL 52 
#define CONV_24_OUT_CH 128 
#define CONV_24_OUT_ROW 52 
#define CONV_24_OUT_COL 52 
#define CONV_24_INP 6 
#define CONV_24_OUP 8 
#define CONV_24_IN_BIT 5 
#define CONV_24_OUT_BIT 3 
#define CONV_24_W_BIT 4 
#define CONV_24_MVTU 1 
#define CONV_24_IM 15 
#define CONV_24_INC_BIT 20 
#define CONV_24_BIAS_BIT 27 
#define CONV_24_W_TILES 128 
#define CONV_24_A_TILES 128 
#define CONV_24_L_SHIFT 16 

// conv_25_DW
//Res_DSP = 5 latency_mvu= 389376 
#define CONV_25_K 3 
#define CONV_25_S 1 
#define CONV_25_P 1 
#define CONV_25_IN_CH 128 
#define CONV_25_IN_ROW 52 
#define CONV_25_IN_COL 52 
#define CONV_25_OUT_CH 128 
#define CONV_25_OUT_ROW 52 
#define CONV_25_OUT_COL 52 
#define CONV_25_INP 8 
#define CONV_25_OUP 8 
#define CONV_25_IN_BIT 3 
#define CONV_25_OUT_BIT 3 
#define CONV_25_W_BIT 4 
#define CONV_25_MVTU 1 
#define CONV_25_IM 11 
#define CONV_25_INC_BIT 21 
#define CONV_25_BIAS_BIT 27 
#define CONV_25_W_TILES 16 
#define CONV_25_A_TILES 128 
#define CONV_25_L_SHIFT 16 

// conv_26_1*1 
//Res_DSP = 51 latency_mvu= 259584 
#define CONV_26_K 1 
#define CONV_26_S 1 
#define CONV_26_P 0 
#define CONV_26_IN_CH 128 
#define CONV_26_IN_ROW 52 
#define CONV_26_IN_COL 52 
#define CONV_26_OUT_CH 72 
#define CONV_26_OUT_ROW 52 
#define CONV_26_OUT_COL 52 
#define CONV_26_INP 8 
#define CONV_26_OUP 12 
#define CONV_26_IN_BIT 3 
#define CONV_26_OUT_BIT 4 
#define CONV_26_W_BIT 4 
#define CONV_26_MVTU 1 
#define CONV_26_IM 15 
#define CONV_26_INC_BIT 20 
#define CONV_26_BIAS_BIT 27 
#define CONV_26_W_TILES 96 
#define CONV_26_A_TILES 72 
#define CONV_26_L_SHIFT 16 

// conv_27_1*1 
//Res_DSP = 250 latency_mvu= 389376 
#define CONV_27_K 1 
#define CONV_27_S 1 
#define CONV_27_P 0 
#define CONV_27_IN_CH 72 
#define CONV_27_IN_ROW 52 
#define CONV_27_IN_COL 52 
#define CONV_27_OUT_CH 960 
#define CONV_27_OUT_ROW 52 
#define CONV_27_OUT_COL 52 
#define CONV_27_INP 12 
#define CONV_27_OUP 40 
#define CONV_27_IN_BIT 4 
#define CONV_27_OUT_BIT 3 
#define CONV_27_W_BIT 4 
#define CONV_27_MVTU 8 
#define CONV_27_IM 15 
#define CONV_27_INC_BIT 19 
#define CONV_27_BIAS_BIT 25 
#define CONV_27_W_TILES 144 
#define CONV_27_A_TILES 120 
#define CONV_27_L_SHIFT 16 

// conv_28
#define CONV_28_K 1 
#define CONV_28_S 1 
#define CONV_28_P 0 
#define CONV_28_IFM_CH 960 
#define CONV_28_IFM_ROW 52 
#define CONV_28_IFM_COL 52 
#define CONV_28_OFM_CH 25 
#define CONV_28_OFM_ROW 52 
#define CONV_28_OFM_COL 52 
#define CONV_28_SIMD 40 
#define CONV_28_PE 5 
#define CONV_28_IN_BIT 3 
#define CONV_28_W_BIT 4 
#define CONV_28_MVTU 1 
#define CONV_28_IM 8 
#define CONV_28_W_TILES 120 
#define CONV_28_L_SHIFT 0 


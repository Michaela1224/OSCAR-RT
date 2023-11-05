# OSCAR-RT

This repository serves as the official code release of the paper [Algorithm/Hardware Codesign for Real-Time On-Satellite CNN-Based Ship Detection in SAR Imagery](https://ieeexplore.ieee.org/document/9739778) (pubilished at IEEE TGRS 2022).

Recently, the convolutional neural network (CNN) based approach for on-satellite ship detection in synthetic aperture radar (SAR) images has received increasing attention, since it does not rely on predefined imagery features and distributions that are required in conventional detection methods. To achieve a high detection accuracy, most of the existing CNN-based methods leverage complex off-the-shelf CNN models for optical imagery. Unfortunately, this usually leads to expensive computational cost, which is hard to process in real time using resource-constrained devices deployed in the harsh satellite environment. 

In this paper, we propose \Toolname, the first end-to-end algorithm/hardware co-design framework for \underline{O}n-\underline{S}atellite \underline{C}NN based S\underline{AR} ship detection, which can simultaneously produce an accurate and hardware-friendly CNN model and an ultra-efficient FPGA-based hardware accelerator that can be deployed on satellites. 
With the real-time on-satellite processing speed in mind, we start from a state-of-the-art compact CNN model for optical imagery.  
To eliminate the sharp decrease in the detection accuracy for SAR imagery, we analyze the discrepancy between the SAR domain and optical domain, and propose to adapt the model by adjusting the output feature size to better detect relatively smaller objects in SAR imagery.
To improve the detection speed, we propose to develop a fully-pipelined inter-layer streaming accelerator architecture, where all the layers of the CNN model can be concurrently processed using on-chip FPGA resources. 
To achieve this architecture, we first propose a hardware-guided, progressive, and structural pruning strategy, which is guided by our modeled hardware metrics and applies state-of-the-art coarse-grained and fine-grained filter pruning, as well as mixed-precision quantization techniques. 
Moreover, to improve the reusability and portability of the hardware accelerator design, we develop a library of highly optimized CNN components in high-level synthesis, together with their performance and resource models. Finally, we map the pruned CNN model onto these hardware library components in a fully-pipelined inter-layer streaming fashion, by adjusting their parallelism factors to balance the execution of each layer and fit into the resource constraint.
Experimental results using the adapted MobileNetV1, MobileNetV2, and SqueezeNet models on the widely used SAR ship detection dataset (SSDD) demonstrate the effectiveness of \Toolname: for the MobileNetV1 model, it achieves an average precision of 94\%, a detection speed of 652 frames per second on the Xilinx VC709 FPGA evaluation board, while consuming about 5.8W power.



## Citation

If OSCAR-RT helps your research, please consider citing:
```
@article{yang2022algorithm,
	title={Algorithm/Hardware Codesign for Real-Time On-Satellite CNN-Based Ship Detection in SAR Imagery},
	author={Yang, Geng and Lei, Jie and Xie, Weiying and Fang, Zhenman and Li, Yunsong and Wang, Jiaxuan and Zhang, Xin},
	journal={IEEE Transactions on Geoscience and Remote Sensing},
	volume={60},
	pages={1--18},
	year={2022},
	publisher={IEEE}
}
```



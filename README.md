# reginalScanlineZbuffer

# 程序原理
[区域扫描线zbuffer](https://xuejiaoyuan.github.io/2017/12/08/002-区域扫描线z-buffer算法及实现记录/)

# 开发环境
**处理器：** Interl(R) Core(TM) i5-6200U CPU @ 2.30GHz, RAM 4.00GB.

**开发环境：** Micorsoft Visual Studio 2015.

# 程序说明
### 输入格式
程序采用dos界面交互输入的方式，可以在Models文件夹下存放需要显示文件，在界面中输入需要显示的文件名（此处不需要填写Models目录，直接输入文件名即可）；并选择是否需要使用加速；
### 输入文件
程序在测试时选择读取.obj文件，读取文件后仅保存了物体的顶点信息和面表的定点序列；若顶点过小或过大，程序会自行对模型进行尺寸放缩；文件处理的是三角面片，因此希望测试的.obj文件使用三角面片构成的模型；可以通过左右方向键调整观察角度；
### 算法加速
在区域扫描步骤中，使用vector存储flag=in的多边形，并使用sort函数对多边形进行排序，确定z值最大的多边形；

定义加速算法，计算多边形的法向量，由于法向量与物体到人眼方向的夹角大于90°时，物体便不可见，因此不需要考虑这部分面片，故将背面的多边形面片进行剔除，从而在区域扫描线处理时可以减少一半的数据处理量，时间复杂度和空间复杂度降低；

```
/////////////////////////////////////////////////
// 		物体初始化操作				//				      
// 读取.obj文件中的顶点和面顶点序列，若需要加速处理，  	//
// 将acc变量设置为true;				//		      
/////////////////////////////////////////////////
void InitializeObj(const string& ObjName, const int winWidth, 
		const int winHeight, bool acc = false);
```

# 结果测试
程序中测试的.obj文件均存放在“regionalScanlineZbuffer/Models/”目录下，下面是测试的模型顶点、面片数量，以及未加速及加速后的处理时间：

<table>
	<tr>
		<td>Obj文件</td>
		<td>顶点数</td>
		<td>面片数</td>
		<td>未加速时间</td>
		<td>加速时间</td>
	</tr>
	<tr>
		<td>bunny.obj</td>
		<td>34834</td>
		<td>69451</td>
		<td>25.333</td>
		<td>11.424</td>
	</tr>
		<tr>
		<td>cat.obj</td>
		<td>1136</td>
		<td>2082</td>
		<td>1.905</td>
		<td>1.071</td>
	</tr>
		<tr>
		<td>deer.obj</td>
		<td>772</td>
		<td>1508</td>
		<td>2.398</td>
		<td>1.288</td>
	</tr>
		<tr>
		<td>IronMan.obj</td>
		<td>129759</td>
		<td>149827</td>
		<td>64.917</td>
		<td>27.977</td>
	</tr>
		<tr>
		<td>horse.obj</td>
		<td>2831</td>
		<td>5333</td>
		<td>4.959</td>
		<td>2.696</td>
	</tr>
</table>

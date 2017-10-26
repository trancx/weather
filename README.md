Env: 	Linux CentOS_7 x86_64

	想要编译的话需要linux环境，windows对于线程的支持不行，所以不可能运行。

需要的库：
LIBS         -lpthread  -lcurl -lm

其他linux机跑起来需要这样：
	1. Qt Creator(5.9.1或者之后的版本）
	2. weather文件夹添加到Qt中
	3. 在项目文件夹里面生成的Makefile增加以上库，并且增加 ./myweather/include 为include路径
	4. 当然上述所提到的库必须得安装


之所以会这样因为这个Makefile是自动生成的，太多文件都是绝对路径，不可能在其他机上make,所以最简单的
办法就是由IDE解决这个问题！

文件夹里面还有 
	project.docx	这是转换之后的实验报告，挺多排版乱了 ：-< 
	project.odt 	这是原始实验报告，但是里面的文档如果存储为docx,会有乱码，所以转换为了PDF
	project.pdf	实验报告pdf 
	tmp.odg     	这是报告里面的示意图，为了更加清楚表示作用
	myweather/  	项目源文件

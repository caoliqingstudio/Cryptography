# 日志

### 2017.7.14 17:55
  并没有看懂如何加载dll文件。其中如下
  
	#include "include/gmp.h"
	#pragma comment (lib , "bin/libgmp-6.1.1.lib")
	//#pragma comment (dll , "bin/libgmp-6.1.1.dll")
	
然后发现并没什么用处。还尝试过把几个文件都放在同一个目录下，没啥卵用。        

~~好想问问你该怎么做呀T_T~~

### 2017.7.18 9:24
   看到类，然后想开始一下spn的部分，然而，发现我连密码学课本上spn模型的加密长度都忘了……是64还是 32 哇还是有区别的，还有密钥生成算法，我也不知道。
   写了一点接口的东西，如下，初始化和设置密钥。然后看了一点多线程，感觉轮密钥和加密可以两个线程就是不知道是否会快。
   还有密钥读取问题，是用long long（假如是64位）还是char数组，还是string。            
	
	class MySPN {
	public:
		MySPN();
		void setKey(std::string key);
		void setKey(unsigned long long key);
	private:
		unsigned long long thisKey;
		
	};
	
~~昨天问你，你说你还没看，就不知道说什么了~~            
分享一个网址[spn加密算法的实现](http://www.xuebuyuan.com/2151326.html)


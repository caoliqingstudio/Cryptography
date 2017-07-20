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

### 2017.7.19 ~~
   今天把题目要求确定了一下，然后呢，就是把基本框架搭好了，把轮密钥生成写了。     
   确定了一下，密钥长度为32，分组长度为16     
   然而，对于运行时候是只加密一各，还是一长串进行加密呢，这个我没确定。。    
   目前设定是只加密一串，理由是，加密一长串涉及到padding，以及密钥分配，难道都用一个密钥加密吗……kidding    
   而且，这只是测试    
       
   ~~你是谁，你在哪里~~    
       
	忘了说了，今天上午把权力的游戏第七季第一集看了，感谢(教主)[李科艺]的种子。

* 开局的二丫太帅了
* 那个堡主 小女孩也很棒
* 我的龙女没以前那么漂亮了，唉
* 雪诺 应该不会仁慈的过分吧
* 珊莎 好漂亮
   
### 2017.7.20 10:30
   今天就是实现了多线程，windows多线程要调用threadCreat，一点不开心，各种要求太多。特别是函数要是静态的。    
   为了传递参数，我把轮密钥存储在一个数组里面了。用一个结构体传递的参数，因为我发现那个只能传递指针，就一个参数。    
   
	struct argumKey {
		unsigned long inputKey;
		struct storeKey
		{
			unsigned short key;
			bool state;
		} roundKey[5];//存储轮密钥
	} threadArgu;
   
   就这样子了。    
   ~~在家待得没有感觉，你是谁~~     
   今天的课设就到这里了。。。
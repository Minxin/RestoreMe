pyc加密部分是利用模拟信号槽，将两个加密函数依次加载，第一个是文件字节倒序，第二个是文件字节横向建立二叉树后反转二叉树。
pyc头部抠掉1字节需要手动还原。
把pyc文件还原为py文件后，放在指定目录下，运行Game.exe。需要事先阅读py脚本，并得到达到条件所需数字序列。Game.exe运行后输入，这是flag的第一部分。
第二部分是理解解释器的对应指令（模仿pyc字节码的load和store），然后得到达到条件所需数字序列。
输入的两部分即是flag。

#Ubuntu14.04下安装osg并配置环境

##前置：
* 更新源:```sudo apt-get update```
* 安装gcc:```sudo apt-get install gcc```
* 安装cmake:```sudo apt-get install cmake-curses-gui```
* 安装git:```sudo apt-get install git```

##安装osg：
1. 下载osg到本地: ```git clone --branch OpenSceneGraph-3.4.0 https://github.com/openscenegraph/OpenSceneGraph.git```, 例如保存在```/home/username/download```下，成功clone后会生成```OpenSceneGraph```文件夹
2. 下载Linux下的OSG依赖三方库: ```sudo apt-get build-dep openscenegraph```
3. cd至```/home/username/download/OpenSceneGraph```下 运行指令make 等待make结束(如果运行make报错，先运行```cmake .```，生成了```cmake_install.cmake```后可运行make)
4. 运行指令```sudo make install``` 等待安装结束
5. 下载osg-data到本地:```git clone https://github.com/openscenegraph/OpenSceneGraph-Data.git``` , 例如保存在```/home/username/download```下，成功clone后会生成```OpenSceneGraph-Data```文件夹

##配置环境：
在主路径的```.bashrc```中添加环境变量：
```
export OSG_FILE_PATH=/home/username/download/OpenSceneGraph-Data
export PATH=/home/username/download/OpenSceneGraph/bin:$PATH
export LD_LIBRARY_PATH=/home/username/download/OpenSceneGraph/lib:$LD_LIBRARY_PATH
```
保存后 运行 ```source .bashrc```生效

如此 运行```osgviewer cow.osg```就能成功渲染出一头牛了


#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
//#include <OpenThreads>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include "server.h"
using namespace std;

class finalDraw : public osg::Camera::DrawCallback//相机更新回调类
{
public:	 
	finalDraw(volatile bool * capture_screenshot_lock_flag_in, volatile bool * send_image_lock_flag_in) {
		_image = new osg::Image;
		_capture_screenshot_lock_flag_in = capture_screenshot_lock_flag_in;
		_send_image_lock_flag_in = send_image_lock_flag_in;
	}//构造函数，分配私有图片类变量内存
	~finalDraw() {

	}
	virtual void operator () (osg::RenderInfo& renderInfo) const//虚函数，实现目的操作
	{
		printf("operator() start!\n");
		glReadBuffer(GL_BACK);//直接调用OpenGL函数，实现对后缓冲区的读取
//		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);//开启线程保护
		osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();//获取当前图形上下文
		int width = gc->getTraits()->width; int height = gc->getTraits()->height;//获取窗口的长宽
		_image->readPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE);//数据填充到图片
		char _name[] = "../files/sendImage/_capture.png";
		char name[] = "../files/sendImage/capture.png";
		osgDB::writeImageFile(*_image, _name);//图片写入到当前程序目录下
		int c;
		while(_capture_screenshot_lock_flag_in == false) {}
		FILE *fpSrc, *fpDest;  //定义两个指向文件的指针
		fpSrc = fopen(_name, "rb");
		if(fpSrc==NULL){
			printf( "_capture.png open failure.");  //源文件不存在的时候提示错误
			exit(0);
		}
		fpDest = fopen(name, "wb");  //以写入二进制的方式打开目标文件
		if(fpDest==NULL){
			printf("capture.png open failure.");
			exit(0);
		}
		while((c=fgetc(fpSrc))!=EOF){  //从源文件中读取数据知道结尾
			fputc(c, fpDest);
		}
		fclose(fpSrc);  //关闭文件指针，释放内存
		fclose(fpDest);
		* _capture_screenshot_lock_flag_in = false;
		* _send_image_lock_flag_in = true;
/*		if(0 == flock(fileno(fpDest), LOCK_EX))
	    {
	        printf("capture.png locked.\n");
			while((c=fgetc(fpSrc))!=EOF){  //从源文件中读取数据知道结尾
				fputc(c, fpDest);
			}
			fclose(fpSrc);  //关闭文件指针，释放内存
	        flock(fileno(fpDest), LOCK_UN);
			fclose(fpDest);
	    }
	    else
	    {
	        printf("lock failed\n");
			fclose(fpDest);
			fclose(fpSrc);  //关闭文件指针，释放内存
	    }
		printf("Captured!\n");
	}
*/
	}
protected:	 
	osg::ref_ptr<osg::Image> _image;//图片变量
	volatile bool * _capture_screenshot_lock_flag_in;
	volatile bool * _send_image_lock_flag_in;
//	mutable OpenThreads::Mutex _mutex;//线程保护对象变量
};
int create_screenshot(float * event_Array, volatile bool * capture_screenshot_lock_flag_in, volatile bool * send_image_lock_flag_in)
{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osgViewer::Viewer viewer;//创建视图类，实现建立窗口等操作
	osg::Node *loadedModel = osgDB::readNodeFile("../files/recvOsg/recv.osg");//应用osgDB文件读写类读入glider模型节点
	viewer.setSceneData(loadedModel);//为视图类设置场景，即观察glider模型
	viewer.setCameraManipulator( new osgGA::TrackballManipulator() ); //设置视点的操作类，类似Vega Prime的MotionDrive
	viewer.realize();//初始化
//	finalDraw * finaldraw = new finalDraw();
	bool once = false;//控制相机更新回调只加入一次
	while(!viewer.done())//程序没有终止，如按下Esc键，程序会终止
	{
		viewer.frame();//更新一帧
		printf("viewer.frame()\n");
//		viewer.getCamera()->setFinalDrawCallback(new finalDraw());//加入更新回调，输出图像

		if(!once && (viewer.elapsedTime()>0) )//没有执行且时间大于5秒，执行下现语句
		{
			once = true;//相机更新回调已经执行
			osg::ref_ptr<finalDraw> finaldraw = new finalDraw(capture_screenshot_lock_flag_in, send_image_lock_flag_in);
			viewer.getCamera()->setFinalDrawCallback(finaldraw);//加入更新回调，输出图像
		}
		if(event_Array[1] == ESCAPE_NUM){
			break;
		}
	}
	printf("viewer.done()\n");
	return 0;
}
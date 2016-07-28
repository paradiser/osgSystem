#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/EventQueue>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
//#include <OpenThreads>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include "server.h"
using namespace std;

PickInfoHandler::PickInfoHandler(float *event_Array)
{
		pickArray = event_Array;
		_signalInfo=false;
}
void PickInfoHandler::PrintInfo()
{
		//for(int i=0;i<5;i++)
		//	std::<"\t"<<pickArray[i];
		//cout<<"\tsignal\t"<<_signalInfo<<endl;
}
/*bool PickInfoHandler::CopyInfo(float * pickInfo)
{
		for(int i=0;i<5;i++)
			pickArray[i]=pickInfo[i];
		_signalInfo=true;
		return true;
}*/
void PickInfoHandler::ClearInfo()	
{
	_signalInfo=false;
}
int PickInfoHandler::getPick(int n)	
{
	return pickArray[n];
}
bool PickInfoHandler::getSignal()	
{
	return _signalInfo;
}
bool PickInfoHandler::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
	osgViewer::Viewer* viewer =dynamic_cast<osgViewer::Viewer*>(&aa);
	if ( viewer )			
	{
		if(this->getSignal()==false)
		{
			
			switch(this->getPick(0))
			{
			case 1:		//PUSHÊó±êÄ³¼ü°´ÏÂ
				viewer->getEventQueue()->mouseButtonPress(this->getPick(2),this->getPick(3),this->getPick(1),this->getPick(4));
				break;
			case 2:		//RELEASEÊó±êÄ³¼üµ¯Æð
				viewer->getEventQueue()->mouseButtonRelease(this->getPick(2),this->getPick(3),this->getPick(1),this->getPick(4));
				break;
			case 4:		//DOUBLECLICÊó±êÄ³¼üË«»÷
				viewer->getEventQueue()->mouseDoubleButtonPress(this->getPick(2),this->getPick(3),this->getPick(1),this->getPick(4));
				break;
			case 8:		//DRAG	Êó±êÄ³¼üÍÏ¶¯
				{
					osgGA:: GUIEventAdapter* event = viewer->getEventQueue()->createEvent();
					viewer->getEventQueue()->mouseWarped(this->getPick(2),this->getPick(3));
					event->setButtonMask(this->getPick(1));
					/*enum MouseButtonMask {
					 LEFT_MOUSE_BUTTON    = 1<<0,
					 MIDDLE_MOUSE_BUTTON  = 1<<1,
					 RIGHT_MOUSE_BUTTON   = 1<<2};*/
					event->setEventType(osgGA::GUIEventAdapter::DRAG);
					event->setTime(this->getPick (4));
					viewer->getEventQueue()->addEvent(event);
					break;
				}
			case 16:		//MOVE Êó±êÒÆ¶¯
				{
					osgGA:: GUIEventAdapter* event = viewer->getEventQueue()->createEvent();
					viewer->getEventQueue()->mouseWarped(this->getPick(2),this->getPick(3));
					event->setEventType(osgGA::GUIEventAdapter::MOVE );
					event->setTime(this->getPick (4));
					viewer->getEventQueue()->addEvent(event);
					break;
				}
			case 32:	//KeyDown
				viewer->getEventQueue()->keyPress(this->getPick(1),this->getPick(4),this->getPick(2));	
				break;
			case 64:	//KeyUP
				viewer->getEventQueue()->keyRelease(this->getPick(1),this->getPick(4),this->getPick(2));	
				break;
			case 128:	//FRAME	Ã¿Ö¡
				viewer->getEventQueue()->frame(this->getPick(4));
				break;
			case 256:	//RESIZE´°¿Ú´óÐ¡¸Ä±ä
				viewer->getEventQueue()->windowResize(0,0,this->getPick(2),this->getPick(3),this->getPick(4));
				break;
			case 512:	//SCROLLÊó±êÂÖ¹ö¶¯,»á»ñÈ¡Ã¶¾Ù±äÁ¿
			{	
				if(this->getPick(1)==3)
					viewer->getEventQueue()->mouseScroll(ea.SCROLL_UP,this->getPick(4));
				else if(this->getPick(1)==4)
					viewer->getEventQueue()->mouseScroll(ea.SCROLL_DOWN,this->getPick(4));
				else if(this->getPick(1)==5)
					viewer->getEventQueue()->mouseScroll2D(this->getPick(2),this->getPick(3));
				else{}
				break;
			}
			case 1024:	//PEN_PRESSURE	ÊÖÐ´°å
				viewer->getEventQueue()->penPressure(this->getPick(1),this->getPick(4));
				break;
			case 2048:	//PEN_ORIENTATION	ÊÖÐ´°åµÄÄ³ÊÂ¼þ
				viewer->getEventQueue()->penOrientation(this->getPick(2),this->getPick(3),this->getPick(1),this->getPick(4));
				break;
			case 4096:	//PPEN_PROXIMITY_ENTER	ÊÖÐ´°åµÄÄ³ÊÂ¼þ
				break;
			case 8192:	//PEN_PROXIMITY_LEAVE	ÊÖÐ´°åµÄÄ³ÊÂ¼þ
				break;
			case 16384:	//CLOSE_WINDOWS	¹Ø±Õ´°¿Ú
				viewer->getEventQueue()->closeWindow(this->getPick(4));
				break;
			case 32768:	//QUIT_APPLICATION		ÍË³ö³ÌÐò
				viewer->getEventQueue()->quitApplication(this->getPick(4));
				break;
			case 65536:	//USER		ÓÃ»§¶¨Òå
			{
				viewer->getEventQueue()->userEvent(new UserInfo(this->getPick(1)));
				const UserInfo* ti =dynamic_cast<const UserInfo*>( ea.getUserData());
				std::cout << "User event is: " <<ti->_count<<std::endl;
				break;
			}
			default:
				break;
			}
			this->PrintInfo();
			this->ClearInfo();
			pickArray[0] = 0;
		}		
	}
	return false;
}

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
//		printf("operator() start!\n");
		glReadBuffer(GL_BACK);//直接调用OpenGL函数，实现对后缓冲区的读取
//		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);//开启线程保护
		osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();//获取当前图形上下文
		int width = gc->getTraits()->width; int height = gc->getTraits()->height;//获取窗口的长宽
		_image->readPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE);//数据填充到图片
		char _name[BUFFER_SIZE] = "../files/sendImage/_capture";
		strcat(_name, IMAGE_FORMAT);
		char name[BUFFER_SIZE] = "../files/sendImage/capture";
		strcat(name, IMAGE_FORMAT);
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
	event_Array[0] = 4;
	event_Array[1] = 4;
	event_Array[2] = 567;
	event_Array[3] = 567;
	event_Array[4] = 0.1;
	osg::ref_ptr<PickInfoHandler> pickInfo = new PickInfoHandler(event_Array);
	viewer.addEventHandler(pickInfo);
	//Ìí¼Ó×´Ì¬ÊÂ¼þ
	viewer.addEventHandler (new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	//´°¿Ú´óÐ¡±ä»¯ÊÂ¼þ 
	viewer.addEventHandler(new osgViewer::WindowSizeHandler); 
	//Ìí¼ÓÒ»Ð©³£ÓÃ×´Ì¬ÉèÖÃ 
	viewer.addEventHandler(new osgViewer::StatsHandler);
	//Ìí¼ÓÂ·¾¶¼ÇÂ¼,°ÑÂ·¾¶¼ÇÂ¼µÄ¹¦ÄÜÌí¼Óµ½viewerµ±ÖÐ£¬ºÍÌí¼Ó×´Ì¬ÊÇÒ»ÑùÑùµÄ 
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);



	viewer.realize();//初始化
//	finalDraw * finaldraw = new finalDraw();
	bool once = false;//控制相机更新回调只加入一次
	while(!viewer.done())//程序没有终止，如按下Esc键，程序会终止
	{
		viewer.frame();//更新一帧
		usleep(50000);
//		printf("viewer.frame()\n");
//		viewer.getCamera()->setFinalDrawCallback(new finalDraw());//加入更新回调，输出图像

		if(!once && (viewer.elapsedTime() > 0) )//没有执行且时间大于5秒，执行下现语句
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
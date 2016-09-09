### ON NeoKylin Linux @Tianchixing‘s House
1.  添加 ELRepo 源:

```shell
sudo rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
sudo rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
```
2. 安装显卡检查程序:
```
sudo yum install nvidia-detect
```
3. 检测显卡型号，并选择对应的驱动:
```
$ nvidia-detect -v
Probing for supported NVIDIA devices...
[10de:06dd] NVIDIA Corporation GF100GL [Quadro 4000]
This device requires the current 346.47 NVIDIA driver kmod-nvidia
```
根据 nvidia-detect 的输出信息，可以知道显卡的型号，以及要使用的驱动版本 367.47 

yum install nvidia-x11-drv.x86_64

4. 安装docker

5. 安装nvidia-docker 
# Install nvidia-docker and nvidia-docker-plugin
wget -P /tmp https://github.com/NVIDIA/nvidia-docker/releases/download/v1.0.0-rc.3/nvidia-docker-1.0.0.rc.3-1.x86_64.rpm
sudo rpm -i /tmp/nvidia-docker*.rpm && rm /tmp/nvidia-docker*.rpm
sudo systemctl start nvidia-docker

# Test nvidia-smi	
nvidia-docker run --rm nvidia/cuda nvidia-smi

6. 下载包含cuda的docker镜像 

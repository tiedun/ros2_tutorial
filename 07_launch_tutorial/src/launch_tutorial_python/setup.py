from setuptools import find_packages, setup

# Python 标准库 os
# 这里主要使用 os.path.join() 拼接文件路径
import os

# 从 glob 模块导入 glob() 函数
# 用于按照通配符规则批量查找文件
from glob import glob

package_name = 'launch_tutorial_python'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),

    # 指定除了 Python 模块之外，
    # 还需要安装到 install 空间中的其它数据文件
    data_files=[
        # 将 resource/<功能包名> 安装到 ament resource index 中
        # 用于 ROS 2 / ament 发现当前功能包
        #
        # 源文件：
        # resource/launch_tutorial_python
        #
        # 目标文件：
        # <功能包安装前缀>/share/ament_index/resource_index/packages/
        # launch_tutorial_python
        #
        # 默认 isolated install 下即：
        # install/launch_tutorial_python/share/
        # ament_index/resource_index/packages/launch_tutorial_python
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),

        # 安装 package.xml
        #
        # 源文件：
        # package.xml
        #
        # 目标文件：
        # <功能包安装前缀>/share/<功能包名>/package.xml
        #
        # 默认 isolated install 下即：
        # install/launch_tutorial_python/share/
        # launch_tutorial_python/package.xml
        ('share/' + package_name, ['package.xml']),

        # 安装 launch/ 目录中的 Launch 文件
        #
        # os.path.join()：
        # 拼接安装目标目录：
        # share/launch_tutorial_python/launch
        #
        # glob('launch/*')：
        # 查找当前功能包 launch/ 目录中的所有文件
        #
        # 构建安装后，这些文件会被复制到：
        # install/launch_tutorial_python/share/launch_tutorial_python/launch/
        (os.path.join('share', package_name, 'launch'), glob('launch/*'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='atguigu',
    maintainer_email='3578283292@qq.com',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
        ],
    },
)

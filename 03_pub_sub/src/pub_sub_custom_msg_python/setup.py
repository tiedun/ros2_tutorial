from setuptools import find_packages, setup

package_name = 'pub_sub_custom_msg_python'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
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
            'publisher=pub_sub_custom_msg_python.publisher:main',
            'subscriber=pub_sub_custom_msg_python.subscriber:main'
        ],
    },
)

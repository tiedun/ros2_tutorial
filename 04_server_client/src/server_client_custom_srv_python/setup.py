from setuptools import find_packages, setup

package_name = 'server_client_custom_srv_python'

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
            'server=server_client_custom_srv_python.server:main',
            'client=server_client_custom_srv_python.client:main'
        ],
    },
)

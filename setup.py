import pybind11
from distutils.core import setup, Extension

ext_modules = [
    Extension(
        'napps_cpp_solver', # имя библиотеки собранной pybind11
        ['CalcSolver.cpp'], # Тестовый файлик который компилируем
        include_dirs=[pybind11.get_include()],  # не забываем добавить инклюды pybind11
        language='c++', # Указываем язык
        extra_compile_args=['-std=c++11', '-stdlib=libc++', '-mmacosx-version-min=10.7']
, # флаг с++11
    ),
]

setup(
    name='napps_cpp_solver', # имя библиотеки собранной pybind11
    version='1.1',
    author='napps',
    author_email='help@nitronapps.ru',
    description='pybind11 extension',
    ext_modules=ext_modules,
    requires=['pybind11'],  # Указываем зависимость от pybind11
    package_dir = {'': 'lib'}
)
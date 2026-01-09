from setuptools import setup, find_packages

setup(
    name='tic_tac_toe',
    version='0.1',
    package_dir={'': 'src'},
    packages=find_packages(where='src'),
    license='MIT',
    description='Tic-Tac-Toe game implementation',
    long_description=open('README.md').read(),
    install_requires=[],
    url='',
    author='',
    author_email='',
    extras_require={
        "dev": ["pytest", "flake8", "black", "nuitka"]
    },
    entry_points={
        "console_scripts": [
            "tic_tac_toe=main:main"
        ]
    },
    test_suite="tests",
)

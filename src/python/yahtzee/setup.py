from setuptools import setup, find_packages

setup(
    name='yahtzee',
    version='0.1',
    package_dir={'': 'src'},
    packages=find_packages(where='src'),
    license='MIT',
    description='Yahtzee game implementation',
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
            "yahtzee=yahtzee.main:main"
        ]
    },
    test_suite="tests",
)

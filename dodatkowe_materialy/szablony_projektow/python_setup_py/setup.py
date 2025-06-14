from setuptools import setup, find_packages

setup(
    name='example_package',
    version='0.1',
    package_dir={'': 'src'},  # This line is added to indicate the source directory
    packages=find_packages(where='src'),  # Specify the source directory
    license='MIT',
    description='A Python package example',
    long_description=open('README.md').read(),
    install_requires=['numpy'],
    url='https://github.com/yourusername/example_package',
    author='Your Name',
    author_email='your.email@example.com',
    extras_require={
        "dev": ["pytest", "flake8", "black", "nuitka"]
    },
    entry_points={
        "console_scripts": [
            "example-cli=example_package.example_module:main"
        ]
    },
    test_suite="tests",
)

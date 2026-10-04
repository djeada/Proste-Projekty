from setuptools import setup

EFFECTS = [
    'donut',
    'mandelbrot_zoom',
    'game_of_life',
    'matrix_rain',
    'doom_fire',
    'spinning_cube',
    'plasma',
    'quicksort_visualizer',
    'maze_solver',
    'ray_tracer',
    'warp_starfield',
]

setup(
    name='terminal_effects',
    version='0.1',
    package_dir={'': 'src'},
    py_modules=EFFECTS,
    license='MIT',
    description='Terminal animations drawn with ANSI escape codes',
    long_description=open('README.md').read(),
    install_requires=[],
    url='',
    author='',
    author_email='',
    extras_require={
        "dev": ["pytest", "flake8", "black"]
    },
    entry_points={
        "console_scripts": [f"{name}={name}:main" for name in EFFECTS]
    },
    test_suite="tests",
)

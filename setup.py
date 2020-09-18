from setuptools import setup
setup(
  name = 'cScell',
  packages = ['cScell'],
  version = '0.0.16',
  license='MIT',
  description = 'A framework made with python for C cli projects',
  author = 'Eugenio Vinicio Berretta',
  author_email = 'euberdeveloper@gmail.com',
  url = 'https://github.com/euberdeveloper/cScell',
  download_url = 'https://github.com/euberdeveloper/cScell/archive/v0.0.16.tar.gz',
  package_data={'cScell': ['templates/*.txt']},
  include_package_data=True,
  entry_points = {
    'console_scripts': ['cScell=cScell.command_line:main'],
  },
  keywords = ['cScell', 'shell', 'c', 'framework'],
  classifiers=[
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: Developers',
    'Topic :: Software Development :: Build Tools',
    'License :: OSI Approved :: MIT License',
    'Programming Language :: Python :: 3.5',
    'Programming Language :: Python :: 3.6',
    'Programming Language :: Python :: 3.7',
    'Programming Language :: Python :: 3.8'
  ],
)
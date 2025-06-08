from distutils.core import setup, Extension

def main():
    setup(name="oled-lib",
          version="1.0.0",
          description="oled_lib_4_py",
          author="ME",
          author_email="look@yourself.ie",
          ext_modules=[Extension("oled",["_oled.c","fonts.c"])]
          )
    
if __name__=="__main__":
    main()
# Define library reference handle for rule:  DirName = TARGET



INCLUDEPATH += $$PWD
LIBS += $$staticReference( $$basename(PWD) )
REFERENCES += $$basename(PWD)


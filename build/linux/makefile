PROG     = rsys_edit

OUTDIR   = ../../bin/linux/
OBJDIR   = $(OUTDIR)obj/
SRCDIR   = ../../src/

CXX      = g++
CXXFLAGS = -Wall -Wextra
OBJ      = $(OBJDIR)main.o $(OBJDIR)rsys_edit.o $(OBJDIR)io.o $(OBJDIR)aes.o $(OBJDIR)ninty-233.o \
           $(OBJDIR)ecc.o $(OBJDIR)sha1.o $(OBJDIR)BigInteger.o $(OBJDIR)BigIntegerAlgorithms.o   \
           $(OBJDIR)BigIntegerUtils.o $(OBJDIR)BigUnsigned.o $(OBJDIR)BigUnsignedInABase.o
LDFLAGS  =
LDLIBS   =


$(PROG): $(OBJ)
	$(CXX) -o $(OUTDIR)$@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)%.o:
	@mkdir -p $(OBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)


$(OBJDIR)main.o:                       $(SRCDIR)main.cpp $(SRCDIR)io.hpp $(SRCDIR)rsys_edit.hpp           \
                                       $(SRCDIR)ninty-233/ninty-233.hpp
$(OBJDIR)rsys_edit.o:                  $(SRCDIR)rsys_edit.cpp $(SRCDIR)rsys_edit.hpp  $(SRCDIR)aes/aes.h  \
                                       $(SRCDIR)ninty-233/ninty-233.hpp $(SRCDIR)io.hpp
$(OBJDIR)io.o:                         $(SRCDIR)io.cpp $(SRCDIR)io.hpp $(SRCDIR)ninty-233/ninty-233.hpp
$(OBJDIR)aes.o:                        $(SRCDIR)aes/aes.c $(SRCDIR)aes/aes.h
$(OBJDIR)ninty-233.o:                  $(SRCDIR)ninty-233/ninty-233.cpp $(SRCDIR)ninty-233/ninty-233.hpp  \
                                       $(SRCDIR)ninty-233/bigint/include/BigIntegerLibrary.hpp            \
                                       $(SRCDIR)ninty-233/ecc/ecc.hpp $(SRCDIR)ninty-233/sha1/sha1.h
$(OBJDIR)ecc.o:                        $(SRCDIR)ninty-233/ecc/ecc.cpp $(SRCDIR)ninty-233/ecc/ecc.hpp
$(OBJDIR)sha1.o:                       $(SRCDIR)ninty-233/sha1/sha1.c $(SRCDIR)ninty-233/sha1/sha1.h
$(OBJDIR)BigInteger.o:                 $(SRCDIR)ninty-233/bigint/src/BigInteger.cpp                       \
                                       $(SRCDIR)ninty-233/bigint/include/BigInteger.hpp                   \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsigned.hpp                  \
                                       $(SRCDIR)ninty-233/bigint/include/NumberlikeArray.hpp
$(OBJDIR)BigIntegerAlgorithms.o:       $(SRCDIR)ninty-233/bigint/src/BigIntegerAlgorithms.cpp             \
                                       $(SRCDIR)ninty-233/bigint/include/BigIntegerAlgorithms.hpp         \
                                       $(SRCDIR)ninty-233/bigint/include/BigInteger.hpp                   \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsigned.hpp                  \
                                       $(SRCDIR)ninty-233/bigint/include/NumberlikeArray.hpp
$(OBJDIR)BigIntegerUtils.o:            $(SRCDIR)ninty-233/bigint/src/BigIntegerUtils.cpp                  \
                                       $(SRCDIR)ninty-233/bigint/include/BigIntegerUtils.hpp              \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsignedInABase.hpp           \
                                       $(SRCDIR)ninty-233/bigint/include/BigInteger.hpp                   \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsigned.hpp                  \
                                       $(SRCDIR)ninty-233/bigint/include/NumberlikeArray.hpp
$(OBJDIR)BigUnsigned.o:                $(SRCDIR)ninty-233/bigint/src/BigUnsigned.cpp                      \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsigned.hpp                  \
                                       $(SRCDIR)ninty-233/bigint/include/NumberlikeArray.hpp
$(OBJDIR)BigUnsignedInABase.o:         $(SRCDIR)ninty-233/bigint/src/BigUnsignedInABase.cpp               \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsignedInABase.hpp           \
                                       $(SRCDIR)ninty-233/bigint/include/BigInteger.hpp                   \
                                       $(SRCDIR)ninty-233/bigint/include/BigUnsigned.hpp                  \
                                       $(SRCDIR)ninty-233/bigint/include/NumberlikeArray.hpp
.PHONY: clean
clean:
	rm -f $(OUTDIR)$(PROG) $(OBJDIR)*.o 

.PHONY: install
install:
	install $(OUTDIR)$(PROG) /usr/local/bin/

.PHONY: uninstall
uninstall:
	rm -f /usr/local/bin/$(PROG)
